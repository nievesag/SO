# ***-***-- INTERFACES DEL SISTEMA --***-***
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
# 
# which bash : /usr/bin/bash
# type bash : bash is /usr/bin/bash
# 
# which cd :
# type cd : is a shell builtin
# 
# which echo : /usr/bin/echo
# type echo : is a shell builtin
# 
# which test : /usr/bin/test
# type test : is a shell builtin

# --- Manejo de cadenas y flujo de caracteres
# *Ejercicio 1:*
> echo -e "zorro\npájaro\nvaca\ncaballo\nabeja" | sort

# *Ejercicio 2:*
> echo -e "zorro\npájaro\nvaca\ncaballo\nabeja" > texto1
> sort texto1

# *Ejercicio 3:*
> cat text1
> echo -e "pera\nmanzana\nplátano\nciruela" > texto1

> cat > texto2 # ahora escribes por entrada estándar pera manzana etc (con ctrl + d dejas de escribir en la entrada estándar (TECLADO))
> cat texto2 # saca pera manzana etc

# *Ejercicio 4:*
> cat texto1 texto2 > texto3

# *Ejercicio 5:*
> wc texto1 # etc

> ls -l texto1 # lo unico igual con wc es el numero de caracteres

# *Ejercicio 6:*
> sudo dmesg | head -n 10 # saca las 10 primeras lineas de mensajes del sistema
> sudo dmesg | tail -n 10 # las 10 últimas

> sudo dmesg | tail -f # las últimas líneas del fichero

# *Ejercicio 7:*
> cat texto1 | tr '\n' '\t' # saca sustituyendo los saltos por tab
> cat texto1 | tr '\n' '\t' > texto1 # esto para modificar el archivo original

# ----- EL SISTEMA DE FICHEROS
# *Ejercicio 1:*
> cd # sin argumentos cambia al directorio del usuario
> pwd # saca el directorio en el que te encuentras
> cd /usr/bin # vas a /usr/bin, si haces pwd saca /usr/bin
> cd - # volver al directorio anterior
> cd ../../usr/bin # de forma relativa...
> cd

# *Ejercicio 2:*
> mkdrir mis_archivos # crea directorio
> ls # saca la lista de directorios

# *Ejercicio 3:*
> mkdir -p mis_archivos/prueba/texto/tmp
# sin -p busca el directorio para crear tmp al final de la ruta... falla...

# *Ejercicio 4:*
> rmdir # borra el directorio solo si esta vacio
# para borrar prueba hay que ir eliminando todos los directorios de dentro poco a poco hasta que este vacio

# *Ejercicio 5:*
> cp texto1 copia1
> mv copia1 fichero1

# *Ejercicio 6:*
> mkdir copia && cp texto1 texto2 fichero1 copia/

# *Ejercicio 7:*
> cp -r copia otra_copia

# *Ejercicio 8:*
> rm texto1 && texto2 && fichero1 # rm borra ficheros
> rm -r copia # va borrando todos los archivos del directorio una vez esté vacio se borra a si mismo

# --- Búsqueda de ficheros
# *Ejercicio 1:*
# find <ruta_de_búsqueda> -name <patrón_nombre>
> find /home/ubuntu -name "texto*" # busca en home todos los ficheros que empiezan por "texto" HAY QUE PONER EL *
> find -name "texto*" # busca donde estás

# *Ejercicio 2:*
> find /home/ubuntu -type d # busca todos los directorios en $HOME
# type: c = ficheros ; d = directorios

# *Ejercicio 3:*
> find / -type d -size +10M # directorios de más de 10M en el sistema

# ----- REDIRECCIONES, TUBERÍAS Y EXPRESIONES REGULARES
# *Ejercicio 1:*
> s -l text* nada* > salida # mete lo que empieza por text a salida
> ls -l text* nada* > salida.out 2> salida.error 
# ls -l => saca la lista de info
# 2> => para que sea la salida estándar de error
# redirigir la salida estándar a salida.out (>) y la salida estándar de error a salida.error (2>)

# *Ejercicio 2:*
# > trunca el contenido del fichero
# >> añade contenido ya existente
> ls -l text* nada* >> salida.out 2>> salida.error 

# *Ejercicio 3:*
> ls -l text* nada* > salida.out 2>&1
# 2>&1 => lo que antes iba a la salida de error ahora lo saca por la 1, es decir, la salida estandar
# de tipo 1 -> salida estandar, de tipo 2 -> error
# 2> saca la dos a salida.error

# *Ejercicio 4:*
> ls -l text* nada* > /dev/null 2>&1 # redirige la salida a /dev/null

# *Ejercicio 5:*
# redirige la entrada estandar ej:
> cat texto1 | sort
> sort < texto1
# hacen lo mismo (aunque el segundo es más eficiente porque no tienes que llamar a ningun comando)

# *Ejercicio 6:*
> grep 'ja' texto1
> grep 'ja$' texto1
> grep 'a.a' texto1
> grep 'al\{1,2}\o' texto1

# ----- PROGRAMACIÓN EN LENGUAJE SHELL
# *Ejercicio 1:*
# *Ejercicio 2:*
# *Ejercicio 3:*
# *Ejercicio 4:*
# *PROYECTO: AGENDA EN SHELL SCRIPT:*

# *****- API DEL SISTEMA OPERATIVO -*****
# ----- COMPILACIÓN Y DEPURACIÓN DE LOS PROGRAMAS
# *Ejercicio 1:*

# ----- GESTIÓN DE ERRORES
# *Ejercicio 2:*
# *Ejercicio 3:*

# ----- LIBRERÍAS Y LLAMADAS AL SISTEMA
# *Ejercicio 1:*
# *Ejercicio 2:*
# *Ejercicio 3:*
# *Ejercicio 4:*
# *Ejercicio 5:*