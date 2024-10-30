#include <ros/ros.h>

#include "ugv_sdk/scout/scout_base.hpp"
#include "base_bringup/EIE_base.hpp"

using namespace EIE_robot;

int main( int argc, char **argv )
{
    std::string device_name;
    int32_t baud_rate = 0;

    if (argc == 2) {
        device_name = {argv[1]};
        std::cout << "Specified CAN: " << device_name << std::endl;
    } else if (argc == 3) {
        device_name = {argv[1]};
        baud_rate = std::stol(argv[2]);
        std::cout << "Specified serial: " << device_name << "@" << baud_rate
                << std::endl;
    } else {
        std::cout << "Usage: app_scout_demo <interface>" << std::endl
                << "Example 1: ./app_scout_demo can0" << std::endl
                << "Example 2: ./app_scout_demo /dev/ttyUSB0 115200" << std::endl;
        return -1;
    }
    
    ros::init( argc, argv, "EIE_base_node" );
    ros::NodeHandle nh;

    // EIE_base_ptr->EnableCommandedMode();     /* 使能控制模式, 已在SendRobotCmd中集成 */
    EIE_base eie_base( device_name, baud_rate, nh );
    try {
        eie_base.SetupSubscription();
    }catch ( const std::exception& e ) {
        std::cerr << "setup Exception caught: " << e.what() << std::endl; 
    }
    
    ros::Rate loop_rate( 50 );
    while ( ros::ok() )
    {
        try {
        eie_base.PublishStatus();
        ros::spinOnce();
        loop_rate.sleep();
        } catch ( std::exception& e ){
            std::cerr << "main Exception caught: " << e.what() << std::endl; 
        }
    }

    return 0;
}