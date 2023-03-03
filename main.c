#include "LIB/STD_TYPES.h"
#include <util/delay.h>

#include "MCAL/DIO_interface.h"
#include "MCAL/PORT_interface.h"
#include "MCAL/TIMER_interface.h"
#include "MCAL/GIE_interface.h"
#include "MCAL/EXTI_interface.h"
#include "MCAL/USART_interface.h"

#include "HAL/ULTSONC_interface.h"
#include "HAL/BLTH_interface.h"
#include "HAL/METDECT_interface.h"
#include "HAL/MOTOR_interface.h"


DC_Motor Left_Motor,Right_Motor;
u8 Motor1_u8Speed = 160;
u8 Motor2_u8Speed = 160;

void Motion_voidFowrd(void);
void Motion_voidBack(void);
void Motion_voidLeft(void);
void Motion_voidRight(void);

void main(void)
{
	u8 Local_u8Motion,Local_u8Obstacle,Local_u8Mine;

	/*Motors Initialization*/

	Right_Motor.Port = DIO_u8PORTA;
	Right_Motor.Pin1 = DIO_u8PIN4;
	Right_Motor.Pin2 = DIO_u8PIN6;

	Left_Motor.Port = DIO_u8PORTA;
	Left_Motor.Pin1 = DIO_u8PIN5;
	Left_Motor.Pin2 = DIO_u8PIN7;


	/*Port Initialization*/
	PORT_voidInit();

	/*Initialize timer1B for generate trigger pulses*/
	TIMER1_u8Init(TIMER1_CHANNEL_B,NORMAL,DISCONNECT,TIMER_DIV_BY_1);

	/*Initialize timer1A for echo pulses */
	TIMER1_u8Init(TIMER1_CHANNEL_A,NORMAL,DISCONNECT,TIMER_DIV_BY_1);

	/*Initialize timer0 for PWM1 */
	TIMER0_u8Init(FAST_PWM,NON_INVERTING_PWM,TIMER_DIV_BY_1);

	/*Initialize timer2 for PWM2 */
	TIMER2_u8Init(FAST_PWM,NON_INVERTING_PWM,TIMER_DIV_BY_1);

	/*Initialize bluetooth*/
	USRAT_voidInit();

	/*Initiate Metal Detector*/
	METDECT_voidInitMetalDetector();

	/*Initiate Bluetooth*/
	BLTH_voidInitBluetooth();

	/*Initiate Ultrasonic*/
	ULTSONC_voidInitUltrasnoic();

	/*EXT2 On Change for Metal Detector*/
	EXTI_voidInt2Init();

	GIE_voidEnable();

	while(1)
	{

		Local_u8Motion = BLTH_u8GetBluetoothReading();
		Local_u8Mine = METDECT_u8GetMetalDetector();
		Local_u8Obstacle = ULTSONC_u8GetUltrasnoic();

		/*Start Motion*/
		if(Local_u8Motion == BLTH_MOVE)
		{

			/*There is an obstacle*/
			if (Local_u8Obstacle == ULTSONC_OBSTACLE)
			{
				/*Turn Left*/
				Motion_voidLeft();
			}

			/*There is a mine*/
			else if (Local_u8Mine == METDECT_MINE)
			{
				/*Stop*/
				MOTOR_voidDCStop(&Left_Motor);
				MOTOR_voidDCStop(&Right_Motor);

				/*Send notification*/
				USART_voidSendString("MineDetected");

				break;
			}

			/*No Obstacle or Mines*/
			else
			{
				/*Move Forward*/
				Motion_voidFowrd();
			}
		}

		/*Bluetooth stop*/
		else
		{
			/*Stop*/
			MOTOR_voidDCStop(&Left_Motor);
			MOTOR_voidDCStop(&Right_Motor);
		}

	}
}
void Motion_voidFowrd(void)
{
	MOTOR_u8DCRotate(&Right_Motor,MOTOR_CLK_WISE,PWM_PIN_1,Motor1_u8Speed);
	MOTOR_u8DCRotate(&Left_Motor,MOTOR_CLK_WISE,PWM_PIN_4,Motor2_u8Speed);
}
void Motion_voidBack(void)
{
	MOTOR_u8DCRotate(&Right_Motor,MOTOR_UNTI_CLK_WISE,PWM_PIN_1,Motor1_u8Speed);
	MOTOR_u8DCRotate(&Left_Motor,MOTOR_UNTI_CLK_WISE,PWM_PIN_4,Motor2_u8Speed);
}

void Motion_voidRight(void)
{
	MOTOR_u8DCRotate(&Right_Motor,MOTOR_UNTI_CLK_WISE,PWM_PIN_1,Motor1_u8Speed);
	MOTOR_u8DCRotate(&Left_Motor,MOTOR_CLK_WISE,PWM_PIN_4,Motor2_u8Speed);
}
void Motion_voidLeft(void)
{
	MOTOR_u8DCRotate(&Right_Motor,MOTOR_CLK_WISE,PWM_PIN_1,Motor1_u8Speed);
	MOTOR_u8DCRotate(&Left_Motor,MOTOR_UNTI_CLK_WISE,PWM_PIN_4,Motor2_u8Speed);
}
