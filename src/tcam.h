#ifndef TCAM_H
#define TCAM_H

#include <ap_int.h>

// TCAM Template Class
template <int KEY_WIDTH, int VALUE_WIDTH, int DEPTH>
class TCAM {
public:
    struct Entry {
        ap_uint<KEY_WIDTH> key;
        ap_uint<KEY_WIDTH> mask;  // Mask for ternary matching
        ap_uint<VALUE_WIDTH> value;
        bool valid;
    };

    volatile Entry entries[DEPTH];

    // Constructor to initialize entries
    TCAM() {
    }


    // ------------------------------------- HS_H mode ---------------------------------------
    template<int KW, int VW, int D, int START, int END>
    struct TernaryMatchIndex
    {
        static int compute(const TCAM<KW, VW, D> &x, const ap_uint<KW> &key) {
            constexpr int MID = (START + END) / 2;

            int idx_left = TernaryMatchIndex<KW, VW, D, START, MID>::compute(x, key);
            int idx_right = TernaryMatchIndex<KW, VW, D, MID + 1, END>::compute(x, key);

            if (idx_left != -1)
                return idx_left;
            else if (idx_right != -1)
                return idx_right;

            return -1;
        }
    };

    template<int KW, int VW, int D, int INDEX>
    struct TernaryMatchIndex<KW, VW, D, INDEX, INDEX>
    {
        static int compute(const TCAM<KW, VW, D> &x, const ap_uint<KW> &key) {
#pragma HLS INLINE
        	if (x.entries[INDEX].valid && !((key ^ static_cast<const ap_uint<KW>>(x.entries[INDEX].key)) & static_cast<const ap_uint<KW>>(x.entries[INDEX].mask)))
        		return INDEX;
        	else
        		return -1;
        }
    };
    // ------------------------------------- HS_H mode ---------------------------------------


    void dummy_add(const ap_uint<KEY_WIDTH> &key, const ap_uint<KEY_WIDTH> &mask, ap_uint<VALUE_WIDTH> &value) {
#pragma HLS PIPELINE
        const int i = key.to_uint();
        entries[i].key = key;
        entries[i].mask = mask;
        entries[i].value = value;
        entries[i].valid = true;
    }

    // Add Entry
    void add_entry(const ap_uint<KEY_WIDTH> &key, const ap_uint<KEY_WIDTH> &mask, const ap_uint<VALUE_WIDTH> &value) {
#pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
#pragma HLS UNROLL
            if (!entries[i].valid) {
                entries[i].key = key;
                entries[i].mask = mask;
                entries[i].value = value;
                entries[i].valid = true;
                break;
            }
        }
    }

    // Remove Entry
    void remove_entry(const ap_uint<KEY_WIDTH> &key, const ap_uint<KEY_WIDTH> &mask) {
#pragma HLS INLINE
        for (int i = 0; i < DEPTH; i++) {
            #pragma HLS UNROLL
            if (entries[i].valid && ap_uint<KEY_WIDTH>(entries[i].key) == key && ap_uint<KEY_WIDTH>(entries[i].mask) == mask) {
                entries[i].valid = false;
                break;
            }
        }
    }

    // Search Entry
    bool search_entry(const ap_uint<KEY_WIDTH> &key, ap_uint<VALUE_WIDTH> &value) {
#if defined(BASELINE)
#pragma HLS INLINE
#else
#pragma HLS PIPELINE
#endif
    	bool flag = false;
    	bool masked[DEPTH];
#pragma HLS ARRAY_PARTITION variable=masked complete dim=1

    	for (int i = 0; i < DEPTH; i++) {
#pragma HLS UNROLL
    		const ap_uint<KEY_WIDTH> entry_key(entries[i].key);
    		const ap_uint<KEY_WIDTH> entry_mask(entries[i].mask);
    		masked[i] = (entries[i].valid && !((key ^ entry_key) & entry_mask)) ? true : false;
    	}
    	for (int i = 0; i < DEPTH; i++) {
#pragma HLS UNROLL
    		if (masked[i]) {
    			value = entries[i].value;
    			flag = true;
    		}
    	}
    	return flag;
    }
};


#endif // TCAM_H
