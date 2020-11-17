#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>

#include "tcp.h"

#define incopy(a)       *((struct in_addr *)a)

int connect_to(struct hostent *host, int portnr, int timeout)
{
		int fd;
		int loop;
		struct sockaddr_in addr;
		struct timeval tv;

		/* create socket */
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
				perror("problem creating socket ");
				exit(2);
		}

		/* Set Socket Read-Write Timeout */
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

		/* initialize address structure */
		memset((void *)&addr, 0, sizeof(addr));
		addr.sin_port   = htons(portnr);
		addr.sin_family = host -> h_addrtype;

		/* try to connect for each of the entries: */
		for(loop=0; ; loop++)
		{
				if ((host -> h_addr_list[loop]) == NULL)
						break;

				addr.sin_addr = incopy(host -> h_addr_list[loop]);
				/* connect to peer */
				if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
				{
						/* connection made, return */
						return fd;
				}
		}
		close(fd);
		if (errno = EALREADY) errno = ETIMEDOUT;
		return -1;
}
