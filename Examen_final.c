/*
 * examenfinalarreglado.c
 *
 *  Created on: May 14, 2024
 *      Author: chris
 */

#include <stdint.h>
#include "stm32lm053xx.h"

void dms(uint16_t n);
void udv(void);
uint8_t dc(uint8_t value_to_decode);
void dd(uint8_t value);
void USART2_write(uint8_t ch);
void USART2_putstring(uint8_t* StringPtr);
uint8_t USART2_read(void);

// Variables globales para el manejo de valores
uint8_t fdd = 0xFF; // valor en el display primer digito
uint8_t sdd = 0xFF;// valor en el display segundo digito
uint8_t tdd = 0xFF;// valor en el display tercer digito
uint8_t frdd = 0xFF;// valor en el display cuarto digito
uint8_t fddisplay = 0xFF;// valor en el display quinto digito
uint8_t storedvalue = 0; // valor a almacenar
int productvalue = 0; // producto para la multiplicacion




// Definición de la tabla de correspondencia entre teclas y números
const uint8_t ktd[] = {
    uno, dos, tres, A, // Columna 1: 1, 2, 3, A
    cuatro, cinco, seis, B, // Columna 2: 4, 5, 6, B
    siete, ocho, nueve, C, // Columna 3: 7, 8, 9, C
    E, cero, F, D // Columna 4: *, 0, #, D
};

// Función para obtener el número correspondiente a una tecla
uint8_t gnfk(uint8_t key){
    switch (key){
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 4: return 4;
        case 5: return 5;
        case 6: return 6;
        case 8: return 7;
        case 9: return 8;
        case 10: return 9;
        case 13: return 0;
        default: return 0;
    }
}

// Función para obtener la tecla correspondiente a un número
uint8_t gkfn(uint8_t number){
    switch (number){
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        case 4: return 4;
        case 5: return 5;
        case 6: return 6;
        case 7: return 8;
        case 8: return 9;
        case 9: return 10;
        case 0: return 13;
        default: return 13;
    }
}


void itoa(int num, char* str) {
    int i = 0;
    int isNegative = 0;

    // Manejo de número cero
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Manejo de números negativos
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Procesa el número
    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }

    // Añade el signo negativo si es necesario
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Añade el terminador nulo

    // Invierte la cadena
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void enviar_mensaje_resultado(const char* mensaje_base, int valor) {
    char mensaje_completo[100];
    char valor_str[10];

    itoa(valor, valor_str);
    strcpy(mensaje_completo, mensaje_base);
    strcat(mensaje_completo, valor_str);

    USART2_putstring((uint8_t *)mensaje_completo);

}



