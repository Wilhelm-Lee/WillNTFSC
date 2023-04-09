##
#     This file is part of project <https://github.com/WilliamPascal/JackNTFS>
#     Copyright (C) 2023  William Pascal
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.
##

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
