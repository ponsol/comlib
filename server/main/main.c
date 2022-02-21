#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "../../lib/mycom/mycom.h"


static char *TAG = "main";

static void mycom_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{

    char *data = (char *) event_data ;

    switch (id) {

       case MYCOM_EVENT_DATA:
           ESP_LOGW(TAG, "mycom event data received ");
	   ESP_LOGI(TAG, "%s", data);
           break;

       case MYCOM_EVENT_CONNECTED:
           ESP_LOGW(TAG, "mycom event connected received ");
           break;

       case MYCOM_EVENT_DISCONNECTED:
           ESP_LOGW(TAG, "mycom event disconnected received ");
           break;

       case MYCOM_EVENT_COMERROR:
           ESP_LOGW(TAG, "mycom event comerror received ");
           break;

       case MYCOM_EVENT_HBEATERROR:
           ESP_LOGW(TAG, "mycom event hbeaterror received ");
           esp_event_post_to(mycom_loop_handle,
           MYCOM_EVENT, MYCOM_EVENT_DISCONNECTED, NULL,0, portMAX_DELAY);
           break;

    }

}


void advert_task(void *arg) {
  char mesg[256];
  mycom_dev *mdev = (mycom_dev *) arg;

  while ( 1 ) {
    sprintf(mesg, "MYDEV %s %d", mdev->ip, mdev->port);
    com_bcast(mesg, strlen(mesg) );
    vTaskDelay( 10000 / portTICK_PERIOD_MS );
  }

}


void app_main(void)
{
char myip[128];


    esp_netif_ip_info_t ip;

    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));
    sprintf(myip, "%d.%d.%d.%d", IP2STR(&ip.ip));

    mycom_dev  mdev = {.type ="udp", .port = 8444 } ;
    strcpy(mdev.ip, myip);
    strcpy(mdev.bcast_ip, "255.255.255.0");

    mycom_main(&mdev);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(mycom_loop_handle, 
			    MYCOM_EVENT,  ESP_EVENT_ANY_ID,
			    mycom_event_handler, mycom_loop_handle, NULL));

    xTaskCreate(advert_task, "advert_task", 1024*4, &mdev, 10, NULL);

}
