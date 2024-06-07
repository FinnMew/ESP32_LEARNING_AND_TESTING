#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <soc/timer_group_reg.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_periph.h>
#include <driver/gptimer.h>
#include <driver/gpio.h>

#define pinLED 6

void app_main() {

    gpio_set_direction(pinLED, GPIO_MODE_OUTPUT) ;

    REG_CLR_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_USE_XTAL) ;      
    REG_SET_FIELD(TIMG_T0CONFIG_REG(0), TIMG_T0_DIVIDER, 65535) ;
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_INCREASE) ;
    uint32_t start_value_low = 0;  
    uint32_t start_value_high = 0; 
    REG_WRITE(TIMG_T0LOADLO_REG(0), start_value_low);
    REG_WRITE(TIMG_T0LOADHI_REG(0), start_value_high);
    REG_WRITE(TIMG_T0LOAD_REG(0), 0); 
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);

    while(1) {

        REG_WRITE(TIMG_T0UPDATE_REG(0), 1) ;
        while (REG_READ(TIMG_T0UPDATE_REG(0)) != 0) ;

        uint32_t timer_value_low = REG_READ(TIMG_T0LO_REG(0));
        uint32_t timer_value_high = REG_READ(TIMG_T0HI_REG(0));
        uint64_t timer_value = ((uint64_t)timer_value_high << 32) | timer_value_low;
  

    if(timer_value >= 1220) {
            gpio_set_level(pinLED, 1) ;
    if(timer_value >= 2441) {
        gpio_set_level(pinLED, 0) ;
        REG_CLR_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);
        REG_WRITE(TIMG_T0LOADLO_REG(0), start_value_low);
        REG_WRITE(TIMG_T0LOADHI_REG(0), start_value_high);
        REG_WRITE(TIMG_T0LOAD_REG(0), 0); 
        REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);

    }
    }

    


    }
}


