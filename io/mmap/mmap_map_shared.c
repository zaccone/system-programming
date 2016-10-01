/*
 * Overwrite file with mmap(2) function with MAP_SHARED flag
 */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SIZE 1024

void read_mem_segment(void *addr, ssize_t len)
{

	static const ssize_t chunk_size = SIZE;
	ssize_t chunk, written;
	ssize_t n = 0;
	char *addr_n = (char *)addr;
	while (n < len) {
		if (len - n >= chunk_size) {
			chunk = chunk_size;
		} else {
			chunk = len - n;
		}
		written = write(STDOUT_FILENO, (void *)addr_n, chunk);
		if (written < 0) {
			perror("read_mem_segment write");
		}
		n += written;
		addr_n += written;
	}

}

void readfile(int fd)
{
	static char buf[SIZE];
	ssize_t n;
	for (;;) {
		n = read(fd, buf, SIZE);
		if (n < 0) {
			perror("readfile: read()");
		}
		if (!n) {
			break;
		}

		write(STDOUT_FILENO, buf, n);
	}
	buf[0] = '\n';
	write(STDOUT_FILENO, buf, 1);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("%s file\n", argv[0]);
		exit(127);
	}
	const char *filename = argv[1];
	int fd = open(filename, O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(127);
	}

	struct stat st;
	if (fstat(fd, &st) < 0) {
		perror("stat");
		exit(127);
	}
	void *fmap = mmap(0, st.st_size, PROT_READ | PROT_WRITE,
			  MAP_SHARED, fd, 0);
	if (fmap == MAP_FAILED) {
		perror("mmap");
		exit(127);
	}
	printf("Reading memory area before modification\n");
	read_mem_segment(fmap, st.st_size);

	const char *txt = "DATA IS OVERWRITTEN HERE!";
	if (ftruncate(fd, strlen(txt)) < 0) {
		perror("ftruncate");
		exit(127);
	}
	memcpy(fmap, txt, strlen(txt));

	printf("\n\nReading memory area after modification\n");
	read_mem_segment(fmap, strlen(txt));
	printf("\n\nRead file after modification\n");
	readfile(fd);
	munmap(fmap, st.st_size);
	return 0;
}
