/*
	Parser/Lexer symbols header for SCCC
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
int SYM_EOF;

int SYM_INT;
int SYM_CHAR;

int SYM_RETURN;
int SYM_IF;
int SYM_ELSE;
int SYM_FOR;
int SYM_WHILE;

int SYM_ID;
int SYM_CONSTANT;
int SYM_STRING;

int SYM_FUNC_DEF;
int SYM_FUNC_DECL;
int SYM_GLOBAL_DEF;
int SYM_GLOBAL_DECL;
int SYM_TRANSLATION_UNIT;
int SYM_COMPOUND_STATEMENT;

int SYM_NEGATE;
int SYM_LE;
int SYM_GE;
int SYM_NE;
int SYM_EQ;
int SYM_LOR;
int SYM_LAND;
int SYM_POINTER;
int SYM_ADDRESS;
int SYM_SIZEOF;

int SYM_STATEMENT_LIST;
int SYM_DECLARATION;
int SYM_FUNC_CALL;
int SYM_EXPR_LIST;
int SYM_DECL_LIST;
int SYM_LOCAL_DECLARATION;
int SYM_DECLARATOR;