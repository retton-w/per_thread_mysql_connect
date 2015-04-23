TARGET=libmy_conn.a

CC = gcc

CFLAGS = -Wall -O2 -I .

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

DEPS = $(patsubst %.o,%.d,$(OBJS))

all : $(TARGET)

$(TARGET):$(OBJS)	
	ar crs $@ $^

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.d *.o $(TARGET)

-include $(DEPS)
