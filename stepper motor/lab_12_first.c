#include <LPC17xx.h>

unsigned long int var1, var2;
unsigned int i = 0, j = 0, k = 0;

void clock_wise(){
	var1 = 0x00000008;
	for(i = 0; i <= 3; i++){
		var1 <<= 1;
		LPC_GPIO0->FIOPIN = var1;
		for(k=0; k<3000;k++);
	}
}

void anti_clock_wise(){
	var1 = 0x00000100;
	for(i = 0; i <= 3; i++){
		var1 >>= 1;
		LPC_GPIO0->FIOPIN = var1;
		for(k=0; k<3000;k++);
	}
}

int main(){
		SystemInit();
		SystemCoreClockUpdate();
	
		LPC_PINCON->PINSEL0 = 0xFFFF00FF;
		LPC_PINCON->PINSEL4 &= 0x0;
		LPC_GPIO0->FIODIR = 0x000000F0;
		LPC_GPIO2->FIODIR = 0x0;
		
	while(1){
		if((LPC_GPIO2->FIOPIN & 1) == 0){
			anti_clock_wise();
		} else clock_wise();
		
	}
}