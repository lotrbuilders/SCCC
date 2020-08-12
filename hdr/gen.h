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

int gen_assign();
int gen_comparison(int type);
int gen_add();
int gen_subtract();
int gen_multiply();
int gen_negate();
int gen_constant(int constant);
int gen_local_var(int location);
int gen_global_var(char *name);
int gen_local_address(int location);
int gen_global_address(char *name);
int gen_function_call(char *name);