IDIR = ./hdr
ODIR = ./obj
CC = gcc
CFLAGS = -I$(IDIR) -m32 -std=gnu99 -c -Wno-int-conversion -Wno-pointer-to-int-cast -Wno-incompatible-pointer-types

vpath %.c ./src
vpath %.h $(IDIR)
vpath %.h ./include
vpath %.o $(ODIR)

_OBJ = errorhandling.o symbols.o lexer.o main.o parser.o node.o eval.o identifiers.o type.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all : compiler

$(ODIR)/errorhandling.o : errorhandling.c stdio.h stdlib.h
	./sccc.sh  $< 2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/symbols.o : symbols.c 
	./sccc.sh  $< 2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/node.o : node.c 
	./sccc.sh  $< 2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/identifiers.o : identifiers.c
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/type.o : type.c identifiers.h symbols.h errorhandling.h
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/lexer.o : lexer.c errorhandling.h symbols.h lexer.h node.h
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/parser.o : parser.c errorhandling.h symbols.h parser.h type.h
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/eval.o : eval.c errorhandling.h eval.h gen.h identifiers.h type.h
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<

$(ODIR)/x86-gen.o : x86-gen.c symbols.h
	./sccc.sh  $<  2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/main.o : main.c lexer.h symbols.h parser.h eval.h 
	./sccc.sh  $< 2>/dev/null
	cp ./tmp.o $@
	#$(CC) $(CFLAGS) -o $@ $<
	
compiler : $(OBJ) $(ODIR)/x86-gen.o
	$(CC) -m32 -o $@ $^
	
ifeq ($(OS),Windows_NT)  
clean :
	del /q "$(ODIR)/"
else
clean :
	-rm  $(OBJ)
endif