int main(void)
{

	//La configuracion de estas máscaras se tomo del proyecto del reloj
	// Habilitar el reloj para GPIOC y GPIOB
	    RCC->IOPENR |= (1 << 2) | (1 << 1) | (1 << 0);

	    // Configurar los pines de GPIOC como salida para los displays
	    uint32_t portC_masks = (0b01 << 8) | (0b01 << 10) | (0b01 << 12) | (0b01 << 14) | (0b01 << 16) | (0b01 << 18);
	    GPIOC->MODER &= ~(0b11 << 8 | 0b11 << 10 | 0b11 << 12 | 0b11 << 14 | 0b11 << 16 | 0b11 << 18);
	    GPIOC->MODER |= portC_masks;

	    // Configurar los pines de GPIOB como salida para los segmentos de los displays
	    GPIOB->MODER &= ~(0xFFFF); // Limpiar los modos de los primeros 8 pines (pines 0 a 7)
	    uint32_t portB_display_masks = (0b01 << 0) | (0b01 << 2) | (0b01 << 4) | (0b01 << 6) |
	                                   (0b01 << 8) | (0b01 << 10) | (0b01 << 12) | (0b01 << 14);
	    GPIOB->MODER |= portB_display_masks; // Establecer los pines del display como salida

	    // Configurar los pines de GPIOB como salida para controlar las columnas del teclado
	    GPIOB->MODER &= ~((0b11 << 24) | (0b11 << 26) | (0b11 << 28) | (0b11 << 30)); // Limpiar la configuración actual para los pines 12 a 15
	    GPIOB->MODER |= ((0b01 << 24) | (0b01 << 26) | (0b01 << 28) | (0b01 << 30)); // Establecer los pines PB12 a PB15 como salidas

	    // Configurar los pines del 16 al 22 de GPIOB con resistencias pull-up
	    GPIOB->PUPDR &= ~((0b11 << 16) | (0b11 << 18) | (0b11 << 20) | (0b11 << 22)); // Limpiar configuración de pull-up/pull-down
	    GPIOB->PUPDR |= ((0b01 << 16) | (0b01 << 18) | (0b01 << 20) | (0b01 << 22)); // Establecer pull-up para pines de entrada

	    // Configurar los pines PB8 a PB11 como entrada (teclado matricial)
	    GPIOB->MODER &= ~((0b11 << 16) | (0b11 << 18) | (0b11 << 20) | (0b11 << 22)); // Limpiar la configuración actual para los pines 8 a 11


	    RCC -> IOPENR |= 1<<0;


	    	//TEST GPIOA PA5
	    	GPIOA ->MODER &= ~(1<<11);

	    	//USART2 PIN CONFIG
	    	//ALTERNATE FUNCTION MODE PA2 Y PA3
	    	GPIOA ->MODER &= ~(1<<4);
	    	GPIOA ->MODER &= ~(1<<6);

	    	//SELECTION OF AF
	    	GPIOA -> AFR[0] |= 1<<10;
	    	GPIOA -> AFR[0] |= 1<<14;

	    	//USART Clock enable
	    	RCC->APB1ENR |= 1<<17;


	    	//Baud rate
	    	USART2->BRR =218;

	    	//ENABLE TX & RX
	    	USART2 ->CR1 |= (1<<2) |(1<<3);

	    	USART2 ->CR1 |= 1<<0;
	// inicia parte de la logica de la ca
	    	while (1)
	    	{
	    	    for (int c = 0; c < 4; c++)
	    	    {
	    	        GPIOB->ODR = (0xF << 12);
	    	        GPIOB->ODR &= ~(1 << (12 + c));
	    	        dms(10);

	    	        uint32_t rows = GPIOB->IDR & (0xF << 8);

	    	        for (int row = 0; row < 4; row++)
	    	        {
	    	            if (!(rows & (1 << (row + 8))))
	    	            {
	    	                uint8_t kn = c * 4 + row;
	    	                // Manejo de las teclas presionadas

	    	                // Función para almacenar los valores
	    	                if (kn == KD) // Tecla D presionada
	    	                {
	    	                    int sv = 0; // Valor almacenado
	    	                    int uv = gnfk(fdd); // Unidad
	    	                    int dv = gnfk(sdd); // Decena
	    	                    int cv = gnfk(tdd); // Centena

	    	                    sv = uv + (dv * 10) + (cv * 100); // Suma los valores de las unidades, decenas y centenas
	    	                    storedvalue = (uint8_t)(storedvalue + sv); // Almacena los valores

	    	                    fdd = sdd = tdd = frdd = 0xff; // Limpia los displays
	    	                }
	    	                // Función suma cuando se presiona A luego de almacenar los datos
	    	                else if (kn == KA)
	    	                {
	    	                    if (storedvalue != 0)
	    	                    {
	    	                        int sv = 0; // Valor almacenado
	    	                        int uv = gnfk(fdd); // Unidad
	    	                        int dv = gnfk(sdd); // Decena
	    	                        int cv = gnfk(tdd); // Centena

	    	                        sv = uv + (dv * 10) + (cv * 100);

	    	                        enviar_mensaje_resultado("Sumando: ", storedvalue);
	    	                        enviar_mensaje_resultado(" + ", sv);

	    	                        storedvalue = (uint8_t)(storedvalue + sv);

	    	                        uv = storedvalue % 10; // Valor en unidades
	    	                        dv = ((storedvalue - uv) % 100) / 10; // Valor en decenas
	    	                        cv = ((storedvalue - dv - uv) % 1000) / 100; // Centenas

	    	                        // Despliega los valores luego de sumarlos
	    	                        fdd = gkfn(uv); // Despliega en el primer dígito del display el valor de la unidad
	    	                        sdd = gkfn(dv); // Despliega en el primer dígito del display el valor de la decena
	    	                        tdd = gkfn(cv); // Despliega en el primer dígito del display el valor de la centena

	    	                        enviar_mensaje_resultado("Resultado de la suma: ", storedvalue);

	    	                        storedvalue = 0;
	    	                    }
	    	                }
	    	                else if (kn == KB) // Función de resta al presionar B
	    	                {
	    	                    if (storedvalue != 0)
	    	                    {
	    	                        int sv = 0;
	    	                        int uv = gnfk(fdd);
	    	                        int dv = gnfk(sdd);
	    	                        int cv = gnfk(tdd);

	    	                        sv = uv + (dv * 10) + (cv * 100);

	    	                        enviar_mensaje_resultado("Restando: ", storedvalue);
	    	                        enviar_mensaje_resultado(" - ", sv);

	    	                        storedvalue = (uint8_t)(storedvalue - sv); // Luego de almacenarlo resta los datos almacenados con los nuevos

	    	                        uv = storedvalue % 10; // Valor en unidades
	    	                        dv = ((storedvalue - uv) % 100) / 10; // Valor en decenas
	    	                        cv = ((storedvalue - dv - uv) % 1000) / 100; // Centenas

	    	                        // Despliega los valores luego de restarlos
	    	                        fdd = gkfn(uv); // Despliega en el primer dígito del display el valor de la unidad
	    	                        sdd = gkfn(dv); // Despliega en el primer dígito del display el valor de la decena
	    	                        tdd = gkfn(cv); // Despliega en el primer dígito del display el valor de la centena

	    	                        enviar_mensaje_resultado("Resultado de la resta: ", storedvalue);

	    	                        storedvalue = 0;
	    	                    }
	    	                }
	    	                else if (kn == KC) // Función de multiplicación
	    	                {
	    	                    if (storedvalue != 0)
	    	                    {
	    	                        int pv = 0; // Producto de la multiplicación
	    	                        int uv = gnfk(fdd);
	    	                        int dv = gnfk(sdd);
	    	                        int cv = gnfk(tdd);
	    	                        int mv = gnfk(frdd); // Miles

	    	                        int iv = uv + (dv * 10) + (cv * 100) + (mv * 1000);

	    	                        enviar_mensaje_resultado("Multiplicación de los siguientes números: ", storedvalue);
	    	                        enviar_mensaje_resultado(" * ", iv);

	    	                        pv = storedvalue * iv; // Genera la multiplicación de los valores
	    	                        // Se implementa el uso de más valores en los displays para multiplicaciones de 2 x 2 dígitos
	    	                        uv = pv % 10; // Valor del producto en unidades
	    	                        dv = (pv / 10) % 10; // Valor del producto en decenas
	    	                        cv = (pv / 100) % 10; // Valor del producto en centenas
	    	                        mv = (pv / 1000) % 10; // Valor del producto en miles

	    	                        fdd = gkfn(uv); // Despliega en el primer dígito del display el valor de la unidad
	    	                        sdd = gkfn(dv); // Despliega en el primer dígito del display el valor de la decena
	    	                        tdd = gkfn(cv); // Despliega en el primer dígito del display el valor de la centena
	    	                        frdd = gkfn(mv); // Despliega en el primer dígito del display el valor de los miles

	    	                        enviar_mensaje_resultado("Resultado de la multiplicación: ", pv);

	    	                        storedvalue = 0;
	    	                    }
	    	                }
	    	                else if (kn == KF) // División
	    	                {
	    	                    int divv = 0;
	    	                    int uv = gnfk(fdd);
	    	                    int dv = gnfk(sdd);
	    	                    int cv = gnfk(tdd);

	    	                    divv = uv + (dv * 10) + (cv * 100);

	    	                    enviar_mensaje_resultado("Se dividen los siguientes números: ", storedvalue);
	    	                    enviar_mensaje_resultado(" / ", divv);

	    	                    if (storedvalue != 0 && divv != 0) // Se asegura que la división no dé error en 0
	    	                    {
	    	                        storedvalue = (uint8_t)(storedvalue / divv);

	    	                        uv = storedvalue % 10;
	    	                        dv = (storedvalue / 10) % 10;
	    	                        cv = (storedvalue / 100) % 10;

	    	                        fdd = gkfn(uv);
	    	                        sdd = gkfn(dv);
	    	                        tdd = gkfn(cv);
	    	                    }

	    	                    enviar_mensaje_resultado("Resultado de la división: ", storedvalue);

	    	                    storedvalue = 0;
	    	                }
	    	                else if (kn == KE) // Limpia los displays y no lo almacena
	    	                {
	    	                    storedvalue = 0;
	    	                    fdd = sdd = tdd = frdd = 0xff;
	    	                    enviar_mensaje_resultado("Se borraron los displays", 0);
	    	                }
	    	                else
	    	                {
	    	                    // Desplaza los valores si no se tiene ningún operador funcionando
	    	                    frdd = tdd;
	    	                    tdd = sdd;
	    	                    sdd = fdd;
	    	                    fdd = kn;
	    	                }

	    	                while (!(GPIOB->IDR & (1 << (row + 8))));
	    	                break;
	    	            }
	    	        }
	    	        GPIOB->ODR |= (1 << (12 + c));
	    	    }

	    	    udv();
	    	    storedvalue = storedvalue % 1000;
	    	}

}

