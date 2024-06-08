#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <soc/timer_group_reg.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_periph.h>
#include <driver/gptimer.h>
#include <driver/gpio.h>
#include "esp_log.h"

#define pinTrig 14
#define pinEcho 13
#define TAG_DIST "Dist"

void timer_setup(void) ;
void timer_start(void) ;
void timer_stop(void) ;
uint64_t timer_get(void) ;

float distance ;
float echoTime ;
uint64_t counter ;
uint32_t start_value_low = 0;  
uint32_t start_value_high = 0; 


void app_main() {

    gpio_set_direction(pinTrig, GPIO_MODE_OUTPUT) ;
    gpio_set_direction(pinEcho, GPIO_MODE_INPUT) ;

    timer_setup() ;
    
    while(1) {

        gpio_set_level(pinTrig, 1) ;
        vTaskDelay(pdMS_TO_TICKS(10)) ;
        gpio_set_level(pinTrig, 0) ;

        while(!gpio_get_level(pinEcho)) ;

        timer_start() ;

        while(gpio_get_level(pinEcho)) ;

        timer_stop() ;
        counter = timer_get() ;

        echoTime = (((float)counter) * (0.000000025));
        distance = (echoTime * 343.0) / 2 * 100.0 ;
        ESP_LOGI(TAG_DIST, "Distance : %f" , distance) ;

        vTaskDelay(pdMS_TO_TICKS(200)) ;
    }
}

void timer_setup(void) {
    REG_CLR_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_USE_XTAL) ;      
    REG_SET_FIELD(TIMG_T0CONFIG_REG(0), TIMG_T0_DIVIDER, 2) ;
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_INCREASE) ;
    REG_WRITE(TIMG_T0LOADLO_REG(0), start_value_low);
    REG_WRITE(TIMG_T0LOADHI_REG(0), start_value_high);
    REG_WRITE(TIMG_T0LOAD_REG(0), 0); 
}

void timer_start(void) {
    REG_WRITE(TIMG_T0LOADLO_REG(0), start_value_low);
    REG_WRITE(TIMG_T0LOADHI_REG(0), start_value_high);
    REG_WRITE(TIMG_T0LOAD_REG(0), 0); 
    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);
}

void timer_stop(void) {
    REG_CLR_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);
}

uint64_t timer_get(void) {
    REG_WRITE(TIMG_T0UPDATE_REG(0), 1) ;
    while (REG_READ(TIMG_T0UPDATE_REG(0)) != 0) ;
    uint32_t timer_value_low = REG_READ(TIMG_T0LO_REG(0));
    uint32_t timer_value_high = REG_READ(TIMG_T0HI_REG(0));
    uint64_t timer_value = ((uint64_t)timer_value_high << 32) | timer_value_low;
    return timer_value ;
}

