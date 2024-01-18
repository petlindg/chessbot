IDIR = include
ODIR = obj
LDIR = lib
SDIR = src
BDIR = bin

CC = gcc
CFLAGS = -I $(IDIR)
LIBS = -lm

_DEPS = types.h bot.h check.h move.h tests.h interface.h board.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o board.o move.o check.o bot.o tests.o interface.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chess: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

debug: $(OBJ)
	$(CC) -g -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)