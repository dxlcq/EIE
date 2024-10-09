#include <lio_ndt/front_end/RegistrationAlgorithm.hpp>

/*--------------------------    NDT    --------------------------*/
void NDTRegistration::SetMaxCorrespondDistance(double distance) {
    ndt_.setMaxCorrespondenceDistance(distance);
}

void NDTRegistration::SetResolution(double resolution) {
    ndt_.setResolution(resolution);
}

void NDTRegistration::SetStepSize(double step_size) {
    ndt_.setStepSize(step_size);
}

void NDTRegistration::SetMaximumIterations(int iterations) {
    ndt_.setMaximumIterations(iterations);
}

void NDTRegistration::SetTransformationEpsilon(double epsilon) {
    ndt_.setTransformationEpsilon(epsilon);
}

void NDTRegistration::Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                            const Eigen::Matrix4f &predict_pose,
                            pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                            Eigen::Matrix4f &result_pose) {
    ndt_.align(*output, predict_pose);
    result_pose = ndt_.getFinalTransformation();}

void NDTRegistration::SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    ndt_.setInputSource(cloud);
}

void NDTRegistration::SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    ndt_.setInputTarget(cloud);
}

/*--------------------------    NDT    --------------------------*/
/*--------------------------    ICP    --------------------------*/
void ICPRegistration::SetMaxCorrespondDistance(double distance) {
    icp_.setMaxCorrespondenceDistance(distance);
}

void ICPRegistration::SetResolution(double resolution) {
    return;
}

void ICPRegistration::SetStepSize(double step_size) {
    return;
}

void ICPRegistration::SetMaximumIterations(int iterations) {
    icp_.setMaximumIterations(iterations);
}

void ICPRegistration::SetTransformationEpsilon(double epsilon) {
    icp_.setTransformationEpsilon(epsilon);
}

void ICPRegistration::Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                            const Eigen::Matrix4f &predict_pose,
                            pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                            Eigen::Matrix4f &result_pose) {
    icp_.align(*output, predict_pose);
    result_pose = icp_.getFinalTransformation();}

void ICPRegistration::SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    icp_.setInputSource(cloud);
}

void ICPRegistration::SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    icp_.setInputTarget(cloud);
}

/*--------------------------    ICP    --------------------------*/
/*--------------------------    OptimizedICP    --------------------------*/

void OptimizedICPRegistration::SetMaxCorrespondDistance(double distance) {
    icp_opti_.SetMaxCorrespondDistance(distance);
}

void OptimizedICPRegistration::SetResolution(double resolution) {
    return;
}

void OptimizedICPRegistration::SetStepSize(double step_size) {
    return;
}

void OptimizedICPRegistration::SetMaximumIterations(int iterations) {
    icp_opti_.SetMaxIterations(iterations);
}

void OptimizedICPRegistration::SetTransformationEpsilon(double epsilon) {
    icp_opti_.SetTransformationEpsilon(epsilon);
}

void OptimizedICPRegistration::Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                            const Eigen::Matrix4f &predict_pose,
                            pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                            Eigen::Matrix4f &result_pose) {
    icp_opti_.Match(source, predict_pose, output, result_pose);}

void OptimizedICPRegistration::SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    return;
}

void OptimizedICPRegistration::SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    icp_opti_.SetTargetCloud(cloud);
}

