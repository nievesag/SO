#!/bin/bash

N="$1"

PROCESOS=$(ps --no-headers -eo pid,comm | tail -n $N)

echo "$PROCESOS"
