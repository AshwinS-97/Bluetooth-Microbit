#include "utility.h"
#include <string.h>

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
 
// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

void printMetrics(float refAngle,float currAngle, int error, float kp,int lcontrol,int rcontrol,char* msg){
    
    
    char refAngleArr[7];
    ftoa(refAngle,refAngleArr,3);

    char currAngleArr[7];
    ftoa(currAngle,currAngleArr,3);

    char errorArr[5];
    intToStr(error,errorArr,4);

    char kpArr[5];
    ftoa(kp,kpArr,3);

    char lcontrolArr[5];
    intToStr(lcontrol,lcontrolArr,2);

    char rcontrolArr[5];
    intToStr(rcontrol,rcontrolArr,2);

    strcat(msg,"[ ");
    strcat(msg,refAngleArr);
    strcat(msg," , ");
    strcat(msg,currAngleArr);
    strcat(msg," , ");
    strcat(msg,errorArr);
    strcat(msg," , ");
    strcat(msg,kpArr);
    strcat(msg," , ");
    strcat(msg,lcontrolArr);
    strcat(msg," , ");
    strcat(msg,rcontrolArr);
    strcat(msg,"] \n");


}