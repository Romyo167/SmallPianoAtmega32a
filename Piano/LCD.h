/*
 * LCD.h
 *
 * Created: 3/31/2020 7:46:12 PM
 *  Author: Ramy Shaaban
 */ 
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DPRT  PORTD
#define LCD_DDDR  DDRD
#define LCD_DPIN  PIND
#define LCD_CPRT  PORTB
#define LCD_CDDR  DDRB
#define LCD_CPIN  PINB
#define LCD_RS    0
#define LCD_RW    1
#define LCD_EN    2
void delay_us(int d);   // This function failed Read .c for explanation
void lcdCommand(unsigned char cmnd);
void lcdData(unsigned char data);
void lcd_init();
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_print(char * str);
void lcd_print5Digitnum(int num);
