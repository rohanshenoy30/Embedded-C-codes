#include <LPC17xx.h>

unsigned int nums[4] = {0x06, 0x5B, 0x4F, 0x66};
unsigned int i, j;

int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	LPC_GPIO1->FIODIR |= 0x07800000;
	
	while(1)
	{
		for(i = 4; i >= 1; i--)
		{
			LPC_GPIO1->FIOPIN = (i-1)<<23;
			LPC_GPIO0->FIOPIN = nums[4-i]<<4;
			for(j = 0; j < 10000; j++);
		}
	}
	return 0;
}