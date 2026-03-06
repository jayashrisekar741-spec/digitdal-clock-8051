/***P2.0 - P2.6** ? 7-Segment segments (a,b,c,d,e,f,g)
**P0.0 - P0.5** ? 6 digits Common Cathode control
**P3.0 - P3.3** ? 4 Push Buttons (Set H, Set M, Alarm, Reset)
**P1.7** ? Buzzer (Alarm)
**XTAL** ? 11.0592 MHz Crystal*/

#include<reg51.h>
sbit DIG1=P0^0; sbit DIG2=P0^1; sbit DIG3=P0^2; // Digit controls
sbit DIG4=P0^3; sbit DIG5=P0^4; sbit DIG6=P0^5;
sbit BUZZ=P1^7;

unsigned char dig_val[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; // 0-9
unsigned char hh=12,mm=30,ss=00; unsigned long ms_count=0;

void delay_ms(unsigned int t) {
    unsigned int i,j; for(i=0;i<t;i++) for(j=0;j<120;j++);
}

void display_update() {
    P2=dig_val[ss%10]; DIG6=0; delay_ms(5); DIG6=1; // Units
    P2=dig_val[ss/10]; DIG5=0; delay_ms(5); DIG5=1; // Tens
    // Continue for mm, hh...
}

void timer_isr() interrupt 1 {
    TH1=(65536-50000)/256; TL1=65536-50000; // 50ms @11.0592MHz
    ms_count+=50;
    if(ms_count>=1000) {
        ms_count=0; ss++;
        if(ss>=60) { ss=0; mm++; }
        if(mm>=60) { mm=0; hh++; }
        if(hh>=24) hh=0;
    }
}

void main() {
    TMOD=0x01; // Timer1 16-bit mode
    TH1=(65536-50000)/256; TL1=65536-50000; // 50ms initial
    IE=0x88; TR1=1; // Enable Timer1 interrupt
    
    P2=0x00; P0=0xFF; // Ports init
    while(1) {
        display_update();
    }
}

