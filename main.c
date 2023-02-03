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







  
// ---------------------- temp1  ABR   -------------------------

/*int nb_station=nb_stations("tempmode1_filtre.txt");
Station * station = lecture_fichier("tempmode1_filtre.txt");

Arbre * arbre = remplir_arbre(station,nb_station);
//if qui vérifie si arbre->station.temp_min est un nan ou pas
remove("tmp.txt");

parcoursPrefixeTemp1(arbre,"tempmode1_filtre.txt","tmp.txt");
//parcoursPrefixeTemp1Decroissant(arbre,"tempmode1_filtre.txt","tmp.txt"); */

//---------------------------pression 1 ABR ---------------------------
// si jamais ya un probleme avec le mode 1 c'est surement a cause de la fonction create_station_press1 car j'ignore les ligne ou les données de pression sont pas fourni je les mets en NAN

/*int nb_station=nb_stations("pressmode1_no_filtrage.txt");
Station * station = lecture_fichier_press1("pressmode1_no_filtrage.txt");
Arbre * arbre = remplir_arbre_press1(station,nb_station);
remove("tmp.txt");
parcoursPrefixePress1(arbre,"pressmode1_no_filtrage.txt","tmp.txt");
//parcoursPrefixePress1Decroissant(arbre,"pressmode1_no_filtrage.txt","tmp.txt"); */



// ---------------------------Humidite ABR ---------------------------
/*int nb_station=nb_stations("humidite_no_filtrage.txt");
Station * station = lecture_fichier_Humidite("humidite_no_filtrage.txt");
Arbre * arbre = remplir_arbre_Humidite(station,nb_station);
remove("tmp.txt");
parcoursPrefixeHumidite(arbre,"humidite_no_filtrage.txt","tmp.txt");
TriDecroissantHumidite("tmp.txt","tmp.txt");*/



// ---------------------------Altitude ABR ---------------------------*

/*int nb_station=nb_stations("altitude_pas_trie.txt");
Station * station = lecture_fichier_altitude("altitude_pas_trie.txt");
Arbre * arbre = remplir_arbre_altitude(station,nb_station);

remove("tmp.txt");
parcoursPrefixeAltitude(arbre,"altitude_pas_trie.txt","tmp.txt");

TriDecroissantAltitude("tmp.txt","tmp.txt"); */


// ----------------------------Vent ABR ------------------------------
/*int nb_station=nb_stations("vent_no_triage.txt");
Station * station = lecture_fichier_Vent("vent_no_triage.txt");
Arbre * arbre = remplir_arbre_Vent(station,nb_station);

remove("tmp.txt");
parcoursPrefixeVent(arbre,"vent_no_triage.txt","tmp.txt");
//parcoursPrefixeVentDecroissant(arbre,"vent_no_triage.txt","tmp.txt"); */


// ---------------------------Temp 2 ABR -----------------------------
/* int nb_station=nb_stations("test.txt");
Station * station = lecture_fichier_Temp2("test.txt");
Arbre * arbre = remplir_arbre_Temp2(station,nb_station);
remove("tmp.txt");
parcoursPrefixeTemp2(arbre,"test.txt","tmp.txt");
//parcoursPrefixeTemp2Decroissant(arbre,"test.txt","tmp.txt"); */

// --------------------------Press 2 ABR ------------------------------

/*int nb_station=nb_stations("test.txt");
Station * station = lecture_fichier_Press2("test.txt");
Arbre * arbre = remplir_arbre_Press2(station,nb_station);
remove("tmp.txt");
parcoursPrefixePress2(arbre,"test.txt","tmp.txt");
//parcoursPrefixePress2Decroissant(arbre,"test.txt","tmp.txt"); */

// --------------------------Temp 3 ABR ------------------------------


/* int nb_station=nb_stations("test.txt");
Station * station = lecture_fichier_Temp3("test.txt");
Arbre * arbre = remplir_arbre_Temp3(station,nb_station);
remove("tmp.txt");
parcoursPrefixeTemp3(arbre,"test.txt","tmp.txt");
//parcoursPrefixeTemp3Decroissant(arbre,"test.txt","tmp.txt"); */

// --------------------------Press 3 ABR ------------------------------

/*int nb_station=nb_stations("test.txt");
Station * station = lecture_fichier_Press3("test.txt");
Arbre * arbre = remplir_arbre_Press3(station,nb_station);
remove("tmp.txt");
parcoursPrefixePress3(arbre,"test.txt","tmp.txt");
//parcoursPrefixePress3Decroissant(arbre,"test.txt","tmp.txt");  */





}