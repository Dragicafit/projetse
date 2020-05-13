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

static gchar **file_add;
static gchar **file_del;
static gchar **file_has;
static gchar **l_tag;

static GOptionEntry entries[] = {
    {"add_tag", 'a', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_add,
     "Ajouter des tags a un fichier", "A"},
    {"tag", 't', G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_STRING_ARRAY, &l_tag, NULL,
     NULL},
    {"del_tag", 'd', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_del,
     "Supprimer des tags d'un fichier", "D"},
    {"list_files_with_tag", 's', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_has,
     "Afficher les fichiers qui ont le tag", "L"},
    {"add_user", 'u', 0, 0, NULL, "Vous ajoute aux membres", NULL},
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

  printf("valide\n");
  return 0;

  int inotifyFd = inotify_init();
  if (inotifyFd == -1) handle_error("inotify_init");

  parcoursDossier(inotifyFd, "/tmp");
  listenFichier(inotifyFd);
  return 0;
}