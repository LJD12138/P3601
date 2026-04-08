#ifndef USB_PROT_FRAME_H_
#define USB_PROT_FRAME_H_

#include "board_config.h"

#if(boardUSB_EN)
#include "main.h"
// #include "Modbus/modbus_proto.h"


// extern			ModbusProtoTx_t 						*tpUsbProtoTx;
// extern 			ModbusProtoRx_t 						*tpUsbProtoRx;

s8 c_usb_cs_ic1_init(void);
s8 c_usb_cs_ic2_init(void);
s8 c_usb_cs_get_ic1_param(void);
s8 c_usb_cs_get_ic2_param(void);
s8 c_usb_set_pwr_cs(u16 pwr);

bool bUsb_SendProtInit(void);
bool bUsb_RecProtInit(void);

#endif  //boardUSB_EN

#endif  //USB_PROT_FRAME_H_
