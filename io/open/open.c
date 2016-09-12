#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE    32
char BUF[BUF_SIZE];

/* Demonstrate that each file descriptor witin a single process has
 * separate file table
 */

void double_open(const char *filename)
{
	int fd1, fd2;

	if ((fd1 = open(filename, O_RDONLY, S_IRWXU)) < 0) {
		perror("fd1");
	}

	if ((fd2 = open(filename, O_RDONLY, S_IRWXU)) < 0) {
		perror("fd2");
	}

	size_t n1, n2;

	printf("lseek before first read, fd1, fd2 %lu, %lu\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR));
	if ((n1 = read(fd1, BUF, 5)) < 0) {
		perror("reading fd1");
	}
	BUF[n1] = '\0';
	printf("lseek after first read, fd1, fd2 %lu, %lu, read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);

	if ((n1 = read(fd2, BUF, 5)) < 0) {
		perror("reading fd2");
	}
	BUF[n1] = '\0';
	printf("lseek after second read, fd1, fd2 %lu, %lu, read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);

	if ((n1 = read(fd1, BUF, 5)) < 0) {
		perror("reading fd1");
	}
	BUF[n1] = '\0';
	printf("lseek after third read, fd1, fd2 %lu, %lu, read: %s\n",
	       lseek(fd1, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR), BUF);

	close(fd1);
	close(fd2);
	return;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Missing filename\n");
		exit(127);
	}
	double_open(argv[1]);
	return 0;
}
