CFLAGS = -Wall -Werror -pedantic -std=c99 -O2

all: timeout_cat

timeout_cat_OBJS = main.o
timeout_cat: $(timeout_cat_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(timeout_cat_OBJS)

clean:
	rm timeout_cat $(timeout_cat_OBJS)

.PHONY: clean	
