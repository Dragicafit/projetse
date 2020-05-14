
#define _DEFAULT_SOURCE

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "constantes.h"
#include "modele.h"

void parcoursDossier(char basePath[]) {
  char path[TAILLE_PATH];
  struct dirent *dp;
  DIR *dir = opendir(basePath);
  int s;

  if (!dir) {
    perror("opendir");
    return;
  }

  while ((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
    if (strlen(basePath) + strlen(dp->d_name) > TAILLE_PATH) continue;

    s = snprintf(path, TAILLE_PATH, "%s/%s", basePath, dp->d_name);
    if (s < 0) handle_error("snprintf");
    if (s > TAILLE_PATH) return;

    switch (dp->d_type) {
      case DT_DIR:
        parcoursDossier(path);
      case DT_REG:
        ajouteFicher(path);
        break;
      default:
        break;
    }
  }
  closedir(dir);
}

void ajouteFicher(char path[TAILLE_PATH]) {
  if (!is_tagged(path)) return;

  for (int i = 0; i < nbFichierEcoute; i++) {
    if (strcmp(fichierEcoute[i], path) == 0) return;
  }
  if (nbFichierEcoute >= TAILLE_FICHIER_ECOUTE) return;

  strcpy(fichierEcoute[nbFichierEcoute++], path);
  printf("fichier ajouté : %s\n", path);
}