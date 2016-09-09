all: clean build
build:
	gcc hole.c -o hole
clean:
	rm -rf file.hole file.reg
	rm -rf hole
test: all
	./hole HOLE file.hole
	./hole REG	file.reg
