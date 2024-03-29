

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

uint8_t myfsm = 0x00;
uint8_t inc_second = 0x00;
uint8_t formato_24_horas = 1; // Inicialmente en formato de 24 horas

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

struct Time_t {
    uint8_t hourDecimal;//
    uint8_t hourUnit;
    uint8_t minuteDecimal;
    uint8_t minuteUnit;
    uint8_t secondDecimal;
    uint8_t secondUnit;
}watch, alarmas[9];

struct Time_t watch = {0}; // Inicializar todas las variables a 0

// Macros para los dígitos del display
#define Zero 0x3F
#define One   0x06
#define Two  0x5B
#define Three  0x4F
#define Four  0x66
#define Five   0x6D
#define Six  0x7D
#define Seven  0x07
#define Eight   0x7F
#define Nine  0x67

void delay_ms(uint16_t n);
uint8_t decoder(uint8_t value_to_decode);
void hour_format();


struct Time_t alarmas[] = {
    {0, 0, 1, 0, 0, 0}, //Colocamos alarmas a 12:10 horas
    {0, 0, 1, 0, 0, 2},
    {0, 0, 1, 0, 0, 4},
    {0, 0, 1, 2, 0, 0},// Colocamos alarmas a 12:12 horas
    {0, 0, 1, 2, 0, 2},
    {0, 0, 1, 2, 0, 4},
	{0, 0, 1, 5, 0, 0}, // Colocamos alarmas a 12:15 horas
	{0, 0, 1, 5, 0, 2},
       {0, 0, 1, 5, 0, 4}
};


const int NUM_ALARMAS = sizeof(alarmas) / sizeof(alarmas[0]);

// Tabla de búsqueda para la función decoder
const uint8_t decoder_table[] = {Zero, One,Two, Three, Four, Five, Six, Seven, Eight, Nine};

void set_time(uint8_t hourDecimal, uint8_t hourUnit, uint8_t minuteDecimal, uint8_t minuteUnit, uint8_t secondDecimal, uint8_t secondUnit) {
    watch.hourDecimal = hourDecimal;
    watch.hourUnit = hourUnit;
    watch.minuteDecimal = minuteDecimal;
    watch.minuteUnit = minuteUnit;
    watch.secondDecimal = secondDecimal;
    watch.secondUnit = secondUnit;
}




