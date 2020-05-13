#ifndef DEF_MODELE
#define DEF_MEDELE

#include <glib.h>
#include <sys/inotify.h>

#include "constantes.h"

struct tag {
  int nbEnfant;
  char name[TAILLE_TAG];
  struct tag *enfants[ENFANT_MAX];
};

struct tag *initTag(char name[TAILLE_TAG]);
void addEnfant(struct tag *parent, struct tag *enfant);
void removeEnfant(struct tag *parent, struct tag *fils);
char isEnfant(struct tag *parent, struct tag *fils);
void add_tag(const char *path, struct tag *t);
void del_tag(const char *path, struct tag *t);
char is_tagged(const char *path);
char is_tag_user(int *fd, uid_t *uid);
void add_user();
void cp_tag(char *f, char *target);
char has_tag(const char *path, struct tag *t);
void parcoursDossier(int inotifyFd, char *basePath);
void verification(int inotifyFd, struct inotify_event *event);
void listenFichier(int inotifyFd);
void ajouteDossier(int inotifyFd, char path[TAILLE_PATH]);
void ajouteFicher(char path[TAILLE_PATH]);
void createHierarchieFile(struct tag *t[], int t_length);
void createHierarchie(g_autoptr(GKeyFile) key_file, struct tag *t);
void readHierarchieFile(struct tag *tags[], int *tags_length);
void createTagsHierarchie(g_autoptr(GKeyFile) key_file, g_autoptr(GError) error,
                          struct tag *t);

#endif
