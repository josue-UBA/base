<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# informacion

- la placa electronica que se esta utilizando es un F401RE.
- el nombre del programa (IDE) es STM32CubeIDE 

![image](https://user-images.githubusercontent.com/30317390/118349393-0bfd7500-b516-11eb-88f6-0e1a5e520305.png)

- Se ingresa al programa y se crea el proyecto con toda la configuracion por default. Este es nuestro proyecto base del cual parten todas las ramas.
- el sistema operativo es Windows 10.
- cada rama tiene su propio archivo README.md. En el se explica en que consiste cada rama.

En este link se encuentra informacion de la placa.

https://os.mbed.com/platforms/ST-Nucleo-F401RE/

link al sistema operativo freertos

https://www.freertos.org/fr-content-src/uploads/2018/07/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf

<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# problemas
- Tener en cuenta hasta el dia de hoy (16-05-2021) no he podido bajar el proyecto y hacer que compile en el IDE. Tener en cuenta porque se va a tener la misma dificultad cuando se intente.

<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# timer e interrupciones

video que hay que seguir: https://www.youtube.com/watch?v=VfbW6nfG4kw

<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# texto-UART

## informacion

- la placa electronica que se esta utilizando es un F401RE.
- el nombre del programa (IDE) es STM32CubeIDE 

![image](https://user-images.githubusercontent.com/30317390/118349393-0bfd7500-b516-11eb-88f6-0e1a5e520305.png)

- Se ingresa al programa y se crea el proyecto con toda la configuracion por default. Este es nuestro proyecto base del cual parten todas las ramas.
- el sistema operativo es Windows 10.
- cada rama tiene su propio archivo README.md. En el se explica en que consiste cada rama.

## enlaces

aquí se explica como trabajar con UART en tarjetas STM32
https://www.youtube.com/watch?v=rkHBXM9AGf4

aqui explican como utilizar la función snprintf
https://stackoverflow.com/questions/5172107/how-to-concatenate-string-and-int-in-c

<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# LCD

## informacion
Nos guiaremos de este video para hacer el trabajo:

https://www.youtube.com/watch?v=rfRJGfK2t-A

En el comentario del video encontramos este link que nos guia a la pagina del autor donde podemos encontrar el codigo.

https://controllerstech.com/i2c-lcd-in-stm32/


<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# B3
## lista de tutorial youtube
https://www.youtube.com/playlist?list=PLfIJKC1ud8gj1t2y36sabPT4YcKzmN_5D

clase 2
NUCLEO-F401RE
## hola
task name: Tarea1_Code
entry function: myTask 1
iddle + 1

task name: Tarea2_Code
entry function: myTask 2
iddle + 1

task name: Tarea_Monitor_Code
entry function: monitor
iddle + 1

ejercicio B3

Tarea A
- tarea: tarea_a
- led: 1 
- Prioridad: +3 
- nombre del handle: task_handle_a
- nombre de la funcion: tarea_A_code

Tarea B
- tarea: tarea_b
- led: 2
- Prioridad: +2 
- nombre del handle: task_handle_b
- nombre de la funcion: tarea_B_code

Tarea C
- tarea: tarea_c
- led: 3
- Prioridad: +1 
- nombre del handle: task_handle_c
- nombre de la funcion: tarea_C_code

Tarea D
- tarea: tarea_d
- led: 4
- Prioridad: +0 
- nombre del handle: task_handle_d
- nombre de la funcion: tarea_D_code

## flujo
--- TA ---
TA suspend 

--- TB ---
blink 3 times led 1
TA resume

--- TA ---
blink 2 times led B
TA suspend 

--- TB ---
TB suspend 

--- TC ---
blink 3 times led 2
TC suspend 

--- TD ---
TA delete
TB delete
TC delete
blink infinete times led 3

<!-- -------------------------------------------------------------------------------------------------------------------------------------- -->
--------------------
# B4
## enlaces

## comentarios
- Al tener las tareas 2 y 3 la misma prioridad los leds prenden casi simultaneamente. Esto es porque ocurre un context switching cada ves que el tick realice una interrupcion.
- Al cambiar el macro configUSE_TIME_SLICING a 0, ocurrira un context switching solo si una tarea de mayor prioridad entra al estado ready o la tarea que esta en el estado "running" entra al estado "blocked" o "suspended"

## solución
lo unico que se hizo fue
- crear una variable global

![image](https://user-images.githubusercontent.com/30317390/118369054-8656e500-b568-11eb-8f3f-b66d4e328ae6.png)

- editar la variable global en la tarea B y crear un while en la tarea C

![image](https://user-images.githubusercontent.com/30317390/118369031-7212e800-b568-11eb-9966-c54aa04d3010.png)

## Proceso

Para solucionar el problema se crea una variable global y le llamamos "bandera".

![image](https://user-images.githubusercontent.com/30317390/118366130-13983a80-b565-11eb-91a8-64d4e7ee860d.png)

Y dentro de la funcion "tarea_C_code" creamos un while que se ejecutara de manera paralela con "tarea_B_code" mientras la variable "bandera" sea 0.

![image](https://user-images.githubusercontent.com/30317390/118366158-2a3e9180-b565-11eb-8896-adbd45693b26.png)

cuando la tarea "tarea_B_code" termine de prender y apagar el led "LED_1", cambiara el valor de la variable "bandera" por 1.

![image](https://user-images.githubusercontent.com/30317390/118366205-696ce280-b565-11eb-9b98-179e2abf6037.png)

antes que se salga del while o inmediatamente despues que se sale del while (depende si es que despues de cambiar el valor de la variable "bandera", la tarea "tarea_B_code" hizo un context switching con ""tarea_C_code" o no)

![image](https://user-images.githubusercontent.com/30317390/118368891-30823d00-b568-11eb-85d4-d475cefd10b4.png)

se resume la tarea "tarea_A_code"

![image](https://user-images.githubusercontent.com/30317390/118368222-830f2980-b567-11eb-95f1-b6d08459faba.png)

la tarea A prende 2 veces el led B y luego se suspende

![image](https://user-images.githubusercontent.com/30317390/118370158-0c285f80-b56c-11eb-938e-7024f57a5e9b.png)

luego la tarea B hace context switching con C y se suspende. Finalmente queda la tarea C con mayor prioridad y prende el LED_2 2 veces.

![image](https://user-images.githubusercontent.com/30317390/118370181-2c581e80-b56c-11eb-9dea-85b7324a1454.png)

Asi queda todo como el ejercicio B3
