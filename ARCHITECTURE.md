# Architecture

Suite à notre première phase de réléfction nous avions prévu d'implémenter à la fois un systeme avec inotify permettant d'attendre toute modification sur les fichiers, et un systeme de commande avec Glib pour pouvoir créer nos différentes fonctions.

Pour tester nos différentes implémentations, nous avons utilisé un docker pour les isoler. (voir `docker-compose.yml`)

## La première idée 

ajouter tag :
```
    xattr ajouter user.tag
    stat st_mode st_uid
    print scanf
    chmod
```

retirer tag :
```
    xattr retirer user.tag
```

cp :
```
    if user in tag 
        cp --preserve=xattr
    else
        cp
```

lister les fichers qui ont un tag :
```
    tab = []
    parcours la liste de fichier
        tab += xattr liste tag
    for (tag.enfants){
        tab += lister les fichers qui ont un tag.enfants[i]
    }
    return tab
```

lister les tag d'un fichier :
```
    xattr liste tag
```

hiérarchie des tags :
```
    struct {name = ""; enfants = [taille_MAX]}
```

ajouter enfant :
```
    tag.enfant[i] = enfant
```

IN_ATTRIB :
```
    ajouter le fichier_ecoute
```

parcours tous les fichiers :
```
    if dossier -> ajouter dossier_ecoute
    xattr liste tag -> ajouter fichier_ecoute
```

clee privee + chemin absolu + tag

## Implémentation actuelle

Nous avons abandonné l'utilisation de inotify ainsi, à chque modification de tags, nous parcouront l'arborescence entière.

On a finalement choisi d'utiliser xattr pour stocker les tag dans les fichiers de la forme : `user.uid.tag=""`.

Un utilisateur doit posséder le fichier `~/.hierarchie` pour accéder aux commandes du projet.

On a donc une fonction qui permet de lire l'arborescence des fichiers, et de lister tous les fichiers qui possèdent un tag.

Il existe des fonctions qui permettent de créer la hiérarchie entre les tags et de les stocker dans le fichier `~/.hiérarchie` pour ne pas les perdre.

Enfin, il y a les fonctions de base qui sont appelé via le Commandline option parser de Glib et qui vont faire le lien entre toutes les informations stockées.

On peut donc :
- S'ajouter aux membres du projet
- Se retirer des membres du projet
- Ajouter des tags à fichier
- Supprimer des tags d'un fichier
- Lister les fichiers qui ont des tags de notre programme
- Créer une hiérarchie (parent enfant) entre les tags
- Copier en conservant les tags (via `alias`)

## Idée d'amélioration

Initialement, on voulait rendre notre liste de tag "unique" en cryptant chaque tag de la manière suivante: clé privée + chemin absolu du fichier + tag pour éviter qu'un autre utilisateur ne modifie notre tag.
