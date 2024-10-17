#include "fusion/my_config.hpp"

my_config::my_config(const std::string &file_path){
    if(file_path.empty()){
        this->file_path = "/home/agilex/dxl/EIE/dev/2-task/config.yaml";
    } else {
        this->file_path = file_path;
    }
    ROS_INFO("YES! Config init!");
}

my_config::~my_config(){}

void my_config::read_config(){
    cv::FileStorage fs(this->file_path, cv::FileStorage::READ);
    if(!fs.isOpened()){
        ROS_INFO("NO! Config file open failed!");
        return ;
    }
    // 读取配置文件中的参数
    fs["CameraExtrinsicMat"] >> this->extrinsic_mat;    // 相机外参
    fs["CameraMat"] >> this->camera_mat;                // 相机内参
    fs["DistCoeff"] >> this->dist_coeff;                // 畸变系数
    fs.release();
    ROS_INFO("YES! Config readed!");

    // 创建旋转矩阵
    this->rotate_mat = cv::Mat(3, 3, cv::DataType<double>::type);   // 旋转矩阵
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            this->rotate_mat.at<double>(i, j) = this->extrinsic_mat.at<double>(i, j);
        }
    }
    // 创建平移矩阵
    this->translate_mat = cv::Mat(3, 1, cv::DataType<double>::type); // 平移矩阵
    this->translate_mat.at<double>(0) = this->extrinsic_mat.at<double>(1, 3);
    this->translate_mat.at<double>(1) = this->extrinsic_mat.at<double>(2, 3);
    this->translate_mat.at<double>(2) = this->extrinsic_mat.at<double>(0, 3);

    // 创建变换矩阵
    for(int i=0; i<=3; i++)
        for(int j=0; j<=3; j++)
            this->transform_mat(i, j) = this->extrinsic_mat.at<double>(i, j);
    
    // 创建逆变换矩阵
    this->inv_transform_mat = this->transform_mat.inverse();
}