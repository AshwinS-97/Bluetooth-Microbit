#include <stdint.h>
#include <string.h>
#include "board.h"
#include "pwm.h"
#include "lib.h"
#include "motor_control.h"


uint8_t lDutyCycle, rDutyCycle;
int frequency = 345;
// int lFrequency = 345;
// int rFrequency = 350;

void move(char direction, int speed)
{
    switch (direction)
    {
    case 'f':
        // rDutyCycle = 50 - speed/2;
        // lDutyCycle = 100 - rDutyCycle;
        rDutyCycle = 50 - speed/2;
        lDutyCycle = 50 + speed/2;
        break;

    case 'l':
        rDutyCycle = 100 - (50 - speed/2);
        lDutyCycle = rDutyCycle;
        break;

    case 'b':
        lDutyCycle = 50 - speed/2;
        rDutyCycle = 100 - lDutyCycle;
        break;

    case 'r':
        rDutyCycle = 50 - speed/2;
        lDutyCycle = rDutyCycle;
        break;
    
    case 's':
        lDutyCycle = 50;
        rDutyCycle = 50;
        break;

    default:
        lDutyCycle = 50;
        rDutyCycle = 50;
        break;
    }
    
    printf("freq = %d, direction: %c, speed = %d, lDutyCycle = %d, rDutyCycle = %d\n\r", frequency, direction, speed, lDutyCycle, rDutyCycle);
    pwm_out(1, frequency, lDutyCycle);
    pwm_out(2, frequency, rDutyCycle);


    // printf("lFreq = %d, rFreq = %d, direction: %c, speed = %d, lDutyCycle = %d, rDutyCycle = %d\n\r", lFrequency, rFrequency, direction, speed, lDutyCycle, rDutyCycle);
    // pwm_out(1, lFrequency, lDutyCycle);
    // pwm_out(2, rFrequency, rDutyCycle);


    // for (int freq=340;freq<350;freq++)
    // {
    //     printf("freq = %d, direction: %c, speed = %d, lDutyCycle = %d, rDutyCycle = %d\n\r", freq, direction, speed, lDutyCycle, rDutyCycle);
    //     pwm_out(1, freq, lDutyCycle);
    //     pwm_out(2, freq, rDutyCycle);
    //     delay_ms(5000);
    // }
}