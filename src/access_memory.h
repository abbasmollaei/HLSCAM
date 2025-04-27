#ifndef ACCESS_MEMORY_H
#define ACCESS_MEMORY_H

#include "config.h"

#if defined(HS_H)

#if defined(USE_BCAM)
bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, ap_uint<CAM_VALUE_WIDTH> &value, int &index, bool write);

#elif defined(USE_TCAM)
bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, const ap_uint<CAM_KEY_WIDTH> mask, ap_uint<CAM_VALUE_WIDTH> &value, int &index, bool write);

#elif defined(USE_STCAM)
ap_uint<CAM_VALUE_WIDTH> access_cam(const ap_uint<CAM_KEY_WIDTH> &key, bool &valid, int &index, bool write);

#endif

#else

#if defined(USE_BCAM)
bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, ap_uint<CAM_VALUE_WIDTH> &value, bool write);

#elif defined(USE_TCAM)
bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, const ap_uint<CAM_KEY_WIDTH> mask, ap_uint<CAM_VALUE_WIDTH> &value, bool write);

#elif defined(USE_STCAM)
ap_uint<CAM_VALUE_WIDTH> access_cam(const ap_uint<CAM_KEY_WIDTH> &key, bool &valid, bool write);
#endif

#endif

#endif // ACCESS_MEMORY_H
