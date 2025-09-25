# *****- INTERFAZ DE LA LINEA DE COMANDOS -*****
# ----- LA SHELL DE BASH
# --- Linea de comandos
# *Ejercicio 1:* Arrancar un terminal y comprobar el prompt de la shell. 
# Cada usuario tiene un prompt distinto configurable. 
# Para cambiar de usuario podemos usar el programa [su]. (super user)
# Ejecutar su - y comprobar si cambia el prompt, regresar al usuario inicial con la orden exit. 
# El comando exit termina la ejecución de la shell y devuelve opcionalmente un código de retorno.
 Para ver el prompt (entorno de la shell): > env, si tiviesemos los permisos para hacer su podríamos cambiar el prompt

# --- Páginas de manual
 > man 1 time
 /[palabra a buscar] te busca palabra en el manual

# --- Comandos y secuencias de comandos
!!!!! control+C sale de la instrucción que estés haciendo en ese momento, nueva instrucción
# *Ejercicio 1:*  Imprimir la frase “Curso de Sistemas Operativos” con un tabulador al principio.
-e pone salto de linea al final en una instrucción echo
> echo -e 'Hola' (salto de linea)
-n quita el salto de linea

# *Ejercicio 2:*
> echo -e 'Curso de\n Sistemas\n Operativos'

# *Ejercicio 3:*
> echo 'línea uno'; echo 'línea dos'; echo 'línea tres' 
# escribe cada string en una linea, ; ->  separa en varias lineas, 
# como si fuesen varios comandos pero lo pones de una en una instruccion

> echo 'línea uno' && echo 'línea dos' && echo 'línea tres'
# escribe cada string en una linea, && -> saca los resultados de la instruccion logica cada uno en una linea

> echo 'línea uno' || echo 'línea dos'; echo 'línea tres'
# elige el que sea de la comprobación or y lo escribe en una línea, escribe en la siguiente 'línea tres' ya que es una instrucción a parte por el ;

(echo En una sub-shell; exit 0) && echo acabó bien || echo acabó mal
# escribe 'En una sub-shell' en una línea, es su propia instrucción
# if(Es una sub-shell) saca el primer argumento del or (en este caso 'acabó bien')

(echo En una sub-shell; exit 1) && echo acabó bien || echo acabó mal
# escribe 'En una sub-shell' en una línea, es su propia instrucción
# if(Es una sub-shell) saca el segundo argumento del or (en este caso 'acabó mal')

# --- Variables de entorno
# *Ejercicio 1:* env consulta el entorno de la shell

# *Ejercicio 2:* 
# Con el prefijo $ puedes consultar el valor de las variables
# Para ver las variables de entorno p.ej: > echo ${USER}
# USER: nombre del usurio del sistema
# UID: id del usuario
# HOME: dirección de home
# PWD: dirección del directorio de trabajo
# SHELL: dirección de la shell 
# $: identifica si eres o no root
# PPID: parent process id
# ?: código de retorno del último programa ejecutado
# PATH: ruta donde estamos trabajando

# ej
(exit 0); echo $?; (exit 4);echo $?
# saca: > 0 4 ; (últimas salidas)

echo $$ $PPID
# saca: > 1332 1301

ps -p $$ -o "pid ppid cmd"
# saca:
# > PID  PPID  CMD
# > 1332 2301  bash

# *Ejercicio 3:*
> VAR1=Curso
> VAR2=de
> VAR3="Sistemas Operativos"
> echo ${VAR1} ${VAR2} ${VAR3}
# ---
# Si haces bash las variables dejan de existir
# Si vuelves a la shell anterior y antes de salir exportas las variables con
# > export VAR1 y haces bash para ir a otra shell y la intentas escribir ahora SÍ existen

# *Ejercicio 4:*
# usar {} para evitar colisiones entre variables
# > echo $VAR1\2 ; si VAR1 es "1+1=" escribe : > 1+1=2 ; con la barra puedes pegar el 2 sin espacio y ahorrarte problemas si hubiese una VAR12

# *Ejercicio 5:*
# \t -> hora ; \w -> directorio
> PS1 ="\t|\w% " 

# *Ejercicio 6:*
> export PATH="$PATH:./" # te lleva al directorio anterior al path (al añadirle ./
> export PATH="$PATH:$PWD" # te pone el del root

# *Ejercicio 7:*
# which determina qué comando se ejecutará cuando se usa en la línea de comandos
# --
# which bash : /usr/bin/bash
# type bash : bash is /usr/bin/bash
# --
# which cd :
# type cd : is a shell builtin
# --
# which echo : /usr/bin/echo
# type echo : is a shell builtin
# --
# which test : /usr/bin/test
# type test : is a shell builtin


# --- Manejo de cadenas y flujo de caracteres

# ----- EL SISTEMA DE FICHEROS
# --- Búsqueda de ficheros

# ----- REDIRECCIONES, TUBERÍAS Y EXPRESIONES REGULARES
# ----- PROGRAMACIÓN EN LENGUAJE SHELL
# ----- PROYECTO: AGENDA EN SHELL SCRIPT

# *****- API DEL SISTEMA OPERATIVO -*****

# ----- COMPILACIÓN Y DEPURACIÓN DE LOS PROGRAMAS

# ----- GESTIÓN DE ERRORES

# ----- LIBRERÍAS Y LLAMADAS AL SISTEMA