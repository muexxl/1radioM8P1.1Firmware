#ifndef MXSUPPORT_H
#define MXSUPPORT_H

#include <stdint.h>

#ifdef STM32GO
    #include <stm32g0xx_hal.h>
#endif

// #include <stm32g0xx_hal_conf.h>
// 
// #endif 

void printObject(void * , int );

void printHex(void *, int);
bool isSerialAnswerY();

/**
 * copy byte wise from source to target
 * 
 * @code
 * bytecopy(&source, &target, &sizeof(source))
 * 
 */
void copyBytewise(const void *source, void *target, int len);

/**
 * 
 * write chars to Serial 
 * 
 * */
void writeToSerial(void *, int );

#endif