#include <ros/ros.h>
#include <pcl/common/transforms.h> // pcl::transformPointCloud 用到这个头文件
#include <glog/logging.h>

#include <lio_ndt/global_defination/global_defination.h.in>
#include <lio_ndt/subscriber/cloud_subscriber.hpp>
#include <lio_ndt/subscriber/imu_subscriber.hpp>
#include <lio_ndt/subscriber/gnss_subscriber.hpp>
#include <lio_ndt/tf_listener/tf_listener.hpp>
#include <lio_ndt/publisher/cloud_publisher.hpp>
#include <lio_ndt/publisher/odometry_publisher.hpp>

using namespace lio_ndt;

int main(int argc, char *argv[])
{
    //初始化日志函数 ，接受一个char*类型的参数也就是argv[0]
    google::InitGoogleLogging(argv[0]);
    //FLAGS_log_dir 设置日志输出目录
    FLAGS_log_dir = WORK_SPACE_PATH + "/Log";
    //是否将所有日志输出到文件 这里选择是
    FLAGS_alsologtostderr = 1;

    //初始化ros节点 与 句柄
    ros::init(argc, argv, "test_frame_node");
    ros::NodeHandle nh;

    //接收点云、imu、gnss、雷达与imu之间的标定信息话题
    //make_shared<CloudSubscriber>: make_shared使用过程就是调用CloudSubscriber中的模板函数推断出创建类型
    //shared_ptr 对象可以与相同的指针相关联，并在内部使用引用计数机制 计数为0是自动释放内存
    //std::make_shared 一次性为int对象和用于引用计数的数据都分配了内存，而new操作符只是为int分配了内存。
    std::shared_ptr<CloudSubscriber> cloud_sub_ptr = std::make_shared<CloudSubscriber>(nh, "/kitti/velo/pointcloud", 100000);
    std::shared_ptr<IMUSubscriber> imu_sub_ptr = std::make_shared<IMUSubscriber>(nh, "kitti/oxts/imu", 1000000);
    std::shared_ptr<GNSSSubscriber> gnss_sub_ptr = std::make_shared<GNSSSubscriber>(nh, "kitti/oxts/gps/fix", 1000000);
    std::shared_ptr<TFListener> lidar_to_imu_ptr = std::make_shared<TFListener>(nh, "velo_link", "imu_link");

    //发布当前扫描、局部地图、全局地图、激光里程计、guss里程计话题 
    std::shared_ptr<CloudPublisher> cloud_pub_ptr = std::make_shared<CloudPublisher>(nh, "current_scan", 100, "map");
    std::shared_ptr<OdometryPublisher> odom_pub_ptr = std::make_shared<OdometryPublisher>(nh, "lidar_odom", "map", "lidar", 100);
    
    //定义用来保存点云 imu gnss 当前时刻的信息容器
    std::deque<CloudData> cloud_data_buff;
    std::deque<IMUData> imu_data_buff;
    std::deque<GNSSData> gnss_data_buff;
    //初始化lidar与imu坐标系之间的转化关系
    Eigen::Matrix4f lidar_to_imu = Eigen::Matrix4f::Identity();
    bool transform_received = false;
    bool gnss_origin_positiion_inited = false;

    ros::Rate rate(100); //设置循环频率100hz

    while (ros::ok())
    {
        ros::spinOnce();
        //把新点云、imu、gnss数据保存到cloud_data_buff imu_data_buff gnss_data_buff
        cloud_sub_ptr->ParseData(cloud_data_buff);
        imu_sub_ptr->ParseData(imu_data_buff);
        gnss_sub_ptr->ParseData(gnss_data_buff);
        //导入lidar与imu位姿关系
        if(!transform_received)
        {
            //以旋转矩阵的形式表示出
            if(lidar_to_imu_ptr->LookupData(lidar_to_imu))
            {
                transform_received = true;
            }
        }
        else
        {
            while (cloud_data_buff.size() > 0 && imu_data_buff.size() > 0 && gnss_data_buff.size() > 0)
            {
                //提取第一个数据
                CloudData cloud_data = cloud_data_buff.front();
                IMUData imu_data = imu_data_buff.front();
                GNSSData gnss_data = gnss_data_buff.front();
                //保证点云数据与imu、gnss数据时间同步
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
                    cloud_data_buff.pop_front();
                    imu_data_buff.pop_front();
                    gnss_data_buff.pop_front();

                    Eigen::Matrix4f odometry_matrix;
                    //初始化gnss数据
                    if (!gnss_origin_positiion_inited)
                    {
                        gnss_data.InitOriginPosition();
                        gnss_origin_positiion_inited = true;
                    }
                    //更新gnss数据 转化到imu坐标系下，并发布
                    gnss_data.UpdataXYZ();
                    odometry_matrix(0,3) = gnss_data.local_E;
                    odometry_matrix(1,3) = gnss_data.local_N;
                    odometry_matrix(2,3) = gnss_data.local_U;
                    odometry_matrix.block<3,3>(0,0) = imu_data.GetOrientationMatrix();
                    odometry_matrix *= lidar_to_imu;

                    pcl::transformPointCloud(*cloud_data.cloud_ptr, *cloud_data.cloud_ptr, odometry_matrix);

                    cloud_pub_ptr->Publish(cloud_data.cloud_ptr);
                    odom_pub_ptr->Publish(odometry_matrix);
                }                
            }
        }
        rate.sleep();
    }

    return 0;
}
