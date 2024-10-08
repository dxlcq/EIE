#pragma once

#include <vector>
#include <string>

#include <Geocentric/LocalCartesian.hpp>
using std::vector;
using std::string;

namespace lio_ndt
{
    class GNSSData
    {
    public:
        double time = 0.0;
        double longitude = 0.0;
        double latitude = 0.0;
        double altitude = 0.0;
        // RTK获得的经纬高坐标要转换为东北天坐标，才能用于局部的导航和定位
        // 东北天（ENU）坐标系
        // U轴垂直于过原点的参考椭球面，指向天顶；N与U轴相互垂直，N轴指向为参考椭球短半轴；E轴完成左手系。
        double local_E = 0.0;
        double local_N = 0.0;
        double local_U = 0.0;
        int status = 0;
        int service = 0;
    
    private:
        static GeographicLib::LocalCartesian geo_converter; // 功能是把椭球体下的地理坐标系转为ENU局部系下的坐标
        static bool origin_position_inited;
    
    public:
        void InitOriginPosition();
        void UpdataXYZ();
    };   
    
}