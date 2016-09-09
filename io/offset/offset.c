#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_MAX 1<<10

int main()
{
	char BUF[BUF_MAX];

	const char *filename = "file";
	const char *letters = "abcdefghijklmnopqrstuvwxyz";
	size_t letters_len = strlen(letters) + 1;
	int fd;
	if ((fd = open(filename, O_RDWR | O_CREAT, S_IRWXU)) < 0) {
		perror("open");
		exit(1);
	}

	if (write(fd, letters, letters_len) != letters_len) {
		perror("write");
		exit(1);
	}

	lseek(fd, 0, SEEK_SET);

	int n;
	if ((n = read(fd, BUF, BUF_MAX)) < 0) {
		perror("read");
		exit(1);
	}
	write(STDOUT_FILENO, BUF, n);
	puts("\n");
	close(fd);
	return 0;
}
