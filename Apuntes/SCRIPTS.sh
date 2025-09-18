SCRIPTS
Importante: Para ejecutar un script debe tener permisos de ejecución, que se pueden otorgar con el comando chmod +x <ruta_del_script> en la consola

--- Argumentos ---
"ARGUMENTO 1: $1"
"Argumento 2: $2"
"Numero: $#"
"Todos: $@"

--- Bucle for ---
# El ; separa comandos (igual que ponerlo en diferentes líneas)
for i in $@ ; do
 echo "Argumento $i"; done
--
indice=1
 for arg in $@ ; do
 echo "Argumento $indice: $arg"
 indice=$((indice+1)) # vas sumando 1 al indice (también se puede ++)
 done

 # $? en consola saca el código de retorno del último programa ejecutado

--- Condicionales ---
 if [[ $# < 5 ]]; then
 echo "Menor de 5"; fi

--- Funciones ---
# multiplica *2 el primer argumento que le pases
function pordos() {
    echo "$((2*$1))"
}

A=$(pordos 5) # saca 5*2

echo "El resultado es $A, $(pordos 5)"

# suma +45 al primer argumento que le pases
VARIABLE=45
function mas45() {
    echo "$(($1+VARIABLE))"
}

B=$(mas45 1) # saca 45+1

echo "El resultado es $B, $(mas45 1)"

--- PRACTICA AGENDA
agenda.txt [formato -> nombre:telefono:mail]:
ruben:1234:correo@ucm
maria:5678:correo@uam
juan:1111:correo@pol

# listar toda la agenda
for l in `cat agenda.txt`; do
 echo $l; done

# coge la linea y la separa por : y accede al primer corte (-f 2 para acceder al segundo etc)
echo "ruben:1234:correo@ucm" | cut -d ':' -f 1
# esto escribe: ruben

# ahora accedemos a todos los nombres
for l in `cat agenda.txt`; do
 NOMBRE=$(echo $l | cut -d ':' -f 1)
 echo "Nombre: $NOMBRE"
 done
# esto escribe: ruben maria juan

# cosas de consola...
read VAR # recoge en VAR la entrada del usuario por consola
# guarda en la agenda el nuevo contacto:
echo "$NOM:$TLF:$MAL" >> agenda.txt 

grep @ucm agenda.txt # saca a todos los que sean estudiantes de la ucm, busca el patrón ucm
grep [73]: agenda.txt # saca todos los que tengan un teléfono que contenga un 7 o un 3

# borra de la agenda un nuevo contacto:
