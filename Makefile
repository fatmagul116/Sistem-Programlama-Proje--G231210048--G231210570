# Derleyici ve Bayraklar
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

# Klasör Yapısı
SRC_DIR = src
OBJ_DIR = src
INC_DIR = include

# Kaynak Dosyalar ve Nesne Dosyaları
# Dizindeki tüm .c dosyalarını otomatik bulur
SRCS = $(SRC_DIR)/tarsau.c $(SRC_DIR)/archive.c $(SRC_DIR)/extract.c
OBJS = $(SRCS:.c=.o)

# Hedef Program Adı
TARGET = tarsau

# Varsayılan Hedef (Sadece 'make' yazınca çalışır)
all: $(TARGET)

# Programı Bağlama (Link)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# .c dosyalarını .o dosyalarına dönüştürme
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Temizlik İşlemi (Derlenmiş dosyaları siler)
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Yeniden Derleme
rebuild: clean all

.PHONY: all clean rebuild