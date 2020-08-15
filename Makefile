IDIR = ./hdr
ODIR = ./obj
CC ?= gcc
CFLAGS = -I$(IDIR) -m32 -std=gnu99 -c -Wno-int-conversion -Wno-pointer-to-int-cast -Wno-incompatible-pointer-types

vpath %.c ./src
vpath %.h $(IDIR)
vpath %.h ./include
vpath %.o $(ODIR)

_OBJ = errorhandling.o symbols.o lexer.o main.o parser.o node.o eval.o identifiers.o type.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all : compiler

$(ODIR)/errorhandling.o : errorhandling.c stdio.h stdlib.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
$(ODIR)/symbols.o : symbols.c 
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
$(ODIR)/node.o : node.c 
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
$(ODIR)/identifiers.o : identifiers.c
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
$(ODIR)/type.o : type.c identifiers.h symbols.h errorhandling.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
	
$(ODIR)/lexer.o : lexer.c errorhandling.h symbols.h lexer.h node.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
	
$(ODIR)/parser.o : parser.c errorhandling.h symbols.h parser.h type.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif

$(ODIR)/eval.o : eval.c errorhandling.h eval.h gen.h identifiers.h type.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif

$(ODIR)/x86-gen.o : x86-gen.c symbols.h
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
	
$(ODIR)/main.o : main.c lexer.h symbols.h parser.h eval.h 
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
	
compiler : $(OBJ) $(ODIR)/x86-gen.o
	gcc -m32 -o $@ $^
	
ifeq ($(OS),Windows_NT)  
clean :
	del /q "$(ODIR)/"
else
clean :
	-rm  $(OBJ)
endif
