/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_hw_config.h"
#include "usb_pwr.h"
#include "usb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
  }
  else
  {
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
  }
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(vu32*)(0x1FFFF7E8);
  Device_Serial1 = *(vu32*)(0x1FFFF7EC);
  Device_Serial2 = *(vu32*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Virtual_Com_Port_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
    Virtual_Com_Port_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
    Virtual_Com_Port_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
    Virtual_Com_Port_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
