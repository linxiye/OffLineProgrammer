#include "usb_lib.h"
#include "drv_log.h"
#include "rtthread.h"
#include "usb_conf.h"
#include "Udisk.h"

const static char *TAG = "UsbApp";

typedef enum USBD_Type_Msg{
    USBD_Udisk_In,
    USBD_Udisk_Out,
    USBD_CDC_In,
    USBD_CDC_Out,
}USBD_Type_Msg_t;

struct usb_msg{
    uint8_t len;
    USBD_Type_Msg_t type;
};

struct UsbApp_Dev {
    struct rt_thread thread;
    uint8_t stack[1024];
    rt_mq_t mq;
    struct usb_msg msg;
};

static struct UsbApp_Dev Dev;
extern uint16_t USB_Rx_Cnt;
extern uint8_t USBD_Endp5_BUFF[64];

void UsbApp_Thread(void *param)
{
    struct usb_msg msg;
    while(1)
    {
        rt_mq_recv(Dev.mq,&msg,sizeof(struct usb_msg),RT_WAITING_FOREVER);
        switch(msg.type)
        {
            case USBD_Udisk_In:
                UDISK_In_EP_Deal();
                break;
            case USBD_Udisk_Out:
                UDISK_Out_EP_Deal(USBD_Endp5_BUFF, USB_Rx_Cnt);
                SetEPRxValid(ENDP5);
                break;
            default :
                break;
        }
    }
}

void Udisk_In_Callback(uint8_t *data,uint32_t len)
{
    Dev.msg.type = USBD_Udisk_In;
    Dev.msg.len = len;
    rt_mq_send(Dev.mq,&Dev.msg,sizeof(struct usb_msg ));
}

void Udisk_Out_Callback(uint8_t *data,uint32_t len)
{
    Dev.msg.type = USBD_Udisk_Out;
    Dev.msg.len = len;
    rt_mq_send(Dev.mq,&Dev.msg,sizeof(struct usb_msg));
}


static struct ch32_usb fun = {
    .Udisk_In_Callback = Udisk_In_Callback,
    .Udisk_Out_Callback = Udisk_Out_Callback
};

int UsbApp_Init(void)
{
    RT_LOGI(TAG,"USBD UDisk Demo");
    RT_LOGI(TAG,"Storage Medium: SPI FLASH");

    Dev.mq = rt_mq_create("usb",sizeof(struct usb_msg),2,RT_IPC_FLAG_FIFO);

    rt_thread_init(&Dev.thread,
                    "USB",
                    UsbApp_Thread,
                    NULL,
                    Dev.stack,
                    sizeof(Dev.stack),
                    11,
                    10);
    rt_thread_startup(&Dev.thread);
     /* USBD init */
    USBD_Callback_Init(&fun);
    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();
    return 0;
}
INIT_APP_EXPORT(UsbApp_Init);
