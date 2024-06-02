#include <stdio.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_websocket_client.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "../mycom.h"



static const char *TAG = "WEBSOCKET";

#define WEBSOCKET_URI  "ws://192.168.1.90:80/ws" 

#define RX_BUF_SIZE 128
static char rx_buffer[RX_BUF_SIZE+1] ;

static esp_websocket_client_handle_t wsclient ;

static xQueueHandle _rx_queue;

uint64_t com_hbeat_ms ;

static int restarted = 0;



static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    int i;

    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
	if ( restarted ) {
          esp_event_post_to(mycom_loop_handle, 
  	         MYCOM_EVENT, MYCOM_EVENT_RESTARTED, &i, sizeof(i), portMAX_DELAY);
	  restarted = 0;
	}
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
	ESP_LOGW(TAG,"disconnected event restart triggered");
        esp_event_post_to(mycom_loop_handle, 
  	         MYCOM_EVENT, MYCOM_EVENT_DISCONNECTED, &i, sizeof(i), portMAX_DELAY);
        break;
    case WEBSOCKET_EVENT_DATA:

        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        ESP_LOGW(TAG, "Total payload length=%d, data_len=%d, current payload offset=%d\r\n", 
			data->payload_len, data->data_len, data->payload_offset);

        int n =  data->data_len ;
	if ( n != 0 ) {
          int max7 = 7 > data->data_len ? 7 : data->data_len ;
	  if ( strncmp(data->data_ptr, "_hbeat_", max7) == 0 ) {
            com_hbeat_ms = esp_timer_get_time()/1000;
            ESP_LOGI(TAG, "received _hbeat_");
          } else {
    	      if ( n > 128 ) n = 128 ;
      	      strncpy(rx_buffer, data->data_ptr, n); *(rx_buffer+n)= 0;
              esp_err_t ret = esp_event_post_to(mycom_loop_handle, 
    	         MYCOM_EVENT, MYCOM_EVENT_DATA, rx_buffer, sizeof(rx_buffer), 
		                                  1000 / portTICK_PERIOD_MS);
              if ( ret  != ESP_OK ) {
   	           ESP_LOGE(TAG,"MYCOM_EVENT_DATA cannot be generated");
   	           ESP_LOGE(TAG,"loss of data");
	      }

          }

	}
        break;

    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
        break;
    }
}


static void hbeat_restart_task(void *arg)
{
    while(1) {
      int tdelay=  esp_timer_get_time()/1000 - com_hbeat_ms ;
      ESP_LOGI(TAG, "hbeat_task %d %d", tdelay, restarted);
      if ( tdelay > 10.0*1000 ) {
        ESP_LOGI(TAG, "restarting for hbeat");
        com_restart();
        com_hbeat_ms =  esp_timer_get_time()/1000 ;
      }
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}


static void hbeat_task(void *arg)
{
    com_hbeat_ms = esp_timer_get_time()/1000;

    while(1) {
      int tdelay  = esp_timer_get_time()/1000 - com_hbeat_ms;
      ESP_LOGI(TAG, "sending hbeat tdelay %d %d", tdelay, restarted);
      esp_websocket_client_send_text(wsclient, "_hbeat_", 7, 0); 
      vTaskDelay(2500 / portTICK_PERIOD_MS);
    }
}


/*
static void rx_task(void *arg)
{

int i ;

    while(1) {

       if ( xQueueReceive(_rx_queue, &i, portMAX_DELAY) ) {
         printf("message received %s\n", rx_buffer );
       }

     vTaskDelay(200 / portTICK_RATE_MS);
    }

}
*/


int send_mesg(char *mesg)
{

     if (esp_websocket_client_is_connected(wsclient)) {
	int len = strlen(mesg);
	if ( len > 128 ) len = 128 ;
        esp_websocket_client_send_text(wsclient, mesg, len, 
			 3000 / portTICK_PERIOD_MS);
	return len;
     }

 return -1;
}


static void websocket_app_start(void)
{
    esp_websocket_client_config_t websocket_cfg = {};

    websocket_cfg.uri = WEBSOCKET_URI;

    ESP_LOGI(TAG, "Connecting to %s...", websocket_cfg.uri);

    wsclient = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(wsclient, WEBSOCKET_EVENT_ANY, websocket_event_handler, NULL);
    esp_websocket_client_start(wsclient);

}


int com_send_mesg( char *mesg, int size ) {
int st ;

  st = send_mesg(mesg);

return st ;
}


int com_send_status(char *mesg, int size ) {

   int st = -1;
   //if ( xQueueReceive(_send_status_queue, &ct, portMAX_DELAY) ) {
//      st = send_mesg(mesg);
 //     send_status = 0;
//   }

return st;
}


int com_recv_mesg( char *mesg, int size, int wait_time ) {
int ct ;
int n;
int i;

  n =-1;
  if ( xQueueReceive(_rx_queue, &i, wait_time / portTICK_PERIOD_MS) ) {
     n = RX_BUF_SIZE ;
     if ( size < n ) n = size ;
     strncpy(mesg, rx_buffer,n);
     memset(rx_buffer, 0, 128);
  }


 return n ;
}


void com_restart(void) {
  ESP_LOGW(TAG,"restart entered");
  esp_err_t ret ;

  if ( restarted  ) {

    //ESP_LOGE(TAG,"destroy client and restart");
    //esp_websocket_client_destroy(wsclient);
    //websocket_app_start();
    //ESP_LOGE(TAG,"fresh restart done");
    //restarted = 0;

    ret =  esp_websocket_client_stop(wsclient) ;
    if ( ret != ESP_OK ) ESP_LOGE(TAG,"restart stop failed");
    ret =  esp_websocket_client_start(wsclient);
    if ( ret != ESP_OK ) ESP_LOGE(TAG,"restart start failed");
  }
  ESP_LOGW(TAG,"restart done");
  restarted++;
}



void com_main(void)
{
    ESP_LOGI(TAG, "Websocket client Startup..");
    _rx_queue = xQueueCreate(1, sizeof(int ) );
    websocket_app_start();
    xTaskCreate(hbeat_task, "hbeat_task", 1024*3, NULL, 10, NULL);
    xTaskCreate(hbeat_restart_task, "hbeat_restart_task", 1024*3, NULL, 10, NULL);
}
