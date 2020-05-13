#ifndef DEF_ARBORESCENCE
#define DEF_ARBORESCENCE

#include <sys/inotify.h>

#include "constantes.h"

void parcoursDossier(int inotifyFd, char *basePath);
void verification(int inotifyFd, struct inotify_event *event);
void listenFichier(int inotifyFd);
void ajouteDossier(int inotifyFd, char path[TAILLE_PATH]);
void ajouteFicher(char path[TAILLE_PATH]);

#endif