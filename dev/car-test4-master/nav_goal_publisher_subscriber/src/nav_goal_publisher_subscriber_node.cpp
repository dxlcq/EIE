#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/String.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

std::string recognizedText;  // 存储识别结果的全局变量

// 回调函数，处理识别结果
void textCallback(const std_msgs::String::ConstPtr& msg)
{
  recognizedText = msg->data;
  ROS_INFO("/text_recognition/result: %s", recognizedText.c_str());
}

// 发布导航目标点并执行导航
void publishAndNavigateGoal(ros::Publisher& pub, MoveBaseClient& ac, double x, double y)
{
    // 创建一个PoseStamped消息，用于发送导航目标点信息
    geometry_msgs::PoseStamped goal_msg;

    // 设置导航目标点的header信息
    goal_msg.header.seq = 0;
    goal_msg.header.stamp = ros::Time::now();
    goal_msg.header.frame_id = "map";

    // 设置导航目标点的位置信息
    goal_msg.pose.position.x = x;
    goal_msg.pose.position.y = y;
    goal_msg.pose.position.z = 0.0;

    // 设置导航目标点的朝向信息
    goal_msg.pose.orientation.x = 0.0;
    goal_msg.pose.orientation.y = 0.0;
    goal_msg.pose.orientation.z = 0.0;
    goal_msg.pose.orientation.w = 1.0;

    // 发布导航目标点消息
    ROS_INFO("Publish navigation target point message");
    pub.publish(goal_msg);

    // 创建一个导航目标点消息
    move_base_msgs::MoveBaseGoal goal;

    // 设置导航目标点的位置信息
    goal.target_pose = goal_msg;

    // 发送导航目标点消息
    ROS_INFO("Send navigation target point message");
    ac.sendGoal(goal);

    // 等待导航完成
    ac.waitForResult();

    // 输出导航结果
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
        ROS_INFO("success");
    } else {
        ROS_WARN("failed");
    }
}

int main(int argc, char** argv) {
    // 初始化ROS节点
    ros::init(argc, argv, "nav_goal_publisher_subscriber");

    // 创建一个NodeHandle
    ros::NodeHandle nh;

    // 创建一个Publisher，将导航目标点消息发布到"/move_base_simple/goal"话题上
    ros::Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 10);

    // 创建一个导航行为客户端
    MoveBaseClient ac("move_base", true);

    // 等待move_base服务器启动
    ROS_INFO("waiting for move_base start");
    ac.waitForServer();

    // 创建一个Subscriber，订阅识别结果话题
    ros::Subscriber text_subscriber = nh.subscribe("/text_recognition/result", 10, textCallback);

    while (ros::ok()) {
        // 等待识别结果
        ROS_INFO("Waiting for recognition result...");

	// 处理回调函数
        ros::spinOnce();

        // 判断识别结果是否为"ABCD"中的某个值
        if (recognizedText == "A") {
ROS_INFO("text_recognition/result:",recognizedText);
            // 设置坐标为A对应的值
            double x = 1.68;
            double y = 8.98;

            // 发布导航目标点并执行导航
            publishAndNavigateGoal(pub, ac, x, y);
        } else if (recognizedText == "B") {
ROS_INFO("text_recognition/result:",recognizedText);
            // 设置坐标为B对应的值
            double x = -0.13;
            double y = 10.70;

            // 发布导航目标点并执行导航
            publishAndNavigateGoal(pub, ac, x, y);
        } else if (recognizedText == "C") {
ROS_INFO("text_recognition/result:",recognizedText);
            // 设置坐标为C对应的值
            double x = -0.50;
            double y = 6.94;

            // 发布导航目标点并执行导航
            publishAndNavigateGoal(pub, ac, x, y);
        } else if (recognizedText == "D") {
ROS_INFO("text_recognition/result:",recognizedText);
            // 设置坐标为D对应的值
	    double x = -3.94;
            double y = 7.68;

            // 发布导航目标点并执行导航
            publishAndNavigateGoal(pub, ac, x, y);
        }


        // 清空识别结果
        recognizedText = "";

        // 休眠一段时间
        ros::Duration(1.0).sleep();
    }

    return 0;
}
