#include "stm32f10x.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

#define Uart_Init			Uart1_Init
#define Uart_Printf			Uart1_Printf
#define Uart_Get_Char		Uart1_Get_Char
#define Uart_Get_Pressed	Uart1_Get_Pressed

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(char *pt);
extern void Uart1_Printf(char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);
extern int Uart1_Get_Int_Num(void);
extern void Uart1_Get_String(char *string);

// Led.c

extern void LED_Init(void);
extern void LED_Display(unsigned int num);
extern void LED_All_On(void);
extern void LED_All_Off(void);

// runtime.c

extern void Clock_Init(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern int Key_Wait_Key_Pressed(void);

// Adc.c

extern void Adc_Cds_Init(void);
extern void Adc_IN5_Init(void);
extern void Adc_Start(void);
extern void Adc_Stop(void);
extern int Adc_Get_Status(void);
extern int Adc_Get_Data(void);

// i2c.c

#define SC16IS752_IODIR				0x0A
#define SC16IS752_IOSTATE			0x0B

extern void I2C_SC16IS752_Init(unsigned int freq);
extern void I2C_SC16IS752_Write_Reg(unsigned int addr, unsigned int data);
extern void I2C_SC16IS752_Config_GPIO(unsigned int config);
extern void I2C_SC16IS752_Write_GPIO(unsigned int data);
