#include "Win32WiringPi.h"

using namespace std;

void wiringPiSetup() {
	cout << "Wiring Pi Setup Called" << endl;
}

void wiringPiSPISetup(unsigned int select,unsigned int Mhz) {
	cout << "Wiring Pi SPI Setup Called for Chip: " << select << " using KHz: " << Mhz << endl;
}

void pinMode(unsigned int pin, unsigned int in_out) {
	cout << "Pin Mode Called - Pin: " << pin << " Input/Output: " << in_out << endl;
}

void digitalWrite(unsigned int pin, unsigned int off_on) {
	cout << "Digital Write Requested - Pin: " << pin << " Off/On: " << off_on << endl;
}

void wiringPiSPIDataRW(unsigned int select, unsigned char * buffer, unsigned int size) {
	cout << "Wiring Pi SPI RW Called - Chip: " << select << " Buffer: " << " Buffer Size: " << size << endl;
}