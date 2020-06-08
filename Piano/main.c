/*
 * Piano.c
 *
 * Created: 4/2/2020 3:17:00 AM
 * Author : Ramy Shaaban
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#define SPEAKER_DDR DDRB
#define SPEAKER 3
#define BUTTON_DDR DDRA
#define BUTTON_PRT PORTA
#define BUTTON_PIN PINA

#define T_C 262
#define T_D 294
#define T_E 330
#define T_F 349
#define T_G 392
#define T_A 440
#define T_B 493

const int C = 0;
const int D = 1;
const int E = 2;
const int F = 3;
const int G = 4;
const int A = 5;
const int B = 6;
const int START = 7;
#define TonePeriodUnit  2000 // in Ticks related to cpu cycle
                              // ( 1000000 / 256 ) * 0.625 S  ~ 2000
int iterator = 0;
/*struct Tone_ {
	char tone;
	int pos;
};
typedef struct Tone_ Tone_arr;
Tone_arr T_r[7] = {{'C' , 1 },{'D' , 3} ,{'E' , 5}  ,{'F' , 7},{'G' , 9},{'A' , 11},{'B' , 3}};
	*/
char happyBirthday [25][3]= {{1 , 'G' , 9} , {1 ,'G' , 9} ,{2 ,'A' , 11} ,{2 ,'G' , 9 } , {2 ,'C' , 1} ,{4 , 'B' , 13} ,{1 ,'G' , 9} , {1 ,'G' , 9} ,{2 ,'A' , 11},{2 ,'G' , 9},{2,'D' , 3},{4 ,'C' , 1 },{1 ,'G' , 9}
	                         ,{1 , 'G' , 9} , {2 , 'G' , 9} ,{2 ,'E' , 5} , {2 , 'C' , 1} , {2 ,'B' , 13} ,{4 ,'A' ,11} , {1 ,'F' , 7} ,{1 ,'F' , 7}, {2 , 'E' ,5 } ,{2 ,'C' , 1} , {2 , 'D' ,3} , {8 , 'C' ,1}} ;
int mrkup = 0;
int markStart = 1; 
int last_pos;
static inline void initTimer0(void) {
	TCCR0 |= (1 << WGM01); /* CTC mode */
	TCCR0 |= (1 << COM00); /* Toggles pin each cycle through */
	TCCR0 |= (1 << CS01) ; /* CPU clock / /1 */ /*count in microsetonds*/
}

void initTimer1(void){
	TCCR1B |= (1 << CS12) | (1 << WGM12);
	TIMSK |= (1 << OCIE1A) ;
}
ISR(TIMER1_COMPA_vect){
	if(markStart == 0){
	lcd_gotoxy(last_pos , 2);
	lcdData(' ');
	int last_tone = happyBirthday[iterator][1];
	iterator++;
	
	OCR1A = happyBirthday[iterator][0] * TonePeriodUnit;
	char Tone = happyBirthday[iterator][1] ;
	
	if(Tone == last_tone){
		if(mrkup == 0){
			happyBirthday[iterator][2]++;
			mrkup = 1;
		}
		else {
			happyBirthday[iterator][2] = happyBirthday[iterator][2];
			mrkup = 0;
		}
	}
	else{
		mrkup = 2;
	}
	
	  last_pos = happyBirthday[iterator][2];
	lcd_gotoxy(happyBirthday[iterator][2] ,2 );
	lcdData(Tone);	
	
	if (mrkup == 1){
		happyBirthday[iterator][2]--;
	}
	//else if(mrkup == 0)
	//happyBirthday[iterator][2]++;
	
	if(mrkup == 2)
	mrkup = 0;
	
	
	if(iterator == 25){
        LCD_CPRT &= ~(1 << LCD_EN);
        lcdCommand(0x01);   // clear lcd
        _delay_us(2000);
		lcd_print("happy Birthday");
		lcd_gotoxy(6,2);
		lcd_print("Start");
		OCR1A = 200;
		markStart = 1;
		iterator =0;
		return;
	}
	} 
	
}
static inline void playNote(uint8_t HalfPeriod) {
	OCR0 = HalfPeriod; /* set pitch */
	SPEAKER_DDR |= (1 << SPEAKER); /* enable output on speaker */
	//while (duration) { /* Variable delay */
	//	_delay_ms(1);
	//	duration--;
	//}
	/* turn speaker off */
}

