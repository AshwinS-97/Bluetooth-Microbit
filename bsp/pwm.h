#ifndef PWM_H
#define PWM_H
#include <stdint.h>

#define PWM_STOP       		0x004     // stop pwm
#define PWM_SEQ0START  		0x008     // start sequence 0
#define PWM_EVENTS_STOPPED  0x104     // event to indicate PWM stopped
#define PWM_EVENTS_SEQ0END  0x110     // event to indicate sequnce 0 ended
#define PWM_EVENTS_PWMEND   0x118     // event to indicate pwm period ended
#define PWM_ENABLE          0x500     // enable / disable pwm
#define PWM_COUNTERTOP      0x508     // to decide PWM frequncy
                                      //   countertop = pwm clock / pwm freq
#define PWM_PRESCALER       0x50C     // divisor for PWM clock (2 power n)
#define PRESCALER_DIV32     5         //  divide by 32
#define PWM_DECODER         0x510     // to set indivudial duty cycles of
                                      //   all 4 PWM channels
                                      // we will use common configuration
                                      // to set all 4 PWM to same duty cycle
#define PWM_SEQ0PTR         0x520     // address where the sequence is stored
#define PWM_SEQ0CNT         0x524     // number of values in the sequence
#define PWM_SEQ0REFRESH     0x528     // additional pwm cycles between two
                                      //   values of the sequence
#define PWM_SEQ0ENDDELAY    0x52C     // additional pwm cycles at the end
#define PWM_PSEL            0x560     // pin select

#define PWM_FREQUENCY       345       //PWM frequency over which the motor is running


void pwm_init(int channel, uint32_t pin);
void pwm_out(int channel, int freq, int duty);
void pwm_stop(int channel);

#endif  /* PWM_H */
