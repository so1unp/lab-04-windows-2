//
// El problema de la cena de filósofos.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <semaphore.h>

// Mueve el cursor a la linea x, columna y (ver codigos ANSI).
#define xy(x, y) printf("\033[%d;%dH", x, y)

// Borra desde el cursor hasta el fin de la linea.
#define clear_eol(x) print(x, 17, "\033[K")

// Borra la pantalla.
#define clear() printf("\033[H\033[J") 

#define N 5
const char *names[N] = { "Socrates", "Kant", "Spinoza", "Wittgenstein", "Popper" };

#define M 5                     
const char *topic[M] = { "Espagueti!", "Vida", "El Ser", "Netflix", "La verdad" };

// Tiempos
int segs_piensa = 1;     // intervalo de pensamiento en [1, seg_piensa] segundos.
int segs_come = 1;

// Mutex
static pthread_mutex_t screen = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tenedor[N];

// Semáforo
sem_t sem_filosofos; // Semáforo global

// Imprime en la posición (x,y) la cadena *fmt.
void print(int y, int x, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    pthread_mutex_lock(&screen);
    xy(y + 1, x), vprintf(fmt, ap);
    xy(N + 1, 1), fflush(stdout);
    pthread_mutex_unlock(&screen);
}

// El filosofo come.
void eat(int id)
{
    int left = id;
    int right = (id + 1) % N;
    int ration, i;

    sem_wait(&sem_filosofos); // Espera turno para intentar comer

    // Para evitar deadlock: el último filósofo toma primero el derecho
    if (id == N - 1) {
        pthread_mutex_lock(&tenedor[right]);
        pthread_mutex_lock(&tenedor[left]);
    } else {
        pthread_mutex_lock(&tenedor[left]);
        pthread_mutex_lock(&tenedor[right]);
    }

    clear_eol(id);
    print(id, 18, "..oO (comiendo)");
    sleep(2);

    for (i = 0, ration = 3 + rand() % 8; i < ration; i++) {
        print(id, 40 + i * 4, "ñam");
        sleep(1 + (rand() % segs_come));
    }

    pthread_mutex_unlock(&tenedor[left]);
    pthread_mutex_unlock(&tenedor[right]);
    sem_post(&sem_filosofos); // Libera el turno
}

// El filosofo piensa.
void think(int id)
{
    int i, t;
    char buf[64] = { 0 };

    do {
        clear_eol(id);

        // Piensa en algo...
        sprintf(buf, "..oO (%s)", topic[t = rand() % M]);

        // Imprime lo que piensa.
        for (i = 0; buf[i]; i++) {
            print(id, i + 18, "%c", buf[i]);
            if (i < 5)
                sleep(1);
        }

        sleep(1 + rand() % segs_piensa);
    } while (t);
}

void *filosofo(void *p)
{
    int id = *(int *) p;
    print(id, 1, "%15s", names[id]);
    while (1) {
        think(id);
        eat(id);
    }
}

int main(int argc, char* argv[])
{
    int i;
    int id[N]; // id para cada filosofo.
    pthread_t tid[N]; 

    if (argc != 3) {
        fprintf(stderr, "Uso: %s segs-piensa segs-come\n", argv[0]);
        fprintf(stderr, "\tsegs-piensa:\tmáxima cantidad de segundos que puede estar pensando.\n");
        fprintf(stderr, "\tsegs-come:\tmáxima cantidad de segundos que puede estar comiendo.\n");
        exit(EXIT_FAILURE);
    }

    if ((segs_piensa = atoi(argv[1])) <= 0) {
        fprintf(stderr, "segs-piensa debe ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    if ((segs_come = atoi(argv[2])) <= 0) {
        fprintf(stderr, "segs-come debe ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    srand(getpid());

    clear(); 

    // Inicializar el semáforo
    sem_init(&sem_filosofos, 0, N - 1); // Solo N-1 filósofos pueden intentar comer a la vez

    // Inicializar los mutex de los tenedores
    for (i = 0; i < N; i++) {
        pthread_mutex_init(&tenedor[i], NULL);
    }

    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(tid + i, 0, filosofo, id + i);
    }

    pthread_exit(0);
}

