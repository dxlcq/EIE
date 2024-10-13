#include <ros/ros.h>
#include <pcl/common/transforms.h>
#include "glog/logging.h"

#include "lio_ndt/global_defination/global_defination.h.in"
#include "lio_ndt/subscriber/cloud_subscriber.hpp"
#include "lio_ndt/subscriber/imu_subscriber.hpp"
#include "lio_ndt/subscriber/gnss_subscriber.hpp"
#include "lio_ndt/tf_listener/tf_listener.hpp"
#include "lio_ndt/publisher/cloud_publisher.hpp"
#include "lio_ndt/publisher/odometry_publisher.hpp"
#include "lio_ndt/front_end/front_end.hpp"

using namespace lio_ndt;

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]); // 初始化日志函数 ，接受一个char*类型的参数也就是argv[0]
    FLAGS_log_dir = WORK_SPACE_PATH + "/Log"; // FLAGS_log_dir 设置日志输出目录
    FLAGS_alsologtostderr = 1; // 是否将所有日志输出到文件 这里选择是

    // 初始化ros节点与句柄
    ros::init(argc, argv, "front_end_node");
    ros::NodeHandle nh;

    // 三个订阅消息：点云、imu、GNSS
    std::shared_ptr<CloudSubscriber> cloud_sub_ptr = std::make_shared<CloudSubscriber>(nh, "/kitti/velo/pointcloud", 100000);
    std::shared_ptr<IMUSubscriber> imu_sub_ptr = std::make_shared<IMUSubscriber>(nh, "/kitti/oxts/imu", 1000000);
    std::shared_ptr<GNSSSubscriber> gnss_sub_ptr = std::make_shared<GNSSSubscriber>(nh, "/kitti/oxts/gps/fix", 1000000);

    // imu 到 lidar 的坐标变换
    std::shared_ptr<TFListener> imu_to_lidar_ptr = std::make_shared<TFListener>(nh, "velo_link", "imu_link");

    // 五个发布消息：点云、局部地图、全局地图、激光里程计、GNSS里程计(作为真值)
    std::shared_ptr<CloudPublisher> cloud_pub_ptr = std::make_shared<CloudPublisher>(nh, "current_scan", 100, "world");
    std::shared_ptr<CloudPublisher> local_map_pub_ptr = std::make_shared<CloudPublisher>(nh, "local_map", 100, "world");
    std::shared_ptr<CloudPublisher> global_map_pub_ptr = std::make_shared<CloudPublisher>(nh, "global_map", 100, "world");
    std::shared_ptr<OdometryPublisher> laser_odom_pub_ptr = std::make_shared<OdometryPublisher>(nh, "laser_odom", "world", "lidar", 100);
    std::shared_ptr<OdometryPublisher> gnss_pub_ptr = std::make_shared<OdometryPublisher>(nh, "gnss", "world", "lidar", 100);

    // 前端
    std::shared_ptr<FrontEnd> front_end_ptr = std::make_shared<FrontEnd>();

    // 定义用来保存点云 imu gnss 当前时刻的信息容器
    std::deque<CloudData> cloud_data_buff;
    std::deque<IMUData> imu_data_buff;
    std::deque<GNSSData> gnss_data_buff;

    // 初始化lidar与imu坐标系之间的转化关系
    Eigen::Matrix4f imu_to_lidar = Eigen::Matrix4f::Identity();
    bool transform_received = false;
    bool gnss_origin_position_inited = false;
    bool front_end_pose_inited = false;

    // 三个指向局部地图、全局地图、当前帧点云容器的指针
    CloudData::CLOUD_PTR local_map_ptr(new CloudData::CLOUD());
    CloudData::CLOUD_PTR global_map_ptr(new CloudData::CLOUD());
    CloudData::CLOUD_PTR current_scan_ptr(new CloudData::CLOUD());

    double run_time = 0.0;
    double init_time = 0.0;
    bool time_inited = false;
    bool has_global_map_published = false;

    ros::Rate rate(100); // 设置循环频率100hz

    while (ros::ok()) 
    {
        ros::spinOnce();
        // 把新点云、imu、gnss数据保存到cloud_data_buff imu_data_buff gnss_data_buff
        cloud_sub_ptr->ParseData(cloud_data_buff);
        imu_sub_ptr->ParseData(imu_data_buff);
        gnss_sub_ptr->ParseData(gnss_data_buff);
        // 导入lidar与imu位姿关系
        if (!transform_received)
        {
            // 以旋转矩阵的形式表示出
            if (imu_to_lidar_ptr->LookupData(imu_to_lidar))
            {
                transform_received = true;
            }
        }
        else
        {
            while (cloud_data_buff.size() > 0 && imu_data_buff.size() > 0 && gnss_data_buff.size() > 0)
            {
                // 提取第一个数据
                CloudData cloud_data = cloud_data_buff.front();
                IMUData imu_data = imu_data_buff.front();
                GNSSData gnss_data = gnss_data_buff.front();

                if (!time_inited) 
                {
                    time_inited = true;
                    init_time = cloud_data.time;
                }
                else
                {
                    run_time = cloud_data.time - init_time; // 点云运行时间
                }

                // 保证点云数据与imu、gnss数据时间同步
                double d_time = cloud_data.time - imu_data.time;
                if (d_time < -0.05)
                {
                    cloud_data_buff.pop_front();
                }
                else if (d_time > 0.05)
                {
                    imu_data_buff.pop_front();
                    gnss_data_buff.pop_front();
                }
                else
                {
                    // pop出读取的传感器数据
                    cloud_data_buff.pop_front();
                    imu_data_buff.pop_front();
                    gnss_data_buff.pop_front();

                    Eigen::Matrix4f odometry_matrix = Eigen::Matrix4f::Identity();
                    // 初始化gnss数据
                    if (!gnss_origin_position_inited) 
                    {
                        gnss_data.InitOriginPosition();
                        gnss_origin_position_inited = true;
                    }
                    // GNSS里程计位姿更新  使用GNSS(平移)数据和IMU(旋转)数据
                    gnss_data.UpdataXYZ();
                    odometry_matrix(0,3) = gnss_data.local_E;
                    odometry_matrix(1,3) = gnss_data.local_N;
                    odometry_matrix(2,3) = gnss_data.local_U;
                    odometry_matrix.block<3,3>(0,0) = imu_data.GetOrientationMatrix();
                    // lidar在世界坐标系下的位姿
                    odometry_matrix *= imu_to_lidar.inverse();
                    gnss_pub_ptr->Publish(odometry_matrix); // 发布真值轨迹位姿

                    // 前端里程计初始化
                    if (!front_end_pose_inited)
                    {
                        front_end_pose_inited = true;
                        front_end_ptr->SetInitPose(odometry_matrix);
                    }
                    // 更新预测位姿作为当前估计位姿
                    // front_end_ptr->SetPredictPose(odometry_matrix);
                    Eigen::Matrix4f laser_matrix = front_end_ptr->Update(cloud_data);
                    laser_odom_pub_ptr->Publish(laser_matrix); // 发布激光里程计

                    // 获取当前帧点云和局部地图并发布(都是下采样)
                    front_end_ptr->GetCurrentScan(current_scan_ptr);
                    cloud_pub_ptr->Publish(current_scan_ptr);
                    if (front_end_ptr->GetNewLocalMap(local_map_ptr))
                    {
                        local_map_pub_ptr->Publish(local_map_ptr);
                    }
                }
                // 如果运行时间超过460s且全局地图还没发布，获取下采样后的全局地图并发布
                if (run_time > 280.0 && !has_global_map_published)
                {
                    if (front_end_ptr->GetNewGlobalMap(global_map_ptr)) 
                    {
                        global_map_pub_ptr->Publish(global_map_ptr);
                        has_global_map_published = true;
                    }
                }
            }
        }
        rate.sleep();
    }
    
    return 0;
}