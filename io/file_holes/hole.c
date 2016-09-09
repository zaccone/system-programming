#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void err_sys(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char **argv)
{

	if (argc < 3) {
		err_sys("./hole [HOLE/REG] filename");
	}

	static const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
	static const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char *filename = argv[2];
	char *mode = argv[1];

	int fd;
	if ((fd = open(filename, O_WRONLY | O_CREAT,
		       S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		err_sys("open err");
	}

	if (write(fd, lowercase, strlen(lowercase)) != strlen(lowercase)) {
		err_sys("write lowercase");
	}

	if (!strncmp(mode, "HOLE", strlen("HOLE") + 1))
		lseek(fd, (off_t) 1 << 10, SEEK_CUR);
	if (write(fd, uppercase, strlen(uppercase)) != strlen(uppercase)) {
		err_sys("write uppercase");
	}
	close(fd);
	return 0;
}