int main(void)
{
    // Habilitar el reloj para GPIOC y GPIOB
    RCC->IOPENR |= (1 << 2) | (1 << 1) | (1 << 0);

    // Definir máscaras para los modos de los pines del puerto C como salida
    uint32_t C_masks = (0b01 << 8) |(0b01 << 10) | (0b01 << 12) | (0b01 << 14) | (0b01 << 16) | (0b01 << 18);

    // Limpiar y configurar los modos de los pines del puerto C como salida
    GPIOC->MODER &= ~(0b11 << 8 | 0b11 << 10 | 0b11 << 12 | 0b11 << 14 | 0b11 << 16 | 0b11 << 18);
    GPIOC->MODER |= C_masks;

    // Definir máscaras para los modos de los pines del puerto B como salida
    uint32_t B_masks = (0b01 << 0) | (0b01 << 2) | (0b01 << 4) | (0b01 << 6) |
                           (0b01 << 8) | (0b01 << 10) | (0b01 << 12) | (0b01 << 14);



    // Limpiar y configurar los modos de los pines del puerto B como salida
    GPIOB->MODER &= ~(0xFFFF); // Limpiar todos los bits
    GPIOB->MODER |= B_masks;



    // Definir máscaras para los modos de los pines del puerto C como entrada
    uint32_t C_masks_input = (0b00 << 26); // PC0 como entrada

    // Limpiar y configurar los modos de los pines del puerto C como entrada
    GPIOC->MODER &= ~(0b11 << 26); // Limpiar los bits correspondientes
    GPIOC->MODER |= C_masks_input;

    // Definir máscaras para los modos de los pines del puerto A como entrada
    uint32_t A_masks_input = (0b00 << 20) | (0b00 << 0); // PA10 como entrada

    // Limpiar y configurar los modos de los pines del puerto A como entrada para PA10 y PA2
    GPIOA->MODER &= ~(0b11 << 20); // Limpiar los bits correspondientes para PA10
    GPIOA->MODER &= ~(0b11 << 0);  // Limpiar los bits correspondientes para PA0
    GPIOA->MODER |= A_masks_input; // Aplicar la máscara para configurar PA10 y PA0 como entradas




    // Definir máscaras para los modos de los pines del puerto A como salida
    uint32_t A_masks_output = (0b01 << 12); // PA6 como salida

    // Limpiar y configurar los modos de los pines del puerto A como salida
    GPIOA->MODER &= ~(0b11 << 12); // Limpiar los bits correspondientes
    GPIOA->MODER |= A_masks_output;


    // funcion cambio de formato
    while (1)
    {
    	// Verificar si el botón está presionado para cambiar el formato de la hora
        if ((GPIOA->IDR & (1 << 10)) == 0)
        {
            // Guardar el tiempo actual antes de convertir al formato de 12 horas
            uint8_t temp_hour = watch.hourDecimal * 10 + watch.hourUnit;
            if (formato_24_horas == 1)
            {
                if (temp_hour >= 13)
                {
                    temp_hour -= 12; // Convierte a funcion de 12 horas
                }
                else if (temp_hour == 0)
                {
                    temp_hour = 12; // 12 AM en lugar de 0 AM
                }
            }
            else
            {
                if (temp_hour == 12)
                {
                    temp_hour = 0; // 12 PM en lugar de 12 AM
                }
                else if (temp_hour < 12)
                {
                    temp_hour += 12; // Convertir a formato de 24 horas
                }
            }

            // Mostrar el tiempo en formato de 12 horas
            set_time(temp_hour / 10, temp_hour % 10, watch.minuteDecimal, watch.minuteUnit, watch.secondDecimal, watch.secondUnit);

            // Cambiar el formato de hora para la próxima vez que se presione el botón
            formato_24_horas = !formato_24_horas;

            // Retraso para evitar falsos contactos (debouncing)
            delay_ms(100); // Ajusta este valor según sea necesario
        }


// incremento de los minutos//

        // Verificar si el botón está presionado para incrementar los minutos
        if ((GPIOA->IDR & (1 << 0)) == 0) // Suponiendo que el botón está conectado al pin PC13
        {
            // Incrementar los minutos
            watch.minuteUnit++;

            // Verificar si se necesita llevar a cabo un carry en los minutos
            if (watch.minuteUnit == 10)
            {
                watch.minuteUnit = 0; // Reiniciar las unidades de minuto a cero
                watch.minuteDecimal++; // Incrementar las decenas de minuto

                // Verificar si se necesita llevar a cabo un carry en las decenas de minuto
                if (watch.minuteDecimal == 6)
                {
                    watch.minuteDecimal = 0; // Reiniciar las decenas de minuto a cero
                }
            }

            // Retraso para evitar falsos contactos (debouncing)
            delay_ms(100); // Ajusta este valor según sea necesario
        }


        // Verificar si el botón está presionado para incrementar la hora de las alarmas
        if (!(GPIOC->IDR & (1 << 13))) { // Ajuste para incrementar la hora de las alarmas.
                    for (int i = 0; i < NUM_ALARMAS; i++) {
                        alarmas[i].hourUnit++;
                        if (alarmas[i].hourUnit > 9) {
                            alarmas[i].hourUnit = 0;
                            alarmas[i].hourDecimal++;
                            if (alarmas[i].hourDecimal >= 2 && alarmas[i].hourUnit >= 4) { // Corrección para formato de 24 horas.
                                alarmas[i].hourDecimal = 0;
                                alarmas[i].hourUnit = 0;
                            }
                        }
                    }
                    delay_ms(100); // Debounce delay.
                }


        // Llamar a la función para mostrar la hora en el display
        hour_format();

        for (int i = 0; i < NUM_ALARMAS; i++) {
                    if (watch.hourDecimal == alarmas[i].hourDecimal && watch.hourUnit == alarmas[i].hourUnit &&
                        watch.minuteDecimal == alarmas[i].minuteDecimal && watch.minuteUnit == alarmas[i].minuteUnit &&
                        watch.secondDecimal == alarmas[i].secondDecimal && watch.secondUnit == alarmas[i].secondUnit) {
                        GPIOA->ODR |= (1 << 6); // Encender LED para indicar alarma.
                        break; // Salir del bucle una vez que se encuentra una alarma activa.
                    } else {
                        GPIOA->ODR &= ~(1 << 6); // Asegurar que el LED esté apagado si no hay alarma.
                  }
        	}
    	}
}

