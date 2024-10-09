#pragma once

#include <deque>
#include <Eigen/Dense>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/registration/ndt.h>
#include <pcl/registration/icp.h> // icp头文件
#include <lio_ndt/sensor_data/cloud_data.hpp>
#include <thread>
#include <mutex>
#include "lio_ndt/method/optimized_ICP_GN.h"
#include "lio_ndt/method/common.h"
#include "lio_ndt/front_end/RegistrationAlgorithm.hpp"

namespace lio_ndt
{
    class FrontEnd
    {
    public:
        class Frame
        {
        public:
            Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();
            CloudData cloud_data;
        };

    public:
        /* 点云配准算法参数 */
        enum AlgorithmType
        {
            NDT_ICP,
            ICP,
            OptimizedICP
        };
    
    public:
        FrontEnd();
        FrontEnd( AlgorithmType type );
        
        Eigen::Matrix4f Update(const CloudData& cloud_data);
        bool SetInitPose(const Eigen::Matrix4f& init_pose);
        bool SetPredictPose(const Eigen::Matrix4f& predict_pose);

        bool GetNewLocalMap(CloudData::CLOUD_PTR& local_map_ptr);
        bool GetNewGlobalMap(CloudData::CLOUD_PTR& global_map_ptr);
        bool GetCurrentScan(CloudData::CLOUD_PTR& current_scan_ptr);

        /* 工厂函数 */
        std::unique_ptr<RegistrationAlgorithm> CreateRegistrationAlgorithm( AlgorithmType type );

    private:
        void UpdateNewFrame(const Frame& new_key_frame);

    private:
        // 变量
        AlgorithmType algorithm_type_;

        pcl::VoxelGrid<CloudData::POINT> cloud_filter_;
        pcl::VoxelGrid<CloudData::POINT> local_map_filter_;
        pcl::VoxelGrid<CloudData::POINT> display_filter_;

        // pcl::NormalDistributionsTransform<CloudData::POINT, CloudData::POINT> ndt_ptr_;
        // pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp_svd_;
        // OptimizedICPGN icp_opti_;
        std::unique_ptr<RegistrationAlgorithm> registration_ptr_;   // 智能指针声明时要使用unique_ptr，创建时使用make_unique
        
        std::deque<Frame> local_map_frames_;
        std::deque<Frame> global_map_frames_;

        bool has_new_local_map_ = false;
        bool has_new_global_map_ = false;

        CloudData::CLOUD_PTR local_map_ptr_;
        CloudData::CLOUD_PTR global_map_ptr_;
        CloudData::CLOUD_PTR result_cloud_ptr_;
        Frame current_frame_;

        Eigen::Matrix4f init_pose_ = Eigen::Matrix4f::Identity();
        Eigen::Matrix4f predict_pose_ = Eigen::Matrix4f::Identity();
    };
}