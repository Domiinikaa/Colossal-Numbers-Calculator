CC       = gcc
CFLAGS   = -std=c23 -pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond \
           -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init \
           -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror \
           -fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover \
           -g -fno-omit-frame-pointer -O1

# Nazwa pliku wynikowego oraz plików źródłowych
TARGET   = colossal
SRCS     = main.c
OBJS     = $(SRCS:.c=.o)

# Domyślny cel
all: $(TARGET)

# Linkowanie pliku wykonywalnego
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Kompilacja plików źródłowych do .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Testowanie pamięci za pomocą Valgrinda
valgrind: $(TARGET)
	valgrind --leak-check=full -q --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean valgrind
