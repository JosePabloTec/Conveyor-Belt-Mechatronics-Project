#include <xc.h>

#define _XTAL_FREQ 16000000

//================ CONFIG BITS ================
#pragma config FOSC = HS        // Oscilador HS
#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
//=============================================

// Sensores
#define SensorArriba PORTDbits.RD0
#define SensorAbajo PORTDbits.RD1

// LEDs

#define Coca   RB1 // Salida
#define Garrafon   RB2 // Salida
#define Paro   RB3 // Salida

//Paro 

#define SenalParo PORTDbits.RD2

void Init_Ports(void)
{
    // Todo digital
    ADCON1 = 0x0F;

    // Servo en RB0
    TRISBbits.TRISB0 = 0;
    LATBbits.LATB0 = 0;

    // Botones en RD0 y RD1
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    
    //
    TRISBbits.TRISB1 = 0;  // Salida
    TRISBbits.TRISB2 = 0;  // Salida
    TRISBbits.TRISB3 = 0;  // Salida
    
    //
    TRISDbits.TRISD2 = 1;  // Entrada (paro de emergencia)
}

void Servo_0(void)
{
    LATBbits.LATB0 = 1;
    
    __delay_us(1000);      // Aproximadamente 45°
    LATBbits.LATB0 = 0;
    __delay_us(20000);     // Completa 20 ms
}

void Servo_30(void)
{
    LATBbits.LATB0 = 1;
    
    __delay_us(1166);      // Aproximadamente 45°
    LATBbits.LATB0 = 0;
    __delay_us(18834);     // Completa 20 ms
}

void Servo_60(void)
{
    LATBbits.LATB0 = 1;
    __delay_us(1500);      // Aproximadamente 90°
    LATBbits.LATB0 = 0;
    __delay_us(18500);     // Completa 20 ms
}

char posicion_actual = 'X';

void delay_astuto(void) //esta función sirve para evitar que una tapa de garrafón sea identificada como de coca
{
    unsigned char i;

    for(i = 0; i < 100; i++)
    {
        Servo_0();      // genera un periodo de 20 ms
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

    while(1)
    {   
        
        prender_foquitos();
        
        if((SensorArriba == 0) && (SensorAbajo == 0))
        {
            delay_astuto();
            posicion_actual = 'B';
        }
        else if(SensorAbajo == 0)
        {
            posicion_actual = 'A';
        }

        if(posicion_actual == 'A')
        {
            Servo_30();
        }
        else if(posicion_actual == 'B')
        {
            Servo_0();
        }
        else
        {
            Servo_60();
        }
        
    }

    return 0;
}