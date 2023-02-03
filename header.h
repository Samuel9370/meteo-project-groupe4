#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 1000
#define TAILLE 512
#define MAX_LINE_LENGTH 100
#define MAX_LOCATIONS 1000

// structure pour un arbre binaire de recherche

typedef struct arbreABR{
    int * tab;
    struct arbreABR * gauche;
    struct arbreABR * droit;

}ArbreABR;

typedef struct AVL{
	struct AVL* fd;
	struct AVL* fg;
	int equilibre;
	char cligne[TAILLE];
	char** ligne;
	int taille_ligne;
	int nbr_moyenne[2];
} avl;


typedef struct station{
    int station_id;
    float temperature;
    float temp_min;
    float temp_max;
    float pression;
    float pression_min;
    float pression_max;
    float humidite_max;
    int altitude;
    float lattitude;
    float longitude;
    float direction;
    float vitesse;
    int noeud_identique_direction; // pour le vent pour les colonnes de direction et vitesse, parfois il y a des données sur la direction et pas la vitesse et inversement donc les moyennes ne seront pas les memes quand on va divisé. dans le meteo.csv c'est a partir de a ligne 1799 ou 1800 que j'ai remarqué ça
    int noeud_identique_vitesse;    
    int noeud_identique;
    char  * date;
    
}Station;

#define MAX_SIZE 62
typedef union{
    int a;
    int b;
    float c;
    float d;
}Data;


typedef struct arbre{
    Station station;
    struct arbre * gauche;
    struct arbre * droit;

}Arbre;


//prototype

ArbreABR * creationNoeud(int * tab);

int Compte_Nbr_colonnes(char * fichier_sortie);
int countChar(char* str, char c);
ArbreABR * insertABR(int * tab, ArbreABR * arbre,int colonne_a_filtrer);
ArbreABR * creationABR(char * fichier_sortie,int colonne_a_filtrer);
ArbreABR * remplir_ABR(char * fichier_entree,int colonne_a_filtrer,int nb_filtrage);
ArbreABR * insertion(ArbreABR * arbre,char * fichier_entree, int * tab,int colonne_a_filtrer,int nb_filtrage);
void AfficheNoeud(ArbreABR * arbre,char * fichier_entree);
void parcoursPrefixe(ArbreABR * arbre,char * fichier_entree,char * fichier_sortie,int nb_filtrage);
void EcritNoeudFichier(ArbreABR * arbre,char * fichier_entree,char * fichier_sortie);
void ParcoursABR_TEMPMODE1_Moyenne(ArbreABR * arbre,char * fichier_entree,int nb_filtrage);
char * strtok2(char * str, const char * delim);
void SupprimeDernierCaractere(char * fichier_entree);



