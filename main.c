#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>

#include "constantes.h"

char *dossierEcoute[TAILLE_ECOUTE_MAX];
char *fichierEcoute[TAILLE_ECOUTE_MAX];
size_t nbDossierEcoute;
size_t nbFichierEcoute;

void parcoursDossier(char *basePath);

int main(int argc, char const *argv[]) {
  parcoursDossier("/tmp");
  return 0;
}

void parcoursDossier(char basePath[]) {
  char path[TAILLE_PATH];
  struct dirent *dp;
  struct stat sb;
  DIR *dir = opendir(basePath);
  int set;
  char val[TAILLE_LIST_ATTR];

  if (!dir) return;

  while ((dp = readdir(dir)) != NULL) {
    snprintf(path, TAILLE_PATH, "%s/%s", basePath, dp->d_name);
    int s = stat(path, &sb);
    if (s < 0) {
      perror("Erreur de lecture des droits...");
      return;
    }
    switch (sb.st_mode & __S_IFMT) {
      case __S_IFDIR:
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
          continue;
        if (nbDossierEcoute < TAILLE_ECOUTE_MAX)
          dossierEcoute[nbDossierEcoute++] = path;

        parcoursDossier(path);
      case __S_IFREG:
        set = listxattr(path, val, TAILLE_LIST_ATTR);
        if (set < 0) {
          perror("Erreur de listage d'un tag");
          return;
        }
        if (set < 1) break;
        printf("fichier ajouté : %s\n", path);
        if (nbFichierEcoute < TAILLE_ECOUTE_MAX)
          fichierEcoute[nbFichierEcoute++] = path;
        break;
      default:
        break;
    }
  }
  closedir(dir);
}