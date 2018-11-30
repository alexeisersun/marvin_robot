#ifndef MORSE_GENERATOR_H_
#define MORSE_GENERATOR_H_

#include <Arduino.h>
#include <RobotDefinitions.h>

extern char morse_Codes[40][10];
extern int buzzerPin;


void PlayMorse(char morse[]);
void PlayMorseString(char morseStr[]);

#endif