#ifndef STCAM_H
#define STCAM_H

#include <ap_int.h>
#include "array_util.h"

// STCAM Template Class
template <int KEY_WIDTH, int VALUE_WIDTH, int DEPTH>
class STCAM {
public:
    struct Entry {
        ap_uint<KEY_WIDTH> key;
        ap_uint<8> mask_shift;
        ap_uint<VALUE_WIDTH> value;
        bool valid;
    };

    volatile Entry entries[DEPTH];

    // Constructor to initialize entries
    STCAM() {
    }

    void dummy_add(const ap_uint<KEY_WIDTH> &key, const ap_uint<8> &mask_shift, const ap_uint<VALUE_WIDTH> &value) {
    	const int i = key.to_uint();
        entries[i].key = key;
        entries[i].mask_shift = mask_shift;
        entries[i].value = value;
        entries[i].valid = true;
    }

    // Add Entry
    void add_entry(const ap_uint<KEY_WIDTH> &key, const ap_uint<8> &mask_shift, const ap_uint<VALUE_WIDTH> &value) {
#pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
#pragma HLS UNROLL
            if (!entries[i].valid) {
                entries[i].key = key;
                entries[i].mask_shift = mask_shift;
                entries[i].value = value;
                entries[i].valid = true;
                break;
            }
        }
    }

    // Remove Entry
    void remove_entry(const ap_uint<KEY_WIDTH> &key, const ap_uint<8> &mask_shift) {
#pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
            #pragma HLS UNROLL
            if (entries[i].valid && ap_uint<KEY_WIDTH>(entries[i].key) == key && ap_uint<8>(entries[i].mask_shift) == mask_shift) {
                entries[i].valid = false;
                break;
            }
        }
    }

    // Search Entry with Longest Prefix Match
    bool search_entry(const ap_uint<KEY_WIDTH> &key, ap_uint<VALUE_WIDTH> &value) {
#if defined(BF)
#pragma HLS INLINE
#else
#pragma HLS PIPELINE
#endif
        ap_uint<8> least_masked = KEY_WIDTH + 1;
        bool found = false;

        ap_uint<8> masked[DEPTH] = {least_masked};
#pragma HLS ARRAY_PARTITION variable=masked complete dim=1

        for (int i = 0; i < DEPTH; i++) {
//#pragma HLS PIPELINE
#pragma HLS UNROLL
            if (entries[i].valid) {
                const ap_uint<KEY_WIDTH> shifted_key = key >> ap_uint<8>(entries[i].mask_shift);
                const ap_uint<KEY_WIDTH> shifted_stored_key = ap_uint<KEY_WIDTH>(entries[i].key) >> ap_uint<8>(entries[i].mask_shift);

                masked[i] = (shifted_key == shifted_stored_key) ? ap_uint<8>(entries[i].mask_shift) : ap_uint<8>(KEY_WIDTH + 1);
            }
        }

        int index = MinIndex<ap_uint<8>, 0, DEPTH - 1>::compute(masked, least_masked);
        if (least_masked < KEY_WIDTH + 1) {
        	value = entries[index].value;
        	found = true;
        }
        return found;
    }

};

#endif // STCAM_H
