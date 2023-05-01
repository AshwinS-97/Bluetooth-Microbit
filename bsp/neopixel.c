#include "board.h"
#include "gpio.h"
#include "lib.h"
#include "neopixel.h"
#include <math.h>
#include "string.h"


void pix_forward(void)
{
    uint8_t gled[5] = {255, 255, 255, 255, 255};
    uint8_t rled[5] = {255, 255, 255, 255, 255};
    uint8_t bled[5] = {255, 255, 255, 255, 255};
    
    // while(strcmp(command,"FORWARD") == 0)
    // {
    //     neopixel_lit(gled,rled,bled);
    //     circular(gled,rled,bled);
    //     delay_ms(200);
    // }

    neopixel_lit(gled,rled,bled);

}

void pix_backward(void)
{
    uint8_t gled[5] = {0, 0, 0, 0, 0};
    uint8_t rled[5] = {255, 255, 255, 255, 255};
    uint8_t bled[5] = {0, 0, 0, 0, 0};
    
    // while(strcmp(command,"FORWARD") == 0)
    // {
    //     neopixel_lit(gled,rled,bled);
    //     circular(gled,rled,bled);
    //     delay_ms(200);
    // }

    neopixel_lit(gled,rled,bled);

}

void pix_left(void)
{
    uint8_t gled[5] = {255, 0, 0, 0, 0};
    uint8_t rled[5] = {255, 0, 0, 0, 0};
    uint8_t bled[5] = {0, 0, 0, 0, 0};
    
    // while(strcmp(command,"FORWARD") == 0)
    // {
    //     neopixel_lit(gled,rled,bled);
    //     circular(gled,rled,bled);
    //     delay_ms(200);
    // }

    neopixel_lit(gled,rled,bled);

}

void pix_right(void)
{
    uint8_t gled[5] = {0, 0, 0, 0, 255};
    uint8_t rled[5] = {0, 0, 0, 0, 255};
    uint8_t bled[5] = {0, 0, 0, 0, 0};
    
    neopixel_lit(gled,rled,bled);

}


void neopixel_lit(uint8_t green[5], uint8_t red[5], uint8_t blue[5])
{

    IOREG32(PORT(PIX) + GPIO_OUTSET) = (1 << PIN(PIX));
    delay_us(60);
    IOREG32(PORT(PIX) + GPIO_OUTCLR) = (1 << PIN(PIX));
    delay_ms(50);

    for (int i =0; i<5; i++)
    {
        num_to_led(green[i]);
        num_to_led(red[i]);
        num_to_led(blue[i]);
    }

    gpio_clear(PIX);    
    delay_us(60);
}


void num_to_led(int intensity)
{
    for(int i=0; i < 8; i++)
    {
        if (intensity & (1 << i))
        {
            NEO_PIXEL_1CODE
        }
        else
        {
            NEO_PIXEL_0CODE
        }


    }
}




