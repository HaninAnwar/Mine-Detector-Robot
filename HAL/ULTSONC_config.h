/***********************************************************************/
/***********************************************************************/
/************************ AUTHER : Hanin Anwar  ************************/
/************************ LAYER  : HAL       	***********************/
/************************ SWC    : ULTRASNOIC   ************************/
/************************ VERSION : 1.00        ************************/
/***********************************************************************/
/***********************************************************************/

#ifndef ULTSONC_CONFIG_H_
#define ULTSONC_CONFIG_H_


#define ULTSONC_TRIGGER_PORT		DIO_u8PORTD
#define ULTSONC_TRIGGER_PIN			DIO_u8PIN5


/*Check each 0.5ms: 500us/(255*0.128us)*/
#define ULTSONC_CheckingTime 	16


/*Minimum distance between obstacle in cm*/
#define ULTSONC_MinObstacle		30

#endif
