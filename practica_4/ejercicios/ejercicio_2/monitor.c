#include "main.h"

#define PIPE_PATH "/tmp/sensor_pipe"
#define TEMP_CRITICA 90.0

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    double last_temp;
    int pipe_fd;
    int sensor_done;
} SharedData;

SharedData shared_data = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER,
    .last_temp = 0.0,
    .pipe_fd = -1,
    .sensor_done = 0};

void *thread_baja_prio(void *arg)
{
    SharedData *data = (SharedData *)arg;
    double temps[3] = {0.0, 0.0, 0.0};
    int index = 0;
    double new_temp;
    int done = 0;

    printf("Monitor (Baja Prio): Thread iniciado. Esperando datos.\n");

    while (!done)
    {
        pthread_mutex_lock(&data->mutex);

        // Sleep hasta que me envien señal
        pthread_cond_wait(&data->cond, &data->mutex);

        new_temp = data->last_temp;
        done = data->sensor_done;

        pthread_mutex_unlock(&data->mutex);

        if (done)
            break;

        temps[index] = new_temp;
        index = (index + 1) % 3; // Circular
        double avg = (temps[0] + temps[1] + temps[2]) / 3.0;
        printf("Monitor (Baja Prio): Promedio 3 ult: %.2f°C\n", avg);
    }

    printf("Monitor (Baja Prio): Thread finalizando.\n");
    return NULL;
}

void *thread_alta_prio(void *arg)
{
    SharedData *data = (SharedData *)arg;
    double temp;
    ssize_t bytes_read;

    printf("Monitor (Alta Prio): Thread iniciado. Leyendo del pipe\n");

    // Cuando el sensor termina de enviar datos, read va a retornar 0
    while ((bytes_read = read(data->pipe_fd, &temp, sizeof(double))) > 0)
    {

        if (bytes_read != sizeof(double))
        {
            fprintf(stderr, "Error de lectura de pipe, tamaño incorrecto\n");
            continue;
        }

        if (temp > TEMP_CRITICA)
        {
            printf("\n!!! AVISO (Alta Prio): Temperatura MUY alta: %.2f°C !!!\n\n", temp);
        }
        else
        {
            pthread_mutex_lock(&data->mutex);

            data->last_temp = temp;

            pthread_cond_signal(&data->cond);

            pthread_mutex_unlock(&data->mutex);
        }
    }

    printf("Monitor (Alta Prio): El sensor cerró el pipe. Finalizando\n");

    // Envio señal al otro thread de que ya terminé
    pthread_mutex_lock(&data->mutex);
    data->sensor_done = 1;
    pthread_cond_signal(&data->cond);
    pthread_mutex_unlock(&data->mutex);

    return NULL;
}

int main()
{
    pthread_t tid_high, tid_low;
    pthread_attr_t attr_high, attr_low;
    struct sched_param param_high, param_low;
    int max_prio, min_prio;
    int ret;

    printf("Monitor: Iniciando...\n");

    if (mkfifo(PIPE_PATH, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    // Obtengo las prioridades max y min
    max_prio = sched_get_priority_max(SCHED_FIFO);
    min_prio = sched_get_priority_min(SCHED_FIFO);
    if (max_prio == -1 || min_prio == -1)
    {
        perror("sched_get_priority");
        exit(EXIT_FAILURE);
    }

    pthread_attr_init(&attr_high);
    pthread_attr_setinheritsched(&attr_high, PTHREAD_EXPLICIT_SCHED); // El hilo no hereda scheduling
    pthread_attr_setschedpolicy(&attr_high, SCHED_FIFO);              // Sino que usa fifo
    param_high.sched_priority = max_prio;
    ret = pthread_attr_setschedparam(&attr_high, &param_high); // Setea la prioridad
    if (ret)
    {
        fprintf(stderr, "Error setschedparam (high): %s\n", strerror(ret));
        exit(1);
    }

    pthread_attr_init(&attr_low);
    pthread_attr_setinheritsched(&attr_low, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr_low, SCHED_FIFO);
    // Seteo proiridad menor
    param_low.sched_priority = (max_prio > min_prio + 10) ? (max_prio - 10) : min_prio;
    ret = pthread_attr_setschedparam(&attr_low, &param_low);
    if (ret)
    {
        fprintf(stderr, "Error setschedparam (low): %s\n", strerror(ret));
        exit(1);
    }

    printf("Monitor: Esperando datos del sensor en %s...\n", PIPE_PATH);
    // Se bloquea hasta que el sensor lo abra para escribir
    shared_data.pipe_fd = open(PIPE_PATH, O_RDONLY);
    if (shared_data.pipe_fd == -1)
    {
        perror("open(PIPE_PATH)");
        exit(EXIT_FAILURE);
    }
    printf("Monitor: Sensor conectado.\n");

    // Creacion de los hilos
    ret = pthread_create(&tid_high, &attr_high, thread_alta_prio, &shared_data);
    if (ret)
        fprintf(stderr, "Error creando hilo high: %s\n", strerror(ret));

    ret = pthread_create(&tid_low, &attr_low, thread_baja_prio, &shared_data);
    if (ret)
        fprintf(stderr, "Error creando hilo low: %s\n", strerror(ret));

    // Limpia atributos
    pthread_attr_destroy(&attr_high);
    pthread_attr_destroy(&attr_low);

    // espera a que terminen de ejecutarse
    pthread_join(tid_high, NULL);
    pthread_join(tid_low, NULL);

    // Cierra y elimina el pipe
    close(shared_data.pipe_fd);
    unlink(PIPE_PATH);

    printf("Monitor: Finalizado.\n");
    return 0;
}
