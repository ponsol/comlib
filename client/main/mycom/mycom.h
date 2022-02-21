
#ifndef __MYCOM_H
#define __MYCOM_H


#ifdef __cplusplus
extern "C" {
#endif


ESP_EVENT_DECLARE_BASE(MYCOM_EVENT);    


enum {
    MYCOM_EVENT_DATA, 
    MYCOM_EVENT_CONNECTED,
    MYCOM_EVENT_COMERROR,
    MYCOM_EVENT_HBEATERROR,
    MYCOM_EVENT_DISCONNECTED 
};

esp_event_loop_handle_t mycom_loop_handle;

#ifdef __cplusplus
}
#endif



#define RX_BUF_SIZE  128

typedef struct {
    char  type[3]  ;
    int   port ;	
    char  ip[128] ;
    char  bcast_ip[128] ;
    int   server ;
} mycom_dev ;

void mycom_main(mycom_dev *mdev);

void com_set_bcast(char *ip);
void com_set_client(char *ip);
void com_main(mycom_dev *mdev);
void com_restart(void);
int  com_send_mesg(char *mesg , int size);
int  com_recv_mesg(char *mesg, int size , int wait_time );
int  com_bcast(char *mesg , int size);
int  com_server(int opt);


#endif
