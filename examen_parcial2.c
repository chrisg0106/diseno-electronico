/*
 * Finalparcial2.c
 *
 *  Created on: Mar 30, 2024
 *      Author: chris
 */


#include <stdint.h>

// Definiciones de registros y direcciones
#define PERIPHERAL_BASE_ADDRESS     0x40000000U
#define AHB_BASE_ADDRESS            (PERIPHERAL_BASE_ADDRESS + 0x00020000U)
#define RCC_BASE_ADDRESS            (AHB_BASE_ADDRESS + 0x00001000U)
#define RCC_IOPENR_ADDRESS          (RCC_BASE_ADDRESS + 0x0000002CU)
#define IOPORT_ADDRESS              (PERIPHERAL_BASE_ADDRESS + 0x10000000U)
#define GPIOA_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000000U)
#define GPIOB_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000400U)
#define GPIOC_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000800U)

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE_ADDRESS)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE_ADDRESS)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE_ADDRESS)
#define RCC ((RCC_RegDef_t*)RCC_BASE_ADDRESS)

typedef struct
{
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t LCKR;
    uint32_t AFR[2];
    uint32_t BRR;
} GPIO_RegDef_t;

typedef struct
{
    uint32_t CR;
    uint32_t ICSCR;
    uint32_t CRRCR;
    uint32_t CFGR;
    uint32_t CIER;
    uint32_t CIFR;
    uint32_t CICR;
    uint32_t IOPRSTR;
    uint32_t AHBRSTR;
    uint32_t APB2RSTR;
    uint32_t APB1RSTR;
    uint32_t IOPENR;
} RCC_RegDef_t;

// Macros para los dígitos del display
#define cero  0x3F
#define uno   0x06
#define dos   0x5B
#define tres  0x4F
#define cuatro  0x66
#define cinco   0x6D
#define seis   0x7D
#define siete  0x07
#define ocho   0x7F
#define nueve  0x67
#define A      0x77
#define B      0x7C
#define C      0x39
#define D      0x4E
#define E      0x79
#define F      0x71

// Definiciones de los pines de las teclas del teclado matricial
#define KA 3
#define KB 7
#define KC 11
#define KD 15
#define KF 14
#define KE 12

// Declaración de funciones
void dms(uint16_t n);
void udv(void);
uint8_t dc(uint8_t value_to_decode);
void dd(uint8_t value);

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
                    // Manejo de las teclas presionadasz

                    //funcion para almacenar los valores
                    if (kn == KD) //Tecla D presionada

                    {

                        int sv = 0; // Valor almacenado

                        int uv = gnfk(fdd); // Unidad

                        int dv = gnfk(sdd);//Decena

                        int cv = gnfk(tdd);//Centena


                        sv = uv + (dv * 10) + (cv * 100); // Suma los valores de las unidades,decenas y centenas

                        storedvalue = (uint8_t)(storedvalue + sv); //almacena los valores

                        fdd = sdd = tdd = frdd = 0xff; //limpia los displays
                    }
                    //Funcion suma cuando se presiona A luego de almacenar los datos
                    else if (kn == KA)
                    {
                        if (storedvalue != 0)
                        {

                            int sv = 0;// Valor almacenado

                            int uv = gnfk(fdd);// Unidad
                            int dv = gnfk(sdd);//Decena
                            int cv = gnfk(tdd);//Centena


                            sv = sv + uv + (dv * 10) + (cv * 100);

                            storedvalue = (uint8_t)(storedvalue + sv);

                            uv = storedvalue % 10;                   //Valor en unidades
                            dv = ((storedvalue - uv) % 100) / 10; //Valor en decenas
                            cv = ((storedvalue - dv - uv) % 1000) / 100; //centenas
                            //Despliega los valores luego de sumarlos
                            fdd = gkfn(uv); //Despliega en el primer digito del display el valor de la unidad
                            sdd = gkfn(dv);//Despliega en el primer digito del display el valor de la decena
                            tdd = gkfn(cv);//Despliega en el primer digito del display el valor de la decena

                            storedvalue = 0;

                        }
                    }

                    else if (kn == KB)
                    {
                        if (storedvalue != 0)
                        {


                            int sv = 0;

                            int uv = gnfk(fdd);
                            int dv = gnfk(sdd);
                            int cv = gnfk(tdd);


                            sv = sv + uv + (dv * 10) + (cv * 100);

                            storedvalue = (uint8_t)(storedvalue - sv); //Luego de almacenarlo resta los datos almacenados con los nuevos

                            uv = storedvalue % 10;                   //Valor en unidades
                            dv = ((storedvalue - uv) % 100) / 10; //Valor en decenas
						   cv = ((storedvalue - dv - uv) % 1000) / 100; //centenas
						   //Despliega los valores luego de sumarlos
						   fdd = gkfn(uv); //Despliega en el primer digito del display el valor de la unidad
						   sdd = gkfn(dv);//Despliega en el primer digito del display el valor de la decena
						   tdd = gkfn(cv);//Despliega en el primer digito del display el valor de la centena

                            storedvalue = 0;

                        }
                    }


                    else if (kn == KC)
                    {
                        if (storedvalue != 0)
                        {
                            int pv = 0; //Producto de la multiplicación

                            int uv = gnfk(fdd);
                            int dv = gnfk(sdd);
                            int cv = gnfk(tdd);

                            int mv = gnfk(frdd); // Miles

                            int iv = uv + (dv * 10) + (cv * 100) + (mv * 1000);

                            pv = storedvalue * iv; // Genera la multiplicacion de los valores
                            //Se implementa el uso de mas valores en los displays para multiplicaciones de 2 x 2 digitos
                            uv = pv % 10; //valor del producto en unidades
                            dv = (pv / 10) % 10; //valor del producto en decenas
                            cv = (pv / 100) % 10;//valor del producto en centenas
                            mv = (pv / 1000) % 10;//valor del producto en miles

                            fdd = gkfn(uv); //Despliega en el primer digito del display el valor de la unidad
						   sdd = gkfn(dv);//Despliega en el primer digito del display el valor de la decena
						   tdd = gkfn(cv);//Despliega en el primer digito del display el valor de la centena

                            frdd = gkfn(mv);//Despliega en el primer digito del display el valor de los miles

                            storedvalue = 0;
                        }
                    }

                    else if (kn == KF) { //Division

                        int divv = 0;

                        int uv = gnfk(fdd);
                        int dv = gnfk(sdd);
                        int cv = gnfk(tdd);

                        divv = uv + (dv * 10) + (cv * 100);
                        if (storedvalue != 0 && divv != 0) { //Se asegura que la division no de error en 0, cuando se divide en 0 da 0
                            storedvalue = (uint8_t)(storedvalue / divv);

                            uv = storedvalue % 10;
                            dv = (storedvalue / 10) % 10;
                            cv = (storedvalue / 100) % 10;

                            fdd = gkfn(uv);
                            sdd = gkfn(dv);
                            tdd = gkfn(cv);
                        }
                        storedvalue = 0;
                    }

                    else if (kn == KE) { //Limpia los displays y no lo almacena
                        storedvalue = 0;
                        fdd = sdd = tdd = frdd = 0xff;
                    }


                    else

                    { //Desplaza los valores si no se tiene ningun operador funcionando
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
