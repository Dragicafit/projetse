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

void add_tag(char *path, char tag[]) {
  int fd;
  uid_t uid;
  if (!is_tag_user(&fd, &uid)) return;
  close(fd);
  char t[TAILLE_ATTR];
  snprintf(t, TAILLE_ATTR, "user.%u.%s", uid, tag);
  int set = setxattr(path, t, "", 0, 0);
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

void del_tag(char *f, char tag[]) {
  int fd;
  uid_t uid;
  if (!is_tag_user(&fd, &uid)) return;
  close(fd);
  char t[TAILLE_ATTR];
  snprintf(t, TAILLE_ATTR, "user.%u.%s", uid, tag);
  int rm = removexattr(f, t);
  if (rm < 0) {
    perror("Erreur de sup tag");
  }
  return;
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

char has_tag(const char *path, char tag[TAILLE_TAG]) {
  char l[TAILLE_LIST_ATTR];
  snprintf(l, TAILLE_LIST_ATTR, "user.%u.%s", getuid(), tag);
  return getxattr(path, l, NULL, NULL) >= 0;
}

char is_tag_user(int *fd, uid_t *uid) {
  *uid = getuid();
  *fd = open(".users", O_RDWR | O_CREAT | O_APPEND, 0600);
  if (*fd < 0) {
    handle_error("Impossible d'ouvrir le fichier");
  }
  char buff[1024];
  read(*fd, buff, sizeof(buff));
  char id[TAILLE_USER];
  snprintf(id, TAILLE_USER, "%u\n", *uid);
  if (memmem(&buff, strlen(buff), id, strlen(id)) != NULL) {
    return 1;
  }
  return 0;
}

void add_user() {
  int fd;
  uid_t uid;
  if (!is_tag_user(&fd, &uid)) {
    char id[TAILLE_USER];
    snprintf(id, TAILLE_USER, "%u\n", uid);
    write(fd, id, strlen(id));
  }
  close(fd);
}

void list_files_by_tag(char tag[],
                       char path_list[TAILLE_FICHIER_ECOUTE_MAX][TAILLE_PATH]) {
  int pos = 0;
  for (int i = 0; i < TAILLE_FICHIER_ECOUTE_MAX; i++) {
    if (has_tag(fichierEcoute[i], tag)) {
      strcpy(path_list[pos++], fichierEcoute[i]);
    }
  }
}

void show_by_tag(char conj[TAILLE_LIST_ATTR][TAILLE_TAG],
                 char dij[TAILLE_LIST_ATTR][TAILLE_TAG], int size_conj,
                 int size_dij) {
  int pos = 0;
  for (int i = 0; i < nbFichierEcoute; i++) {
    char test = 1;
    for (int j = 0; j < size_conj; j++) {
      if (!has_tag(fichierEcoute[i], conj[j])) {
        test = 0;
        break;
      }
    }
    for (int j = 0; j < size_dij; j++) {
      if (has_tag(fichierEcoute[i], dij[j])) {
        test = 0;
        break;
      }
    }
    if (test) printf("%s\n", fichierEcoute[i]);
  }
}

void cp_tag(char *f, char *target) {
  int fd;
  uid_t uid;
  if (!is_tag_user(&fd, &uid)) {
    execlp("cp", "cp", f, target, NULL);
    return;
  }
  execlp("cp", "cp", "--preserve=xattr", f, target, NULL);
}
