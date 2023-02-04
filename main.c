#include "header.h"
 // le fichier c peut avoir plusieurs paremetres tq -f nomdufichier.txt 
 // -f fichier a filtré .txt
 // -o nom du fichier txt qui v a etre la sortie
 // --tab// --abr // --avl mode de tri si aucun mode alors avl
 // -r option optionnel si ya loption alors decroissant sinon croissant
int main(int argc, char * argv[]){
// argc represente le nombre darguments du programme + nom du programme lui meme donc faut faire -1
// argv[1] represente le premier argument.

// ex ./exec -f entre.txt -o sortie.txt --abr -r -t1 le mode en dernier
int nbr_argument=argc;
nbr_argument--;


char * input_file = argv[2]; // 2 on stock le fichier d'entrée a trié
char * output_file = argv[4]; // 4 on stock le fichier de sortie la ou on va mettre les données filtrés.

char * mode=argv[nbr_argument];


char * mode_sort = NULL; // pour l'instant on définie a NULL car ce n'est pas une option obligatoire.
int decroissant=0;
for(int i=1;i<=nbr_argument;i++){
    // on regarde dans les arguments si ya le mode décroissant ou pas
    if(strcmp(argv[i],"-r")==0){
        decroissant=1;
    }
    // on regarde dans les arguments si ya le mode de tri
    if(strcmp(argv[i],"--tab")==0){
        mode_sort="tab";
    }
    if(strcmp(argv[i],"--abr")==0){
        mode_sort="abr";
    }
    if(strcmp(argv[i],"--avl")==0){
        mode_sort="avl";
    }
}

Analyse_des_arguments(mode,decroissant,input_file,output_file,nbr_argument,mode_sort);

}
