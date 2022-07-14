#include "utilities.h"

uint8_t clip(int input) {
    uint8_t result = 0;
    result = input;
    if (input > 255) {
        result = 255;
    } else if (input < 0) {
        result = 0;
    }

    return result;
}