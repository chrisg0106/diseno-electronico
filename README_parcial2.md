
Calculadora con STM32 y Teclado Matricial
Introducción
Este proyecto tiene como objetivo implementar una calculadora básica utilizando un microcontrolador STM32, un teclado matricial de 4x4 y 4 displays de 7 segmentos para mostrar los resultados. La calculadora permite realizar operaciones aritméticas simples como suma, resta, multiplicación y división.

Descripción del Proyecto
La calculadora está diseñada para proporcionar una interfaz intuitiva y fácil de usar para realizar operaciones matemáticas básicas. Utiliza un teclado matricial para la entrada de números y operadores, y muestra los resultados en displays de 7 segmentos.

Funcionamiento
La calculadora permite almacenar los valores y luego ser capaz de operar con ellos, para poder hacer que funcione unicamente es necesario conectar el microcontrolador y empezar a presionar las teclas, la calculadora funciona correctamente con valores de dos digitos. 

Un ejemplo de su funcionamiento es presionar un numero de 2 digitos, presionar la D para su almacenamiento, luego presionar el numero con el que se quiere multiplicar y presionar "C" para que devuelva el producto.

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/9e76a47d-e20c-41b3-83f5-fe5f9f41d9d9)


1. Suma (+)
Al presionar el operador de suma "A", la calculadora suma los números ingresados anteriormente y muestra el resultado en los displays de 7 segmentos. Por ejemplo, si se ingresan los números "123" y "456" y luego se presiona "+", la calculadora mostrará "579".


![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/d6f44091-0710-45e0-9527-542361d4c835)



2. Resta (-)
Al presionar el operador de resta "B", la calculadora resta el segundo número ingresado del primero y muestra el resultado en los displays de 7 segmentos. Por ejemplo, si se ingresan los números "100" y "50" y luego se presiona "-", la calculadora mostrará "50".

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/29803f96-8a43-478c-ad1b-01fc36ae610b)



3. Multiplicación (*)
Al presionar el operador de multiplicación "C", la calculadora multiplica los números ingresados anteriormente y muestra el resultado en los displays de 7 segmentos. Por ejemplo, si se ingresan los números "7" y "8" y luego se presiona "*", la calculadora mostrará "56".
Esta permite el resultado de más de 4 digitos.

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/72b77aac-2721-4af6-a98e-9c70345ec2da)



5. División (/)
Al presionar el operador de división "#", la calculadora divide el primer número ingresado por el segundo y muestra el resultado en los displays de 7 segmentos. Es importante tener en cuenta que la división entre cero no está permitida y puede resultar en un comportamiento inesperado.

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/e294df7c-d672-4467-9dcf-dbc3c36a8787)


6. Borrado (E)
La tecla de borrado "*" permite al usuario eliminar los valores ingresados y reiniciar el proceso de ingreso de datos. Al presionar esta tecla, la calculadora borra los números ingresados y reinicia el proceso de entrada de datos. Esto es útil cuando el usuario desea corregir un error o iniciar una nueva operación.
Esta funcion regresa todos los valores a 0.

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/ab948af7-a181-49f4-aca8-110a8bce5aca)



Hardware Utilizado
Microcontrolador: STM32 Nucleo L053R8
Teclado Matricial: 4x4
Displays de 7 Segmentos: 4

Conclusiones
Este proyecto proporciona una solución práctica y funcional para realizar operaciones matemáticas básicas utilizando un microcontrolador STM32 y componentes electrónicos simples. La calculadora implementada es fácil de usar y ofrece una interfaz intuitiva para usuarios de todas las edades.




Ejemplo de funcionamiento de una multiplicación:

1. Se presiona el número 20
![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/fc7f0b0b-f14d-4fa2-adfd-4cc8d2e7f41c)

2. Se presiona la "D" para almacenarlo

![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/ed53fab5-4908-4037-b300-aefd3c82b054)


3.  Se presiona el número por el cual quiero multiplicar el 20, en este caso el 25
![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/b82cd50b-23e9-40c4-93b5-d803d88eb0c5)

4.  Se presiona la tecla "C" para multiplicar dandonos el resultado esperado 500
![image](https://github.com/chrisg0106/diseno-electronico/assets/68700454/3b7bce5d-75f6-406b-b5ca-5f89bc8ea83a)



