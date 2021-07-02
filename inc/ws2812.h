#ifndef _LIB_WS2812_H_
#define _LIB_WS2812_H_


#include <mbed.h>

class WS2812
{
public:
    WS2812(int num_leds, PinName pin, NRF_PWM_Type *pwm);
    ~WS2812();

    int init();

    int write(const uint8_t leds[][3]);

private:
    const PinName _pin;
    NRF_PWM_Type *const _pwm;

    const int _num_leds;
    uint16_t *const _seq;

    static void make_byte_seq(uint16_t seq[], uint8_t x);
};


#endif
