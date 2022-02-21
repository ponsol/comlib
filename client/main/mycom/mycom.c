#include "string.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_event_base.h"

#include "mycom.h" 

static const char* TAG = "mycom";

ESP_EVENT_DEFINE_BASE(MYCOM_EVENT);

esp_event_loop_handle_t mycom_loop_handle;

void mycom_main(mycom_dev *mdev) {


     esp_event_loop_args_t mycom_loop_args = {
        .queue_size = 5,
        .task_name = "mycom_loop_task", // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 4096,
        .task_core_id = tskNO_AFFINITY
    };


    ESP_ERROR_CHECK(esp_event_loop_create(&mycom_loop_args, &mycom_loop_handle) );

    com_set_client(mdev->bcast_ip);
    com_set_bcast(mdev->bcast_ip);
    if ( strlen(mdev->bcast_ip) > 0 ) com_server(1);

    com_main(mdev);
}
