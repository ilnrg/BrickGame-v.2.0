OS = $(shell uname)
ifeq ($(OS),Darwin)
	LIBS =
	M =
	IE = --ignore-errors inconsistent
	QMAKE = cd desk && /usr/local/Qt-6.6.2/bin/qmake ../src/gui/desktop
	MV = mv desk/BrickGame.app/Contents/MacOS/BrickGame build/BrickGame_desktop
else ifeq ($(OS),Linux)
	LIBS = -pthread -lrt -lsubunit
	M = -lm
	IE = --ignore-errors mismatch
	QMAKE = cd desk && qmake ../src/gui/desktop
	MV = mv desk/BrickGame build/BrickGame_desktop
endif

FLAGS = -Wall -Werror -Wextra
C_STD = -std=c11
C++_STD = -std=c++20
CURS = -lncurses
C_TEST_LIB = -lcheck
CC_TEST_LIB = -lgtest

C = *.c
CC = *.cc
H = *.h
O = *.o
BG = BrickGame
T = tetris
S = snake
T_SOURCE = $(T)/$(C)
S_SOURCE = $(S)/$(CC)
TETRIS_TEST = tetris_tests
SNAKE_TEST = snake_tests
COMMON_BACK_TEST = common_back_tests
CLI_COMMON = src/gui/cli/common
BACK_COMMON = src/brick_game/common
F_BACK = src/brick_game
F_CLI = src/gui/cli
F_DESKTOP = src/gui/desktop
MAIN = $(F_CLI)/$(CC)
COMMON = $(CLI_COMMON)/$(C) $(BACK_COMMON)/$(C) 
T_BACK = $(F_BACK)/$(T_SOURCE)
T_FRONT = $(F_CLI)/$(T_SOURCE)
S_BACK = $(F_BACK)/$(S_SOURCE)
S_FRONT = $(F_CLI)/$(S_SOURCE)
C_SOURCE = $(T_BACK) $(T_FRONT) $(COMMON)
CC_SOURCE = $(MAIN) $(S_BACK) $(S_FRONT)
CPP_SOURCE = $(F_DESKTOP)/*.cpp
SOURCES = $(C_SOURCE) $(CC_SOURCE)
HEADERS = $(F_BACK)/*/$(H) $(F_CLI)/$(H) $(F_CLI)/*/$(H) $(CLI_COMMON)/$(H) $(BACK_COMMON)/$(H)
BG_LIB = all_objects.a
DIR = build
DEL = rm -rf

all: fix_style install

install: clean cli desktop
	$(DEL) build/*.dSYM

cli:
	mkdir $(DIR)
	gcc $(FLAGS) $(C_STD) -c $(C_SOURCE)
	ar rc $(BG_LIB) $(O)
	g++ -g $(FLAGS) $(C++_STD) -o $(DIR)/$(BG)_console $(CC_SOURCE) $(BG_LIB) $(CURS) $(LIBS) $(M)

desktop:
	mkdir desk
	$(QMAKE)
	cd desk && make
	$(MV)
	$(DEL) desk

uninstall:
	$(DEL) $(DIR)

dvi: clean
	doxygen Doxyfile
	open ./html/index.html

dist: clean
	tar -cvf $(BG)_v.2.0.tar.gz ../src

test: clean
	gcc -g $(FLAGS) -o $(TETRIS_TEST) src/tests/$(TETRIS_TEST).c $(T_BACK) $(BACK_COMMON)/$(C) $(CURS) $(C_TEST_LIB) $(LIBS) $(M)
	./$(TETRIS_TEST)
	rm tetrisHS.txt
	gcc -g $(FLAGS) -o $(COMMON_BACK_TEST) src/tests/$(COMMON_BACK_TEST).c $(BACK_COMMON)/$(C) $(CURS) $(C_TEST_LIB) $(LIBS) $(M)
	./$(COMMON_BACK_TEST)
	gcc $(FLAGS) $(C_STD) -c $(BACK_COMMON)/$(C)
	ar rc common.a common_back.o
	g++ -g $(FLAGS) -o $(SNAKE_TEST) src/tests/$(SNAKE_TEST).cc $(S_BACK) common.a $(CURS) $(CC_TEST_LIB) $(LIBS) $(M)
	./$(SNAKE_TEST)
	rm snakeHS.txt

gcov_report: clean snake_report tetris_report
	lcov -a $(SNAKE_TEST)_report.info -a $(TETRIS_TEST)_report.info -o $(BG)_report.info
	genhtml -o report $(BG)_report.info
	open ./report/index.html

snake_report:
	mkdir $(DIR)
	gcc $(FLAGS) $(C_STD) -c $(BACK_COMMON)/$(C)
	ar rc common.a common_back.o
	g++ --coverage -o $(SNAKE_TEST) src/tests/$(SNAKE_TEST).cc $(S_BACK) common.a $(CURS) $(CC_TEST_LIB) $(LIBS) $(M)
	./$(SNAKE_TEST)
	lcov -t "$(SNAKE_TEST)" -o $(SNAKE_TEST).info -c -d . $(IE)
	lcov --remove $(SNAKE_TEST).info '/usr/*' -o $(SNAKE_TEST)_report.info

tetris_report:
	gcc --coverage -o $(TETRIS_TEST) src/tests/$(TETRIS_TEST).c $(T_BACK) $(BACK_COMMON)/$(C) $(CURS) $(C_TEST_LIB) $(LIBS) $(M)
	./$(TETRIS_TEST)
	lcov -t "$(TETRIS_TEST)" -o $(TETRIS_TEST).info -c -d . $(IE)
	genhtml -o report $(TETRIS_TEST).info
	lcov --remove $(TETRIS_TEST).info '/usr/*' -o $(TETRIS_TEST)_report.info

valgrind_tetris: test
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log.txt ./$(TETRIS_TEST)
	grep -e "ERROR SUMMARY" leaks_log.txt -n > all_err.log
	grep -e "fn" leaks_log.txt -n > fn_err.log

valgrind_snake: test
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log.txt ./$(SNAKE_TEST)
	grep -e "ERROR SUMMARY" leaks_log.txt -n > all_err.log
	grep -e "fn" leaks_log.txt -n > fn_err.log

check_style:
	clang-format -n $(SOURCES) $(CPP_SOURCE)
	clang-format -n $(HEADERS)

fix_style:
	clang-format -i $(SOURCES) $(CPP_SOURCE)
	clang-format -i $(HEADERS)

clean: 
	$(DEL) brickgame
	$(DEL) *.o
	$(DEL) *.out
	$(DEL) *.a
	$(DEL) *.info
	$(DEL) *.gcno
	$(DEL) *.gcda
	$(DEL) report
	$(DEL) *.txt
	$(DEL) *.log
	$(DEL) *.dSYM
	$(DEL) *.tar.gz
	$(DEL) html
	$(DEL) $(DIR)
	$(DEL) desk
	$(DEL) *_tests
	$(DEL) $(COMMON_BACK_TEST)
	$(DEL) $(F_DESKTOP)/BrickGame
	$(DEL) $(F_DESKTOP)/build*
	$(DEL) gui/build-*
	$(DEL) $(F_DESKTOP)/BrickGame.pro.*

rebuild: clean all