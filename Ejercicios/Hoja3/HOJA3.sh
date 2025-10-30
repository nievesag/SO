# Ejercicio 1:

> ps -Ao user # Mostrar todos los procesos del usuario actual en formato extendido
> ps axo pid\ pgrp\ sess\ stat\ comm\ ppid # procesos del sistema, incluyendo el identifi cador del proceso, el identifi cador del grupo de procesos, el identifi cador de sesión, el estado y el comando con todos sus argumentos.
> ps -H axo pid\ pgrp\ sess\ stat\ comm # muestra los procesos indentando para mostrar los hijos
# comparten la sesion (sess)
# cuando es nuevo de pgrp se pone el pid del proceso que lo crea

# Ejercicio 2:
> nano mips

# Ejercicio 4:
> cd /proc # entra en el proceso
> ls # enseña el proceo
> cd 1 # te metes en el 1 (por ejemplo) para buscar los ficheros
> ls -l cwd # para ver la informacion de cwd (por ejemplo)

# Ejercicio 6:
# - kill

# HUP   -> 
# INT   ->
# KILL  ->
# STOP  ->
# CONT  ->
# 0     ->