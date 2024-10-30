#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>

namespace EIE_robot
{
    class Traj_Pub
    {
        public:
            Traj_Pub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size );
            ~Traj_Pub();

            // void PublishTraj();
            void odomCallback( const nav_msgs::Odometry::ConstPtr& odom_msg );

        private:
            ros::NodeHandle nh_;
            ros::Publisher Traj_Pub_;
            ros::Subscriber odom_sub_;

            visualization_msgs::Marker traj_msg_;
    };
}