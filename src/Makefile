.PHONY: install uninstall clean dvi dist tests gcov_report check valgrind

SHELL=/bin/bash
CC=gcc -D_POSIX_C_SOURCE=200809L -Wall -Werror -Wextra -ggdb
CFLAGS= -std=c11
BINFLD=./bin
BINTESTFLD=./bin_test
DIROBJ=./obj
DVISOURCE=./BrickGame_v1.0.tex
LDFLAGS= -lncurses
DIRTET = ./brick_game/tetris
DIRGUI = ./gui/cli

CPP_FILES := $(shell find . -name "*.cpp")
C_FILES := $(shell find . -name "*.c")
H_FILES := $(shell find . -name "*.h")
O_FILES := $(shell find . -name "*.o")


OS=$(shell uname -s)
ifeq ($(OS),Linux)
CHECKFLAGS=-lcheck -lgcov --coverage -lsubunit -lrt -lpthread -lm
endif
ifeq ($(OS),Darwin)
CHECKFLAGS=-lcheck -lgcov --coverage
endif

GCOVFLAGS=-fprofile-arcs -ftest-coverage
STATICLIB=BrickGame_v1.0.a
DIRTESTS=./tests
DIRFUNCTESTS=./functions_tests
DIRGCOV=./gcov
DIRGENHTML=./genhtml
APPNAME=BrickGame_v1.0
DIRBUILD=./build
DIRDIST=./dist

all: install dvi

$(STATICLIB):
	$(CC) $(CFLAGS) -c $(DIRTET)/*.c $(DIRGUI)/*.c
	ar r $(STATICLIB) *.o
	rm -f *.o

install: 
	mkdir -p $(DIRBUILD)
	$(CC) -c $(CFLAGS) $(DIRTET)/*.c $(DIRGUI)/*.c 
	$(CC) ./*.o -o $(DIRBUILD)/$(APPNAME) $(LDFLAGS)
	touch $(DIRBUILD)/highscore.txt
	rm -f *.o

uninstall:
	rm -drf $(DIRBUILD)

dvi:
	mkdir -p $(DIRBUILD)
	xelatex -output-directory=$(DIRBUILD) ./materials/$(DVISOURCE)
	rm -f $(DIRBUILD)/*.log 
	rm -f $(DIRBUILD)/*.aux

dist: $(STATICLIB) all
	rm -drf $(DIRDIST)
	mkdir -p $(DIRDIST)
	cp -r $(DIRBUILD)/* $(DIRDIST)
	mv -f $(STATICLIB) $(DIRDIST)
	cd $(DIRDIST) && tar cvzf $(APPNAME).tgz * && cd ..	
	rm $(DIRDIST)/$(APPNAME)
	rm $(DIRDIST)/*.pdf
	rm $(DIRDIST)/$(STATICLIB)
	rm -drf $(DIRBUILD)


tests: 	$(STATICLIB)
	$(CC) $(CFLAGS) -c $(DIRTET)/*.c $(GCOVFLAGS)
	$(CC) $(CFLAGS) -c $(DIRFUNCTESTS)/*.c
	mkdir -p $(DIROBJ)
	mv -f *.o $(DIROBJ)
	mkdir -p $(DIRTESTS)
	$(CC) $(CFLAGS) $(DIROBJ)/*.o $(STATICLIB) $(CHECKFLAGS) $(LDFLAGS) -o $(DIRTESTS)/s21_brickgame_test 

run_tests:	
	$(DIRTESTS)/s21_brickgame_test

gcov_report: tests run_tests
	mkdir -p $(DIRGCOV)
	gcov $(DIRTET)/*.c $(DIRGUI)/*.c -o ./
	mv -f *.gcov $(DIRGCOV)
	mv -f *.gcda $(DIRGCOV)
	mv -f *.gcno $(DIRGCOV)
	lcov -d $(DIRGCOV) -c -o ./html.info
	genhtml -o $(DIRGENHTML) html.info
	open $(DIRGENHTML)/index.html

check:
	cp ../materials/linters/.clang-format ./
	clang-format -style=Google -n $(C_FILES) $(H_FILES) $(CPP_FILES)
	clang-format -style=Google -i $(C_FILES) $(H_FILES) $(CPP_FILES)
	cppcheck --enable=all --suppress=missingIncludeSystem $(C_FILES) $(H_FILES) $(CPP_FILES)
	rm -f .clang-format

valgrind: tests
	>valgrep.txt
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all $(DIRTESTS)/s21_brickgame_test 2>>valgrep.txt
	
	grep -e "Command" -e "jump" -e "Y: " valgrep.txt| grep -v "Y: 0" | cat

clean:
	rm -f $(O_FILES)
	rm -f ./*.out
	rm -f ./*.a		
	rm -f ./*.gc*
	rm -f ./*.log 
	rm -f ./*.info
	rm -f ./*.txt
	rm -drf $(DIRTESTS)
	rm -drf $(DIRGCOV)
	rm -drf $(DIRGENHTML)
	rm -drf $(DIRDIST)
	rm -drf $(DIROBJ)

rebuild: clean uninstall genuicandmoc all

# sudo apt install imagemagick-6.q16