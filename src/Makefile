# Constants
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g -fmax-errors=1
LDFLAGS=-lcheck -pthread -lm

ifeq ($(shell uname), Linux)
	LDFLAGS += -lsubunit -lrt
endif

# Converage
GCOVFLAGS=--coverage
GCOV_DIR=gcov

# Directories
TEST_DIR=tests
SRC_DIR=.
OBJ_DIR=obj
OBJ_DIR_GCOV=obj/$(GCOV_DIR)
OBJ_DIR_TEST=obj/$(TEST_DIR)
BUILD_DIR=build
INSTALL_DIR=install

# Files
SRCS=$(wildcard $(SRC_DIR)/s21_*.c)
SRCS_TEST=$(wildcard $(TEST_DIR)/s21_*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS_TEST=$(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR_TEST)/%.o,$(SRCS_TEST))
OBJS_GCOV=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_GCOV)/%.o,$(SRCS))

# Builds
PROGRAM=S21_3DViewer_v1
TEST=test
TEST_BUILD=test_build

# Docs
DOC=doc

.PHONY: all $(TEST) $(TEST_BUILD) build install uninstall gcov_report lcov_report dvi dist clean rebuild clang_n clang_i valgrind

all: install

$(TEST_BUILD): $(OBJS_GCOV) $(OBJS_TEST)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(GCOVFLAGS) -o $(TEST)

$(TEST): $(TEST_BUILD)
	./$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_TEST)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR_TEST)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_GCOV)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR_GCOV)
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $< -o $@

build:
	@mkdir -p $(BUILD_DIR)
	cd ./$(BUILD_DIR)/ && qmake ../ && make

install: build
	@mkdir -p $(INSTALL_DIR)
	@cp -r $(BUILD_DIR)/$(PROGRAM)* $(INSTALL_DIR)/

uninstall:
	rm -rf $(INSTALL_DIR)

gcov_report: $(TEST_BUILD)
	./$(TEST) || true
	mkdir -p $(GCOV_DIR)
	rm -rf $(GCOV_DIR)/*.gcda $(GCOV_DIR)/*.gcno
	gcovr --html-details -o $(GCOV_DIR)/index.html --filter "$(SRC_DIR)/*"
	open $(GCOV_DIR)/index.html

lcov_report: $(TEST_BUILD)
	./$(TEST) || true
	mkdir -p $(GCOV_DIR)
	lcov -o $(GCOV_DIR)/Coverage_Report.info -c -d $(OBJ_DIR_GCOV)
	lcov -r $(GCOV_DIR)/Coverage_Report.info  -o $(GCOV_DIR)/Coverage_Report.info '/Library/*'
	genhtml -o $(GCOV_DIR) $(GCOV_DIR)/Coverage_Report.info 
	open $(GCOV_DIR)/index.html

dvi:
	cd $(DOC) && doxygen doxygen.conf
	open $(DOC)/html/index.html

dist:
	mkdir -p $(PROGRAM)/src
	@cp -r Makefile *.c *.h *.pro *.cpp *.ui thirdparty/ $(PROGRAM)/src 2>/dev/null || true
	tar cvzf $(PROGRAM).tgz $(PROGRAM)/
	rm -rf $(PROGRAM)/

clean:
	rm -rf $(OBJ_DIR) $(GCOV_DIR) $(BUILD_DIR) $(TEST) $(LIB) $(DOC)/html $(DOC)/latex

rebuild: clean all

clang_n:
	clang-format -n --style=Google $(SRC_DIR)/*.[ch] $(TEST_DIR)/*.[ch]
	clang-format -n --style=Google $(SRC_DIR)/*.cpp

clang_i:
	clang-format -i --style=Google $(SRC_DIR)/*.[ch] $(TEST_DIR)/*.[ch]
	clang-format -i --style=Google $(SRC_DIR)/*.cpp

valgrind: $(TEST_BUILD)
	valgrind --tool=memcheck --leak-check=yes ./$(TEST)

valgrind_macos: $(TEST_BUILD)
	leaks -atExit -- ./$(TEST) | grep LEAK:
