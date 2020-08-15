int fprintf(int *stream,char *format);
int printf(char *format);

int fputs(char *s,int *stream);
int puts(char *s);

int fgetc(int *stream);
int ungetc(int c,int *stream);

int *stdin;
int *stderr;
int *stdout;

#define EOF -1