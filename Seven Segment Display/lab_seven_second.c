#include <LPC17xx.h>

unsigned int nums[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned int i, j;
int arr[4];

void TimerInit(int ms)
{
	
}

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
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	LPC_GPIO1->FIODIR |= 0x07800000;	
	
	while(1)
	{
		for(arr[3] = 9; arr[3] >= 0; arr[3]--){
			for(arr[2] = 9; arr[2] >= 0; arr[2]--){
				for(arr[1] = 9; arr[1] >= 0; arr[1]--){
					for(arr[0] = 9; arr[0] >= 0; arr[0]--){
						for(i = 0; i < 4; i++){
							LPC_GPIO1->FIOPIN = i<<23;
							LPC_GPIO0->FIOPIN = nums[arr[i]]<<4; 	
							Delay(1000);
						}
					}
				}
			}
		}
	}
	return 0;
}