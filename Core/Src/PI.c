/*
 * PI.c
 *
 *  Created on: Dec 16, 2022
 *      Author: Clément CHAPUIS & Aurélien SUTRA
 */


#include "PI.h"

/**
  * @brief  	This function makes the initialization for the PI controller.
  * @param[in]	PIController	The structure for all the PI parameters
  * @retval 	None
*/
void PIController_Init(PIController *pi)
{
	pi->integrator = 0.5;
	pi->prevError  = 0.0;

	pi->out = 0.0;
}

/**
  * @brief  	This function gives the temperature from the BMP module.
  * @param[in]	PIController	The structure for all the PI parameters
  * @param[in]	setpoint		Float which definite the speed we should have
  * @param[in]	measurement		Float which definite the speed we measure
  * @retval 	pi->out			The out parameter for the PI
*/
float PIController_Update(PIController *pi, float setpoint, float measurement)
{
	float error = setpoint - measurement;

	float proportional = pi->Kp * error;

	pi->integrator = pi->integrator + 0.5 * pi->Ki * pi->T * (error + pi->prevError);

	if(pi->integrator > pi->limMaxInt)
	{
		pi->integrator = pi->limMaxInt;
	} else if(pi->integrator < pi->limMinInt)
	{
	pi->integrator = pi->limMinInt;
	}

	pi->out = proportional + pi->integrator;

	if(pi->out > pi->limMax)
	{
		pi->out = pi->limMax;
	} else if (pi->out < pi->limMin)
	{
		pi->out = pi->limMin;
	}

	pi->prevError       = error;

	return pi->out;
}
