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
$(ODIR)/identifiers.o : identifiers.c gen.h node.h symbols.h
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

$(ODIR)/sccc.o : sccc.c
ifneq ($(CC),sccc)
	$(CC) $(CFLAGS) -o $@ $<
else
	./sccc.sh  $< $@ 2>/dev/null
endif
	
compiler : $(OBJ) $(ODIR)/x86-gen.o
	gcc -m32 -o $@ $^
	
sccc : sccc.o
	gcc -m32 -o $@ $^
	
	
ifeq ($(OS),Windows_NT)  
clean :
	del /q "$(ODIR)/"
else
clean :
	-rm  $(OBJ) $(ODIR)/x86-gen.o $(ODIR)/sccc.o
endif
