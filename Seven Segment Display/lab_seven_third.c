#include <LPC17xx.h>

unsigned int nums[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned int i, j, k, num;
int arr[4];

void Delay(int ms)
{
	LPC_TIM0->CTCR = 0;
	LPC_TIM0->PR = 2;
	LPC_TIM0->TCR = 2;
	LPC_TIM0->TCR = 1;
	while(LPC_TIM0->TC < ms);
	LPC_TIM0->TCR = 0;
}

int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;
	LPC_PINCON->PINSEL4 &= 0xFFFFFFFC;
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	LPC_GPIO1->FIODIR |= 0x07800000;
	LPC_GPIO2->FIODIR &= 0xFFFFFFFE;	
	
	i = 9999;
	while(1)
	{
		unsigned int sq = LPC_GPIO2->FIOPIN & 0x1;
		if(sq == 1) i = (i+1) % 10000;
		else i = (i-1) % 10000;
		
		num = i;
		for(j = 0; j < 4; j++){
			LPC_GPIO1->FIOPIN = j<<23;
			LPC_GPIO0->FIOPIN = nums[(num%10)]<<4;
			num /= 10;
			for(k = 0; k < 10000; k++);
		}
	}
	return 0;
}