# Architecture

Suite à notre première phase de réléfction nous avions prévu d'implémenter à la fois un systeme avec inotify permettant d'attendre toute modification sur les fichiers, et un systeme de commande avec Glib pour pouvoir créer nos différentes fonctions.

## La première idée 

ajouter tag :
    xattr ajouter user.tag
    stat st_mode st_uid
    print scanf
    chmod

retirer tag :
    xattr retirer user.tag

cp :
    if user in tag 
        cp --preserve=xattr
    else
        cp

lister les fichers qui ont un tag :
    tab = []
    parcours la liste de fichier
        tab += xattr liste tag
    for (tag.enfants){
        tab += lister les fichers qui ont un tag.enfants[i]
    }
    return tab

lister les tag d'un fichier :
    xattr liste tag

hiérarchie des tags :
    struct {tag = ""; enfants = [taille_MAX]}

ajouter enfant :
    tag.enfant[i] = enfant

IN_ATTRIB :
    ajouter le fichier_ecoute

parcours tous les fichiers :
    if dossier -> ajouter dossier_ecoute
    xattr liste tag -> ajouter fichier_ecoute

clee privee + chemin absolu

## Implémentation actuelle 

On a finalement choisi de stocker les tags dans un fichier ".hierarchie' qui nous permet à chaque execution d'une commande d'avoir la liste des tags et leurs enfants précédemment utilisé.

De même pour les utilisateurs, il faut posséder le fichier ".hierarchie' pour accéder aux commandes du projet.

On a donc une fonction qui permet de lire l'arborescence des fichiers, et de lister tous les fichiers qui possèdent un tag.

Ainsi que des fonctions qui permettent de créer la hiérarchie entre les tags et de les stocker dans le fichier .hiérarchie pour ne pas les perdre.

Enfin, il y'a les fonctions de base qui sont appelé via Glib et qui vont faire le lien entre toutes les informations stockées.

On peut donc :
- S'ajouter aux membres du projet
- Ajouter un tag à fichier
- Supprimer un tag d'un fichier
- Lister les fichiers qui ont des tags de notre programme
- Créer une hiérarchie (parent enfant) entre les tags

## Ce qu'on aurait pu ajouter

Initialement, on voulait rendre notre liste de tag "unique" en cryptant chaque tag de la manière suivante: clé privée + chemin absolu du fichier + tag.
