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
char is_tag_user();
void add_user();
void remove_user();
void cp_tag(int argc, char *argv[]);
char has_tag(const char *path, tag *t);
void show_by_tag(char conj[TAILLE_CONJ][TAILLE_TAG],
                 char dij[TAILLE_CONJ][TAILLE_TAG], int size_conj,
                 int size_dij);
void parcoursDossier(char *basePath);
void ajouteFicher(char path[TAILLE_PATH]);
char getPathHierarchie(char path[TAILLE_PATH]);
void createHierarchieFile();
void createHierarchie(GKeyFile *key_file, tag *t);
void readHierarchieFile();
void createTagsHierarchie(GKeyFile *key_file, GError *error, tag *t);
tag *rechercheTag(char t[TAILLE_TAG]);

#endif
