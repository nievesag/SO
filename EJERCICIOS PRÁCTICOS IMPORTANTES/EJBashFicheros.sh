#!/bin/bash

# considerar el directorio /run

# obtiene las caracteristicas detalladas de todos los ficheros
ls -aliF /run

# coges el primer caracter con cut y te saltas la primera linea (tail a partir de la linea 2)
ls -l /run | cut -c 1 | tail -n +2
