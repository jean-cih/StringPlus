CFLAGS = -Wall -Werror -Wextra -std=c11
CC = gcc

All_C_FILES = $(wildcard *.c */*.c)
LIB_SOURCE = $(ALL_C_FILES ! -name 's21_string_test.c')
LIB_OBJECTS = $(LIB_SOURCE:.c=.o)

all: clean s21_string.a test clang gcov_report


test: clean


s21_string.a: $(LIB_OBJECTS)
	ar rcs $@ $^
	ranlib $@


gcov_report:


clang:
	cp ./../materials/linters/.clang-format ./
	clang-format -i $(ALL_C_FILES)

clean:
	



