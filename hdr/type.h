/*
	Type operations header for SCCC
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
int TYPE_INT;
int TYPE_CHAR;
int TYPE_INTPTR;
int TYPE_CHARPTR;
int TYPE_PTRPTR;
char *declarator_name(int **ast);
char *declarator_type(int **ast,int type);
int find_type(int **ast);
int find_lvalue_type(int **ast);