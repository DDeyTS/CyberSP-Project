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
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compilação de cada .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

run:
	@echo "Running..."
	@$(BUILD_DIR)/$(TARGET)

DEBUG_FLAGS := -g -O0
debug: CFLAGS := $(filter-out -O2,$(CFLAGS)) $(DEBUG_FLAGS)
debug: clean all 
	@echo "Iniciando GDB"
	@gdb $(BUILD_DIR)/$(TARGET)

.PHONY: all clean