void dms(uint16_t n) {
    for (uint32_t i = 0; i < n * 10; i++) {
        __asm__("NOP");
    }
}

void udv(void) {// Muestra como se veran los valores en los displays, asi mismo apaga los demás displays cuando no se usa

    if (frdd != 0xFF) {
    	//apaga los demas displays
        GPIOC->BSRR = 1 << (5 + 16);
        GPIOC->BSRR = 1 << (6 + 16);
        GPIOC->BSRR = 1 << (8 + 16);
        GPIOC->BSRR = 1 << (7 + 16);
        //Enciende el display a utilizar
        GPIOC->BSRR = 1 << 9;

        GPIOB->BSRR = 0xFF << 16;
        GPIOB->BSRR = dc(frdd);
        dms(5);
    }

    if (tdd != 0xFF) {
        GPIOC->BSRR = 1 << (5 + 16);
        GPIOC->BSRR = 1 << (6 + 16);
        GPIOC->BSRR = 1 << (9 + 16);
        GPIOC->BSRR = 1 << (7 + 16);

        GPIOC->BSRR = 1 << 8;

        GPIOB->BSRR = 0xFF << 16;
        GPIOB->BSRR = dc(tdd);
        dms(5);
    }

    if (sdd != 0xFF) {
        GPIOC->BSRR = 1 << (5 + 16);
        GPIOC->BSRR = 1 << (8 + 16);
        GPIOC->BSRR = 1 << (9 + 16);
        GPIOC->BSRR = 1 << (7 + 16);

        GPIOC->BSRR = 1 << 6;

        GPIOB->BSRR = 0xFF << 16;
        GPIOB->BSRR = dc(sdd);
        dms(5);
    }

    if (fdd != 0xFF) {
        GPIOC->BSRR = 1 << (6 + 16);
        GPIOC->BSRR = 1 << (8 + 16);
        GPIOC->BSRR = 1 << (9 + 16);
        GPIOC->BSRR = 1 << (7 + 16);

        GPIOC->BSRR = 1 << 5;

        GPIOB->BSRR = 0xFF << 16;
        GPIOB->BSRR = dc(fdd);
        dms(5);
    }
}



