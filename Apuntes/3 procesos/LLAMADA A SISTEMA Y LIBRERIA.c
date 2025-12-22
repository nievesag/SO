// miuname.c
// llamada a sistema
#include <sys/utsname.h>
#include <stdio.h>

int main()
{
    // struct utsname *buf = malloc; // siempre que se utilice un puntero tiene que estar inicializado  / malloc (new en c++)
    // no conviene usar memoria dinámica por lo que es mejor:
    struct utsname buf; // se define en la pila

    int rc = uname(&buf); // se pasa por referencia

    printf("Nodename: %s\n", buf.nodename); // a la hora de gestionar cadenas en c es iportante usar %s

    return 0;
}

// -------

// miID.c
// llamada a libreria

#include <sys\types.h>
#include <pwd.h>
#include <stdio.h>

// para poder hacer: > miID 0
// y que devuelva:   > Usuario: root // el 0 siempre es root (?

// siempre que hacemos llamadas al sistema hay que asegurarse de que sean reentrantes (se puede asegurar que actúa igual con cada llamada) 
// si no puede asegurarse se puede llamar a _r (en vez de getpwuid -> getpwuid_r)
struct passwd *getpwuid_r(int uid) 
{
    static struct passwd pw; // static: variable global en el ambito de la función
    pw.pw_name = "ruben";
    return &pw;
}

int main(int argc, char *argc[])
{
    uid_t uid = atoi(argv[1]); // argv[1] es una cadena de texto, atoi para pasar a int porque a getpwuid hay que pasarle int

    struct passwd *pw = getpwuid(uid_t uid);

    // gestión error
    if(pw == NULL)
    {
        // antes de dereferenciar asegurarse de que está bien
        perror("getpwuid()");
        return 1;
    }

    // salida
    printf("Nombre: %s\n", pw->pw_name);
    printf("UID: %i\n", pw->pw_nuid);

    return 0;

    // consola:
    // > gcc -o miID miID.c         # abre el fichero
    // > ./miID 0                   # ejecuta con 0
    // > saca: Nombre: root UID: 0  # salida del programa
}