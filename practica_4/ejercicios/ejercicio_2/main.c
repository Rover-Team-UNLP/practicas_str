#include "main.h"

#define PATH_PIPE "/tmp/sensor_pipe"
#define PATH_ARCHIVO "datos.txt"

int main()
{
    FILE *data_file;
    int pipe_fd;
    long wait_ns;
    double temp_c;
    struct timespec wait_time;

    // Crea el fifo si no existe
    if (mkfifo(PATH_PIPE, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    // Lo abre
    data_file = fopen(PATH_ARCHIVO, "r");
    if (data_file == NULL)
    {
        perror("fopen(DATA_FILE)");
        fprintf(stderr, "Error al abrir datos.txt\n");
        exit(EXIT_FAILURE);
    }
    printf("ARchivo de datos abierto");

    // Se bloquea hasta que el monitor abra el pipe para leer
    pipe_fd = open(PATH_PIPE, O_WRONLY);
    if (pipe_fd == -1)
    {
        perror("open(PATH_PIPE)");
        exit(EXIT_FAILURE);
    }
    printf("Pipe abierto");

    while (fscanf(data_file, "%ld\t%lf\n", &wait_ns, &temp_c) == 2)
    {
        wait_time.tv_sec = 0;
        wait_time.tv_nsec = wait_ns;
        clock_nanosleep(CLOCK_MONOTONIC, 0, &wait_time, NULL); // Flag 0 = espera relativa

        if (write(pipe_fd, &temp_c, sizeof(double)) == -1)
        {
            perror("write(pipe_fd)");
            break;
        }
        printf("Sensor: Enviado -> %.2f grados\n", temp_c);
    }

    printf("Sensor: fin del archivo de datos\n");
    fclose(data_file);
    close(pipe_fd);
}