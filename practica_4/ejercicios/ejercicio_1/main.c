#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define CANTIDAD_ITERACIONES (1000)
#define CANTIDAD_THREADS (2)
#define PERIOD_NS (10000000L) // 10.000.000 ns = 10 ms = 0.01 s

/*
    Para compilar, ejecutar
    gcc -Wall main.c -o ejercicio1 -lrt -lpthread
*/

double promedio_latencias[CANTIDAD_THREADS];
int id_threads[CANTIDAD_THREADS];

void timespec_add_ns(struct timespec *t, long ns)
{
    t->tv_nsec += ns;
    while (t->tv_nsec >= 1000000000L)
    {
        t->tv_nsec -= 1000000000L;
        t->tv_sec++;
    }
}

long timespec_dif_ns(struct timespec *t1, struct timespec *t2)
{
    return (t1->tv_sec - t2->tv_sec) * 1000000000L + (t1->tv_nsec - t2->tv_nsec);
}

void funcion_thread(void *arg)
{
    int id = *(int *)arg;
    struct timespec time_proximo_wakeup, time_actual_wakeup;
    long latencia_total_ns = 0;
    long latencia_actual_ns;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &time_proximo_wakeup);
    for (i = 0; i < CANTIDAD_ITERACIONES; i++)
    {
        timespec_add_ns(&time_proximo_wakeup, PERIOD_NS);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time_proximo_wakeup, NULL);
        clock_gettime(CLOCK_MONOTONIC, &time_actual_wakeup);
        latencia_actual_ns = timespec_dif_ns(&time_actual_wakeup, &time_proximo_wakeup);

        if (!(i % 100))
            printf("[Thread %d]: iteracion %d\n", id, i);

        if (latencia_actual_ns > 0)
            latencia_total_ns += latencia_actual_ns;
    }

    promedio_latencias[id] = (double)latencia_total_ns / (double)CANTIDAD_ITERACIONES;
    return NULL;
}

int main(void)
{
    pthread_t threads[CANTIDAD_THREADS];
    pthread_attr_t attr;

    // Setear atributos
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); // El scope de scheduling lo seteo para los pthreads
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    int max_prioridad = sched_get_priority_max(SCHED_FIFO);
    struct sched_param param;
    param.sched_priority = max_prioridad;
    pthread_attr_setschedparam(&attr, &param);

    int ret;

    // Crear los threads
    for (int i = 0; i < CANTIDAD_THREADS; i++)
    {
        id_threads[i] = i;
        ret = pthread_create(&threads[i], &attr, &funcion_thread, &id_threads[i]);
        if (ret != 0)
            fprintf(stderr, "Error creando thread %d: %s\n", i, strerror(i));
    }

    pthread_attr_destroy(&attr);

    // Esperar a que terminen
    for (int i = 0; i < CANTIDAD_THREADS; i++)
    {
        ret = pthread_join(threads[i], NULL);
        if (ret != 0)
            fprintf(stderr, "Error uniendo thread %d: %s\n", i, strerror(i));
    }

    // Mostrar resultados
    printf("\nResultados: \n");
    for (int i = 0; i < CANTIDAD_THREADS; i++)
        printf("Thread %d: latencia promedio = %.2f ns.\n", i, promedio_latencias[i]);
}