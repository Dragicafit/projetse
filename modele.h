#ifndef MODELE
#define MEDELE

#include "constantes.h"

typedef struct tag {
  int nbEnfant;
  char name[TAILLE_TAG];
  struct tag* enfants[ENFANT_MAX];
} tag;

tag* initTag(char name[TAILLE_TAG]);
void removeEnfant(tag* parent, tag* fils);
char isEnfant(tag* parent, tag* fils);

#endif
