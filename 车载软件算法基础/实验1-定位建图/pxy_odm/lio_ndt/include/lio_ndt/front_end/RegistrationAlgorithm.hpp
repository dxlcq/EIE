#pragma once

#include <pcl/registration/ndt.h>
#include <pcl/registration/icp.h>

#include "lio_ndt/method/optimized_ICP_GN.h"

class RegistrationAlgorithm
{
public:
    virtual ~RegistrationAlgorithm() = default;
    virtual void SetTransformationEpsilon(double epsilon) = 0;
    virtual void SetMaximumIterations(int iterations) = 0;
    virtual void SetStepSize(double step_size) {}
    virtual void SetResolution(double resolution) {}
    virtual void SetMaxCorrespondDistance(double distance) {}

    virtual void Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                        const Eigen::Matrix4f &predict_pose,
                        pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                        Eigen::Matrix4f &result_pose) = 0;

    virtual void SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) = 0;
    virtual void SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) = 0;

    
};

class NDTRegistration : public RegistrationAlgorithm { 
    public:
    void SetTransformationEpsilon(double epsilon) override;
    void SetMaximumIterations(int iterations) override;
    void SetStepSize(double step_size) override;
    void SetResolution(double resolution) override;
    void SetMaxCorrespondDistance(double distance) override;
    void Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                    const Eigen::Matrix4f &predict_pose,
                    pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                    Eigen::Matrix4f &result_pose) override;
    void SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    void SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    private:
    pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt_;
 };

class ICPRegistration : public RegistrationAlgorithm { 
    public:
    void SetTransformationEpsilon(double epsilon) override;
    void SetMaximumIterations(int iterations) override;
    void SetStepSize(double step_size) override;
    void SetResolution(double resolution) override;
    void SetMaxCorrespondDistance(double distance) override;
    void Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                    const Eigen::Matrix4f &predict_pose,
                    pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                    Eigen::Matrix4f &result_pose) override;
    void SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    void SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    private:
    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp_;
};

class OptimizedICPRegistration : public RegistrationAlgorithm { 
    public:
    void SetTransformationEpsilon(double epsilon) override;
    void SetMaximumIterations(int iterations) override;
    void SetStepSize(double step_size) override;
    void SetResolution(double resolution) override;
    void SetMaxCorrespondDistance(double distance);
    void Align( const pcl::PointCloud<pcl::PointXYZ>::Ptr &source,
                    const Eigen::Matrix4f &predict_pose,
                    pcl::PointCloud<pcl::PointXYZ>:: Ptr &output,
                    Eigen::Matrix4f &result_pose) override;
    void SetInputSource(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    void SetInputTarget(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) override;
    private:
    OptimizedICPGN icp_opti_;
};