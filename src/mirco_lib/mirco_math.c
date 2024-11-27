#include <mirco_lib/mirco_math.h>

int mirco_power_int(int num, int times) {
    int ret = 1;
    if (times != 0)
        ret = mirco_power_int(num, times - 1) * num;
    return ret;
}