uint8_t dc(uint8_t number){
	uint8_t key = 0;
//Nos indica que valores va a tomar cada posicion presionada, no se incluyen los operadores
    	switch (number){
    		case 0: key = 0; break;
    		case 1: key = 1; break;
    		case 2: key = 2; break;
    		case 4: key = 4; break;
    		case 5: key = 5; break;
    		case 6: key = 6; break;
    		case 8: key = 8; break;
    		case 9: key = 9; break;
    		case 10: key = 10; break;
    		case 13: key = 13; break;
    		default: return 13;
    	}
    	return ktd[key];
}


void dd(uint8_t value) {
    // Verifica si el valor almacenado es un dígito válido (excluyendo A, B, C, D, * y #)
    if (value < 10) {
        // Actualiza los displays con el valor almacenado
    	tdd = sdd;
        sdd = fdd; // Mueve el primer dígito al segundo dígito
        fdd = value; // Establece el primer dígito como el valor almacenado
    }
}


void USART2_write(uint8_t ch){

	while(!(USART2->ISR & 0x0080)){}
	USART2->TDR = ch;
}


uint8_t USART2_read(void){

	while(!(USART2->ISR & 0x0020)){}
	return USART2->RDR;

}


void USART2_putstring(uint8_t* StringPtr){
	while(*StringPtr != 0x00)
	{
		USART2_write(*StringPtr);
		StringPtr++;
	}
	USART2_write(0x0D);
	USART2_write(0x0A);
}

