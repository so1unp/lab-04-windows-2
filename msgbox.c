#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

#define USERNAME_MAXSIZE 15 // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE 100        // Máximo tamaño del texto del mensaje.

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg
{
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
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

    /*
        argv[0] = nombre del programa
        argv[1] = comando (option)
        argv[2] = nombre de cola de mensajes
        argv[3] = mensaje (opcional)

    */

    switch (option)
    {
    case 's':
        if (argc < 4)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        if (argv[2][0] != '/')
        {
            fprintf(stderr, "El nombre de la cola debe comenzar con /\n");
            exit(EXIT_FAILURE);
        }

        mqd_t fileDescriptor = mq_open(argv[2], O_WRONLY);
        if (fileDescriptor == -1)
        {
            perror("Error al abrir la cola de mensajes");
            exit(EXIT_FAILURE);
        }
        msg_t msg_a_enviar;
        getlogin_r(msg_a_enviar.sender, USERNAME_MAXSIZE); // obtener el nombre de usuario

        strcopy(msg_a_enviar.text, argv[3]); // copiar el mensaje a la estructura

        mqd_t fileDescriptor = mq_open(argv[2], O_WRONLY);
        if (fileDescriptor == -1)
        {
            perror("Error al abrir la cola de mensajes");
            exit(EXIT_FAILURE);
        }
        if (mq_send(fileDescriptor, (char *)&msg_a_enviar, sizeof(msg_t), 0) == -1)
        {
            perror("Error al enviar el mensaje");
            exit(EXIT_FAILURE);
        }
        if (mq_close(fileDescriptor) == -1)
        {
            perror("Error al cerrar la cola de mensajes");
            exit(EXIT_FAILURE);
        }

        printf("Enviar %s a la cola %s\n", argv[3], argv[2]);

        break;
    case 'r':
        printf("Recibe el primer mensaje en %s\n", argv[2]);
        break;
    case 'a':
        printf("Imprimer todos los mensajes en %s\n", argv[2]);
        break;
    case 'l':
        printf("Escucha indefinidamente por mensajes\n");
        break;
    case 'c':

        if (argc < 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        if (argv[2][0] != '/')
        {
            fprintf(stderr, "El nombre de la cola debe comenzar con /\n");
            exit(EXIT_FAILURE);
        }

        if (mq_open(argv[2], O_CREAT | O_EXCL | O_RDWR, 0644, NULL) == -1)
        {
            perror("Error al crear la cola de mensajes");
            exit(EXIT_FAILURE);
        }
        mq_close(mq_open(argv[2], O_RDWR));

        printf("Cola de mensajes %s creada\n", argv[2]);

        break;
    case 'd':
        if (argc < 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }
        if (argv[2][0] != '/')
        {
            fprintf(stderr, "El nombre de la cola debe comenzar con /\n");
            exit(EXIT_FAILURE);
        }
        if (mq_unlink(argv[2]) == -1)
        {
            perror("Error al borrar la cola de mensajes");
            exit(EXIT_FAILURE);
        }
        printf("Borra la cola de mensajes %s\n", argv[2]);
        break;
    case 'h':
        usage(argv);
        break;
    default:
        fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
