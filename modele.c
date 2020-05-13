#include "modele.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include "constantes.h"

struct tag* initTag(char name[TAILLE_TAG]) {
  struct tag* t = calloc(1, sizeof(struct tag));
  t->nbEnfant = 0;
  strcpy(t->name, name);
  return t;
}

void addEnfant(struct tag* parent, struct tag* enfant) {
  parent->enfants[parent->nbEnfant++] = enfant;
}

void removeEnfant(struct tag* parent, struct tag* fils) {
  for (int i = 0; i < parent->nbEnfant; i++) {
    if (parent->enfants[i] != fils) continue;
    parent->enfants[i] = 0;
    parent->nbEnfant--;
  }
}

char isEnfant(struct tag* parent, struct tag* fils) {
  for (int i = 0; i < parent->nbEnfant; i++)
    if (parent->enfants[i] == fils) return 1;
  return 0;
}
