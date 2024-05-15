# Calculadora con STM32 Nucleo L053R8

Este proyecto implementa una calculadora utilizando un microcontrolador STM32 Nucleo L053R8. La calculadora utiliza un teclado matricial 4x4 para la entrada de datos y muestra los resultados en displays de 7 segmentos. Adicionalmente, se implementa comunicación serial para mostrar mensajes en una consola.

## Características

- **Teclado Matricial 4x4**: Permite la entrada de números y operaciones.
- **Displays de 7 Segmentos**: Muestra los dígitos y resultados de las operaciones.
- **Operaciones Aritméticas**:
  - **Suma**: Presionando la tecla `A`.
  - **Resta**: Presionando la tecla `B`.
  - **Multiplicación**: Presionando la tecla `C`.
  - **División**: Presionando la tecla `F`.
- **Comunicación Serial**: Muestra los mensajes de operación y resultados en una consola serial.

## Esquema de Funcionamiento

1. **Ingreso de Dígitos**:
   - Se ingresan los números a través del teclado matricial.
   - Los números se muestran en los displays de 7 segmentos.
   
2. **Almacenamiento de Valores**:
   - Presionando la tecla `D` se almacenan los valores ingresados.
   
3. **Operaciones Aritméticas**:
   - Después de almacenar el primer número, se ingresa el segundo número.
   - Presionando las teclas `A`, `B`, `C`, o `F` se realizan las operaciones aritméticas.
   - Los resultados se muestran en los displays y en la consola serial.

4. **Borrar**:
   - Presionando la tecla `E` se borran los displays y la consola serial.

## Hardware Necesario

- STM32 Nucleo L053R8
- Teclado matricial 4x4
- Displays de 7 segmentos
- Conexión para comunicación serial (por ejemplo, un cable USB a TTL)

## Código Fuente

El código fuente del proyecto se encuentra en el archivo `examenfinal.c`. A continuación se presenta una descripción general de las funciones principales:

### Funciones Principales

- `main()`: Función principal que contiene el bucle `while` para el manejo de la calculadora.
- `itoa(int num, char* str)`: Convierte un entero a una cadena.
- `enviar_mensaje_resultado(const char* mensaje_base, int valor)`: Construye y envía un mensaje combinando un mensaje base y un valor numérico.
- `gnfk(uint8_t key)`: Obtiene el número correspondiente a una tecla del teclado matricial.
- `gkfn(uint8_t number)`: Obtiene la tecla correspondiente a un número.
- `dms(uint16_t n)`: Introduce un retardo en milisegundos.
- `udv()`: Actualiza los displays de 7 segmentos.
- `USART2_write(uint8_t ch)`: Escribe un carácter en la consola serial.
- `USART2_read()`: Lee un carácter de la consola serial.
- `USART2_putstring(uint8_t* StringPtr)`: Envía una cadena a la consola serial.

### Ejemplo de Uso

```c
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
                    // (Lógica de almacenamiento de valores)
                }
                else if (kn == KA) // Función suma
                {
                    // (Lógica de suma)
                }
                else if (kn == KB) // Función de resta
                {
                    // (Lógica de resta)
                }
                else if (kn == KC) // Función de multiplicación
                {
                    // (Lógica de multiplicación)
                }
                else if (kn == KF) // Función de división
                {
                    // (Lógica de división)
                }
                else if (kn == KE) // Limpia los displays y no lo almacena
                {
                    storedvalue = 0;
                    fdd = sdd = tdd = frdd = 0xff;
                    enviar_mensaje_resultado("Dígitos borrados", 0);
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
