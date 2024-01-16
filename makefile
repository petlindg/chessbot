IDIR = include
ODIR = obj
LDIR = lib
SDIR = src
BDIR = bin

CC = gcc
CFLAGS = -I $(IDIR)
LIBS = -lm

_DEPS = board.h bot.h check.h interface.h move.h tests.h types.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = board.o interface.o move.o check.o bot.o tests.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chess: $(OBJ)
	echo $(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

debug: $(OBJ)
	$(CC) -g -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)