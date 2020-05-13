
#define _DEFAULT_SOURCE

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "constantes.h"
#include "modele.h"

void parcoursDossier(int inotifyFd, char basePath[]) {
  char path[TAILLE_PATH];
  struct dirent *dp;
  DIR *dir = opendir(basePath);
  int s;

  if (!dir) {
    perror("opendir");
    return;
  }
  ajouteDossier(inotifyFd, basePath);

  while ((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
    if (strlen(basePath) + strlen(dp->d_name) > TAILLE_PATH) continue;

    s = snprintf(path, TAILLE_PATH, "%s/%s", basePath, dp->d_name);
    if (s < 0) handle_error("snprintf");
    if (s > TAILLE_PATH) return;

    switch (dp->d_type) {
      case DT_DIR:
        parcoursDossier(inotifyFd, path);
      case DT_REG:
        ajouteFicher(path);
        break;
      default:
        break;
    }
  }
  closedir(dir);
}

void listenFichier(int inotifyFd) {
  struct inotify_event *event;
  char *p;
  char buf[BUF_LEN] __attribute__((aligned(8)));
  int numRead;

  for (;;) {
    numRead = read(inotifyFd, buf, BUF_LEN);
    if (numRead == 0) handle_error("read() from inotify fd returned 0!");
    if (numRead == -1) handle_error("read");

    for (p = buf; p < buf + numRead;) {
      event = (struct inotify_event *)p;
      verification(inotifyFd, event);

      p += sizeof(struct inotify_event) + event->len;
    }
  }
}

void verification(int inotifyFd, struct inotify_event *event) {
  char path[TAILLE_PATH];
  int s;
  struct stat sb;

  if (event->len == 0) return;
  if (event->wd >= TAILLE_DOSSIER_ECOUTE_MAX) return;

  s = snprintf(path, TAILLE_PATH, "%s/%s", dossierEcoute[event->wd],
               event->name);
  if (s < 0) handle_error("snprintf");
  if (s > TAILLE_PATH) return;

  s = lstat(path, &sb);
  if (s < 0) {
    printf("Erreur de lecture des droits...\n");
    return;
  }

  switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:
      if (event->mask & IN_CREATE) ajouteDossier(inotifyFd, path);
    case S_IFREG:
      if (event->mask & IN_ATTRIB) ajouteFicher(path);
      break;
    default:
      break;
  }
}

void ajouteDossier(int inotifyFd, char path[TAILLE_PATH]) {
  int wd = inotify_add_watch(inotifyFd, path, IN_ATTRIB | IN_CREATE);

  if (wd == -1) handle_error("inotify_add_watch");
  if (wd >= TAILLE_DOSSIER_ECOUTE_MAX) return;

  strcpy(dossierEcoute[wd], path);
  printf("ajoute dossier : %d, %s\n", wd, path);
}

void ajouteFicher(char path[TAILLE_PATH]) {
  if (!is_tagged(path)) return;

  for (int i = 0; i < nbFichierEcoute; i++) {
    if (strcmp(fichierEcoute[i], path) == 0) return;
  }
  if (nbFichierEcoute >= TAILLE_FICHIER_ECOUTE_MAX) return;

  strcpy(fichierEcoute[nbFichierEcoute++], path);
  printf("fichier ajouté : %s\n", path);
}