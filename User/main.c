#include "clock.h"
#include "beep.h"
#include "uartContact.h"

int main()
{ 
	initUart();
	enableClock();
	//setTime();
	initBEEP();
	led();
}
	




