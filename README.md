# Projet : Systeme d'exploitation 

Ce projet a été réalisé par Berthel Antoine, Herbet Damien et Payet Tom.

## Compilation

Pour remplacer la commande cp, il faudra executer :

```bash
sudo ./init && . /etc/profile
```

Pour compiler le projet il vous suffira d'executer la commande :

```bash
make
```

## Execution des commandes

Le projet permet de faire la suite d'instruction suivante :

Pour afficher l'aide :

```bash
./main -h
```

S'ajouter aux membres :

```bash
./main -u
```

Se retirer des membres :

```bash
./main --remove_user
```

Ajouter un tag à un fichier :

```bash
./main -a [file name] -a ... -t [tag] -t ...
```

Supprimer des tags d'un fichier:

```bash
./main -d [file name] -d ... -t [tag] -t ...
```

Afficher les fichiers qui ont le tag:

```bash
./main -c [tag (conjonction)] -c ... -n [tag (disjonction)] -n ...
```

Creer une hierarchie entre les tags:

```bash
./main -p [tag parent] -e [liste de tags enfants] -e ...
```
