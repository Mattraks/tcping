#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>

#include "io.h"
#include "tcp.h"

static volatile int stop = 0;

void usage(void)
{
	fprintf(stderr, "Usage\n");
	fprintf(stderr, "tcping [options] <destination>\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  <destination>      dns name or ip address\n");
	fprintf(stderr, "  -c <count>         count how many times to connect\n");
	fprintf(stderr, "  -f                 flood connect (no delays)\n");
	fprintf(stderr, "  -h                 print help and exit\n");
	fprintf(stderr, "  -i <interval>      interval delay between each connect (e.g. 1)\n");
	fprintf(stderr, "  -p <port>          portnr portnumber (e.g. 80)\n");
	fprintf(stderr, "  -q                 quiet, only returncode\n");
	fprintf(stderr, "  -t <timeout>       time to wait for response (e.g. 3)\n\n");
}

void handler(int sig)
{
	stop = 1;
}

int main(int argc, char *argv[])
{
	char *hostname = NULL;
	int portnr = 80;
	int c;
	int timeout = 3;
	int count = -1, curncount = 0;
	int wait = 1, quiet = 0;
	int ok = 0, err = 0;
	double min = 999999999999999.0, avg = 0.0, max = 0.0;
	struct hostent  *hostdnsentries;

		while((c = getopt(argc, argv, "p:c:i:t:fq?")) != -1)
		{
			switch(c)
		{
		case 'p':
			portnr = atoi(optarg);
			break;

		case 'c':
			count = atoi(optarg);
			break;

		case 'i':
			wait = atoi(optarg);
			break;

		case 't':
			timeout = atoi(optarg);
			break;

		case 'f':
			wait = 0;
			break;

		case 'q':
			quiet = 1;
			break;

		case '?':
		default:
			usage();
			return 0;
		}
	}

	if (optind >= argc) {
//		fprintf(stderr, "No hostname given\n");
		usage();
		return 3;
	}
	hostname = argv[optind];

	hostdnsentries = gethostbyname(hostname);
	if (hostdnsentries == NULL)
	{
		fprintf(stderr, "%s: Name or service not known\n", hostname);
		return 2;
	}

	if (!quiet)
		printf("tcping %s:%d\n", hostname, portnr);

	signal(SIGINT, handler);
	signal(SIGTERM, handler);

	while((curncount < count || count == -1) && stop == 0)
	{
		double ms;
		double dstart, dend;
		struct timeval start, end;
		struct timezone tz;
		int fd;

		if (gettimeofday(&start, &tz) == -1)
		{
			perror("gettimeofday");
			break;
		}

		for(;;)
		{
			fd = connect_to(hostdnsentries, portnr, timeout);
			if (fd == -1)
			{
				printf("error connecting to host: %s\n", strerror(errno));
				err++;
				break;
			}

			ok++;

			close(fd);

			if (gettimeofday(&end, &tz) == -1)
			{
				perror("gettimeofday");
				break;
			}

			dstart = (((double)start.tv_sec) + ((double)start.tv_usec)/1000000.0);
			dend = (((double)end.tv_sec) + ((double)end.tv_usec)/1000000.0);
			ms = (dend - dstart) * 1000.0;
			avg += ms;
			min = min > ms ? ms : min;
			max = max < ms ? ms : max;

			printf("connected to %s:%d, seq=%d time=%.2f ms\n", hostname, portnr, curncount, (dend - dstart) * 1000.0);

			break;
		}

		curncount++;

		if (curncount != count)
			sleep(wait);
	}

	if (!quiet)
	{
		printf("--- %s:%d ping statistics ---\n", hostname, portnr);
		printf("%d connects, %d ok, %3.2f%% failed\n", curncount, ok, (((double)err) / ((double)count)) * 100.0);
		printf("round-trip min/avg/max = %.1f/%.1f/%.1f ms\n", min, avg / (double)ok, max);
	}

	if (ok)
		return 0;
	else
		return 127;
}
