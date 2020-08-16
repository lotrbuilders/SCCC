/*
	Parse/Lexer symbol defenitions for SCCC
    Copyright (C) 2020  Daan Oosterveld

    SCCC is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    SCCC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
int SYM_EOF=-1;

int SYM_INT=128;
int SYM_CHAR=129;

int SYM_RETURN=144;
int SYM_IF=145;
int SYM_ELSE=147;
int SYM_FOR=148;
int SYM_WHILE=149;

int SYM_ID=192;
int SYM_CONSTANT=193;
int SYM_STRING=194;

int SYM_FUNC_DEF=256;
int SYM_FUNC_DECL=257;
int SYM_GLOBAL_DEF=258;
int SYM_GLOBAL_DECL=259;
int SYM_TRANSLATION_UNIT=260;
int SYM_COMPOUND_STATEMENT=261;

int SYM_NEGATE=512;
int SYM_LE=513;
int SYM_GE=514;
int SYM_NE=515;
int SYM_EQ=516;
int SYM_LOR=517;
int SYM_LAND=518;
int SYM_POINTER=519;
int SYM_ADDRESS=520;
int SYM_SIZEOF=521;

int SYM_STATEMENT_LIST=1024;
int SYM_DECLARATION=1025;
int SYM_FUNC_CALL=1026;
int SYM_EXPR_LIST=1027;
int SYM_DECL_LIST=1028;
int SYM_LOCAL_DECLARATION=1029;
int SYM_DECLERATOR=1030;