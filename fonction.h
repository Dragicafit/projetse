#ifndef DEF_FONCTION
#define DEF_FONCTION

#include <sys/types.h>
#include <unistd.h>

#include "constantes.h"

void add_tag(char *f, char tag[]);
void del_tag(char *f, char tag[]);
char is_tagged(const char *path);
char is_tag_user(int *fd, uid_t *uid);
void add_user();
void cp_tag(char *f, char *target);
char has_tag(const char *path, char tag[TAILLE_TAG]);
void show_by_tag(char conj[TAILLE_LIST_ATTR][TAILLE_TAG],
                 char dij[TAILLE_LIST_ATTR][TAILLE_TAG], int size_conj,
                 int size_dij);

#endif