#include <lio_ndt/tf_listener/tf_listener.hpp>

#include <Eigen/Geometry>

namespace lio_ndt
{
    TFListener::TFListener(ros::NodeHandle& nh, std::string base_frame_id, std::string child_frame_id):nh_(nh), base_frame_id_(base_frame_id), child_frame_id_(child_frame_id)
    {}

    bool TFListener::LookupData(Eigen::Matrix4f& transform_matrix)
    {
        try
        {
            tf::StampedTransform transform; // 定义存放变换关系的变量
            // child 相对于 base 的坐标关系，ros::Time(0):查找最近两个时间的坐标，计算坐标关系
            listener_.lookupTransform(base_frame_id_, child_frame_id_ ,ros::Time(0), transform);
            // 姿态关系转化成4x4旋转矩阵
            TransformToMatrix(transform, transform_matrix);
            return true;
        }
        catch(tf::TransformException &ex)
        {
            return false;
        }
    }

    bool TFListener::TransformToMatrix(const tf::StampedTransform& transform, Eigen::Matrix4f& transform_matrix)
    {
        // 定义平移变量
        Eigen::Translation3f tl_btol(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());

        double roll, pitch, yaw;
        tf::Matrix3x3(transform.getRotation()).getEulerYPR(yaw, pitch, roll);
        
        // 定义旋转变量
        Eigen::AngleAxisf rot_x_btol(roll, Eigen::Vector3f::UnitX()); // 轴角表达形式（旋转向量）
        Eigen::AngleAxisf rot_y_btol(pitch, Eigen::Vector3f::UnitY());
        Eigen::AngleAxisf rot_z_btol(yaw, Eigen::Vector3f::UnitZ());

        // 此矩阵为 child_frame_id 到 base_frame_id 的转换矩阵 得到4x4旋转矩阵
        transform_matrix = (tl_btol * rot_z_btol * rot_y_btol * rot_x_btol).matrix();

        return true;
    }
}