// temp 1
Arbre * creationNoeudTemp(Station station);
Station create_station(int id_station,float temperature);
Arbre * remplir_ABRTemp(char * fichier_entree,int colonne_a_filtrer, int nb_filtrage);
int nb_stations(char * fichier_entree);
Station * lecture_fichier(char * fichier_entree);
void affiche_station(Station * station);
Arbre * insertion_station(Arbre * arbre,Station station);
Arbre * remplir_arbre(Station * tab_station,int nb_station);
void parcoursPrefixeTemp1(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void EcritNoeudFichierTemp1(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void supprimer_delimiteur(char * fichier_entree,int nb_colonnes);
void SupprimeDernierCaractere(char * fichier_entree);
void gestion_meme_noeud(Arbre * arbre,Station station);
void parcoursPrefixeTemp1Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);

// press 1
Station create_station_press1(int id_station,float pression);
void EcritNoeudFichierPress1(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixePress1(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_press1(Arbre * arbre,Station station);
Arbre * remplir_arbre_press1(Station * tab_station,int nb_station);
Station * lecture_fichier_press1(char * fichier_entree);
void parcoursPrefixePress1Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);


// humidite 
Station create_station_Humidite(int id_station,float humidite,float lattitude,float longitude);
void EcritNoeudFichierHumidite(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixeHumidite(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Humidite(Arbre * arbre,Station station);
Arbre * remplir_arbre_Humidite(Station * tab_station,int nb_station);
Station * lecture_fichier_Humidite(char * fichier_entree);
void TriDecroissantHumidite(char * fichier_entree,char * fichier_sortie);
int compare_rows(const void * a, const void * b);

// altitude
Station create_station_altitude(int id_station,int altitude,float lattitude,float longitude);
void EcritNoeudFichierAltitude(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixeAltitude(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Altitude(Arbre * arbre,Station station);
Arbre * remplir_arbre_altitude(Station * tab_station,int nb_station);
Station * lecture_fichier_altitude(char * fichier_entree);
void TriDecroissantAltitude(char * fichier_entree,char * fichier_sortie);
int compare_rows_altitude(const void * a, const void * b);

//vent
Station create_station_Vent(int id_station,float direction,float vitesse,float lattitude, float longitude);
void EcritNoeudFichierVent(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixeVent(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Vent(Arbre * arbre,Station station);
Arbre * remplir_arbre_Vent(Station * tab_station,int nb_station);
Station * lecture_fichier_Vent(char * fichier_entree);
void parcoursPrefixeVentDecroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);

//temp 2 
Station create_station_Temp2(float temperature,char * date);
Station * lecture_fichier_Temp2(char * fichier_entree);
Arbre * insertion_station_Temp2(Arbre * arbre,Station station);
Arbre * remplir_arbre_Temp2(Station * tab_station,int nb_station);
void parcoursPrefixeTemp2(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void EcritNoeudFichierTemp2(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
int compare_dates(const char *date1, const char *date2);
void parcoursPrefixeTemp2Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);

//press 2 

Station create_station_Press2(char * date,float pression);
void EcritNoeudFichierPress2(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixePress2(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Press2(Arbre * arbre,Station station);
Arbre * remplir_arbre_Press2(Station * tab_station,int nb_station);
Station * lecture_fichier_Press2(char * fichier_entree);
void parcoursPrefixePress2Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);

//temp 3 
Station create_station_Temp3(int station_id,char * date,float temperature);
void EcritNoeudFichierTemp3(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixeTemp3(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Temp3(Arbre * arbre,Station station);
Arbre * remplir_arbre_Temp3(Station * tab_station,int nb_station);
Station * lecture_fichier_Temp3(char * fichier_entree) ;
void parcoursPrefixeTemp3Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);

//press 3

Station create_station_Press3(int station_id,char * date,float temperature);
void EcritNoeudFichierPress3(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
void parcoursPrefixePress3(Arbre * arbre,char * fichier_entree,char * fichier_sortie);
Arbre * insertion_station_Press3(Arbre * arbre,Station station);
Arbre * remplir_arbre_Press3(Station * tab_station,int nb_station);
Station * lecture_fichier_Press3(char * fichier_entree) ;
void parcoursPrefixePress3Decroissant(Arbre * arbre,char * fichier_entree,char * fichier_sortie);


//main

void Analyse_mod(char * mode,char * mode_sort);
void Analyse_des_arguments(char * mode, int decroissant, char * fichier_entree, char * fichier_sortie, int nbr_argument,char * mode_sort);
void Tri_HumiditeABR(int mode_decroissant, char * fichier_entree, char * fichier_sortie);
void Tri_AltitudeABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Temp1ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_VentABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Press3ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Press2ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Press1ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Temp3ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
void Tri_Temp2ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie);

typedef struct {
    int id;
    float temp;
} Temp;

void Tri_Temp1TAB(int mode_decroissant, char *fichier_entree, char *fichier_sortie);
int compare(const void * a, const void * b);
void addLine(char * line);
int TrouveLigne(char * line);
void Tri_HumiditeTAB(int decroissant, char * fichier_entree, char * fichier_sortie);
void LectureLignes(char * fichier_entree);
void EcrireDansLigne(char * fichier_sortie);
/*
//avl
void verifpchar(char* c);
void verifpint(int* n);
void verifn(int n);

void verifavl(avl a);
void verifpavl(avl* a);



void verifpfile(FILE * file);

void verifmode(char* c);



int min3(int a, int b, int c);
int max2(int a, int b);
int max3(int a, int b, int c);
int min2(int a, int b);




int compare_dates(const char *date1, const char *date2);

int infstrictavl(avl* a, avl* b, int idx, char* type);



char** strtotab(char* l, int* nl, char* type);
char** creationligne(char* l, int* nl, char* type);

avl* creationavl(char* l, char* type);
avl* rotadroite(avl* a);
avl* rotagauche(avl* a);
avl* doublerotagauche(avl* a);
avl* doublerotadroite(avl* a);
avl* equilibreravl(avl* a);
avl* insertavl(avl* a, avl* nv, int* h, int idx, char* type);
avl* insertavl_appel(avl* a, char* ligne, int idx, char* type);


avl* construireavl(char* nomf, int idx, char* type);




void parcourscroissantavl(avl* a, FILE* fichier);
void parcourscroissantavl_appel(avl* a, char* nomf);
void parcoursdecroissantavl(avl* a, FILE* fichier);
void parcoursdecroissantavl_appel(avl* a, char* nomf);
void traiteravl(avl* a, FILE* out_fichier);

avl* finiravl(avl* a, char* type);

char* actualiseravl_cligne(avl* a);


int tri_p1_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_p1(avl* a, avl* b, int idx);
avl* egalavl_p1(avl* a, avl* b, int i);
avl* finiravl_p1(avl* a, int i);

int tri_p2_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_p2(avl* a, avl* b, int idx);
avl* egalavl_p2(avl* a, avl* b, int i);
avl* finiravl_p2(avl* a, int i);

int tri_p3_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_p3(avl* a, avl* b, int idx);
avl* egalavl_p3(avl* a, avl* b, int i);
avl* finiravl_p3(avl* a, int i);

int tri_t1_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_t1(avl* a, avl* b, int idx);
avl* egalavl_t1(avl* a, avl* b, int i);
avl* finiravl_t1(avl* a, int i);

int tri_t2_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_t2(avl* a, avl* b, int idx);
avl* egalavl_t2(avl* a, avl* b, int i);
avl* finiravl_t2(avl* a, int i);

int tri_t3_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_t3(avl* a, avl* b, int idx);
avl* egalavl_t3(avl* a, avl* b, int i);
avl* finiravl_t3(avl* a, int i);

int tri_w_avl(char* input_fich, char* output_fich, int decroissant);
int infstrictavl_w(avl* a, avl* b, int idx);
avl* egalavl_w(avl* a, avl* b, int i);
avl* finiravl_w(avl* a, int i);

int tri_h_avl(char* input_fich, char* output_fich);
int infstrictavl_h(avl* a, avl* b, int idx);
avl* egalavl_h(avl* a, avl* b, int i);
avl* finiravl_h(avl* a, int i);


int tri_m1_avl(char* input_fich, char* output_fich);
int tri_m2_avl(char* input_fich, char* output_fich);
int infstrictavl_m1(avl* a, avl* b, int idx);
int infstrictavl_m2(avl* a, avl* b, int idx);
avl* egalavl_m(avl* a, avl* b, int i);
avl* finiravl_m(avl* a, int i); */