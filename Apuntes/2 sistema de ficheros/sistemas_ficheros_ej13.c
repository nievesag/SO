> truncate --size 100M fs.ext2  // inicializa el tamaño del inodo, reserva el tamaño, no ocupa nada pero ocupará 100M

> mkfs.ext2 // make filesystem, hace un sistema de ficheros, sistema de ficheros extended 2 (que usa gestion de bloques indexada)

// montar el sistema de ficheros en un punto del directorio para poder usarlo
> sudo mount -t ext2 -o loop ./fs.ext2 ./mnt/
// ejecutar desde super usuario, montar tipo ext2, loopea (directorio dentro de directorio)

> dd if=/dev/random of=./sparse bs=1024 count=1 seek=5192
> ls -lhis // para mostrar con los i-nodos

> debugfs -R "stat <12>" ../fs.ext2 // debugear filesystems, el stat entre corchetes poner el i-nodo que queramos
