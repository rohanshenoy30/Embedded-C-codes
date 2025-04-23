#include <LPC17xx.h>

unsigned int i, j;
unsigned long LED = 0x00000010;
unsigned char counter = 0;  // 8-bit counter
unsigned char direction = 1; // 1 for up, 0 for down

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    // Configure P0.4 to P0.11 as outputs (for LEDs)
    LPC_PINCON->PINSEL0 &= 0xFF0000FF;   // Set P0.8 to P0.15 to GPIO (clear 8-15 bits)
    LPC_GPIO0->FIODIR |= 0x00000FF0;     // Set P0.4 to P0.11 as outputs

    // Configure P2.12 (SW2) as input (for key press)
    LPC_PINCON->PINSEL4 &= ~0x00003000;   // Set P2.12 to GPIO (clear 12-13 bits in PINSEL4)
    LPC_GPIO2->FIODIR &= ~0x00001000;     // Set P2.12 as input (clear the 12th bit)

    while(1)
    {
        // Check if the switch is pressed (active low)
        if ((LPC_GPIO2->FIOPIN & 0x00001000) == 0)  // Button pressed (active low)
        {
            // Start counting down while the switch is held down
            direction = 0; // Set direction to down
        }
        else
        {
            // Start counting up when the switch is released
            direction = 1; // Set direction to up
        }

        // Update the counter based on the direction
        if (direction)  // Counting up
        {
            if (counter < 255)
                counter++; // Increment counter
        }
        else  // Counting down
        {
            if (counter > 0)
                counter--; // Decrement counter
        }

        // Display the counter on P0.4 to P0.11 (Shift counter to match LED positions)
        LPC_GPIO0->FIOPIN = (counter << 4);  // Shift counter to align with P0.4 to P0.11

        // Simple delay to make the counting visible on LEDs
        for (j = 0; j < 100000; j++);
    }
}