void activate_segment_and_digit(uint8_t segment, uint8_t digit) {
    GPIOB->BSRR = 0xFFFF0000; // Limpiar todos los bits del puerto B
    GPIOC->BSRR = 0xFFFF0000; // Limpiar todos los bits del puerto C
    GPIOC->BSRR = 0X01 << segment; // Encender segmento
    GPIOB->BSRR = decoder(digit); // Encender dígito
}


void hour_format() {
    static uint8_t segmentMap[] = {5, 6, 8, 9, 7, 4}; // Mapeo de segmentos a estados

    if (myfsm >= 0 && myfsm < 6) { // Asegurar que myfsm esté dentro del rango
        uint8_t digit = 0;
        switch (myfsm) {
            case 0: digit = watch.secondUnit; break;
            case 1: digit = watch.secondDecimal; break;
            case 2: digit = watch.minuteUnit; break;
            case 3: digit = watch.minuteDecimal; break;
            case 4: digit = watch.hourUnit % 10; break;
            case 5: digit = watch.hourDecimal; break;
        }

        activate_segment_and_digit(segmentMap[myfsm], digit);
    }

    myfsm = (myfsm + 1) % 6; // Circular a través de los estados

    delay_ms(1); // Mantiene el retardo para temporización
    // Incremento del tiempo (mantener la lógica original aquí)
    inc_second++;
    if (inc_second == 250) {
        inc_second = 0;
        // Asume que la lógica para incrementar la hora, minuto y segundo se mantiene sin cambios
        increment_time(); // Función para incrementar el tiempo
    }
}

void increment_time() {
    watch.secondUnit++;
    if (watch.secondUnit >= 10) {
        watch.secondUnit = 0;
        watch.secondDecimal++;
        if (watch.secondDecimal >= 6) {
            watch.secondDecimal = 0;
            watch.minuteUnit++;
            if (watch.minuteUnit >= 10) {
                watch.minuteUnit = 0;
                watch.minuteDecimal++;
                if (watch.minuteDecimal >= 6) {
                    watch.minuteDecimal = 0;
                    watch.hourUnit++;
                    if (watch.hourUnit >= 10 || (watch.hourDecimal == 2 && watch.hourUnit >= 4)) {
                        watch.hourUnit = 0;
                        watch.hourDecimal++;
                        if (watch.hourDecimal >= 3) {
                            watch.hourDecimal = 0; // Reiniciar a 00 después de 24 horas
                        }
                    }
                }
            }
        }
    }
}


void delay_ms(uint16_t n)
{
    uint16_t i;
    for (; n > 0; n--)
    {
        for (i = 0; i < 140; i++)
        {
            // Pequeño retraso para lograr una pausa de 1 ms aproximadamente
            asm("NOP");
        }
    }
}

uint8_t decoder(uint8_t value_to_decode)
{
    if (value_to_decode < 10)
    {
        return decoder_table[value_to_decode];
    }
    else
    {
        // Manejar el caso de un valor fuera de rango
        return 0;
    }
}
