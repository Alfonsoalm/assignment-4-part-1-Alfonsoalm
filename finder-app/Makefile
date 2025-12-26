
# Makefile para construir finder-app/writer con soporte de compilación cruzada

# Prefijo de compilador cruzado (p.ej., make CROSS_COMPILE=aarch64-none-linux-gnu-)
CC := $(CROSS_COMPILE)gcc

# SRCSFlags de compilación y enlace
CFLAGS := -Wall -Wextra -O2
# que son estas lineas?
# 1. `CFLAGS := -Wall -Wextra -O2` define las banderas de compilación que se utilizarán al compilar los archivos fuente.
#    - `-Wall` habilita la mayoría de las advertencias del compilador.
#    - `-Wextra` habilita advertencias adicionales que no están cubiertas por `-Wall`.
#    - `-O2` es una bandera de optimización que indica al compilador que optimice el código para mejorar el rendimiento sin aumentar demasiado el tiempo de compilación
LDFLAGS :=

# Fuentes y objetos
SRCS := writer.c
OBJS := $(SRCS:.c=.o) # Lista de archivos objeto

# Binario objetivo
TARGET := writer

# Objetivo por defecto: construir la app
.PHONY: all
all: $(TARGET)

# Enlazado del binario final
# Explica esstas lineas:
# 1. `$(TARGET): $(OBJS)` define la regla para construir el binario objetivo a partir de los archivos objeto.
# 2. `$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)` es el comando que utiliza el compilador para enlazar los
#    archivos objeto y generar el binario final, aplicando las banderas de compilación y enlace especificadas.
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Regla genérica para construir archivos objeto desde archivos fuente
# Compilación de .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Limpieza: elimina binario y objetos
clean: 
	$(RM) $(TARGET) $(OBJS)

