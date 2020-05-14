#include "modele.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include "constantes.h"

tag* initTag(char name[TAILLE_TAG]) {
  tag* t = calloc(1, sizeof(tag));
  t->nbEnfant = 0;
  strcpy(t->name, name);
  return t;
}

void addEnfant(tag* parent, tag* enfant) {
  parent->enfants[parent->nbEnfant++] = enfant;
}

void removeEnfant(tag* parent, tag* fils) {
  for (int i = 0; i < parent->nbEnfant; i++) {
    if (parent->enfants[i] != fils) continue;
    parent->enfants[i] = 0;
    parent->nbEnfant--;
  }
}

char isEnfant(tag* parent, tag* fils) {
  for (int i = 0; i < parent->nbEnfant; i++)
    if (parent->enfants[i] == fils) return 1;
  return 0;
}
