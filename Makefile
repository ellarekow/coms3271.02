main:main.c
	gcc ./terrain.c ./main.c -Wall -Werror -o main

clean:
	rm -f *~ terrain main core