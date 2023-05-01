#define PIX 2

#ifndef IOREG32
#define IOREG32(addr) (*(volatile unsigned long *) (addr))
#endif

static inline void nop() { asm("nop"); }

void num_to_led(int intensity);
void neopixel_lit(uint8_t green[5], uint8_t red[5], uint8_t blue[5]);
void pix_forward(void);
void pix_backward(void);
void pix_left(void);
void pix_right(void);


// 26 Clock cycle - 0.4us
#define T0H     nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop();
// 55 Clock cycle - 0.85us --removed 1
#define T1H     nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;
                
// 26 Clock cycle - 0.4us
#define T1L     nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop();
// 55 Clock cycle - 0.85us --removed 1
#define T0L     nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;nop() ; nop();\
                nop(); nop(); nop(); nop();nop ();nop() ;


// #define NEO_PIXEL_RESET_CODE 


#define NEO_PIXEL_1CODE IOREG32 (PORT(PIX) + GPIO_OUTSET) = (1 << PIN(PIX));\
    T1H\
    IOREG32(PORT(PIX) + GPIO_OUTCLR) = (1 << PIN(PIX));\
    T1L

#define NEO_PIXEL_0CODE IOREG32 (PORT(PIX) + GPIO_OUTSET) = (1 << PIN(PIX));\
    T0H\
    IOREG32(PORT(PIX) + GPIO_OUTCLR) = (1 << PIN(PIX));\
    T0L

#define NEO_PIXEL_ON_COLOUR NEO_PIXEL_1CODE \
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE\
    NEO_PIXEL_1CODE

#define NEO_PIXEL_OFF_COLOUR NEO_PIXEL_0CODE \
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE\
    NEO_PIXEL_0CODE


#define NEO_PIXEL_RGB NEO_PIXEL_ON_COLOUR\
    NEO_PIXEL_ON_COLOUR\
    NEO_PIXEL_ON_COLOUR

#define NEO_PIXEL_G NEO_PIXEL_ON_COLOUR\
    NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_OFF_COLOUR

#define NEO_PIXEL_R NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_ON_COLOUR\
    NEO_PIXEL_OFF_COLOUR

#define NEO_PIXEL_B NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_ON_COLOUR

#define NEO_PIXEL_OFF NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_OFF_COLOUR\
    NEO_PIXEL_OFF_COLOUR
