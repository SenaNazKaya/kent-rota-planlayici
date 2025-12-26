CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src/core -I./src/gui

# Mac için Raylib
RAYLIB_INC = -I/opt/homebrew/include
RAYLIB_LIB = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit
NCURSES_LIB = -lncurses

# Hedefler
TARGET_CLI = rota_cli
TARGET_TUI = rota_tui
TARGET_GUI = rota_gui

# Kaynak dosyaları
CLI_SRC = src/archive/main_cli.c \
	src/core/durak.c \
	src/core/graf.c \
	src/core/fileio.c \
	src/core/bfs.c \
	src/core/dijkstra.c \
	src/core/minheap.c \
	src/core/queue.c

TUI_SRC = src/archive/main_ui.c \
	src/core/durak.c \
	src/core/graf.c \
	src/core/fileio.c \
	src/core/bfs.c \
	src/core/dijkstra.c \
	src/core/minheap.c \
	src/core/queue.c \
	src/archive/ui.c

GUI_SRC = src/main_raylib.c \
	src/core/durak.c \
	src/core/graf.c \
	src/core/fileio.c \
	src/gui/gui.c

.PHONY: all cli tui gui clean

all: cli tui gui

cli: $(CLI_SRC)
	$(CC) $(CFLAGS) -o $(TARGET_CLI) $(CLI_SRC)
	@echo "✅ CLI versiyonu derlendi: ./$(TARGET_CLI)"

tui: $(TUI_SRC)
	$(CC) $(CFLAGS) -o $(TARGET_TUI) $(TUI_SRC) $(NCURSES_LIB)
	@echo "✅ NCurses versiyonu derlendi: ./$(TARGET_TUI)"

gui: $(GUI_SRC)
	$(CC) $(CFLAGS) -o $(TARGET_GUI) $(GUI_SRC) $(RAYLIB_INC) $(RAYLIB_LIB)
	@echo "✅ Raylib GUI versiyonu derlendi: ./$(TARGET_GUI)"

clean:
	rm -f $(TARGET_CLI) $(TARGET_TUI) $(TARGET_GUI) *.o
	@echo "🧹 Temizlendi"

run-cli: cli
	./$(TARGET_CLI)

run-tui: tui
	./$(TARGET_TUI)

run-gui: gui
	./$(TARGET_GUI)
