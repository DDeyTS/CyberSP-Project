# Nome do executável final
TARGET = cyber

# Diretórios
SRC_DIR := source
INC_DIR := include
BUILD_DIR := build

# Compilador e flags
# CC = gcc

CFLAGS := -Wall -Wextra -O2 \
	-I$(INC_DIR) -Ivendor/tmx/src \
	`pkg-config --cflags allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5`

LDFLAGS := -Lvendor/tmx
LDLIBS :=  \
				-ltmx -lxml2 -lz -lm \
				`pkg-config --libs allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5` \


# Arquivos fonte e objetos
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Regra padrão
all: $(BUILD_DIR)/$(TARGET)

# Linkagem final
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compilação de cada .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

.PHONY: all clean

# # Nome do executável
# TARGET = cyber
#
# # Compilador
# CC = gcc
#
# # Arquivos fonte
# SRC = main.c collision.c tile_render.c bitmap.c dialoguesys.c textdat.c debug.c input.c game.c
#
# # Flags de compilação
# CFLAGS = -Wall -O2 -g `pkg-config --cflags allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5`
#
# # Bibliotecas necessárias
# LIBS = `pkg-config --libs allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5` -ltmx -lxml2 -lz -lm
#
# # Regra principal
# all: $(TARGET)
#
# $(TARGET): $(SRC)
# 	@ echo $<
# 	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)
#
# # Limpeza
# clean:
# 	@echo cleaning...
# 	@rm -f $(TARGET)
#
# run:
# 	@echo running...
# 	@./cyber
#
