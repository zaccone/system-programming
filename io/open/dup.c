#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE    32
char BUF[BUF_SIZE];

/*
 * Demonstrate that file descriptor retuner by dup() function share file table
 * with original file descriptor.
 *
 * Here, fd1 opens a file and fd2 is assigned by a return value of the dup()
 * function. After calling read() function on fd1 fd2 and fd1 we can see that
 * file offset is increased for both file descriptors.
 *
 */

void duplicates(const char *filename)
{
	int fd1, fd2;

	if ((fd1 = open(filename, O_RDONLY, S_IRWXU | S_IRGRP | S_IROTH)) < 0) {
		perror("fd1");
	}

	fd2 = dup(fd1);

	size_t n1, n2;

	printf("lseek before first read, fd1, fd2 %lu, %lu\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR));
	if ((n1 = read(fd1, BUF, 5)) < 0) {
		perror("reading fd1");
	}
	BUF[n1] = '\0';

	printf("lseek before second read, fd1, fd2 %lu, %lu, read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);
	if ((n1 = read(fd2, BUF, 5)) < 0) {
		perror("reading fd2");
	}
	BUF[n1] = '\0';

	printf("lseek before third read, fd1, fd2 %lu, %lu read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);
	if ((n1 = read(fd1, BUF, 5)) < 0) {
		perror("reading fd1");
	}
	BUF[n1] = '\0';

	printf("lseek after third read, fd1, fd2 %lu, %lu read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);
	close(fd1);
	close(fd2);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("missing filename\n");
		exit(127);
	}

	duplicates(argv[1]);
	return 0;
}
