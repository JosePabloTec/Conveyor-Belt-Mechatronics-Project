
/*
 * File:   servo_travieso.c
 * Author: Jose Pablo Manriquez Amavizca
 *
 * Created on June 2, 2026, 11:56 AM
 */

// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
// Cambió:
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
// Cambió:
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 16000000


// Sensores
#define SensorArriba RD0
#define SensorAbajo RD1

// LEDs

#define Coca   RB1 // Salida
#define Garrafon   RB2 // Salida
#define Paro   RB3 // Salida

//Paro 

#define SenalParo RD2

// Actuador

#define Servo RB0

void Init_Ports(void)
{
    
    ADCON1 = 0x0F; // nos aseguramos que todos los pines sean digitales.
    // Servo en RB0
    TRISBbits.TRISB0 = 0; //salida

    // Sensores en RD0 y RD1
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    
    // LEDs
    TRISBbits.TRISB1 = 0;  // Salida
    TRISBbits.TRISB2 = 0;  // Salida
    TRISBbits.TRISB3 = 0;  // Salida
    
    //
    TRISDbits.TRISD2 = 1;  // Entrada (paro de emergencia)
    
    //Inicializamos todas las salidas en 0, todos los LEDs apagados
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;

}

void Servo_garrafon(void)
{
    Servo = 1;
    // Señal PWM
    // ¿como se controla un servo? https://www.youtube.com/watch?v=GXHQemuRtWU
    __delay_us(1833);      // 60°
    Servo = 0;
    __delay_us(18167);     // Completa 20 ms
}

void Servo_coca(void)
{
    Servo = 1;
    
    __delay_us(1667);      // 30°
    Servo = 0;
    __delay_us(18333);     // Completa 20 ms
}

void Servo_default(void)
{
    Servo = 1;
    __delay_us(1500);      // 0°
    Servo = 0;
    __delay_us(18500);     // Completa 20 ms
}

void delay_astuto(void) //esta función sirve para evitar que una tapa de garrafón sea identificada como de coca
{
    unsigned char i;

    for(i = 0; i < 100; i++)
    {
        Servo_garrafon();      // genera un periodo de 20 ms, por 100 son 20,000 ms o 2 segundos
    }
}

void secuencia_alto(void){
    while(1){
        Paro = 1;
        Coca = 0;
        Garrafon = 0;
        __delay_ms(800);
        Paro = 0;
        Coca = 0;
        Garrafon = 0;
        __delay_ms(800);
        if (SenalParo == 1){
            break;
        }
    }
}

void prender_foquitos(void){
    if((SensorArriba == 0) && (SensorAbajo == 0)){
        Garrafon = 1;
        Coca = 0;
    }
    
    else if (SensorAbajo == 0 && SensorArriba == 1){
        Coca = 1;
        Garrafon = 0;
    }
    
    else if (SenalParo == 0){
        secuencia_alto();
    }
    
    else{
        Coca = 0;
        Garrafon = 0;
        Paro = 0;
    }
}



int main(void)
{
    Init_Ports();
    char posicion_actual = 'X';
    
    while(1)
    {   
        
        prender_foquitos();
        
        if((SensorArriba == 0) && (SensorAbajo == 0))
        {
            delay_astuto();
            posicion_actual = 'g';
        }
        else if(SensorAbajo == 0)
        {
            posicion_actual = 'c';
        }

        if(posicion_actual == 'c')
        {
            Servo_coca();
        }
        else if(posicion_actual == 'g')
        {
            Servo_garrafon();
        }
        else if (posicion_actual == 'X')
        {
            Servo_default();
        }
        
    }

}