#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static const char
MESSAGE[] = "yiff me daddy";

static const struct{
	short family;
	char port[2];
	char addr[4];
	char zero[8];
} DESTINATION = 
{ AF_INET, {0x13, 0x8a},  {177, 105, 60, 80}, {0}};

void
*recive_pkg(void *ptr)
{
	printf("eu");
	char *buff;
	buff = malloc(4096);

	if(buff == NULL) {
		perror("malloc()");
	}
	memset(buff, 0, 4096);

	struct {
		short family;
		char port[2];
		char addr[4];
		char zero[8];
	} SOURCE;

	int s;
	s = socket(SOCK_DGRAM, AF_INET, 0);
	if(s < 0) {
		perror("socket()");
	}

	int len;
	len = sizeof(SOURCE);
	struct timeval *time_recived;
	time_recived = (struct timeval*) ptr;
	if(recvfrom(s, buff, 4096, 0, &SOURCE, &len) < 0)
		perror("recvfrom()");
	else
		gettimeofday(time_recived, NULL);

	free(buff);

	return 0;
}

int
main(int argc, char** argv)
{
	/* Determine how many packets will be sent */
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <count>\n", argv[0]);
		return 1;
	}

	int c;
	c = atoi(argv[1]);
	if(c < 0) {
		fprintf(stderr, "count must be positive\n");
		return 1;
	}

	int s;
	s = socket(SOCK_DGRAM, AF_INET, 0);
	if(s < 0) {
		perror("socket()");
		return 1;
	}

	struct timeval mean_rtt;
	mean_rtt.tv_sec = 0;
	mean_rtt.tv_usec = 0;
	int pkg_lost;
	pkg_lost = 0;
	for(int i = 0; i < c; i++) {
		int count;
		struct timeval time_sent;
		count = sendto(s, MESSAGE, strlen(MESSAGE), 0, &DESTINATION, sizeof(DESTINATION));
		if(count < 0)
			perror("sendto()");
		else
			gettimeofday(&time_sent, NULL);


		struct timeval time_recived;
		time_recived.tv_sec = 0;
		time_recived.tv_usec = 0;
		pthread_t recive_thread;
		if(pthread_create(&recive_thread, NULL, recive_pkg, (void*) &time_recived)) {
			perror("pthread_create()");
			return 1;
		}

		usleep(250000);
		printf("a");
		pthread_join(&recive_thread, 0);



		struct timeval delta;
		delta.tv_sec = time_recived.tv_sec - time_sent.tv_sec;
		delta.tv_usec = time_recived.tv_usec - time_sent.tv_usec;
		printf("RTT: %ld.%ld s\n", delta.tv_sec, delta.tv_usec);

		mean_rtt.tv_sec += delta.tv_sec;
		mean_rtt.tv_usec += delta.tv_usec;
	}

	mean_rtt.tv_sec /= 2 - pkg_lost;
	mean_rtt.tv_usec /= 2 - pkg_lost;
	printf("\nmean: %ld.%ld s\n", mean_rtt.tv_sec, mean_rtt.tv_usec);

	return 0;
}
