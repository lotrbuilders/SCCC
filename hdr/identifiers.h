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

