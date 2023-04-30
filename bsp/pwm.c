#include <stdint.h>
#include <string.h>
#include "pwm.h"
#include "lib.h"

#ifndef IOREG32
#define IOREG32(addr) (*(volatile unsigned long *) (addr))
#endif

#define PWM (uint32_t[]){0x4001C000, 0x40021000, 0x40022000, 0x4002D000}


static uint16_t s_sequence;

#define PWM_CLK 500000
uint32_t channelAddr;

void pwm_init(int channel, uint32_t pin)
{
    IOREG32(PWM[channel] + PWM_PRESCALER) = PRESCALER_DIV32; // Configuration for PWM_CLK // PWM frequency selection
    IOREG32(PWM[channel] + PWM_PSEL) = pin; // PWM pin selection
    IOREG32(PWM[channel] + PWM_ENABLE) = 1; // PWM module enable register
}

void pwm_out(int channel, int freq, int duty)
{
    uint16_t countertop;

    countertop =  PWM_CLK / freq;
    IOREG32(PWM[channel] + PWM_COUNTERTOP) = countertop;

    IOREG32(PWM[channel] + PWM_SEQ0PTR) = (uint32_t) &s_sequence;
    IOREG32(PWM[channel] + PWM_SEQ0CNT) = 1;
    IOREG32(PWM[channel] + PWM_SEQ0REFRESH) = 0;

    s_sequence = (countertop * duty) / 100;
    // printf("countertop = %d\ts_sequence = %d\n\r", countertop, s_sequence);

    IOREG32(PWM[channel] + PWM_SEQ0START) = 1;
}

void pwm_stop(int channel)
{
    IOREG32(PWM[channel] + PWM_STOP) = 1;
    while (IOREG32(PWM[channel] + PWM_EVENTS_STOPPED) == 0);
    IOREG32(PWM[channel] + PWM_EVENTS_STOPPED) = 0;
}
