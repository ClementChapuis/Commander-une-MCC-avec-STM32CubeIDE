/*
 * Const.h
 *
 *  Created on: 3 janv. 2023
 *      Author: cleme
 */

#ifndef INC_CONST_H_
#define INC_CONST_H_

#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9
// LF = line feed, saut de ligne
#define ASCII_LF 0x0A
// CR = carriage return, retour chariot
#define ASCII_CR 0x0D
// DEL = delete
#define ASCII_DEL 0x7F
#define ARR_VAL 1024
#define ALPHA_MAX 100
#define ADC_BUFFER_SIZE 64
#define VAL_MAX 65535

//	Define for the PI - Current enslavement
#define PI_KP  0.02
#define PI_KI  0.000001

#define PI_LIM_MIN 	0.0
#define PI_LIM_MAX	0.9

#define PI_LIM_MIN_INT 0.0
#define PI_LIM_MAX_INT  0.9

#define SAMPLE_TIME_S 0.000064

#endif /* INC_CONST_H_ */
