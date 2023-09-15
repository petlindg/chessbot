IDIR = include
ODIR = obj
LDIR = lib
SDIR = src
BDIR = bin

CC = gcc
CFLAGS = -I $(IDIR)
LIBS = -lm

_DEPS = chess.h interface.h types.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o chess.o interface.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chess: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~