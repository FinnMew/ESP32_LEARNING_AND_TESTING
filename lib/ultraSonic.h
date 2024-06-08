#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <driver/gpio.h>


void timer_setup(void);
void timer_start(void);
void timer_stop(void);
uint64_t timer_get(void);
void init_ultraSonic(gpio_num_t pinTrig, gpio_num_t pinEcho);
float get_ultraSonic_dist(void);

#ifdef __cplusplus
}
#endif

#endif