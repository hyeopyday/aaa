# Makefile for Omok Game

# 컴파일러 및 플래그
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
LDFLAGS = -lwinmm -lm

# 디렉토리
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# 타겟 실행 파일
TARGET = $(BUILD_DIR)/OmokGame.exe

# 소스 파일
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/game.c \
       $(SRC_DIR)/ui.c \
       $(SRC_DIR)/sound.c \
       $(SRC_DIR)/ai.c \
       $(SRC_DIR)/timer.c \
       $(SRC_DIR)/animation.c

# 오브젝트 파일
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# 기본 타겟
all: $(TARGET)

# 빌드 디렉토리 생성
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 실행 파일 생성
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "=========================================="
	@echo "  Build Successful!"
	@echo "  Output: $(TARGET)"
	@echo "=========================================="

# 오브젝트 파일 생성
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 실행
run: $(TARGET)
	./$(TARGET)

# 정리
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Cleaned build files"

# 완전 정리
distclean: clean
	rm -rf $(BUILD_DIR)

# 도움말
help:
	@echo "Omok Game Makefile"
	@echo ""
	@echo "사용법:"
	@echo "  make          - 프로젝트 빌드"
	@echo "  make run      - 빌드 후 실행"
	@echo "  make clean    - 빌드 파일 정리"
	@echo "  make distclean- 모든 생성 파일 제거"
	@echo "  make help     - 도움말 표시"

.PHONY: all run clean distclean help