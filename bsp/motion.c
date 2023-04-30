#include "lsm303agr.h"
#include "estimator.h"
#include "motor_control.h"
#include <math.h>
#include "pwm.h"
#include "board.h"
#include "../rtx/cmsis_os2.h"
#include "motion.h"

float referenceHeading =0.0;
float currentHeading = 0.0;
float controlSignal = 0.0;
int error =0;
int lControlSignal=0;
int rControlSignal=0;
float kP = kPbase;

void forward_motion(void *args)
{
    // sensor data placeholder
    float accData[3] = {0.0, 0.0, 0.0};
    float magData[3] = {0.0, 0.0, 0.0};
    float angles[3] = {0.0, 0.0, 0.0};

    // initializing variables for PD control
    int base = 60;
    // float referenceHeading, currentHeading;
    // float controlSignal;
    // int error;
    // float kP = kPbase;
    // int lControlSignal, rControlSignal;

    // reding the actual sensor value into the placeholder data structure
    accReadXYZ(accData);
    magReadXYZ(magData);
    referenceHeading = estimate_heading_z(accData, magData, angles);

    while (1)
    {
        accReadXYZ(accData);
        magReadXYZ(magData);
        currentHeading = estimate_heading_z(accData, magData, angles);

        error = referenceHeading - currentHeading;
        if (error > 250)
        {
            error = error - 360;
        }
        else if (error < -250)
        {
            error = error + 360;
        }

        controlSignal = kP * error;

        rControlSignal = base - 2 + controlSignal;
        lControlSignal = (100 - base) + controlSignal;

        if (rControlSignal < 50)
        {
            rControlSignal = 50;
        }
        else if (rControlSignal > 99)
        {
            rControlSignal = 99;
        }

        if (lControlSignal < 2)
        {
            lControlSignal = 2;
        }
        else if (lControlSignal > 50)
        {
            lControlSignal = 50;
        }

        pwm_out(PWM_CH1, PWM_FREQUENCY, rControlSignal);
        pwm_out(PWM_CH2, PWM_FREQUENCY, lControlSignal);
        osDelay(100);
    }
}

void backward_motion(void *args)
{
    // sensor data placeholder
    float accData[3] = {0.0, 0.0, 0.0};
    float magData[3] = {0.0, 0.0, 0.0};
    float angles[3] = {0.0, 0.0, 0.0};

    // initializing variables for PD control
    int base = 40;
    // float referenceHeading, currentHeading;
    // float controlSignal;
    // int error;
    // float kP = kPbase;
    // int lControlSignal, rControlSignal;

    // reding the actual sensor value into the placeholder data structure
    accReadXYZ(accData);
    magReadXYZ(magData);
    referenceHeading = estimate_heading_z(accData, magData, angles);

    while (1)
    {
        accReadXYZ(accData);
        magReadXYZ(magData);
        currentHeading = estimate_heading_z(accData, magData, angles);

        error = referenceHeading - currentHeading;
        if (error > 250)
        {
            error = error - 360;
        }
        else if (error < -250)
        {
            error = error + 360;
        }

        controlSignal = kP * error;

        rControlSignal = base - 2 + controlSignal;
        lControlSignal = (100 - base) + controlSignal;

        if (rControlSignal < 1)
        {
            rControlSignal = 1;
        }
        else if (rControlSignal > 50)
        {
            rControlSignal = 50;
        }

        if (lControlSignal < 50)
        {
            lControlSignal = 50;
        }
        else if (lControlSignal > 99)
        {
            lControlSignal = 99;
        }

        pwm_out(PWM_CH1, PWM_FREQUENCY, rControlSignal);
        pwm_out(PWM_CH2, PWM_FREQUENCY, lControlSignal);
        osDelay(100);
    }
}

void stop_motion(void)
{
    pwm_out(PWM_CH1, PWM_FREQUENCY, STOPPING_PWM);
    pwm_out(PWM_CH2, PWM_FREQUENCY, STOPPING_PWM);
}

void spot_left(void *arg)
{
    float accData[3] = {0.0, 0.0, 0.0};
    float magData[3] = {0.0, 0.0, 0.0};
    float angles[3] = {0.0, 0.0, 0.0};

    // int base = 60;
    // float referenceHeading, currentHeading;
    // float controlSignal;
    // int error;
    
    float kP = 0.05;
    // int lControlSignal, rControlSignal;

    accReadXYZ(accData);
    magReadXYZ(magData);

    referenceHeading = estimate_heading_z(accData, magData, angles) + 90;

    while (1)
    {
        accReadXYZ(accData);
        magReadXYZ(magData);
        currentHeading = estimate_heading_z(accData, magData, angles);

        error = referenceHeading - currentHeading;
        if (error > 250)
        {
            error = error - 360;
        }
        else if (error < -250)
        {
            error = error + 360;
        }

        controlSignal = 50 + kP * error;

        if (controlSignal < 1)
        {
            controlSignal = 1;
        }
        else if (controlSignal > 99)
        {
            controlSignal = 99;
        }

        pwm_out(1, PWM_FREQUENCY, controlSignal);
        pwm_out(2, PWM_FREQUENCY, controlSignal);
        osDelay(100);
    }
}

void spot_right(void *arg)
{
    float accData[3] = {0.0, 0.0, 0.0};
    float magData[3] = {0.0, 0.0, 0.0};
    float angles[3] = {0.0, 0.0, 0.0};

    // int base = 60;
    // float referenceHeading, currentHeading;
    // float controlSignal;
    // int error;
    
    float kP = 0.05;
    // int lControlSignal, rControlSignal;

    accReadXYZ(accData);
    magReadXYZ(magData);

    referenceHeading = estimate_heading_z(accData, magData, angles) - 90;

    while (1)
    {
        accReadXYZ(accData);
        magReadXYZ(magData);
        currentHeading = estimate_heading_z(accData, magData, angles);

        error = referenceHeading - currentHeading;
        if (error > 250)
        {
            error = error - 360;
        }
        else if (error < -250)
        {
            error = error + 360;
        }

        controlSignal = 50 + kP * error;

        if (controlSignal < 1)
        {
            controlSignal = 1;
        }
        else if (controlSignal > 99)
        {
            controlSignal = 99;
        }

        pwm_out(1, PWM_FREQUENCY, controlSignal);
        pwm_out(2, PWM_FREQUENCY, controlSignal);
        osDelay(100);
    }
}