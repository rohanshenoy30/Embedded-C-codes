#include<LPC17xx.h>
#include<stdlib.h>

unsigned char msg1[13]="Rolling Dice";
unsigned char key;
unsigned long int temp1 = 0;

// Function prototypes
void lcd_init(void);
void write(int, int);
void delay_lcd(unsigned int);
void lcd_comdata(int, int); 
void clear_ports(void);
void lcd_puts(unsigned char *);

int main()
{
    unsigned char k;
    lcd_init();
    temp1 = 0x80;
    lcd_comdata(temp1, 0);
    delay_lcd(800);
    lcd_puts(&msg1[0]);
    
    while(1)
    {
        if(!(LPC_GPIO2->FIOPIN & 1<<12))  // Check if button pressed (assuming active low)
        {
            k = (rand()%6)+1;    // Generate random number 1-6
            k = k + 0x30;        // Convert to ASCII
            temp1 = 0xc0;        // Move cursor to second line
            lcd_comdata(temp1, 0);
            delay_lcd(800);
            lcd_puts(&k);        // Display the dice result
        }
    }
}

void lcd_init(void) {
    /*Ports initialized as GPIO */
    LPC_PINCON->PINSEL1 &= 0xFC003FFF; //P0.23 to P0.28
    /*Setting the directions as output */
    LPC_GPIO0->FIODIR |= 0x0F<<23 | 1<<27 | 1<<28;
    
    clear_ports();
    delay_lcd(3200);
    lcd_comdata(0x33, 0); 
    delay_lcd(30000);
    lcd_comdata(0x32, 0);
    delay_lcd(30000);
    lcd_comdata(0x28, 0); //function set
    delay_lcd(30000);
    lcd_comdata(0x0c, 0); //display on cursor off
    delay_lcd(800);
    lcd_comdata(0x06, 0); //entry mode set increment cursor right
    delay_lcd(800);
    lcd_comdata(0x01, 0); //display clear
    delay_lcd(10000);
}

void lcd_comdata(int temp1, int type) {
    int temp2 = temp1 & 0xf0;    //move data (26-8+1) times : 26 - HN place, 4 - Bits
    temp2 = temp2 << 19;         //data lines from 23 to 26
    write(temp2, type);
    temp2 = temp1 & 0x0f;        //26-4+1
    temp2 = temp2 << 23; 
    write(temp2, type);
    delay_lcd(1000);
}

void write(int temp2, int type) {    /*write to command/data reg */
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;       // Assign the value to the data lines 
    if(type==0)
        LPC_GPIO0->FIOCLR = 1<<27;   // clear bit RS for Command
    else
        LPC_GPIO0->FIOSET = 1<<27;   // set bit RS for Data
    LPC_GPIO0->FIOSET = 1<<28;      // EN=1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = 1<<28;      // EN =0
}

void delay_lcd(unsigned int r1)
{
    unsigned int r;
    for(r=0;r<r1;r++);
}

void clear_ports(void) {    /* Clearing the lines at power on */
    LPC_GPIO0->FIOCLR = 0x0F<<23; //Clearing data lines
    LPC_GPIO0->FIOCLR = 1<<27;    //Clearing RS line
    LPC_GPIO0->FIOCLR = 1<<28;    //Clearing Enable line
}

void lcd_puts(unsigned char *buf1) {
    unsigned int i=0;
    unsigned int temp3;
    while(buf1[i]!='\0') {
        temp3 = buf1[i];
        lcd_comdata(temp3, 1);
        i++;
        if(i==16)
            lcd_comdata(0xc0, 0);
    }
}