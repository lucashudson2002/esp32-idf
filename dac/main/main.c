#include "driver/dac.h"

void app_main()
{
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_voltage(DAC_CHANNEL_1, 100);
}