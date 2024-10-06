#include <lio_ndt/front_end/front_end.hpp>

#include <cmath>
#include <pcl/common/transforms.h>
#include <glog/logging.h>

namespace lio_ndt
{
    FrontEnd::FrontEnd():icp_opti(OptimizedICPGN()),
                         local_map_ptr_(new CloudData::CLOUD()),
                         global_map_ptr_(new CloudData::CLOUD()),
                         result_cloud_ptr_(new CloudData::CLOUD())
        {
            // 设置默认参数，以免类的使用者在匹配之前忘了设置参数
            icp_opti.SetMaxCorrespondDistance(1);
            icp_opti.SetMaxIterations(2);
            icp_opti.SetTransformationEpsilon(0.5);
            cloud_filter_.setLeafSize(1.5f,1.5f,1.5f);
            local_map_filter_.setLeafSize(1.0f,1.0f,1.0f);
            display_filter_.setLeafSize(1.0f,1.0f,1.0f); 
        }
    
    // 输入当前帧扫描的点云图，对位姿进行更新
    Eigen::Matrix4f FrontEnd::Update(const CloudData& cloud_data)
    {
        // 参数初始化
        current_frame_.cloud_data.time = cloud_data.time;
        // 剔除无效点云：输入点云，输出点云，索引
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(*cloud_data.cloud_ptr, *current_frame_.cloud_data.cloud_ptr, indices);
        // 下采样滤波
        CloudData::CLOUD_PTR filtered_cloud_ptr(new CloudData::CLOUD()); // 定义过滤后的点云指针
        cloud_filter_.setInputCloud(current_frame_.cloud_data.cloud_ptr);
        cloud_filter_.filter(*filtered_cloud_ptr);
        // 四个位姿
        static Eigen::Matrix4f step_pose = Eigen::Matrix4f::Identity();
        static Eigen::Matrix4f last_pose = init_pose_;
        static Eigen::Matrix4f predict_pose = init_pose_;
        static Eigen::Matrix4f last_key_frame_pose = init_pose_;

        // 局部地图容器中没有关键帧，代表是第一帧数据
        // 此时把当前帧数据作为第一帧，并更新局部地图容器和全局地图容器
        if(local_map_frames_.size() == 0)
        {
            current_frame_.pose = init_pose_;
            UpdateNewFrame(current_frame_);
            return current_frame_.pose;
        }
        // 不是第一帧，就正常匹配
        icp_opti.Match(filtered_cloud_ptr,predict_pose,result_cloud_ptr_,current_frame_.pose);
        std::cout<<"fitness score:"<<icp_opti.GetFitnessScore()<<std::endl;
        

        // 此处采用运动模型来做位姿预测（当然也可以用IMU）更新相邻两帧的相对运动
        step_pose = last_pose.inverse() * current_frame_.pose;
        predict_pose = current_frame_.pose * step_pose;
        last_pose = current_frame_.pose;

        // 匹配之后根据距离判断是否需要生成新的关键帧，如果需要，则做相应更新
        if (fabs(last_key_frame_pose(0,3) - current_frame_.pose(0,3)) + 
            fabs(last_key_frame_pose(1,3) - current_frame_.pose(1,3)) +
            fabs(last_key_frame_pose(2,3) - current_frame_.pose(2,3)) > 2.0) 
        {
            UpdateNewFrame(current_frame_);
            last_key_frame_pose = current_frame_.pose;
        }

        return current_frame_.pose;
    }

    bool FrontEnd::SetInitPose(const Eigen::Matrix4f& init_pose)
    {
        init_pose_ = init_pose;
        return true;
    }

    bool FrontEnd::SetPredictPose(const Eigen::Matrix4f& predict_pose)
    {
        predict_pose_ = predict_pose;
        return true;
    }
    
    void FrontEnd::UpdateNewFrame(const Frame& new_key_frame)
    {
        Frame key_frame = new_key_frame;
        // 这一步的目的是为了把关键帧的点云保存下来。由于用的是共享指针，所以只是直接复制了一个指针而已
        // 此时无论你放多少个关键帧在容器里面，这些关键帧点云指针都是指向的同一个点云
        key_frame.cloud_data.cloud_ptr.reset(new CloudData::CLOUD(*new_key_frame.cloud_data.cloud_ptr));
        CloudData::CLOUD_PTR transformed_cloud_ptr(new CloudData::CLOUD());

        // 更新局部地图
        local_map_frames_.push_back(key_frame);
        while (local_map_frames_.size() > 20) // 局部地图滑窗队列大小固定为20
        {
            local_map_frames_.pop_front();
        }
        local_map_ptr_.reset(new CloudData::CLOUD()); // 更新局部地图指针
        
        // 遍历滑窗
        for(size_t i = 0; i < local_map_frames_.size(); ++i)
        {
            // 恢复下采样的点云数据，参数：输入，输出，估计的姿态
            pcl::transformPointCloud(*local_map_frames_.at(i).cloud_data.cloud_ptr, *transformed_cloud_ptr, local_map_frames_.at(i).pose);
            *local_map_ptr_ += *transformed_cloud_ptr;
        }
        has_new_local_map_ = true;

        // 更新匹配的目标点云
        if (local_map_frames_.size() < 10) // 如果局部地图数量少于10个，直接设置为目标点云
        {
            icp_opti.SetTargetCloud(local_map_ptr_);
        }
        else // 否则，先对局部地图进行下采样，再加进去
        {
            CloudData::CLOUD_PTR filtered_local_map_ptr(new CloudData::CLOUD());
            local_map_filter_.setInputCloud(local_map_ptr_);
            local_map_filter_.filter(*filtered_local_map_ptr);
            icp_opti.SetTargetCloud(filtered_local_map_ptr);
        }
        
        // 更新全局地图
        global_map_frames_.push_back(key_frame);
        if(global_map_frames_.size() % 100 != 0)
        {
            return;
        }
        else
        {
            global_map_ptr_.reset(new CloudData::CLOUD());
            for(size_t i = 0; i < global_map_frames_.size(); ++i)
            {
                pcl::transformPointCloud(*global_map_frames_.at(i).cloud_data.cloud_ptr, *transformed_cloud_ptr, global_map_frames_.at(i).pose);
                *global_map_ptr_ += *transformed_cloud_ptr;
            }
            has_new_global_map_ = true;
        }
    }

    bool FrontEnd::GetNewLocalMap(CloudData::CLOUD_PTR& local_map_ptr)
    {
        if (has_new_local_map_)
        {
            display_filter_.setInputCloud(local_map_ptr_);
            display_filter_.filter(*local_map_ptr);
            return true;
        }
        return false;
    }

    bool FrontEnd::GetNewGlobalMap(CloudData::CLOUD_PTR& global_map_ptr)
    {
        if (has_new_global_map_)
        {
            display_filter_.setInputCloud(global_map_ptr_);
            display_filter_.filter(*global_map_ptr);
            return true;
        }
        return false;  
    }

    bool FrontEnd::GetCurrentScan(CloudData::CLOUD_PTR& current_scan_ptr)
    {
        display_filter_.setInputCloud(result_cloud_ptr_);
        display_filter_.filter(*current_scan_ptr);
        return true;
    }
}