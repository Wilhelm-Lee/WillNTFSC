CC = /bin/x86_64-w64-mingw32-gcc # Jack, 编译器选好了随时跟我说一声。这个是暂时用的。
FLAG = -std=c99 -Wall
NAME = Dumpling
DIR_OUT = out/
DIR_DEBUG = $(DIR_OUT)Debug/
DIR_RELEASE = $(DIR_OUT)Release/

DEBUG = $(DIR_DEBUG)$(NAME).exe
RELEASE = $(DIR_RELEASE)$(NAME).exe

$(DEBUG): main.o
	$(CC) $(FLAG) src/main.c -o $(DEBUG)

$(RELEASE): main.o
	$(CC) $(FLAG) src/main.c -o $(DEBUG)

main.o:

.PHONY: clean
clean:
	-rm -rv *.o *.out $(DIR_DEBUG)*.exe $(DIR_RELEASE)*.exe
