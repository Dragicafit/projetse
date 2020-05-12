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