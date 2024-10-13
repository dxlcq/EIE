#include <iostream>
#include <thread>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/registration/ndt.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <pcl/visualization/pcl_visualizer.h>

using namespace std::chrono_literals;

int main(int argc, char **argv){
    pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);   // 目标点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr source_cloud(new pcl::PointCloud<pcl::PointXYZ>);   // 源点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr filter_cloud(new pcl::PointCloud<pcl::PointXYZ>);   // 过滤后的点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);   // 输出点云

    // 加载目标点云
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("../data/room_scan1.pcd", *target_cloud) == -1){
        PCL_ERROR("Could not find pcd \n");
        return (-1);
    }
    std::cout << "load " << target_cloud->size() << " data points from target_cloud" << endl;

    // 加载源点云
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("../data/room_scan2.pcd", *source_cloud) == -1){
        PCL_ERROR("Could not find pcd \n");
        return (-1);
    }
    std::cout << "load " << source_cloud->size() << " data points from source_cloud" << endl;

    // 过滤
    pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;          // 近似体素网格滤波器初始化
    approximate_voxel_filter.setLeafSize(0.3, 0.3, 0.3);                        // 每个体素的大小为 0.3 个单位。同一体素内的点将被近似为单个点，从而有效地减少云中的点的数量
    approximate_voxel_filter.setInputCloud(source_cloud);                       // 源点云
    approximate_voxel_filter.filter(*filter_cloud);                             // 过滤后的点云
    std::cout << "Filter cloud contain " << filter_cloud->size() << "data points from source_cloud" << endl;

    // 设置初始条件
    char* end;
    double a[5] = {-1, 0.69, 1, 0, 0};  // 初始条件
    //for(int i=1; i<=4; i++) a[i] = std::strtod(argv[i], &end);

    Eigen::AngleAxisf init_rotation(a[1], Eigen::Vector3f::UnitZ());            // 创建绕 Z 轴旋转的角度
    Eigen::Translation3f init_translasition(a[2], a[3], a[4]);                  // 一个平移向量
    Eigen::Matrix4f init_guss = (init_translasition * init_rotation).matrix();  // 平移和旋转组合成单个变换矩阵

    // =======================   ndt   =======================
    pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;
    
    // 设置 NDT 参数
    ndt.setTransformationEpsilon(0.001);    // 变换精度
    ndt.setStepSize(0.1);                   // 步长
    ndt.setResolution(2);                   // 网格精度
    ndt.setMaximumIterations(50);           // 最大迭代次数

    // 设置输入源点云和目标点云
    ndt.setInputSource(filter_cloud);
    ndt.setInputTarget(target_cloud);

    // 进行配准
    ndt.align(*output_cloud, init_guss);

    // 输出 NDT 结果
    std::cout << "初始参数：" << a[1] << " " << a[2] << " " << a[3] << " " << a[4] << " NDT 匹配得分: " << ndt.getFitnessScore() << " 收敛: " << ndt.hasConverged() << std::endl;
    
    //return 0;
    
    // =======================   ndt   =======================
    
    // pcl 3D 查看器
    pcl::visualization::PCLVisualizer::Ptr viewer_final(new pcl::visualization::PCLVisualizer("3D viewer"));
    viewer_final->setBackgroundColor(0, 0, 0);
    // 添加目标点云
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_color(target_cloud, 255, 0, 0);
    viewer_final->addPointCloud<pcl::PointXYZ>(target_cloud, target_color, "target_cloud");
    viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "target_cloud");
    // 添加输出点云
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> output_color(output_cloud, 0, 255, 0);
    viewer_final->addPointCloud<pcl::PointXYZ>(output_cloud, output_color, "output_cloud");
    viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "output_cloud");
    // 添加一个全局坐标系，单位长度为1.0
    viewer_final->addCoordinateSystem(1.0, "global");
    viewer_final->initCameraParameters();   // 初始化相机参数

    while (!viewer_final->wasStopped()){
        viewer_final->spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}