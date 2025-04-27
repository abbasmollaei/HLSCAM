#include "access_memory.h"
#include "bcam.h"
#include "tcam.h"
#include "stcam.h"

#if defined(USE_BCAM) // -------- BCAM --------
static BCAM<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH> bcam;

#if defined(HS_H)

bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, ap_uint<CAM_VALUE_WIDTH> &value, int &index, bool write)
{
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=bcam.entries complete dim=1
#pragma HLS AGGREGATE variable=bcam.entries

	if (write) {
		bcam.dummy_add(key, value);
		index = 0;
	}
	else {
		index = BCAM<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH>::BinaryMatchIndex<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH, 0, CAM_DEPTH - 1>::compute(bcam, key);
	}
	return (index != -1);
}

#else

bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, ap_uint<CAM_VALUE_WIDTH> &value, bool write)
{
#if defined(BF)
#pragma HLS bind_storage variable=bcam.entries type=ram_2p impl=lutram
#pragma HLS ARRAY_PARTITION variable=bcam.entries off

#elif defined(HS)
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=bcam.entries complete dim=1
#pragma HLS AGGREGATE variable=bcam.entries

#elif defined(BL)
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=bcam.entries type=cyclic factor=16 dim=1
#pragma HLS bind_storage variable=bcam.entries type=ram_2p impl=lutram
#pragma HLS AGGREGATE variable=bcam.entries

#elif defined(HASH_KEY)
#pragma HLS PIPELINE
#pragma HLS bind_storage variable=bcam.entries type=ram_2p impl=lutram

#endif

	bool flag = false;
	if (write) {
		bcam.dummy_add(key, value);
		flag = true;
	}
	else {
		flag = bcam.search_entry(key, value);
	}
	return flag;
}

#endif

#elif defined(USE_TCAM)  // -------- TCAM --------
static TCAM<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH> tcam;

#if defined(HS_H)

bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, const ap_uint<CAM_KEY_WIDTH> mask, ap_uint<CAM_VALUE_WIDTH> &value, int &index, bool write)
{
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=tcam.entries complete dim=1
#pragma HLS AGGREGATE variable=tcam.entries

	if (write) {
		tcam.dummy_add(key, mask, value);
		index = 0;
	}
	else {
		index = TCAM<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH>::TernaryMatchIndex<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH, 0, CAM_DEPTH - 1>::compute(tcam, key);
	}
	return (index != -1);
}

#else

bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, const ap_uint<CAM_KEY_WIDTH> mask, ap_uint<CAM_VALUE_WIDTH> &value, bool write)
{
#if defined(BF)
#pragma HLS bind_storage variable=tcam.entries type=ram_2p impl=lutram
#pragma HLS ARRAY_PARTITION variable=tcam.entries off

#elif defined(HS)
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=tcam.entries complete dim=1
#pragma HLS AGGREGATE variable=tcam.entries

#elif defined(BL)
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=tcam.entries type=cyclic factor=16 dim=1
#pragma HLS bind_storage variable=tcam.entries type=ram_2p impl=lutram
#pragma HLS AGGREGATE variable=tcam.entries

#endif

	bool flag = false;
	if (write) {
		tcam.dummy_add(key, mask, value);
		flag = true;
	}
	else {
		flag = tcam.search_entry(key, value);
	}
	return flag;
}

#endif

// -------------------------------------------------------------------------------------------------------------------------

#elif defined(USE_STCAM) // -------- STCAM --------
static STCAM<CAM_KEY_WIDTH, CAM_VALUE_WIDTH, CAM_DEPTH> stcam;

bool access_cam(const ap_uint<CAM_KEY_WIDTH> &key, ap_uint<CAM_VALUE_WIDTH> &value, bool write)
{
#if defined(BF)
#pragma HLS bind_storage variable=stcam.entries type=ram_2p impl=lutram
#pragma HLS ARRAY_PARTITION variable=stcam.entries off

#elif defined(HS)
#pragma HLS PIPELINE
//#pragma HLS bind_storage variable=stcam.entries type=ram_2p impl=lutram
#pragma HLS ARRAY_PARTITION variable=stcam.entries complete dim=1

#elif defined(BL)
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=stcam.entries type=cyclic factor=2 dim=1
#pragma HLS bind_storage variable=stcam.entries type=ram_2p impl=lutram
#pragma HLS AGGREGATE variable=stcam.entries

#endif

	bool flag = false;
	if (write) {
		stcam.dummy_add(key, key.range(7, 0), value);
		flag = true;
	}
	else {
		flag = stcam.search_entry(key, value);
	}
	return flag;
}

#endif
