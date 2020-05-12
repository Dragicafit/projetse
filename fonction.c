#define _GNU_SOURCE
#include "fonction.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include "constantes.h"

char user[TAILLE_USER];

void add_tag(char *f, char tag[]) {
  char val[TAILLE_ATTR];
  snprintf(val, TAILLE_ATTR, "%s.%s", user, tag);
  int set = setxattr(f, val, "", 0, 0);
  if (set < 0) {
    perror("Erreur d'ajout du tag");
    return;
  }
  struct stat sb;
  int s = stat(f, &sb);
  if (s < 0) {
    perror("Erreur de lecture des droits...");
    return;
  }
  if (!(sb.st_mode & S_IWGRP) && !(sb.st_mode & S_IWOTH)) return;
  char c = '\0';
  while (c != 'y' || c != 'n') {
    printf(
        "Attention, votre fichier peut etre modifie par d'autres utilisateurs. "
        "Voulez vous le proteger ? (y/n)");
    scanf("%c", &c);
  }
  if (c != 'n') return;
  sb.st_mode |= S_IWGRP;
  sb.st_mode |= S_IWOTH;
  int mod = chmod(f, sb.st_mode);
  if (mod < 0) {
    perror("Erreur de protection");
    return;
  }
  return;
}

void del_tag(char *f, char tag[]) {
  char val[TAILLE_ATTR];
  snprintf(val, TAILLE_ATTR, "%s.%s", user, tag);
  int rm = removexattr(f, val);
  if (rm < 0) {
    perror("Erreur de sup tag");
  }
  return;
}

char is_tagged(const char *path) {
  char list[100];
  char list2[100];
  char list3[100];
  listxattr(path, list, sizeof(list));
  while (1) {
    int s = sscanf(list, "%[a-zA-Z0-9].%[a-zA-Z0-9]", list2, list3);
    if (s == 2) return 1;
    if (s == -1) return 0;
  }
  return 0;
}

char is_tag_user(int *fd, uid_t *uid) {
  *uid = getuid();
  *fd = open(".users", O_RDWR | O_CREAT | O_APPEND, 0600);
  if (*fd < 0) {
    perror("Impossible d'ouvrir le fichier");
    return 0;
  }
  char buff[1024];
  read(*fd, buff, sizeof(buff));
  char id[10];
  snprintf(id, 10, "%u\n", *uid);
  if (memmem(&buff, sizeof(buff), id, sizeof(uid_t)) != NULL) {
    return 1;
  }
  return 0;
}

void add_user() {
  int fd;
  uid_t uid;
  if (!is_tag_user(&fd, &uid)) {
    char id[10];
    snprintf(id, 10, "%u\n", uid);
    write(fd, id, strlen(id));
  }
  close(fd);
}

void cp_tag(char *f) {}