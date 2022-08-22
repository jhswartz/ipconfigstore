CFLAGS += -std=c99 -Wall -Werror -pedantic

ipconfigstore:
	$(CC) -o ipconfigstore src/*.c $(CFLAGS)

clean:
	$(RM) ipconfigstore
