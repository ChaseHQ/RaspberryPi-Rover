#include <iostream>

#define INPUT 0
#define OUTPUT 1

void wiringPiSetup();
void wiringPiSPISetup(unsigned int select,unsigned int Mhz);

void pinMode(unsigned int pin, unsigned int in_out);
void digitalWrite(unsigned int pin, unsigned int off_on);
void wiringPiSPIDataRW(unsigned int select, unsigned char * buffer, unsigned int size);
