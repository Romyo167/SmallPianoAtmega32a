/*
 * LCD.c
 *
 * Created: 3/31/2020 7:45:51 PM
 *  Author: Ramy Shaaban
 */ 

#include "LCD.h"

// This function failed because it requires a time constant


/*void delay_us(int d){
	_delay_us(d);
}*/

void lcdCommand(unsigned char cmnd){
	LCD_DPRT = cmnd & 0xF0;
	LCD_CPRT &= ~(1 << LCD_RS);
	LCD_CPRT &= ~(1 << LCD_RW);
	LCD_CPRT |= (1 << LCD_EN);
	_delay_us(1);
	LCD_CPRT &= ~(1 << LCD_EN);
	_delay_us(100);
	LCD_DPRT = cmnd << 4;
	LCD_CPRT |= (1 << LCD_EN);
	_delay_us(1);
	LCD_CPRT &= ~(1 << LCD_EN);
	_delay_us(100);
}

void lcdData(unsigned char data){
	LCD_DPRT = data & 0xF0;
	LCD_CPRT |= (1 << LCD_RS);
	LCD_CPRT &= ~(1 << LCD_RW);
	LCD_CPRT |= (1 << LCD_EN);
	_delay_us(1);
	LCD_CPRT &= ~(1 << LCD_EN);
	LCD_DPRT = data << 4;
	LCD_CPRT |= (1 << LCD_EN);
	_delay_us(1);
	LCD_CPRT &= ~(1 << LCD_EN);
	_delay_us(100);
}

void lcd_init(){
	LCD_DDDR |= 0xF0;
	LCD_CDDR |= 0x07;
	LCD_CPRT &= ~(1 << LCD_EN);
	lcdCommand(0x33);
	lcdCommand(0x32);          
	lcdCommand(0x28);   
	lcdCommand(0x0e);   // display on , cursor on
	lcdCommand(0x01);   // clear lcd
	_delay_us(2000);
	lcdCommand(0x06);
}

void lcd_gotoxy(unsigned char x, unsigned char y){
	unsigned char firstCharAdr[] = { 0x80 , 0xC0 ,0x94 , 0xD4};
		lcdCommand(firstCharAdr[y-1] + x - 1);
		_delay_us(100);
}

void lcd_print(char * str){
	unsigned char i = 0;
	while(str[i] != 0)
	{
		lcdData(str[i]);
		i++;
	}
}
void lcd_print5Digitnum(int num){
	int digit1 = num % 10;
	num = num /10;
	int digit2 = num % 10;
	num = num /10;
	int digit3 = num % 10;
	num = num /10;
	int digit4 = num % 10;
	num = num /10;
	int digit5 = num % 10;
	
	
	lcdData(digit5 + 0x30);
	lcdData(digit4 + 0x30);
	lcdData(digit3 + 0x30);
	lcdData(digit2 + 0x30);
	lcdData(digit1 + 0x30);
	
	
}