#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ultraSonic.h"

#define TAG_DIST "Dist"

void app_main() {

    init_ultraSonic(14, 13) ;
    
    while(1) {

        float distance = get_ultraSonic_dist() ;
        ESP_LOGI(TAG_DIST, "Distance : %f" , distance) ;

        vTaskDelay(pdMS_TO_TICKS(200)) ;
    }
}



