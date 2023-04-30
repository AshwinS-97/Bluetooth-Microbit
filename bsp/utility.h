#include <stdio.h>
#include <math.h>


void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);
void printMetrics(float refAngle,float currAngle, int error, float kp,int lcontrol,int rcontrol,char* msg);