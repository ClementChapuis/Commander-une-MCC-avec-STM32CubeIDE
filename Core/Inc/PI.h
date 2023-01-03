/*
 * PI.h
 *
 *  Created on: Dec 16, 2022
 *      Author: cleme
 */

#ifndef INC_PI_H_
#define INC_PI_H_

typedef struct {

	/* Controller gains */
	float Kp;
	float Ki;

	/* Output limits */
	float limMin;
	float limMax;

	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;

} PIController;

void PIController_Init(PIController *);
float PIController_Update(PIController *, float, float);

#endif /* INC_PI_H_ */
