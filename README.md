# meteo-project-groupe4

PRE-REQUIS

INSTALLEZ GNUPLOT AVEC "sudo apt install gnuplot"





"Pour faire fonctionner le script, vous devez obligatoirement avoir un -f suivis du nom du fichier contenant les données triés ( ex: -f data.csv ) et un -o suivis du nom du fichier de sortie. Vous devez aussi obligatoirement entrer au moins une option selon votre/vos choix parmis -h ( pour avoir une carte interpolé et coloré de l'altitude), -m ( pour avoir la même chose mais de l'humidité), -w (pour avoir un diagramme de type vecteur) et -p ou -t suivis de 1,2 ou 3 (ex:-p3)( t pour température et p pour pression, 1 pour avoir un diagramme de type barres d'erreur, 2 pour un diagramme de type ligne simple et 3 pour un diagramme de type multi-ligne). En option vous pouvez préciser une zone en ajoutant - suivis de A pour Antilles, G pour Guyane, F pour France, S pour St-Pierre et Miquellon, O pour Océan Indien et Q pour l'océan Antartique. Si vous voulez trier par date ajouter -d suivis de la date minimum et maximum au format AAAA-MM-DD( par exemple -d 2010-01-01 2010-05-06). L'ordre de tout ces options n'est pas important mais ne doivent pas être collée (ex: -A-p1 n'est pas bon mais -A -p1 l'est). Si rien ou --avl est ajouté, les données seront triés en AVL, si vous voulez trier les données en ABR ou en tableau, ajoutez --abr ou --tab. Pour trier les donner de façon décroissante, ajoutez -r.

Pour faire fonctionner le script, vous devez obligatoirement 
avoir un -f suivis du nom du fichier contenant les données triés ( ex: -f data.csv ) et un -o suivis du nom du fichier de sortie. 
Vous devez aussi obligatoirement entrer au moins une option selon votre/vos choix parmis :
-h ( pour avoir une carte interpolé et coloré de l'altitude), -m ( pour avoir la même chose mais de l'humidité),
-w (pour avoir un diagramme de type vecteur) et -p ou -t suivis de 1,2 ou 3 (ex:-p3)( t pour température et p pour pression, 1 pour avoir un diagramme de type barres d'erreur, 2 pour un diagramme de type ligne simple et 3 pour un diagramme de type multi-ligne). En option vous pouvez préciser une zone en ajoutant - suivis de A pour Antilles, G pour Guyane, F pour France, S pour St-Pierre et Miquellon, O pour Océan Indien et Q pour l'océan Antartique. Si vous voulez trier par date ajouter -d suivis de la date minimum et maximum au format AAAA-MM-DD( par exemple -d 2010-01-01 2010-05-06). L'ordre de tout ces options n'est pas important mais ne doivent pas être collée (ex: -A-p1 n'est pas bon mais -A -p1 l'est). 
Si rien ou --avl est ajouté, les données seront triés en AVL, si vous voulez trier les données en ABR ou en tableau, ajoutez --abr ou --tab. Pour trier les donner de façon décroissante, ajoutez -r."
erreur de buffer d'avl
