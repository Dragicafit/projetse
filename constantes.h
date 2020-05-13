#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#include <sys/types.h>

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

#define TAILLE_TAG 255
#define TAILLE_USER 255
#define TAILLE_DOSSIER_ECOUTE_MAX 10000
#define TAILLE_FICHIER_ECOUTE_MAX 1000
#define TAILLE_PATH 1024
#define TAILLE_LIST_ATTR 1000
#define TAILLE_ATTR TAILLE_TAG + TAILLE_USER + 1

#define BUF_LEN (10 * (sizeof(struct inotify_event) + 300 + 1))

extern char dossierEcoute[TAILLE_DOSSIER_ECOUTE_MAX][TAILLE_PATH];
extern char fichierEcoute[TAILLE_FICHIER_ECOUTE_MAX][TAILLE_PATH];
extern size_t nbFichierEcoute;

#endif