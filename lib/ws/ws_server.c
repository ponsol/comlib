#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include "cJSON.h"


static const char *TAG = "WSSERVER";

#define RX_BUF_SIZE  128

uint64_t com_hbeat_ms;

static char rx_buffer[RX_BUF_SIZE +1] ;

static xQueueHandle _ws_rx_queue;

struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
    int status ;
} ;

typedef struct _status_mesg_ {
  char mesg[129];
  int  len;
} status_mesg_t ;


struct async_resp_arg  rclient ;


static esp_err_t mesg_handler(httpd_req_t *req)
{

   if (req->method == HTTP_GET) {
        ESP_LOGI(TAG, "Handshake done, the new connection was opened");
        return ESP_OK;
   }

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *) rx_buffer;
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 128);
    *(rx_buffer + ws_pkt.len ) = 0;
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
        return ret;
    }
    ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
 
    rclient.hd = req->handle;
    rclient.fd = httpd_req_to_sockfd(req);
    rclient.status = 1;

    if (strncmp(rx_buffer, "_hbeat_", 7) == 0 )  {
        uint64_t th =  esp_timer_get_time()/1000;
        com_hbeat_ms = th;
        ret = httpd_ws_send_frame(req, &ws_pkt);
    } else {
       int ct = 1;
       xQueueSend(_ws_rx_queue, &ct, 0);

       ws_pkt.type = HTTPD_WS_TYPE_TEXT;
       char tbuf[15];
       strcpy(tbuf,"ack"); strncat(tbuf, (char *)ws_pkt.payload,10);
       ws_pkt.payload = (uint8_t *) tbuf;
       ret = httpd_ws_send_frame(req, &ws_pkt);

       if (ret != ESP_OK) {
           ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
       }

    }

    return ret;
}


int com_recv_mesg(char *mesg, int size, int wait_time) {
int ct;

  if ( xQueueReceive(_ws_rx_queue, &ct, wait_time / portTICK_PERIOD_MS) ) {
     int n = size;
     if ( n > 128 )  n =  128;
     strncpy(mesg,rx_buffer,n);
     memset(rx_buffer, 0, 128);
     return n;
  }

  return -1;
}

 
void ws_async_send(void *arg) {

   status_mesg_t *stm = ( status_mesg_t *) arg;

   httpd_ws_frame_t ws_pkt;
   memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
   int n ;
   ws_pkt.payload = (uint8_t*)stm->mesg;
   ws_pkt.len = stm->len;
   ws_pkt.type = HTTPD_WS_TYPE_TEXT;


   httpd_ws_send_frame_async( rclient.hd,  rclient.fd, &ws_pkt);

   printf("async send done %s %d\n",stm->mesg, stm->len);
}


int com_send_mesg(char *mesg, int size) {
   int n ;
   n = strlen(mesg);
   if ( n  > size ) n = size;
   if ( n  > 128 ) n = 128;
   *(mesg+n) = 0;

   status_mesg_t stm;
   strncpy(stm.mesg, mesg,n); stm.len  = n ;
   httpd_queue_work(rclient.hd, ws_async_send, (void *) &stm);
   printf("send mesg recevived %s\n", mesg);
return 0;
}


static const httpd_uri_t ws = {
        .uri        = "/ws",
        .method     = HTTP_GET,
        .handler    = mesg_handler,
        .user_ctx   = NULL,
        .is_websocket = true
};


static httpd_handle_t server = NULL;

static httpd_handle_t start_webserver(void)
{

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 5;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Registering the ws handler
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &ws);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void com_disconnect(void) {
esp_err_t ret ;

  if ( rclient.status ) {
    ret = httpd_sess_trigger_close(rclient.hd, rclient.fd);
    ESP_LOGI(TAG, "Client session disconnected!");
    rclient.status = 0;
  }
  if ( ret ) {}
}

void com_main(void)
{
    _ws_rx_queue = xQueueCreate(1, sizeof(int) );
    server = start_webserver();
    com_hbeat_ms = esp_timer_get_time()/1000;
    ESP_LOGI(TAG, "WS server started");


}
