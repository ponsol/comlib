#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "../mycom/mycom.h"


static int port = 8444 ;

static const char *TAG = "udpcom";

static char server_ip[128];
static char client_ip[128];
static char bcast_ip[128];

static char hbeat_mesg[] = "HBEAT" ;

static int  remote_alive = 0;
static int  remoteok = 0;

static xQueueHandle _hbeat_queue;

char rx_buffer[128];

static int isserver = 0;

static int server_sock = -1;



int com_hbeat_ms = 4*1000;
int oldrec_time  = 0;
int oldsend_time = 0;



static void udp_server_task(void *pvParameters)
{
    int addr_family = AF_INET;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;
    ip_protocol = IPPROTO_IP;

    while (1) {

     if (strlen(server_ip) > 0 ) {

     struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
     dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
     dest_addr_ip4->sin_family = AF_INET;
     dest_addr_ip4->sin_port = htons(port);
     dest_addr_ip4->sin_addr.s_addr = inet_addr( (char *) server_ip );

      if (server_sock < 0 ) {


         server_sock = socket(addr_family, SOCK_DGRAM, ip_protocol);

        if (server_sock > -1) {
           ESP_LOGI(TAG, "Server socket created");
           int bc = 1;
           if (setsockopt(server_sock, SOL_SOCKET, SO_BROADCAST, &bc, sizeof(bc)) < 0){
               ESP_LOGE(TAG, "Server socket failed to set options: errno %d", errno);
                shutdown(server_sock, 0);
                close(server_sock);
		server_sock = -1;

	   } else {
               int err = bind(server_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
               if (err < 0) {
                   ESP_LOGE(TAG, "Server socket unable to bind: errno %d", errno);
                   shutdown(server_sock, 0);
                   close(server_sock);
		   server_sock = -1;
                }
                else  {
                 ESP_LOGI(TAG, "Sever socket bound server:port %s:%d", server_ip, port);
		}
	   }


        } else { //ifsock

             ESP_LOGE(TAG, "Server socket: unable to create for ip %s:%d errno %d", 
                      server_ip, port, errno);
	     int i ;
             esp_event_post_to(mycom_loop_handle,
             MYCOM_EVENT, MYCOM_EVENT_COMERROR, &i, sizeof(i), portMAX_DELAY);

	}
      } //if previous sock



     }//if ip
      vTaskDelay( 1000 / portTICK_PERIOD_MS );

    }

}


static void udp_server_rx_task(void *pvParameters)
{
    //int addr_family = (int)pvParameters;
    char addr_str[128];
    char mesg[10] ;
    

    while (1) {

        ESP_LOGI(TAG, "Waiting for data");
        struct sockaddr_in6 source_addr; 
        socklen_t socklen = sizeof(source_addr);

        if ( server_sock > -1 ) {

            int len = recvfrom(server_sock, rx_buffer, 
 	               sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            if (len > -1 ) {

                if (source_addr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                } else if (source_addr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                }
	        remote_alive = 1;

                rx_buffer[len] = 0; 
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);


		if ( strncmp(rx_buffer, "HBEAT", 5) != 0 ) {
                  esp_event_post_to(mycom_loop_handle,
                  MYCOM_EVENT, MYCOM_EVENT_DATA, rx_buffer, sizeof(rx_buffer), portMAX_DELAY);
		} else {
                  ESP_LOGI(TAG, "Received hbeat");
		  strncpy(mesg, rx_buffer, 9 );
	          xQueueSend(_hbeat_queue, mesg, portMAX_DELAY);
		}
            }

	}//serversock

      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }

}


void com_set_bcast(char *devip) {
    remote_alive = 1;
    if (strlen(devip) > 0 ) strcpy(bcast_ip, devip);
}


void com_set_client(char *devip) {
    remote_alive = 1;
    if ( strlen(devip) > 0 ) remoteok = 1;
    if (strlen(devip) > 0 ) strcpy(client_ip, devip);
    if (strlen(devip) > 0 ) strcpy(bcast_ip, devip);
}



int com_send_mesg_basic(char *devip, char *mesg, int size) {

   struct sockaddr_in dest_addr;
   dest_addr.sin_family = AF_INET;
   dest_addr.sin_port = htons(port);
   dest_addr.sin_addr.s_addr = inet_addr( (char *) devip );


   int addr_family = AF_INET;

   int ip_protocol = IPPROTO_IP;

   int client_sock = -1;

      if ( client_sock < 0 )  {
         client_sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
         if (client_sock < 0) {
            ESP_LOGE(TAG, "Client socket: unable to create: errno %d", errno);
         } else {
            ESP_LOGI(TAG, "Client socket created for %s:%d", (char *) devip, port);
	 }
      }


      if ( client_sock > -1 ) {
       int err = sendto(client_sock, mesg, size, 0, 
            		(struct sockaddr *)&dest_addr, sizeof(dest_addr));
         if (err < 0) {
            ESP_LOGE(TAG, "Client socket: error occurred during sending: errno %d", errno);
       	    return -1 ;
         } else {
            ESP_LOGE(TAG, "Mesg sent");
	 }
	 shutdown(client_sock,0);
	 close(client_sock);

      } else {
          ESP_LOGW(TAG, "Client socket is not open  %d", errno);
          return -1;
      }

 
 return 0;
}


int com_send_mesg(char *mesg, int size) {

 if ( remote_alive == 0 ) {
   ESP_LOGE(TAG, "client is not alive");
   return -1;
 }

 return com_send_mesg_basic(client_ip, mesg, size);
}


int com_bcast(char *mesg, int size) {
   return com_send_mesg_basic(bcast_ip, mesg, size);
}


int com_send_hbeat() {
   if ( ! remoteok ) return -1;
   if ( (isserver == 1 ) || ( remote_alive == 1 ) ) {
     ESP_LOGW(TAG, "sending hbeat");
     return com_send_mesg_basic(client_ip, hbeat_mesg, strlen(hbeat_mesg) );
   }

return 0;
}



static void hbeat_task(void *arg)
{
int now ;
char mesg[10];

    while (1) {

      memset(mesg,0,10);
      xQueueReceive(_hbeat_queue, &mesg, pdMS_TO_TICKS(com_hbeat_ms) );
      now = esp_timer_get_time()/1000 ;

      if ( remoteok ) {
          if ( oldrec_time == 0 )  oldrec_time = now ;
	  if ( strncmp(mesg, "HBEAT", 5) == 0 ) oldrec_time = now;

           printf("old: %d %d\n", oldrec_time + 3*com_hbeat_ms, now );
          if ( now > (oldrec_time + 3*com_hbeat_ms) ) {
               //hbeat lost
               int i ;
               esp_event_post_to(mycom_loop_handle,
               MYCOM_EVENT, MYCOM_EVENT_HBEATERROR, &i, sizeof(i), portMAX_DELAY);
	       remoteok = 0;
	       if ( isserver != 1 ) remote_alive = 0;
          }

             printf("send: %d %d %d\n", oldsend_time , com_hbeat_ms, now );
          if ( oldsend_time == 0 || ( now >= (oldsend_time + com_hbeat_ms) ) ) {
            //send out a hbeat message
             printf("sending hbeat\n");
             com_send_hbeat();
       	     oldsend_time = now;
          }

      } //remoteok
    } //while
}


int  com_server(int opt) {
 if ( opt == 1 ) isserver = 1;
 if ( opt == 0 ) isserver = 0;
return isserver ;
}


void com_restart() {
  ESP_LOGW(TAG, "RESTARTING");
}


void com_common_main(mycom_dev  *mdev)
{
   port = mdev->port;
  _hbeat_queue = xQueueCreate(5, 10*sizeof( char) );
  strcpy(server_ip, mdev->ip);
  xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 10, NULL);
  xTaskCreate(udp_server_rx_task, "udp_server_rx", 4096, (void*)AF_INET, 10, NULL);
  xTaskCreate(hbeat_task, "hbeat_task", 4096, (void*)AF_INET, 10, NULL);
}
