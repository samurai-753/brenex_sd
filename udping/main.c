#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

static const char
MESSAGE[] = "yiff me daddy";

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

	int s;
	s = socket(SOCK_DGRAM, AF_INET, 0);
	if(s < 0) {
		perror("socket()");
		return 1;
	}

	int count;
	count = sendto(s, MESSAGE, strlen(MESSAGE), 0, &DESTINATION, sizeof(DESTINATION));
	if(count < 0)
		perror("sendto()");
	else
		printf("%d\n", count);

	char *buff;
	buff = malloc(4096);

	if(buff == NULL){
		perror("malloc()");
		return 1;
	}
	memset(buff, 0, 4096);

	struct{
		short family;
		char port[2];
		char addr[4];
		char zero[8];
	} SOURCE;

	int len;
	len = sizeof(SOURCE);

	count = recvfrom(s, buff, 4096, 0, &SOURCE, &len);
	if(count < 0)
		perror("recvfrom()");
	else
		printf("%s\n", buff);

	free(buff);
	return 0;
}
