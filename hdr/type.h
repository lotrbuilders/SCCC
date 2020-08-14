int TYPE_INT;
int TYPE_CHAR;
int TYPE_INTPTR;
int TYPE_CHARPTR;
int TYPE_PTRPTR;
char *declarator_name(int **ast);
char *declarator_type(int **ast,int type);
int find_type(int **ast);
int find_lvalue_type(int **ast);