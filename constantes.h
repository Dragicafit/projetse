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
#define TAILLE_BUF 255
#define TAILLE_FICHIER_ECOUTE 1000
#define TAILLE_PATH 1024
#define TAILLE_LIST_ATTR 1000
#define TAILLE_ATTR TAILLE_TAG + TAILLE_USER + 6
#define ENFANT_MAX 124
#define TAILLE_LIST_TAG 1000
#define TAILLE_CONJ 1000

extern struct tag *list_tags[TAILLE_LIST_TAG];
extern int tags_length;
extern char fichierEcoute[TAILLE_FICHIER_ECOUTE][TAILLE_PATH];
extern size_t nbFichierEcoute;

#endif