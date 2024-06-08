#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <soc/timer_group_reg.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_periph.h>
#include <driver/gptimer.h>
#include <driver/gpio.h>
#include "esp_log.h"

#define pinLED 6
#define TAG "MAIN"

static void IRAM_ATTR timer_isr(void* arg) {

    REG_WRITE(TIMG_INT_CLR_TIMERS_REG(0), TIMG_T0_INT_CLR) ;

    static bool led_on = false;
    led_on = !led_on;
    gpio_set_level(pinLED, led_on);

    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_ALARM_EN) ;
}

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

    uint32_t start_alarm_low = 1220;  
    uint32_t start_alarm_high = 0; 
    REG_WRITE(TIMG_T0ALARMLO_REG(0), start_alarm_low) ;
    REG_WRITE(TIMG_T0ALARMHI_REG(0), start_alarm_high) ;
    REG_SET_BIT(TIMG_INT_ENA_TIMERS_REG(0), TIMG_T0_INT_ENA) ;
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_AUTORELOAD);

    uint32_t reload_value_low = 0;  
    uint32_t reload_value_high = 0; 
    REG_WRITE(TIMG_T0LOADLO_REG(0), reload_value_low) ;
    REG_WRITE(TIMG_T0LOADHI_REG(0), reload_value_high) ;
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_ALARM_EN);

    esp_intr_alloc(ETS_TG0_T0_LEVEL_INTR_SOURCE, 0, timer_isr, NULL, NULL) ;


    while(1) {
          
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


