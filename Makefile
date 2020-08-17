#	Makefile for SCCC
#   Copyright (C) 2020  Daan Oosterveld

#   SCCC is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.

#   SCCC is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

all : compiler sccc

$(ODIR)/errorhandling.o : errorhandling.c stdio.h stdlib.h
	$(CC) $(CFLAGS) -o $@ $<

$(ODIR)/symbols.o : symbols.c 
	$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/node.o : node.c 
	$(CC) $(CFLAGS) -o $@ $<

$(ODIR)/identifiers.o : identifiers.c gen.h node.h symbols.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/type.o : type.c identifiers.h symbols.h errorhandling.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/lexer.o : lexer.c errorhandling.h symbols.h lexer.h node.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(ODIR)/parser.o : parser.c errorhandling.h symbols.h parser.h type.h
	$(CC) $(CFLAGS) -o $@ $<

$(ODIR)/eval.o : eval.c errorhandling.h eval.h gen.h identifiers.h type.h
	$(CC) $(CFLAGS) -o $@ $<


$(ODIR)/x86-gen.o : x86-gen.c symbols.h
	$(CC) $(CFLAGS) -o $@ $<

	
$(ODIR)/main.o : main.c lexer.h symbols.h parser.h eval.h 
	$(CC) $(CFLAGS) -o $@ $<

$(ODIR)/sccc.o : sccc.c
	$(CC) $(CFLAGS) -o $@ $<
	
compiler : $(OBJ) $(ODIR)/x86-gen.o
	cc -m32 -o $@ $^
	
sccc : $(ODIR)/sccc.o
	cc -m32 -o $@ $^
	
	
ifeq ($(OS),Windows_NT)  
clean :
	del /q "$(ODIR)/"
else
clean :
	-rm  $(OBJ) $(ODIR)/x86-gen.o $(ODIR)/sccc.o
endif
