#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s [OPCION] [PARAMETRO]\n", argv[0]);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "\t-c semaforo valor\tcrea semáforo con el nombre y valor inicial indicado.\n");
    fprintf(stderr, "\t-u semaforo\t\trealiza un UP en el semáforo indicado.\n");
    fprintf(stderr, "\t-d semaforo\t\trealiza un DOWN en el semaforo indicado.\n");
    fprintf(stderr, "\t-b semaforo\t\telimina el semáforo indicado.\n");
    fprintf(stderr, "\t-i semaforo\t\tinformación del semáforo indicado.\n");
    fprintf(stderr, "\t-h\t\t\timprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch(option) {
        case 'c':
            break;
        case 'u':
            break;
        case 'd':
            break;
        case 'b':
            break;
        case 'i':
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Opción desconocida: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
