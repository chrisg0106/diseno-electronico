#include <stdint.h>


//Peripheral & bus address
#define PERIPHERAL_BASE_ADDRESS 0x40000000U
#define AHB_BASE_ADDRESS 	(PERIPHERAL_BASE_ADDRESS + 0x00020000U)

//RCC BASE ADDRESS

#define RCC_BASE_ADDRESS	(AHB_BASE_ADDRESS + 0x00001000U)
#define RCC_IOPENR_ADDRESS	(RCC_BASE_ADDRESS + 0x0000002CU)

//IOPORT BASE ADDRESS
#define IOPORT_BASE_ADDRESS	(PERIPHERAL_BASE_ADDRESS + 0x10000000U)

//GPIO BASE & SPECIFIC ADDRESS
#define GPIOA_BASE_ADDRESS (IOPORT_BASE_ADDRESS + 0x00000000U)
#define GPIOA_MODER_REG	(GPIOA_BASE_ADDRESS + 0x00000000U)
#define GPIOA_ODR_REG (GPIOA_BASE_ADDRESS + 0x00000014U)

#define GPIOC_BASE_ADDRESS (IOPORT_BASE_ADDRESS + 0x00000800U)
#define GPIOC_MODER_REG	(GPIOC_BASE_ADDRESS + 0x00000000U)
#define GPIOC_ODR_REG (GPIOC_BASE_ADDRESS + 0x00000014U)
#define GPIOC_IDR_REG (GPIOC_BASE_ADDRESS + 0x00000010U)
#define GPIOC_PUPD_REG (GPIOC_BASE_ADDRESS + 0x0000000CU)

void delay_ms(uint16_t n);

int main(void)
{
    /* Loop forever */
	uint32_t *ptr_rcc_iopenr =  RCC_IOPENR_ADDRESS; //se crea un pointer para iopenr
	uint32_t *ptr_gpioa_moder = GPIOA_MODER_REG; //se crea un pointer variable para GPIOA MODER
	uint32_t *ptr_gpioa_odr = GPIOA_ODR_REG;  // SE crea un pointer variable para GPIOA ODR

	uint32_t *ptr_gpioc_moder = GPIOC_MODER_REG; //se crea un pointer variable para GPIOC MODER
	uint32_t *ptr_gpioc_idr = GPIOC_IDR_REG; //se crea un pointer variable para GPIOC IDR
	//uint32_t *ptr_gpioc_pupd = GPIOC_PUPD_REG; //se crea un pointer variable para GPIOC IDR
	uint32_t *ptr_gpioc_odr = GPIOC_ODR_REG;


	*ptr_rcc_iopenr |= 1<<0;//habilita el clock de GPIOA
	*ptr_rcc_iopenr |= 1<<2;//habilita el clock de GPIOC

	//GPIOA PA5
	//*ptr_gpioa_moder &= ~(1<<11);//coloca el pin como salida

	//GPIOC PC7
	*ptr_gpioc_moder &= ~(1<<14);//coloca el pin como entrada
	*ptr_gpioc_moder &= ~(1<<15);//coloca el pin como entrada

	//GPIOC PC8
	*ptr_gpioc_moder &= ~(1<<16);//coloca el pin como entrada
	*ptr_gpioc_moder &= ~(1<<17);//coloca el pin como entrada
	//GPIOC PC9
	*ptr_gpioc_moder &= ~(1<<19);//coloca el pin como salida
	//GPIOC PC0
	*ptr_gpioc_moder &= ~(1<<0);//coloca el pin como entrada
	*ptr_gpioc_moder &= ~(1<<1);//coloca el pin como entrada

	//Colocarlo como pull up
	//*ptr_gpioc_pupd |= (1<<1);

	uint8_t j=0; //contador


	while(1){
		if(*ptr_gpioc_idr & (1<<8)  )//Activa el puerto PC8
			{

					*ptr_gpioc_odr &= ~(1<<9);
						delay_ms(200);
					*ptr_gpioc_odr	|= 1<<9;
						delay_ms(200);
				}
				else{
					*ptr_gpioc_odr &= ~(1<<9);
					delay_ms(500);
					*ptr_gpioc_odr	|= 1<<9;
					delay_ms(500);

				}

		if(*ptr_gpioc_idr & (1<<7)  )//Activa el puerto PC8
					{

							j++; //aumenta el valor del contador
							printf("Valor de j: %d\n", j); //Imprime en la terminal el valor del contador
						}

	}

}
void delay_ms(uint16_t n){
		uint16_t i;
		for(; n>0; n--)
			for (i=0; i<140; i++);

	}


