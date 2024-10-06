#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/registration/icp.h>
#include <Eigen/Dense>

#include "optimized_ICP_GN.h"

using namespace std;

int main(int argc, char **argv){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_target_ptr(new pcl::PointCloud<pcl::PointXYZ>);                   // 目标点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source_ptr(new pcl::PointCloud<pcl::PointXYZ>);                   // 源点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source_opti_transformed_ptr(new pcl::PointCloud<pcl::PointXYZ>);  // 高斯牛顿优化的
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source_svd_transformed_ptr(new pcl::PointCloud<pcl::PointXYZ>);   // SVD优化的

    pcl::io::loadPCDFile("../data/room_scan2.pcd", *cloud_target_ptr);
    pcl::io::loadPCDFile("../data/room_scan1.pcd", *cloud_source_ptr);

    // 初始化变化矩阵
    Eigen::Matrix4f T_predict, T_final;
    T_predict.setIdentity();

    double a[13];
    char* end;
    for(int i=1; i<=12; i++)
        a[i] = std::strtod(argv[i], &end);

    T_predict << a[1], a[2], a[3], a[4],
                 a[5], a[6], a[7], a[8],
                 a[9], a[10],a[11],a[12],
                 0, 0, 0, 1;
    /*
    T_predict << 0.765, 0.643, -0.027, -1.472,
                  -0.6, 0.765, -0.023, 1.3,
                 0.006, 0.035, 0.999, -0.1,
                0, 0, 0, 1;
    */

    // =======================   optimized icp   =======================
    OptimizedICPGN icp_opt;
    icp_opt.SetTargetCloud(cloud_target_ptr);
    icp_opt.SetMaxIterations(30);               // 最大迭代
    icp_opt.SetMaxCorrespondDistance(0.3);      // 最大对应距离
    icp_opt.SetTransformationEpsilon(1e-4);     // 收敛阈值
    bool result = icp_opt.Match(cloud_source_ptr, T_predict, cloud_source_opti_transformed_ptr, T_final);
    //std::cout << "OPT ICP 分数: "<< icp_opt.GetFitnessScore(0.3) << " 收敛: " << icp_opt.HasConverged() << std::endl;
    // =======================   optimized icp   =======================

    // =======================   svd icp   =======================
    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp_svd;           // 定义一个 ICP 对象
    icp_svd.setInputTarget(cloud_target_ptr);
    icp_svd.setInputSource(cloud_source_ptr);
    icp_svd.setMaxCorrespondenceDistance(0.3);                                  // 最大对应距离
    icp_svd.setMaximumIterations(30);                                           // 迭代次数
    icp_svd.setEuclideanFitnessEpsilon(1e-4);                                   // 欧几里得误差收敛阈值
    icp_svd.setTransformationEpsilon(1e-4);                                     // 变换收敛阈值
    icp_svd.align(*cloud_source_svd_transformed_ptr, T_predict);
    //std::cout << "SVD ICP 分数: " << icp_svd.getFitnessScore() << std::endl;
    // =======================   svd icp   =======================

    std::cout << "初始参数:" << std::endl;
    for(int i=1; i<=12; i++){
        cout << a[i] << " ";
        if(i%4 == 0) cout << std::endl;
    }

    std::cout << "OPT ICP 分数: "<< icp_opt.GetFitnessScore(0.3) << " 收敛: " << icp_opt.HasConverged() << std::endl;
    std::cout << "SVD ICP 分数: " << icp_svd.getFitnessScore() << std::endl;    

    return 0;

    // 可视化
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("viewer"));
    viewer->initCameraParameters();

    int v1(0);
    int v2(1);
    viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
    viewer->setBackgroundColor(0, 0, 0, v1);
    viewer->addText("Optimized ICP", 10, 10, "optimized icp", v1);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> source_opti_color(
        cloud_source_opti_transformed_ptr, 255, 0, 0);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_source_opti_transformed_ptr, source_opti_color, "source opti cloud", v1);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_color_0(cloud_target_ptr, 0, 0, 255);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_target_ptr, target_color_0, "target cloud1", v1);

    viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
    viewer->setBackgroundColor(0.0, 0.0, 0.0, v2);
    viewer->addText("SVD ICP", 10, 10, "svd icp", v2);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_color_1(cloud_target_ptr, 0, 0, 255);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_target_ptr, target_color_1, "target cloud2", v2);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> source_svd_color(cloud_source_svd_transformed_ptr,
                                                                                     0, 255, 0);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_source_svd_transformed_ptr, source_svd_color, "source svd cloud", v2);

    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "source opti cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "source svd cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "target cloud1");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "target cloud2");
    viewer->addCoordinateSystem(1.0);

    viewer->setCameraPosition(0, 0, 20, 0, 10, 10, v1);
    viewer->setCameraPosition(0, 0, 20, 0, 10, 10, v2);

    viewer->spin();

    return 0;
}