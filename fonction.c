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

void add_user() {
  uid_t uid = getuid();
  int fd = open(".users", O_RDWR | O_CREAT | O_APPEND, 0600);
  if (fd < 0) {
    perror("Impossible de créer le fichier");
    return;
  }
  char buff[1024];
  read(fd, buff, sizeof(buff));
  char id[10];
  snprintf(id, 10, "%u\n", uid);
  if (memmem(&buff, sizeof(buff), id, sizeof(uid_t)) == NULL) {
    write(fd, id, strlen(id));
  }
  close(fd);
}

void cp_tag(char *f) {}