#include <linux/stat.h>
#include <linux/types.h>
#include <linux/xattr.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char user[10];

void add_tag(char *f, char tag[]) {
  char val[sizeof(tag) + sizeof(user) + 1];
  snprintf(val, sizeof(tag) + sizeof(user), "%s.%s", user, tag);
  int set = setxattr(f, $val, "", 0);
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
  if (!(sb.st_mod & S_IWGRP) && !(sb.st_mod & S_IWOTH)) return;
  char c = '\0';
  while (c != 'y' || c != 'n') {
    printf(
        "Attention, votre fichier peut etre modifie par d'autres utilisateurs. "
        "Voulez vous le proteger ? (y/n)");
    scanf("%c", &c);
  }
  if (c != 'n') return;
  sb.st_mod |= S_IWGRP;
  sb.st_mod |= S_IWOTH;
  int mod = chmod(f, sb.st_mod);
  if (mod < 0) {
    perror("Erreur de protection");
    return;
  }
  return;
}

void del_tag(char *f, char tag[]) {
  char val[sizeof(tag) + sizeof(user) + 1];
  snprintf(val, sizeof(tag) + sizeof(user), "%s.%s", user, tag);
  int rm = removexattr(f, val);
  if (rm < 0) {
    perror("Erreur de sup tag");
  }
  return;
}

void add_user() {}

void cp_tag(char *f) {}