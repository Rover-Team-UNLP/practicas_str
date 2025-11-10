CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS_R = -lrt -lpthread

all: sensor monitor

sensor: main.c
	$(CC) $(CFLAGS) main.c -o sensor $(LDFLAGS_R)

monitor: monitor.c
	$(CC) $(CFLAGS) monitor.c -o monitor $(LDFLAGS_R)

clean:
	rm -f sensor monitor /tmp/sensor_pipe