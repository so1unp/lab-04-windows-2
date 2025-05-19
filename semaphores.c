#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

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
    if (argc < 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-')
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch (option)
    {
    case 'c':

        if (argc != 4)
        {
            fprintf(stderr, "Error: Se requiere un nombre y un valor inicial para el semáforo.\n");
            exit(EXIT_FAILURE);
        }
        char *nombreSemaforo = argv[2];
        int valorInicial = atoi(argv[3]);
        if (valorInicial < 0)
        {
            fprintf(stderr, "error: el valor inicial debe ser un numero positivo\n");
            exit(EXIT_FAILURE);
        }

        sem_t *semaforo = sem_open(nombreSemaforo, O_CREAT | O_EXCL, 0644, valorInicial);
        if (semaforo == SEM_FAILED)
        {
            perror("Error al crear el semáforo");
            exit(EXIT_FAILURE);
        }

        printf("Semáforo %s creado con valor inicial %d\n", nombreSemaforo, valorInicial);
        if (sem_close(semaforo) == -1)
        {
            perror("Error al cerrar el semáforo");
            exit(EXIT_FAILURE);
        }

        break;
    case 'u':
        if (argc != 3)
        {
            fprintf(stderr, "Error: Se requiere un nombre para el semáforo.\n");
            exit(EXIT_FAILURE);
        }
        char *nombreSemaforoUp = argv[2];
        sem_t *semaforoUp = sem_open(nombreSemaforoUp, 0);
        if (semaforoUp == SEM_FAILED)
        {
            perror("Error al abrir el semáforo");
            exit(EXIT_FAILURE);
        }
        if (sem_post(semaforoUp) == -1)
        {
            perror("Error al realizar UP en el semáforo");
            exit(EXIT_FAILURE);
        }
        printf("UP realizado en el semáforo %s\n", nombreSemaforoUp);
        if (sem_close(semaforoUp) == -1)
        {
            perror("Error al cerrar el semáforo");
            exit(EXIT_FAILURE);
        }
        break;
    case 'd':
        if (argc != 3)
        {
            fprintf(stderr, "Error: Se requiere un nombre para el semáforo.\n");
            exit(EXIT_FAILURE);
        }
        char *nombreSemaforoDown = argv[2];
        sem_t *semaforoDown = sem_open(nombreSemaforoDown, 0);
        if (semaforoDown == SEM_FAILED)
        {
            perror("Error al abrir el semáforo");
            exit(EXIT_FAILURE);
        }
        if (sem_wait(semaforoDown) == -1)
        {
            perror("Error al realizar DOWN en el semáforo");
            exit(EXIT_FAILURE);
        }
        printf("DOWN realizado en el semáforo %s\n", nombreSemaforoDown);
        if (sem_close(semaforoDown) == -1)
        {
            perror("Error al cerrar el semáforo");
            exit(EXIT_FAILURE);
        }
        break;
    case 'b':
        if (argc != 3)
        {
            fprintf(stderr, "Error: Se requiere un nombre para el semáforo.\n");
            exit(EXIT_FAILURE);
        }
        char *nombreSemaforoABorrar = argv[2];
        sem_t *semaforoBorrado = sem_open(nombreSemaforoABorrar, 0);
        if (semaforoBorrado == SEM_FAILED)
        {
            perror("Error al abrir el semáforo");
            exit(EXIT_FAILURE);
        }
        if (sem_close(semaforoBorrado) == -1)
        {
            perror("Error al cerrar el semáforo");
            exit(EXIT_FAILURE);
        }
        if (sem_unlink(nombreSemaforoABorrar) == -1)
        {
            perror("Error al eliminar el semáforo");
            exit(EXIT_FAILURE);
        }
        printf("Semáforo %s eliminado\n", nombreSemaforoABorrar);
        break;
    case 'i':
        if (argc != 3)
        {
            fprintf(stderr, "Error: Se requiere un nombre para el semáforo.\n");
            exit(EXIT_FAILURE);
        }
        char *nombreSemaforoInfo = argv[2];
        sem_t *semaforoInfo = sem_open(nombreSemaforoInfo, 0);
        if (semaforoInfo == SEM_FAILED)
        {
            perror("Error al abrir el semáforo");
            exit(EXIT_FAILURE);
        }
        int valorSemaforo;
        if (sem_getvalue(semaforoInfo, &valorSemaforo) == -1)
        {
            perror("Error al obtener el valor del semáforo");
            exit(EXIT_FAILURE);
        }
        printf("Valor del semáforo %s: %d\n", nombreSemaforoInfo, valorSemaforo);
        if (sem_close(semaforoInfo) == -1)
        {
            perror("Error al cerrar el semáforo");
            exit(EXIT_FAILURE);
        }
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
