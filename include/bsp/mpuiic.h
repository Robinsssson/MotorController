#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "system/sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
// #define SOFT_IIC_SCL_PIN GPIO_Pin_13
// #define SOFT_IIC_SDA_PIN GPIO_Pin_15
// #define SOFT_IIC_GPIO_PORT GPIOF
// #define SOFT_IIC_GPIO_CLK RCC_APB2Periph_GPIOF
//IO��������
#define MPU_SDA_IN()  {GPIOF->CRH&=0X0FFFFFFF;GPIOF->CRH|=0X80000000;}
#define MPU_SDA_OUT() {GPIOF->CRH&=0X0FFFFFFF;GPIOF->CRH|=0X30000000;}

//IO��������	 
#define MPU_IIC_SCL    PFout(13) 		//SCL
#define MPU_IIC_SDA    PFout(15) 		//SDA	 
#define MPU_READ_SDA   PFin(15) 		//����SDA 


//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















