#include<LPC17xx.h>
#define RS_CTRL 0x08000000;
#define EN_CTRL 0x10000000;
#define DT_CTRL 0x07800000;

unsigned char msg3[4] = {"MIT"};
	unsigned char msg4[19] = {"Department of CSE:"};
unsigned long int temp1 = 0, temp2 = 0;
void lcd_init(void);
void lcd_com(void);
void clear_ports(void);
void delay_lcd(unsigned int);
void lcd_puts(unsigned char*);
void lcd_data(void);
void wr_cn(void);
void wr_dn(void);



void lcd_init(void)
{
	LPC_GPIO0->FIODIR |= DT_CTRL;
	LPC_GPIO0->FIODIR |= RS_CTRL;
	LPC_GPIO0->FIODIR |= EN_CTRL;
	clear_ports();
	delay_lcd(3200);
	temp1 = 0x33;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x32;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x28;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x0c;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);
	return;
}

void clear_ports(void)
{
	LPC_GPIO0->FIOCLR = DT_CTRL;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOCLR = EN_CTRL;
}

void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r=0; r<r1; r++);
	return;
}

void lcd_com(void)
{
	temp2 = temp1 & 0xf0;
	temp2 = temp2<<19;
	wr_cn();
	delay_lcd(30000);
	temp2 = temp1 & 0x0f;
	temp2 = temp2<<23;
	wr_cn();
	delay_lcd(30000);
	return;
}

void wr_cn()
{
	LPC_GPIO0->FIOPIN = temp2;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
}

void lcd_puts(unsigned char* buf)
{
	unsigned int i=0;
	while(buf[i]!='\0')
	{
		temp1=buf[i];
		lcd_data();
		delay_lcd(800);
		i++;
		if(i==16)
		{
			temp1=0xc0;
			lcd_com();
			delay_lcd(800);
		}
	} 
	return;
}

void lcd_data(void)
{
	temp2 = temp1 & 0xf0;
	temp2 = temp2<<19;
	wr_dn();
	delay_lcd(30000);
	temp2 = temp1 & 0x0f;
	temp2 = temp2<<23;
	wr_dn();
	delay_lcd(30000);
	return;
}

void wr_dn()
{
	LPC_GPIO0->FIOPIN = temp2;
	LPC_GPIO0->FIOSET = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	return;
}


void scan(void);
unsigned char msg1[13]="key pressed";
unsigned char row,flag,key;
unsigned long int i,var1,temp,temp2,temp3;
unsigned char scan_code[16]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,
	0x14,0x24,0x44,0x84,0x18,0x28,0x48,0x88};
unsigned char ascii_code[16]={'0','1','2','3','4','5',
	'6','7','8','9','A','B','C','D','E','F'};

int main(void)
{
	LPC_GPIO2->FIODIR = 0x3c00;
	LPC_GPIO1->FIODIR = 0xf87fffff;
	lcd_init();
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
	lcd_puts(&msg1[0]);
	while(1)
	{
		while(1)
		{
			for(row=1; row<5; row++)
			{
				if(row==1)
					var1 = 0x400;
				else if(row==2)
					var1 = 0x800;
				else if(row==3)
					var1 = 0x1000;
				else if(row==4)
					var1 = 0x2000;
				temp = var1;
				LPC_GPIO2->FIOCLR = 0x3c00;
				LPC_GPIO2->FIOSET = var1;
				flag = 0;
				scan();
				if(flag==1)
					break;
			}
			if(flag==1)
				break;
		}
		for(i=0; i<16; i++)
		{
			if(key==scan_code[i])
			{
				key = ascii_code[i];
				break;
			}
		}
		temp1 = 0xc0;
		lcd_com();
		delay_lcd(800);
		lcd_puts(&key);
	}
	return 0;
}

void scan(void)
{
	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0x07800000;
	if(temp3!=0)
	{
		flag = 1;
		temp3>>=19;
		temp>>=10;
		key = temp3|temp;
	}
}