#include <dirent.h>
#include <glib.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "arborescence.h"
#include "constantes.h"
#include "fonction.h"

char dossierEcoute[TAILLE_DOSSIER_ECOUTE_MAX][TAILLE_PATH];
char fichierEcoute[TAILLE_FICHIER_ECOUTE_MAX][TAILLE_PATH];
size_t nbFichierEcoute;

int main(int argc, char const *argv[]) {
  int inotifyFd = inotify_init();
  if (inotifyFd == -1) handle_error("inotify_init");

  parcoursDossier(inotifyFd, "/");
  listenFichier(inotifyFd);
  return 0;
}