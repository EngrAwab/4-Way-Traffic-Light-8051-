
#include <xc.h>

// Assuming a 20MHz crystal frequency
#include "all_data.h"
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & BOREN_OFF & LVP_OFF & CPD_OFF & WRT_OFF & CP_OFF);

// Define GPIO pins

#define L_1 RB0
#define L_2 RB1
#define R_1 RB2
#define R_2 RB3
#define B_1 RB6
#define B_2 RB7
#define U_1 RB4
#define U_2 RB5

// Define lights
#define G_U RE0
#define Y_U RE1
#define R_U RE2
#define G_R RC3
#define Y_R RC4
#define R_R RC5
#define G_L RA1
#define Y_L RA4
#define R_L RA5
#define R_B RC6
#define Y_B RC1
#define G_B RC7            ///not working 

unsigned int LED_STATUS[] = {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1}; // GYR---URLB

unsigned int Max_one = 8, Max_sec = 0, Open_one = 9, Open_sec = 2, Mid_one = 1, Mid_sec = 0;
unsigned int cout = 0, one_l = 1, sec_l = 2, count = 0, one_r = 0, sec_r = 1, one_b = 9, sec_b = 0, one_u = 0, sec_u = 3, one_temp, sec_temp, R_tem = 0, Y_tem = 0, G_tem = 0;

void evaluate(unsigned int One, unsigned int Sec, unsigned int R_T, unsigned Y_T, unsigned G_T) {
    One = One - 1;
    if (One == -1) {
        One = 9;
        Sec = Sec - 1;
    }
    if (Sec == -1) {
        if (G_T == 0) {
            One = Max_one;
            Sec = Max_sec;
            R_T = 0;
            G_T = 1;
            Y_T = 1;
        } else if (Y_T == 0) {
            One = Open_one;
            Sec = Open_sec;
            Y_T = 1;
            G_T = 0;
            R_T = 1;
        } else if (R_T == 0) {
            One = Mid_one;
            Sec = Mid_sec;
            Y_T = 0;
            R_T = 1;
            G_T = 1;
        }
    }

    R_tem = R_T;
    G_tem = G_T;
    Y_tem = Y_T;
    one_temp = One;
    sec_temp = Sec;
}

void display(unsigned int a) {
    unsigned char value[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    PORTD = value[a];
}

void delay(int time) {
    for (int a = 0 ; a <1200; a++){}
}

void LED() {
R_U = (__bit)LED_STATUS[0];
Y_U = (__bit)LED_STATUS[1];
G_U = ~(__bit)LED_STATUS[2];

R_R = (__bit)LED_STATUS[3];
Y_R = (__bit)LED_STATUS[4];
G_R = ~(__bit)LED_STATUS[5];

G_L = (__bit)LED_STATUS[6];
Y_L = (__bit)LED_STATUS[7];
R_L = ~(__bit)LED_STATUS[8];

G_B = (__bit)LED_STATUS[9];
Y_B = (__bit)LED_STATUS[10];
R_B = ~(__bit)LED_STATUS[11];


}

void __interrupt() isr() {
    
    if (TMR1IF) {
        
        TMR1IF = 0; // Clear Timer0 interrupt flag
        count++;
        if (count == 19) {
            count = 0;
            evaluate(one_l, sec_l, LED_STATUS[6], LED_STATUS[7], LED_STATUS[8]);
            one_l = one_temp;
            sec_l = sec_temp;
            LED_STATUS[6] = R_tem;
            LED_STATUS[7] = Y_tem;
            LED_STATUS[8] = G_tem;

            evaluate(one_r, sec_r, LED_STATUS[3], LED_STATUS[4], LED_STATUS[5]);
            one_r = one_temp;
            sec_r = sec_temp;
            LED_STATUS[3] = R_tem;
            LED_STATUS[4] = Y_tem;
            LED_STATUS[5] = G_tem;

            evaluate(one_u, sec_u, LED_STATUS[0], LED_STATUS[1], LED_STATUS[2]);
            one_u = one_temp;
            sec_u = sec_temp;
            LED_STATUS[0] = R_tem;
            LED_STATUS[1] = Y_tem;
            LED_STATUS[2] = G_tem;

            evaluate(one_b, sec_b, LED_STATUS[9], LED_STATUS[10], LED_STATUS[11]);
            one_b = one_temp;
            sec_b = sec_temp;
            LED_STATUS[9] = R_tem;
            LED_STATUS[10] = Y_tem;
            LED_STATUS[11] = G_tem;

            LED();
        }
    }
}

void main() {
    TRISD = 0;
    TRISB = 0;
    TRISA = 0;
    TRISE = 0;
    TRISC = 0; // RC6 and RC7 are inputs
    GIE = 1;
      // Initialize Timer1 value
    TMR1IF = 0;  // Clear Timer1 interrupt flag
    TMR1IE = 1;  // Enable Timer1 interrupt
    PEIE = 1;
    
    // Enable global interrupts
    T1CON=0x21;
    TMR1H=0;
    ADCON0bits.ADON = 0;

    // Configure all ADC pins as digital I/O
    ADCON1 = 0xFF;
    PORTA=1;
    while (1) {
        L_1 = 1;
        display(one_l);
        delay(1);
        L_1 = 0;
        L_2 = 1;
        display(sec_l);
        delay(1);
        L_2 = 0;

        R_1 = 1;
        display(one_r);
        delay(1);
        R_1 = 0;
        R_2 = 1;
        display(sec_r);
        delay(1);
        R_2 = 0;

        B_1 = 1;
        display(one_b);
        delay(1);
        B_1 = 0;
        B_2 = 1;
        display(sec_b);
        delay(1);
        B_2 = 0;

        U_1 = 1;
        display(one_u);
        delay(1);
        U_1 = 0;
        U_2 = 1;
        display(sec_u);
        delay(1);
        U_2 = 0;

        
    }
}
