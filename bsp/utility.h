#include <stdio.h>
#include <math.h>
#include <stdint.h>


void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);

float norm (const float* v, uint8_t size);
float dot(const float* v, const float* w, uint8_t size);