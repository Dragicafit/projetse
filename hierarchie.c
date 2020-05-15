#include <dirent.h>
#include <glib.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "constantes.h"
#include "modele.h"

char getPathHierarchie(char path[TAILLE_PATH]) {
  const char *homedir = getenv("HOME");
  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }
  int s = snprintf(path, TAILLE_PATH, "%s/.hierarchie", homedir);
  if (s < 0) handle_error("snprintf error");
  if (s >= TAILLE_PATH) {
    perror("snprintf error");
    return -1;
  }
  return 0;
}

void createHierarchieFile() {
  char path[TAILLE_PATH];
  if (getPathHierarchie(path) < 0) return;

  g_autoptr(GKeyFile) key_file = g_key_file_new();
  g_autoptr(GError) error = NULL;

  for (int i = 0; i < tags_length; i++) {
    createHierarchie(key_file, list_tags[i]);
  }

  if (!g_key_file_save_to_file(key_file, path, &error)) {
    g_warning("Error saving key file: %s", error->message);
    return;
  }
}

void createHierarchie(GKeyFile *key_file, tag *t) {
  int i;
  gchar *list[ENFANT_MAX];
  for (i = 0; i < t->nbEnfant; i++) {
    list[i] = t->enfants[i]->name;
  }
  if (i == 0) return;
  g_key_file_set_string_list(key_file, "tags", t->name,
                             (const gchar *const *)list, t->nbEnfant);
}

void readHierarchieFile() {
  char path[TAILLE_PATH];
  if (getPathHierarchie(path) < 0) return;

  g_autoptr(GError) error = NULL;
  g_autoptr(GKeyFile) key_file = g_key_file_new();

  if (!g_key_file_load_from_file(key_file, path, G_KEY_FILE_NONE, &error)) {
    if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
      g_warning("Error loading key file: %s", error->message);
    return;
  }

  gsize len;
  g_autofree gchar **val = g_key_file_get_keys(key_file, "tags", &len, &error);
  if (val == NULL && !g_error_matches(error, G_KEY_FILE_ERROR,
                                      G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
    g_warning("Error finding key in key file: %s", error->message);
    return;
  }

  for (int i = 0; i < len; i++) {
    char name[TAILLE_TAG];
    int s = snprintf(name, TAILLE_TAG, "%s", val[i]);
    if (s < 0) handle_error("snprintf error");
    if (s >= TAILLE_PATH) {
      perror("snprintf error");
      return;
    }

    tag *newTag = rechercheTag(name);
    createTagsHierarchie(key_file, error, newTag);
  }
}

void createTagsHierarchie(GKeyFile *key_file, GError *error, tag *t) {
  gsize len;
  g_autofree gchar **val =
      g_key_file_get_string_list(key_file, "tags", t->name, &len, &error);
  for (int i = 0; i < len; i++) {
    if (t->nbEnfant >= ENFANT_MAX) break;
    t->enfants[t->nbEnfant++] = rechercheTag(val[i]);
  }
}
