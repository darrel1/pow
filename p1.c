/* Body Control Module-Power Window
Description - Anti Pinch Mechanism and automatic closing of power windows when it rains
Version -1.0
Date -03/02/2020
Author: Darrel Reesha Pinto
Employee ID:142874*/

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif
#include<avr/interrupt.h>
#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>

#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)

struct {
  volatile unsigned int FLAG_ISR0:1;
  volatile unsigned int FLAG_ISR1:1;
  }FLAG_BIT;
int main()
{
    DDRC = 0x01; //Makes RC0 output pin for servo motor
    PORTC = 0x00;

    CLR_BIT(DDRD,PD2); //obstacle interrupt0
    CLR_BIT(PORTD,PD2); //pull down
    CLR_BIT(DDRD,PD3);  //rain interrupt1
    CLR_BIT(PORTD,PD3);//pull down
    SET_BIT(DDRD,PD7); //display down
    SET_BIT(DDRD,PD4); //display up
    CLR_BIT(DDRD,PD5); //power window up
    CLR_BIT(PORTD,PD5); //pull up
    CLR_BIT(DDRD,PD6);  //power window down
    CLR_BIT(PORTD,PD6); //pull up

    EICRA|=(1<<ISC11);//Rising Edge
    EICRA|=(1<<ISC10);

    EICRA|=(1<<ISC01);//Rising Edge
    EICRA|=(1<<ISC00);

    EIMSK|=(1<<INT0); //Local Interrupt enable
    EIMSK|=(1<<INT1); //Local Interrupt enable
    sei();//global interrupt Enable

    while(1) //super loop
    {
          uint8_t Pin_read1;
          uint8_t Pin_read2;  
          Pin_read1=PIND;
          Pin_read2=PIND;

    if(FLAG_BIT.FLAG_ISR0==1)//int0 obstacle
      {
        SET_BIT(PORTD,PD7); //display down
        FLAG_BIT.FLAG_ISR0=0;
        _delay_ms(1000);
        CLR_BIT(PORTD,PD7);

    PORTC = 0x01;
    _delay_us(2500);
    PORTC = 0x00;
   _delay_ms(2000);
      }

      if(FLAG_BIT.FLAG_ISR1==1)//int1 RAIN
      {
        SET_BIT(PORTD,PD4); //display UP
        FLAG_BIT.FLAG_ISR1=0;
        _delay_ms(1000);
        CLR_BIT(PORTD,PD4);

    PORTC = 0x01;
    _delay_us(1000);
    PORTC = 0x00;
    _delay_ms(2000);

      }

    if ((Pin_read1 & (1<<PD5)))//reading power window up switch status
      {
        SET_BIT(PORTD,PD4);// LED ON
        _delay_ms(100);
        CLR_BIT(PORTD,PD4);

        PORTC = 0x01;
    _delay_us(1000);
    PORTC = 0x00;
    _delay_ms(2000);

      }

      else
      if ((Pin_read2 & (1<<PD6)))//reading power window down switch status
      {
        SET_BIT(PORTD,PD7);// LED ON
        _delay_ms(100);
        CLR_BIT(PORTD,PD7);

    PORTC = 0x01;
    _delay_us(2500);
    PORTC = 0x00;
   _delay_ms(2000);

      }

    }
}

ISR(INT0_vect)
{
 cli();
 FLAG_BIT.FLAG_ISR0=1;
 sei();
}
ISR(INT1_vect)
{
 cli();
 FLAG_BIT.FLAG_ISR1=1;
 sei();
}


