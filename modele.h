#ifndef DEF_MODELE
#define DEF_MEDELE

#include <glib.h>
#include <glib/gprintf.h>

#include "constantes.h"

typedef struct tag {
  int nbEnfant;
  char name[TAILLE_TAG];
  struct tag *enfants[ENFANT_MAX];
} tag;

tag *initTag(char name[TAILLE_TAG]);
void addEnfant(tag *parent, tag *enfant);
void removeEnfant(tag *parent, tag *fils);
char isEnfant(tag *parent, tag *fils);
void add_tag(const char *path, tag *t);
void del_tag(const char *path, tag *t);
char is_tagged(const char *path);
char is_tag_user(int *fd, uid_t *uid);
void add_user();
void cp_tag(char *f, char *target);
char has_tag(const char *path, tag *t);
void show_by_tag(gchar **conj, gchar **dij, int size_conj, int size_dij);
void parcoursDossier(char *basePath);
void ajouteFicher(char path[TAILLE_PATH]);
void createHierarchieFile(tag *t[], int t_length);
void createHierarchie(GKeyFile *key_file, tag *t);
void readHierarchieFile(tag *tags[], int *tags_length);
void createTagsHierarchie(GKeyFile *key_file, GError *error, tag *t);
tag *rechercheTag(char t[TAILLE_TAG]);
#endif
