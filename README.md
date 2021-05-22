# lista de tutorial youtube
https://www.youtube.com/playlist?list=PLfIJKC1ud8gj1t2y36sabPT4YcKzmN_5D

#  cuando comience a hacer el programa necesitare esto
https://os.mbed.com/platforms/ST-Nucleo-F401RE/

clase 2
NUCLEO-F401RE
# hola
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

# flujo
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
