#include <dirent.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "constantes.h"
#include "modele.h"

static gchar **file_add;
static gchar **file_del;
static gchar **tag_conj;
static gchar **tag_dij;
static gchar **l_tag;
static gchar *tag_parent = "";
static gchar **tag_enfant;
static gboolean add_u = FALSE;
static gboolean remove_u = FALSE;
static gboolean cp = FALSE;

static GOptionEntry entries[] = {
    {"add_tag", 'a', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_add,
     "Ajouter des tags a un fichier", NULL},
    {"tag", 't', 0, G_OPTION_ARG_STRING_ARRAY, &l_tag, NULL, NULL},
    {"del_tag", 'd', 0, G_OPTION_ARG_FILENAME_ARRAY, &file_del,
     "Supprimer des tags d'un fichier", NULL},
    {"conjonction", 'c', 0, G_OPTION_ARG_STRING_ARRAY, &tag_conj,
     "Liste de tags que le fichier doit avoir", NULL},
    {"disjonction", 'n', 0, G_OPTION_ARG_STRING_ARRAY, &tag_dij,
     "Liste de tags que le fichier ne doit pas avoir", NULL},
    {"add_user", 'u', 0, G_OPTION_ARG_NONE, &add_u, "Vous ajoute aux membres",
     NULL},
    {"remove_user", 0, 0, G_OPTION_ARG_NONE, &remove_u,
     "Vous retire des membres", NULL},
    {"creer_hierarchie", 'p', 0, G_OPTION_ARG_STRING, &tag_parent,
     "Creer une hierarchie de tag parent (-p) / [liste d'enfants] (-e)", NULL},
    {"creer_hierarchie", 'e', 0, G_OPTION_ARG_STRING_ARRAY, &tag_enfant,
     "Creer une hierarchie de tag parent (-p) / [liste d'enfants] (-e)", NULL},
    {"cp", 0, 0, G_OPTION_ARG_NONE, &cp, "alias cp", NULL},
    {NULL}};

char fichierEcoute[TAILLE_FICHIER_ECOUTE][TAILLE_PATH];
size_t nbFichierEcoute;
tag *list_tags[TAILLE_LIST_TAG];
int tags_length;

int main(int argc, char *argv[]) {
  nbFichierEcoute = 0;
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
  int s_enfant = tag_enfant != NULL ? g_strv_length(tag_enfant) : 0;
  char t_p = strcmp(tag_parent, "") != 0;

  if (cp) cp_tag(argc - 2, &argv[2]);

  if (add_u) {
    add_user();
    return 0;
  }
  if (remove_u) {
    remove_user();
    return 0;
  }
  if (s_add > 0 && s_tag > 0) {
    for (int i = 0; i < s_add; i++) {
      for (int j = 0; j < s_tag; j++) {
        add_tag(file_add[i], initTag(l_tag[j]));
      }
    }
    return 0;
  }
  if (s_del > 0 && s_tag > 0) {
    for (int i = 0; i < s_del; i++) {
      for (int j = 0; j < s_tag; j++) {
        del_tag(file_del[i], initTag(l_tag[j]));
      }
    }
    return 0;
  }
  if (s_conj > 0 && s_dij > 0 && s_conj < TAILLE_CONJ && s_dij < TAILLE_CONJ) {
    char nconj[TAILLE_CONJ][TAILLE_TAG];
    for (int i = 0; i < s_conj; i++) {
      if (strlen(tag_conj[i]) > TAILLE_TAG) continue;
      strcpy(nconj[i], tag_conj[i]);
    }
    char ndij[TAILLE_CONJ][TAILLE_TAG];
    for (int i = 0; i < s_dij; i++) {
      if (strlen(tag_dij[i]) > TAILLE_TAG) continue;
      strcpy(ndij[i], tag_dij[i]);
    }
    parcoursDossier("/");
    readHierarchieFile();
    show_by_tag(nconj, ndij, s_conj, s_dij);
    return 0;
  }

  if (t_p && s_enfant > 0) {
    readHierarchieFile();
    tag *parent = rechercheTag(tag_parent);
    printf("Tag : %d\n", list_tags[0]->nbEnfant);
    for (int i = 0; i < s_enfant; i++) {
      if (parent->nbEnfant >= ENFANT_MAX) break;
      tag *enfant = rechercheTag(tag_enfant[i]);
      char out = 0;
      for (int j = 0; j < parent->nbEnfant; j++) {
        if (parent->enfants[j] == enfant) {
          out = 1;
          break;
        }
      }
      if (out) continue;
      parent->enfants[parent->nbEnfant++] = enfant;
    }
    createHierarchieFile();
    return 0;
  }
  return 0;
}