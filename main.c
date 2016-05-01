#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *endptr;
	struct timeval orig_timeout;
	struct timeval timeout;
	fd_set readfds;
	int select_result;
	char *buf;
	ssize_t read_result;
	ssize_t written;
	ssize_t write_result;

	if (argc != 3) {
		fprintf(stderr, "Usage: timeout_cat <timeout_s> <timeout_us>\n");
		return 1;
	}

	orig_timeout.tv_sec = strtol(argv[1], &endptr, 10);
	if (endptr == argv[1] || endptr[0] != '\0') {
		fprintf(stderr, "Couldn't parse timeout_s\n");
		return 2;
	}

	orig_timeout.tv_usec = strtol(argv[2], &endptr, 10);
	if (endptr == argv[2] || endptr[0] != '\0') {
		fprintf(stderr, "Couldn't parse timeout_us\n");
		return 3;
	}

	buf = malloc(1 * 1024 * 1024);
	for (;;) {
		timeout = orig_timeout;
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		select_result = select(0 + 1, &readfds, NULL, NULL, &timeout);
		if (select_result != 1) {
			fprintf(stderr, "select %d %d\n", select_result, errno);
			return 0;
		}

		read_result = read(0, buf, sizeof(buf));
		if (read_result <= 0) {
			fprintf(stderr, "read %zd %d\n", read_result, errno);
			return 0;
		}

		written = 0;
		while (written < read_result) {
			write_result = write(1, &buf[written], read_result - written);
			if (write_result <= 0) {
				fprintf(stderr, "write %zd %d\n", write_result, errno);
				return 0;
			}
			written += write_result;
		}
	}
}
