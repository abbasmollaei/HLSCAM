#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

template<typename T, int START, int END>
struct MinIndex
{
    static int compute(const T x[], T& min) {
        constexpr int MID = (START + END) / 2;

        T min_left = min, min_right = min;
        const int min_idx_left = MinIndex<T, START, MID>::compute(x, min_left);
        const int min_idx_right = MinIndex<T, MID + 1, END>::compute(x, min_right);

        if (min_left >= min_right) {
            min = min_right;
            return min_idx_right;
        } else {
        	min = min_right;
            return min_idx_left;
        }
    }
};

template<typename T, int INDEX>
struct MinIndex<T, INDEX, INDEX>
{
    static int compute(const T x[], T& min) {
    	min = x[INDEX];
        return INDEX;
    }
};

#endif // ARRAY_UTIL_H
