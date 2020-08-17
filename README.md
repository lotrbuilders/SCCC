# sccc
sccc (self compiling C compiler) is a compiler for a subset of C, which can compile itself and produces x86 assembly.

## Implemented parts of C
This compiler implements a subset of C99. Supported are:
1. addition, subtraction and multiplication and negation.
2. all comparison operators.
3. logical or and logical and.
4. pointers, references and sizeof(type)
5. function calls, identifiers, constants and strings
6. if, if-else, while, for and compound statements
7. return statements and expression statements
8. local and global int, char, and pointer declerations and defenitions

## Limitations
sccc also has a few implementation limitations, which can be imported.
1. a global variable can't be both declared and defined, only one of the two is possible in a single source file
2. the compiler itself does not test if a function is declared, instead this is left to the assembler
3. because of point 2 one can also call a function before its declared or defined

## Structure of sccc
sccc needs a couple of components. The preprocessor, the actual c compiler, an assembler, and a linker and a master component to combine these separete programs. For simplicity sccc uses the systems standard c preprocessor, nasm as an assembler and the systems standard linker. The components are called by the master program sccc. The systems standard c library is also used during linking.

### Structure of the compiler
The compiler component follows a standard structure. The program is first broken down into lexical components, which are parsed by a handwritten recursive descent parser. The resulting abstract syntax tree is then evaluated by a (mostly) backend independent evaluator, which calls the backend to create output assembly.

### Intermidiate code
The backend responds to a series of function calls as layed out in hdr/gen.h This assumes an accumulator-stack machine. The evaluator assumes right to left evaluation, where arguments are only passed on the stack. This limits the possible backend ABI's at this moment. The only backend currently implemented is an x86 backend, which creates working, but inefficient assembly. Both a better code generator and a peephole optimizer could solve this.

## Compiling, installing and running
The compiler can be compiled with any c compiler. One can either use the Makefile with or use the shell scripts run and run-self. The last two automatically use the compiler to test a c program called test.c.
To do this first an object directory needs to be created at <top>/obj.
 ```
 mkdir ./obj/
 make all
 ```
To actually use sccc as a standard the program and include files will need to the designated sccc directory as specified in src/sccc.c. This is normally /usr/local/lib/sccc/.
The following sequence of commands will set this up. Note this has to be done after compilation
 ```
 mkdir /usr/local/lib/sccc/
 cp ./compiler /usr/local/lib/sccc/
 cp ./sccc /usr/local/lib/sccc/
 cp -r ./include/ /usr/local/include/
 ```
To get sccc to be recognized as a command in the terminal, it can either be added to any standard /bin/ folder, added as a link to said folder or the sccc directory can be added to the system path add startup
 
 ## Licensing
 The main files of this program are licensed under GPL 3.0. as specified in the accompanying license file. Specifically anything in the top level directory and anything in /src and /hdr and the /include header. However, everything in the /testing folder is licensed under an MIT license as specified in said folder. Programs which are compiled by sccc, but do not use any sccc source code fall under normal use of the compiler and are thus not bound by the GPL license. The header files in /include/ can also be used in any files, which are compiled with sccc, specifically using these files in '#include <{header name}>' is also seen as normal use of this software. However, editing these files does not constitute normal use.
