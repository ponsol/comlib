#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "mycom.h"

#define DMA_CHAN    0
#define PIN_NUM_MOSI 12
#define PIN_NUM_MISO 13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   17
#define GPIO_HANDSHAKE 16


static const char *TAG = "SPICOM";

spi_device_handle_t spidev;

static SemaphoreHandle_t rdySem = NULL;
static xQueueHandle spi_queue;


WORD_ALIGNED_ATTR char spi_sendbuf[128] = {0};
WORD_ALIGNED_ATTR char spi_recvbuf[128] = {0};

int spi_failed = 0;


int spislave_intr=0;

static void IRAM_ATTR gpio_handshake_isr_handler(void* arg)
{
    static uint32_t lasthandshaketime;
    uint32_t currtime=xthal_get_ccount();
    uint32_t diff=currtime-lasthandshaketime;
    //240*1000 1ms
    if (diff<240*1000) return; //ignore everything <1ms after an earlier irq
    lasthandshaketime=currtime;

    spislave_intr++;

    BaseType_t mustYield=false;
    xSemaphoreGiveFromISR(rdySem, &mustYield);
    if (mustYield) portYIELD_FROM_ISR();
}

void spi_pre_cb(spi_transaction_t *trans) {
  vTaskDelay(50 / portTICK_PERIOD_MS);
}



void spi_post_cb(spi_transaction_t *trans) {
//  memset(spi_sendbuf, 0, 128);
}



void spi_send_task(void *arg) {

   esp_err_t ret;
   spi_transaction_t t;
   int ct ;


    while(1) {

     //if (xQueueReceive(spi_queue, &ct, portMAX_DELAY)) {

        memset(&t, 0, sizeof(t));
        memset(spi_recvbuf, 0, 128);
	int n = strlen(spi_sendbuf);
	if ( n > 127 ) n = 127;

	n = 32 ;
        t.length=n*8;
	printf("spi waiting take %s %d\n", spi_sendbuf, spislave_intr);
	sprintf(spi_sendbuf, "%d HHHHHHHHHHHHHHHHHHH", spislave_intr );
        t.tx_buffer=spi_sendbuf;
        t.rx_buffer=spi_recvbuf;

        t.rx_buffer=NULL;

	//wait for slave to be ready
	xSemaphoreTake(rdySem, portMAX_DELAY);
        ret=spi_device_transmit(spidev, &t);
	printf("spi sending done\n" );
	if ( ret == ESP_OK ) {
          printf("Received: %s\n", spi_recvbuf);
	} else {
          spi_failed++;
	  ESP_LOGI(TAG, "send failed"); 
	}

     //} if queue

     vTaskDelay(500 / portTICK_PERIOD_MS);
    }



}


void com_send_mesg( char *mesg, int size ) {

    esp_err_t ret;

    int n = 127;
    if ( size < 127 ) n = size;
    memset(spi_sendbuf, 0, 128);
    strncpy(spi_sendbuf, mesg, n);
    printf("com sending data %s",spi_sendbuf );

    int ct = 1;
    //xQueueSend(spi_queue, &ct, portMAX_DELAY);
    //xSemaphoreGive(rdySem);
}


void com_recv_mesg( char *mesg, int size ) {

}


void com_main(void)
{
    esp_err_t ret;



    ESP_LOGI(TAG, "enter spi setup...");

    spi_queue = xQueueCreate(10, sizeof( int ) );

    rdySem=xSemaphoreCreateBinary();
    //xSemaphoreGive(rdySem);


    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
//	.max_transfer_sz=128*8,
    };

    spi_device_interface_config_t devcfg={
//        .command_bits=0,
//        .address_bits=0,
//        .dummy_bits=0,
        .clock_speed_hz=5*1000*1000,      
        .mode=0,                              
        .spics_io_num=PIN_NUM_CS,              
        .queue_size=1,                          
        .cs_ena_posttrans=10,     
//        .post_cb=spi_post_cb,  
        .pre_cb=NULL,
        .post_cb=NULL,
    };

    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK,  GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS,  GPIO_PULLUP_ONLY);


    gpio_config_t io_conf={
        .intr_type=GPIO_PIN_INTR_POSEDGE,
        .mode=GPIO_MODE_INPUT,
        .pull_up_en=1,
        .pin_bit_mask=(1<<GPIO_HANDSHAKE)
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_HANDSHAKE, GPIO_PIN_INTR_POSEDGE);
    gpio_isr_handler_add(GPIO_HANDSHAKE, gpio_handshake_isr_handler, NULL);
    ESP_LOGI(TAG, "handshake isr init done");


    ret=spi_bus_initialize(HSPI_HOST, &buscfg, DMA_CHAN);
    ESP_LOGI(TAG, "bus init done");
    ESP_ERROR_CHECK(ret);
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spidev);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "device added");

    xTaskCreate(spi_send_task, "spi_send_task", 2048, NULL, 10, NULL);

} 


