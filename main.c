#include <dirent.h>
#include <glib.h>
#include <glib/gprintf.h>
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

static gchar **file_add;
static gchar **file_del;
static gchar **tag_conj;
static gchar **tag_dij;
static gchar **l_tag;
static gboolean user = FALSE;
static gboolean show = FALSE;

static GOptionEntry entries[] = {
    {"add_tag", 'a', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_add,
     "Ajouter des tags a un fichier", "A"},
    {"tag", 't', 0, G_OPTION_ARG_STRING_ARRAY, &l_tag, NULL, NULL},
    {"del_tag", 'd', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_del,
     "Supprimer des tags d'un fichier", "D"},
    {"list_files_with_tag", 's', 0, G_OPTION_ARG_NONE, &show,
     "Afficher les fichiers qui ont le tag", NULL},
    {"conjonction", 'c', 0, G_OPTION_ARG_STRING_ARRAY, &tag_conj,
     "Liste de tag que le fichier doit avoir", NULL},
    {"disjonction", 'n', 0, G_OPTION_ARG_STRING_ARRAY, &tag_dij,
     "Liste de tag que le fichier ne doit pas avoir", NULL},
    {"add_user", 'u', 0, G_OPTION_ARG_NONE, &user, "Vous ajoute aux membres",
     NULL},
    {NULL}};

char dossierEcoute[TAILLE_DOSSIER_ECOUTE_MAX][TAILLE_PATH];
char fichierEcoute[TAILLE_FICHIER_ECOUTE_MAX][TAILLE_PATH];
size_t nbFichierEcoute;

int main(int argc, char *argv[]) {
  GError *error = NULL;
  GOptionContext *context;
  context = g_option_context_new("- test");
  g_option_context_add_main_entries(context, entries, NULL);

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print("option parsing failed: %s\n", error->message);
    exit(1);
  }
  int s_add = file_add != NULL ? g_strv_length(file_add) : 0;
  int s_del = file_del != NULL ? g_strv_length(file_del) : 0;
  int s_conj = tag_conj != NULL ? g_strv_length(tag_conj) : 0;
  int s_dij = tag_dij != NULL ? g_strv_length(tag_dij) : 0;
  int s_tag = l_tag != NULL ? g_strv_length(l_tag) : 0;

  if (user) {
    add_user();
    return 0;
  } else if (s_add > 0 && s_tag > 0) {
    for (int i = 0; i < s_add; i++) {
      for (int j = 0; j < s_tag; i++) {
        add_tag(file_add[i], l_tag[j]);
      }
    }
    return 0;
  } else if (s_del > 0 && s_tag > 0) {
    for (int i = 0; i < s_del; i++) {
      for (int j = 0; j < s_tag; i++) {
        del_tag(file_del[i], l_tag[j]);
      }
    }
    return 0;

  } else if (show && (s_conj > 0 || s_dij > 0)) {
    show_by_tag(tag_conj, tag_dij, s_conj, s_dij);
    return 0;
  }
  int inotifyFd = inotify_init();
  if (inotifyFd == -1) handle_error("inotify_init");
  parcoursDossier(inotifyFd, "/tmp");
  return 0;
}