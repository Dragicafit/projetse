#ifndef DEF_FONCTION
#define DEF_FONCTION

#include <sys/types.h>
#include <unistd.h>

void add_tag(char *f, char tag[]);
void del_tag(char *f, char tag[]);
char is_tagged(const char *path);
char is_tag_user(int *fd, uid_t *uid);
void add_user();
void cp_tag(char *f, char *target);

#endif