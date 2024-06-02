#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include "ucom.h"

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define UART_NUM  UART_NUM_2


xQueueHandle ucom_squeue;

static const char *TAG = "UCOM";


char rx_buffer[UCOM_RX_BUF_SIZE+1];
char tx_buffer[UCOM_RX_BUF_SIZE+1];

void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM, UCOM_RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM, data, len);
    printf( "written: %d, val: %s\n", txBytes, data );
    return txBytes;
}



static void tx_task(void *arg)
{
int nb;
    esp_log_level_set(TAG, ESP_LOG_INFO);
    while (1) {
      if ( strlen(tx_buffer) > 0 ) {
        nb = sendData(TAG, tx_buffer);
	if ( nb > 0 ) {
	}
      }
       vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


static void rx_task(void *arg)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    int rxBytes ;
    int ct = 0;

    while (1) {

        rxBytes = uart_read_bytes(UART_NUM, (uint8_t*) rx_buffer, 
		             UCOM_RX_BUF_SIZE, 500 / portTICK_PERIOD_MS);
//		             UCOM_RX_BUF_SIZE, 500 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            rx_buffer[rxBytes] = 0;
            ESP_LOGI(TAG, "****************");
            ESP_LOGI(TAG, "Read %d bytes: '%s'", rxBytes, rx_buffer);
            ESP_LOG_BUFFER_HEXDUMP(TAG, rx_buffer, rxBytes, ESP_LOG_INFO);
	    ct = rxBytes;
	    xQueueSend(ucom_squeue, &ct, portMAX_DELAY);
        } 
    }
}


int com_send_mesg( char *mesg, int size ) {
int st ;

  //int i = strlen(mesg);
 // if ( size < i ) *(mesg+size) = 0;

  st = sendData(TAG, mesg);

return st ;
}


int com_recv_mesg( char *mesg, int size ) {
int n ;
int ct ;

  while ( 1 ) {
    n =-1;
    if ( xQueueReceive(ucom_squeue, &ct, 1000 / portTICK_PERIOD_MS) ) {
       n = UCOM_RX_BUF_SIZE ;
       if ( size < n ) n = size ;
       strncpy(mesg, rx_buffer,n);
    }

    break;
  }

 return n ;
}



void com_main(void)
{
    ucom_squeue = xQueueCreate(1, sizeof( int ) );
    init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, 10, NULL);
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, 9, NULL);
}
