#include <lio_ndt/sensor_data/gnss_data.hpp>

#include <glog/logging.h>

// 静态成员变量必须在类外初始化
bool lio_ndt::GNSSData::origin_position_inited = false;
// 经纬度原点初始化
GeographicLib::LocalCartesian lio_ndt::GNSSData::geo_converter;

namespace lio_ndt
{
    void GNSSData::InitOriginPosition()
    {
        // Reset 作用是重置原点，LocalCartesian构造函数是默认在(0,0,0)也就是地心
        geo_converter.Reset(latitude, longitude, altitude);
        origin_position_inited = true;
    }

    void GNSSData::UpdataXYZ()
    {
        if(!origin_position_inited)
        {
            LOG(WARNING) << "GeoConverter has not set origin position";
        }
        // 把经纬度转换为ENU，前三个传入，后三个传出(x,y,z)
        geo_converter.Forward(latitude, longitude, altitude, local_E, local_N, local_U);
    }
}