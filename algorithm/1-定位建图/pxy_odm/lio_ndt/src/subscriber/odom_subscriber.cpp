#include <lio_ndt/subscriber/odom_subscriber.hpp>

namespace lio_ndt
{
    OdometrySubscriber::OdometrySubscriber(ros::NodeHandle &nh, std::string topic_name, int buff_size, std::string output_file) : nh_(nh)
    {
        output_file_.open(output_file);
        if ( !output_file_.is_open() ) 
        {
            ROS_ERROR("Failed to open file for writing odometry data");
            ros::shutdown();
        }else {
            /* 写入csv文件头 */
            output_file_ << "timestamp, x, y, z \n";
            std::cout << " openfile success !!" << std::endl;
        }
        subscriber_ = nh_.subscribe<nav_msgs::Odometry>( topic_name, buff_size, &OdometrySubscriber::msg_callback, this );
    };

    OdometrySubscriber::~OdometrySubscriber() { if ( output_file_.is_open() ) output_file_.close(); }

    void OdometrySubscriber::msg_callback( const nav_msgs::Odometry::ConstPtr & msg  )
    {
        /* 读取数据 */
        double x, y, z, time_stamp;
        x           = msg->pose.pose.position.x;
        y           = msg->pose.pose.position.y;
        z           = msg->pose.pose.position.z;
        time_stamp  = msg->header.stamp.toSec();

        /* 写入数据 */
        output_file_ << time_stamp << ", " << x << ", " << y << ", " << z << "\n";
    }
}