int main(void)
{
	initTimer0();
	initTimer1();
	lcd_init();
	OCR1A = 200;
	sei();
	lcd_print("Happy Birthday");
	lcd_gotoxy(6,2);
	lcd_print("Start"); 
	int s = 0;
	
	BUTTON_DDR &= ~(1 << C | 1 << D | 1 << E | 1 << F | 1 << G | 1 << A | 1 << B | 1<< START); 
	BUTTON_PRT |= (1 << C | 1 << D | 1 << E | 1 << F | 1 << G | 1 << A | 1 << B | 1 << START);
    /* Replace with your application code */
	int TC = 62500/T_C ;       // 1/tonefrequency * 125000(to change unit of ticking to 8microseconds)
	                            // then divide by two to get half period
	int TD = 62500/T_D ;
	int TE = 62500/T_E ;
	int TF = 62500/T_F ;
	int TG = 62500/T_G ;
	int TA = 62500/T_A ;
	int TB = 62500/T_B ;
		_delay_ms(2000);
		while( (BUTTON_PIN & (1 << START)) != 0 );
			LCD_CPRT &= ~(1 << LCD_EN);
			lcdCommand(0x01);   // clear lcd
			_delay_us(2000);
			lcd_gotoxy(1 , 1);
			lcd_print("Happy Birthday");
			OCR1A =  happyBirthday[iterator][0]  * TonePeriodUnit;
			char Tone = happyBirthday[iterator][1] ;
			last_pos = happyBirthday[iterator][2];
			lcd_gotoxy(happyBirthday[iterator][2] , 2 );
			lcdData(Tone);
			markStart = 0;
					
    while (1) 
    {
		if(markStart == 1){
		if( (BUTTON_PIN & (1 << START)) == 0 ){
			LCD_CPRT &= ~(1 << LCD_EN);
			lcdCommand(0x01);   // clear lcd
			_delay_us(2000);
			lcd_gotoxy(1 , 1);
			lcd_print("Happy Birthday");
			OCR1A = happyBirthday[iterator][0]  * TonePeriodUnit;
			char Tone = happyBirthday[iterator][1] ;
			last_pos = happyBirthday[iterator][2];
			lcd_gotoxy(happyBirthday[iterator][2] , 2 );
			lcdData(Tone);
			markStart = 0;
			}
		}
		if( (BUTTON_PIN & (1 << C)) == 0 ){
			playNote(TC );
			while( (BUTTON_PIN & (1 << C)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER); 
		}
		if( (BUTTON_PIN & (1 << D)) == 0 ){
			playNote(TD );
			while( (BUTTON_PIN & (1 << D)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		if( (BUTTON_PIN & (1 << E)) == 0 ){
			playNote(TE );
			while( (BUTTON_PIN & (1 << E)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		if( (BUTTON_PIN & (1 << F)) == 0 ){
			playNote(TF );
			while( (BUTTON_PIN & (1 << F)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		if( (BUTTON_PIN & (1 << G)) == 0 ){
			playNote(TG );
			while( (BUTTON_PIN & (1 << G)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		if( (BUTTON_PIN & (1 << A)) == 0 ){
			playNote(TA );
			while( (BUTTON_PIN & (1 << A)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		if( (BUTTON_PIN & (1 << B)) == 0 ){
			playNote(TB );
			while( (BUTTON_PIN & (1 << B)) == 0 );
			SPEAKER_DDR &= ~(1 << SPEAKER);
		}
		

    }
	}


