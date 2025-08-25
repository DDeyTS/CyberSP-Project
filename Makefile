# Nome do executável
TARGET = cyber

# Compilador
CC = gcc

# Arquivos fonte
SRC = main.c collision.c tile_render.c bitmap.c dialoguesys.c textdat.c debug.c input.c game.c

# Flags de compilação
CFLAGS = -Wall -O2 -g `pkg-config --cflags allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5`

# Bibliotecas necessárias
LIBS = `pkg-config --libs allegro-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5` -ltmx -lxml2 -lz -lm

# Regra principal
all: $(TARGET)

$(TARGET): $(SRC)
	@ echo $<
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Limpeza
clean:
	@echo cleaning...
	@rm -f $(TARGET)

run:
	@echo running...
	@./cyber

