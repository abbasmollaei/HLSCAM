#ifndef BCAM_H
#define BCAM_H

#include "config.h"

// BCAM Template Class
template <int KEY_WIDTH, int VALUE_WIDTH, int DEPTH>
class BCAM {
public:
    struct Entry {
        ap_uint<KEY_WIDTH> key;
        ap_uint<VALUE_WIDTH> value;
        bool valid;
    };

    volatile Entry entries[DEPTH];

    // Constructor to initialize entries
    BCAM() {
    }

    // ------------------------------------- HS_H mode ---------------------------------------
    template<int KW, int VW, int D, int START, int END>
    struct BinaryMatchIndex
    {
        static int compute(const BCAM<KW, VW, D> &x, const ap_uint<KW> &key) {
            constexpr int MID = (START + END) / 2;

            int idx_left = BinaryMatchIndex<KW, VW, D, START, MID>::compute(x, key);
            int idx_right = BinaryMatchIndex<KW, VW, D, MID + 1, END>::compute(x, key);

            if (idx_left != -1)
                return idx_left;
            else if (idx_right != -1)
                return idx_right;

            return -1;
        }
    };

    template<int KW, int VW, int D, int INDEX>
    struct BinaryMatchIndex<KW, VW, D, INDEX, INDEX>
    {
        static int compute(const BCAM<KW, VW, D> &x, const ap_uint<KW> &key) {
#pragma HLS INLINE
        	if (x.entries[INDEX].valid && (static_cast<const ap_uint<KW>>(x.entries[INDEX].key) == key))
        		return INDEX;
        	else
        		return -1;
        }
    };
    // ------------------------------------- HS_H mode ---------------------------------------


    void dummy_add(const ap_uint<KEY_WIDTH> &key, ap_uint<VALUE_WIDTH> &value) {
        const int i = key.to_uint();
    	entries[i].key = key;
        entries[i].value = value;
        entries[i].valid = true;
    }

    // Add Entry
    void add_entry(const ap_uint<KEY_WIDTH> &key, const ap_uint<VALUE_WIDTH> &value) {
        #pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
            #pragma HLS UNROLL
            if (!entries[i].valid) {
                entries[i].key = key;
                entries[i].value = value;
                entries[i].valid = true;
                break;
            }
        }
    }

    // Remove Entry
    void remove_entry(ap_uint<KEY_WIDTH> key) {
        #pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
            #pragma HLS UNROLL
            if (entries[i].valid && (entries[i].key == key)) {
                entries[i].valid = false;
                break;
            }
        }
    }

    // Search Entry
    bool search_entry(const ap_uint<KEY_WIDTH> &key, ap_uint<VALUE_WIDTH> &value) {
#if defined(BF)
#pragma HLS INLINE
#else
#pragma HLS PIPELINE
#endif
    	bool flag = false;
        for (int i = 0; i < DEPTH; i++) {
            #pragma HLS UNROLL
            if (entries[i].valid && (ap_uint<KEY_WIDTH>(entries[i].key) == key)) {
                value = entries[i].value;
                flag = true;
            }
        }
        return flag;
    }
};

#endif // BCAM_H
