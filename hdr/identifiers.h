int add_identifier(char *name);
int isvalid(char *name);
int isglobal(char *name);
int islocal(char *name);
int locate_identifier(char *name);
int enter_block();
int leave_block();