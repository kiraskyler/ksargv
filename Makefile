# kiraskyler@163.com
# V0.0.3.20210623

CC :=gcc
CFLAGS :=-Wall -g -O0 -std=c99

template :template.o ksargv.o
	$(CC) $(CFLAGS) -o $@ $^

template.o :
ksargv.o :

all: template

clean:
	rm -f template template.o ksargv.o