/*
	Identifier operations header for SCCC
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
int add_identifier(char *name,int type);
int add_argument_identifier(char *name,int type,int loc);
int add_global(char *name,int type);
int isvalid(char *name);
int isglobal(char *name);
int islocal(char *name);
int locate_identifier(char *name);
int find_id_type(char *name);
int enter_block();
int leave_block(int reset_stack);

