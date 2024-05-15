/*
 * stm32lm053xx.h
 *
 *  Created on: May 14, 2024
 *      Author: chris
 */

#ifndef STM32LM053XX_H_

#include <stdint.h>
#define STM32LM053XX_H_

#define PERIPHERAL_BASE_ADDRESS     0x40000000U
#define AHB_BASE_ADDRESS            (PERIPHERAL_BASE_ADDRESS + 0x00020000U)
#define APB1_BASE_ADDRESS 			(PERIPHERAL_BASE_ADDRESS + 0x00000000U)

#define RCC_BASE_ADDRESS            (AHB_BASE_ADDRESS + 0x00001000U)


#define RCC_IOPENR_ADDRESS          (RCC_BASE_ADDRESS + 0x0000002CU)
#define IOPORT_ADDRESS              (PERIPHERAL_BASE_ADDRESS + 0x10000000U)
#define GPIOA_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000000U)
#define GPIOB_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000400U)
#define GPIOC_BASE_ADDRESS          (IOPORT_ADDRESS + 0x00000800U)
#define USART2_BASE_ADDRESS			(APB1_BASE_ADDRESS + 0x00004400U)



#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE_ADDRESS)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE_ADDRESS)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE_ADDRESS)
#define RCC ((RCC_RegDef_t*)RCC_BASE_ADDRESS)


#define USART2 ((USART_RegDef_t*)USART2_BASE_ADDRESS)




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
    uint32_t AHBENR;
    uint32_t APB2ENR;
    uint32_t APB1ENR;
    uint32_t IOPSMENR;
    uint32_t AHBSMENR;
    uint32_t APB2SMENR;
    uint32_t APB1SMENR;
    uint32_t CCIPR;
    uint32_t CSR;
} RCC_RegDef_t;




typedef struct{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t CR3;
	uint32_t BRR;
	uint32_t GTPR;
	uint32_t RTOR;
	uint32_t RQR;
	uint32_t ISR;
	uint32_t ICR;
	uint32_t RDR;
	uint32_t TDR;
}USART_RegDef_t;



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




#endif /* STM32LM053XX_H_ */
