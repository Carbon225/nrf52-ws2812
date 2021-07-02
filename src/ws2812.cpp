#include <ws2812.h>

#define WS_PWM_COUNTERTOP (20)
#define WS_PWM_ONE_HIGH (13 | (1 << 15))
#define WS_PWM_ZERO_HIGH (6 | (1 << 15))
#define WS_SEQ_TERMINATOR (1 << 15)

#define SEQ_SIZE(num_leds) (8 * 3 * num_leds + 1)


WS2812::WS2812(int num_leds, PinName pin, NRF_PWM_Type *pwm)
    : _pin(pin)
    , _pwm(pwm)
    , _num_leds(num_leds)
    , _seq(new uint16_t[SEQ_SIZE(num_leds)])
{

}

WS2812::~WS2812()
{
    delete [] _seq;
}

int WS2812::init()
{
    NRF_GPIO->OUTCLR = (GPIO_OUTCLR_PIN0_Clear << (GPIO_OUTCLR_PIN0_Pos + _pin));
    NRF_GPIO->DIRSET = (GPIO_DIRSET_PIN0_Set << (GPIO_DIRSET_PIN0_Pos + _pin));

    _pwm->PSEL.OUT[0] = (_pin << PWM_PSEL_OUT_PIN_Pos) |
                        (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

    _pwm->ENABLE = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);

    _pwm->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);

    _pwm->PRESCALER = (PWM_PRESCALER_PRESCALER_DIV_1 << PWM_PRESCALER_PRESCALER_Pos);

    _pwm->COUNTERTOP = ((WS_PWM_COUNTERTOP) << PWM_COUNTERTOP_COUNTERTOP_Pos);

    _pwm->LOOP = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);

    _pwm->DECODER = (PWM_DECODER_LOAD_Common << PWM_DECODER_LOAD_Pos) |
                    (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);

    _pwm->SEQ[0].PTR = ((uint32_t)(_seq) << PWM_SEQ_PTR_PTR_Pos);
    _pwm->SEQ[0].CNT = (SEQ_SIZE(_num_leds) << PWM_SEQ_CNT_CNT_Pos);

    _pwm->SEQ[0].REFRESH = 0;
    _pwm->SEQ[0].ENDDELAY = 0;

    return 0;
}

int WS2812::write(const uint8_t leds[][3])
{
    const uint8_t *data = (const uint8_t*) leds;

    for (int i = 0; i < 3 * _num_leds; ++i)
    {
        make_byte_seq(&_seq[i * 8], data[i]);
    }

    _pwm->TASKS_SEQSTART[0] = 1;

    return 0;
}

void WS2812::make_byte_seq(uint16_t seq[], uint8_t x)
{
    for (int i = 7; i >= 0; --i)
    {
        if (x & 1)
            seq[i] = WS_PWM_ONE_HIGH;
        else
            seq[i] = WS_PWM_ZERO_HIGH;
        x >>= 1;
    }
    seq[8] = WS_SEQ_TERMINATOR;
}
