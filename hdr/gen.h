/*
	Backend interface header for SCCC
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
int gen_prolog();
int gen_function_prolog(char *name);
int gen_function_epilog();
int gen_extern(char *name);
int gen_common(char *name);
int gen_decleration(char *name,int value);

int gen_label(int label_count);
int gen_pop();
int gen_push_constant(int constant);
int gen_dup();
int gen_subtract_sp(int count);
int gen_normalize();

int gen_return();

int gen_jz(int label_count);
int gen_jnz(int label_count);
int gen_jmp(int label_count);

int gen_assign(int size);
int gen_comparison(int type);
int gen_add();
int gen_subtract();
int gen_multiply();
int gen_negate();
int gen_pointer(int size);

int gen_constant(int constant);
int gen_local_var(int location,int size);
int gen_global_var(char *name,int size);
int gen_local_address(int location);
int gen_global_address(char *name);
int gen_function_call(char *name);
int gen_func_arg();
int gen_padding(int argc,int stackloc);

int gen_string(char *str,int count);
int gen_load_string(int count);


int TARGET_SIZEOF_INT;
int TARGET_ARGUMENT_START;
int TARGET_VARIABLE_START;
int TARGET_STACK_RESET;