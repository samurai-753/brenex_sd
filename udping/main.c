#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static char MESSAGE[] = "yiff me daddy UwU.";

static const struct{
	short family;
	char port[2];
	char addr[4];
	char zero[8];
} DESTINATION =
{ AF_INET, {0x13, 0x8a},  {177, 105, 60, 80}, {0}};

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

	/* Initialize the socket */
	int s;
	s = socket(SOCK_DGRAM, AF_INET, 0);
	if(s < 0) {
		perror("socket()");
		return 1;
	}

	/* Set the timeout */
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 250000;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, sizeof timeout);

	struct timeval mean_rtt;
	mean_rtt.tv_sec = 0;
	mean_rtt.tv_usec = 0;
	int pkg_lost;
	pkg_lost = 0;

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
	unsigned int len;
	len = sizeof(SOURCE);

	for(int i = 0; i < c; i++) {
		/* Sent the message */
		struct timeval time_sent;
		MESSAGE[17] = i % 93 + 33;
		if(sendto(s, MESSAGE, strlen(MESSAGE), 0, &DESTINATION, sizeof(DESTINATION)) < 0)
			perror("sendto()");
		else
			gettimeofday(&time_sent, NULL);

		struct timeval time_recived;
		if(recvfrom(s, buff, 4096, 0, &SOURCE, &len) < 0) {
			pkg_lost++;
			printf("package lost\n");
		} else {
			gettimeofday(&time_recived, NULL);

			/* Calculate the RTT */
			struct timeval delta;
			delta.tv_sec = time_recived.tv_sec - time_sent.tv_sec;
			delta.tv_usec = time_recived.tv_usec - time_sent.tv_usec;

			if(delta.tv_usec < 0) {
				delta.tv_sec--;
				delta.tv_usec += 1000000;
			}

			long int diff;
			diff = delta.tv_sec * 1000 + delta.tv_usec / 1000;
			printf("pkg #%3d\trtt: %ld ms\n", i, diff);

			mean_rtt.tv_sec += delta.tv_sec;
			mean_rtt.tv_usec += delta.tv_usec;
		}
	}

	mean_rtt.tv_sec /= c - pkg_lost;
	mean_rtt.tv_usec /= c - pkg_lost;
	long int total;
	total = mean_rtt.tv_sec * 1000 + (mean_rtt.tv_usec / 1000);
	printf("\nmean: %ld ms", total);

	float result;
	result = (float) pkg_lost / (float) c * 100;
	printf("\tpackets lost: %3.2f (%2d)\n", result, pkg_lost);

	free(buff);
	return 0;
}
