#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>

#include "constantes.h"
#include "modele.h"

char user[TAILLE_USER];

void add_tag(const char *path, tag *t) {
  uid_t uid = getuid();
  if (!is_tag_user()) return;
  char buff[TAILLE_ATTR];
  snprintf(buff, TAILLE_ATTR, "user.%u.%s", uid, t->name);
  int set = setxattr(path, buff, "", 0, 0);
  if (set < 0) {
    perror("Erreur d'ajout du tag");
    return;
  }
  struct stat sb;
  int s = stat(path, &sb);
  if (s < 0) {
    perror("Erreur de lecture des droits...");
    return;
  }
  if (!(sb.st_mode & S_IWGRP) && !(sb.st_mode & S_IWOTH)) return;
  char c[TAILLE_BUF] = "";
  while (strcmp(c, "y") != 0 && strcmp(c, "yes") != 0) {
    printf(
        "Attention, votre fichier peut etre modifie par d'autres utilisateurs. "
        "Voulez vous le proteger ? (y/n)");
    scanf("%s", c);
    if (strcmp(c, "no") == 0 || strcmp(c, "n") == 0) return;
  }
  sb.st_mode &= ~S_IWGRP;
  sb.st_mode &= ~S_IWOTH;
  int mod = chmod(path, sb.st_mode);
  if (mod < 0) {
    perror("Erreur de protection");
    return;
  }
  return;
}

void del_tag(const char *path, tag *t) {
  uid_t uid = getuid();
  if (!is_tag_user()) return;
  char l[TAILLE_ATTR];
  snprintf(l, TAILLE_ATTR, "user.%u.%s", uid, t->name);
  if (removexattr(path, l) >= 0) return;
  for (int i = 0; i < t->nbEnfant; i++) del_tag(path, t->enfants[i]);
}

char is_tagged(const char *path) {
  char list[TAILLE_LIST_ATTR];
  ssize_t count = 0;
  int add = 0;
  int s;
  ssize_t size = listxattr(path, list, TAILLE_LIST_ATTR);
  if (size < 0) perror("Erreur de listage d'un tag");
  if (size < 1) return 0;
  while (count < size) {
    s = sscanf(&list[count], "user.%*u%n.%*[a-zA-Z0-9]", &add);
    if (s == 0) return 1;
    count += add + 1;
  }
  return 0;
}

char has_tag(const char *path, tag *t) {
  char l[TAILLE_LIST_ATTR];
  snprintf(l, TAILLE_LIST_ATTR, "user.%u.%s", getuid(), t->name);
  if (getxattr(path, l, NULL, 0) >= 0) return 1;
  for (int i = 0; i < t->nbEnfant; i++) {
    if (has_tag(path, t->enfants[i])) return 1;
  }
  return 0;
}

char is_tag_user() {
  char path[TAILLE_PATH];
  if (getPathHierarchie(path) < 0) return 0;

  int fd = open(path, O_RDONLY);
  if (fd < 0) return 0;
  close(fd);
  return 1;
}

void add_user() {
  char path[TAILLE_PATH];
  if (getPathHierarchie(path) < 0) return;

  int fd = open(path, O_CREAT, 0600);
  if (fd < 0) handle_error("impossible de créer le fichier");
  close(fd);
}

void remove_user() {
  char path[TAILLE_PATH];
  if (getPathHierarchie(path) < 0) return;

  int fd = remove(path);
  if (fd < 0) handle_error("impossible de supprimer le fichier");
}

tag *rechercheTag(char t[TAILLE_TAG]) {
  for (int i = 0; i < tags_length; i++) {
    if (strcmp(list_tags[i]->name, t) == 0) {
      return list_tags[i];
    }
  }
  tag *new_tag = initTag(t);
  if (tags_length < TAILLE_LIST_TAG) list_tags[tags_length++] = new_tag;
  return new_tag;
}

void show_by_tag(char conj[TAILLE_CONJ][TAILLE_TAG],
                 char dij[TAILLE_CONJ][TAILLE_TAG], int size_conj,
                 int size_dij) {
  for (int i = 0; i < nbFichierEcoute; i++) {
    char test = 1;
    for (int j = 0; j < size_conj; j++) {
      tag *c = rechercheTag(conj[j]);
      if (!has_tag(fichierEcoute[i], c)) {
        test = 0;
      }
    }
    if (!test) continue;
    for (int j = 0; j < size_dij; j++) {
      tag *d = rechercheTag(dij[j]);
      if (has_tag(fichierEcoute[i], d)) {
        test = 0;
      }
    }
    if (test) printf("%s\n", fichierEcoute[i]);
  }
}

void cp_tag(int argc, char *argv[]) {
  char preserve = !!is_tag_user();

  char **argv_new = calloc(argc + preserve + 2, sizeof(char *));

  argv_new[0] = "cp";
  if (preserve) argv_new[1] = "--preserve=xattr";
  for (int i = 0; i < argc; i++) argv_new[i + preserve + 1] = argv[i];
  argv_new[argc + preserve + 1] = NULL;

  execvp("cp", argv_new);
}
