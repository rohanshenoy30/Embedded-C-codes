#include <LPC17xx.h>

unsigned int i, j;
unsigned long LED = 0x00000010;
unsigned char counter = 1;  // 8-bit counter for ring counter, start with bit 0 set
unsigned char lastButtonState = 1;  // Last button state (1 means not pressed, 0 means pressed)

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
        // Read the key press (P2.12) - switch is active low
        unsigned char buttonState = (LPC_GPIO2->FIOPIN & 0x00001000) == 0;  // Button pressed (active low)

        // Check for button press (debouncing the press)
        if (buttonState == 0 && lastButtonState == 1)  // Button is pressed
        {
            // Implementing the ring counter logic: Rotate counter
            counter = (counter << 1) | ((counter >> 7) & 0x01);  // Shift left and wrap the leftmost bit to the rightmost
        }

        // Update the last button state for debouncing
        lastButtonState = buttonState;

        // Display the counter on P0.4 to P0.11 (Shift counter to match LED positions)
        LPC_GPIO0->FIOPIN = (counter << 4);  // Shift counter to align with P0.4 to P0.11

        // Simple delay to make the counting visible on LEDs
        for (j = 0; j < 1000; j++);
    }
}
