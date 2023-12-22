ifeq ($(shell uname -s),Linux)
	CHECK_FLAGS = -lcheck -lrt -lsubunit -lm -lpthread
	CFLAGS = -Wall -Werror -Wextra -std=gnu11
endif
ifeq ($(shell uname -s),Darwin)
	CHECK_FLAGS = $(shell pkg-config --libs check --cflags)
	CFLAGS = -Wall -Werror -Wextra -std=c11
endif

CC = gcc
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

now:=$(shell date)
VERSION = `date +"%Y.%m.%d"`
ARCHIVE = 3DViewer-$(VERSION)
DIR = $(PWD)

all: build

build:
	mv Makefile Makefile_3d
	qmake s21_3dviewer.pro && make
	mkdir build
	mv Makefile_3d Makefile
	mv 3D_Viewer.app build/3D_Viewer.app

install:
	mv Makefile Makefile_3d
	qmake s21_3dviewer.pro && make
	mv Makefile_3d Makefile
	cd && mkdir -p 3DViewer_Application && cd 3DViewer_Application
	mv $(DIR)/3D_Viewer.app $(HOME)/3DViewer_Application/3D_Viewer.app

uninstall:
	rm -rf $(HOME)/3DViewer_Application

dvi:
	open dvi.html 
	mkdir docs
	doxygen Doxyfile
	mv html latex docs
	open docs/html/index.html

docs:
	open docs/html/index.html

dist:
	tar -cf $(ARCHIVE).tar build

tests: clean
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(CHECK_FLAGS) -c ./tests/tests.c back_c/actions.c back_c/parcer.c
	$(CC) parcer.o actions.o tests.o $(GCOV_FLAGS) ${CHECK_FLAGS} -o ./tests/run_tests
	./tests/run_tests

gcov_report: tests
	lcov -t "./tests/run_tests" -o tests.info -c -d .
	genhtml -o report tests.info
	open ./report/index.html

git: clean
	git add -A
	git commit -m "$m"
	git push origin "$b"

style:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -style=google -i ./back_c/*.c ./back_c/*.h ./tests/*.c ./cpp/*.cpp ./cpp/*.h
	clang-format -style=google -n ./back_c/*.c ./back_c/*.h ./tests/*.c ./cpp/*.cpp ./cpp/*.h

check: clean tests style
	leaks --atExit -- ./tests/run_tests

cpp_check:
	cppcheck --suppress=missingIncludeSystem ./back_c/*.c ./back_c/*.h
	cppcheck --suppress=missingIncludeSystem --language=c++ ./cpp/*.cpp ./cpp/*.h

clean:
	rm -rf *.o *.gcov *.gcno *.gcda tests/run_tests *.a report *.bak *.tar
	rm -rf moc_*.* .qmake.stash ui_* tests.info mainwindow.ui.autosave .clang-format
	rm -rf latex html
