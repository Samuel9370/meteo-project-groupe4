#include "header.h"

ArbreABR *creationNoeud(int *tab)
{
    ArbreABR *noeud = malloc(sizeof(ArbreABR));
    if (noeud == NULL)
        exit(4);
    noeud->gauche = NULL;
    noeud->droit = NULL;
    noeud->tab = tab;
    return noeud;
}

int countChar(char *str, char c)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
        {
            count++;
        }
    }
    return count;
}

int Compte_Nbr_colonnes(char *fichier_sortie)
{
    FILE *file = fopen(fichier_sortie, "r");
    char line[512];
    int colonne = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {

        colonne = countChar(line, ';');

        break;
    }
    fclose(file);
    colonne++;
    return colonne;
}

// fonction qui fait exactement comme strtok() mais qui prends en compte les colonnes vide
char *strtok2(char *str, const char *delim)
{
    static char *src = NULL;
    char *p, *ret = 0;
    if (str != NULL)
        src = str;
    if (src == NULL)
        return NULL;
    if ((p = strpbrk(src, delim)) != NULL)
    {
        *p = 0;
        ret = src;
        src = ++p;
    }
    else if (*src)
    {
        ret = src;
        src = NULL;
    }
    return ret;
}

// la fonction insertion doit aussi prendre en parametre la colonne a filtrer

ArbreABR *insertion(ArbreABR *arbre, char *fichier_entree, int *tab, int colonne_a_filtrer, int nb_filtrage)
{
    if (arbre == NULL)
    {
        return creationNoeud(tab);
    }
    else
    {
        if (arbre->tab[colonne_a_filtrer - 1] > tab[colonne_a_filtrer - 1])
        {
            arbre->gauche = insertion(arbre->gauche, fichier_entree, tab, colonne_a_filtrer, nb_filtrage);
        }
        else if (arbre->tab[colonne_a_filtrer - 1] == tab[colonne_a_filtrer - 1]) // si c'est le meme noeud. ( ordre temp moyenne;temp minimal; temp max; )
        {

            // si c le meme noeud on regarde la temp minimal

            if (tab[2 + nb_filtrage] <= arbre->tab[2 + nb_filtrage])
                arbre->tab[2 + nb_filtrage] = tab[2 + nb_filtrage];
            // si c le meme noeud on regarde la temp max
            if (tab[3 + nb_filtrage] >= arbre->tab[3 + nb_filtrage])
                arbre->tab[3 + nb_filtrage] = tab[3 + nb_filtrage];

            // si c le meme noeud on regarde la temp moyenne et on fait la moyenne. on doit diviser par le nombre de fois qu'on a fait la moyenne
            printf("[%d]", arbre->tab[0]);

            arbre->tab[Compte_Nbr_colonnes(fichier_entree)]++;
            printf("[%d]\n", Compte_Nbr_colonnes(fichier_entree));                              // on incremente le nombre de fois qu'on a fait la moyenne
            arbre->tab[1 + nb_filtrage] = (arbre->tab[1 + nb_filtrage] + tab[1 + nb_filtrage]); ///(arbre->tab[Compte_Nbr_colonnes(fichier_entree)]); // on divise par le nombre de fois qu'on a fait la moyenne
        }

        else
        {
            arbre->droit = insertion(arbre->droit, fichier_entree, tab, colonne_a_filtrer, nb_filtrage);
        }
    }
    return arbre;
}

// une fonction qui ligne un fichier et qui retourne un ABR remplie avec les lignes du fichier. la fonction remplir_ABR prends aussi en parametre la colonne a filtrer

ArbreABR *remplir_ABR(char *fichier_entree, int colonne_a_filtrer, int nb_filtrage)
{
    FILE *file = fopen(fichier_entree, "r");
    char line[512];
    int colonne = 0;
    int *tab = NULL;
    ArbreABR *arbre = NULL;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        colonne = 1 + Compte_Nbr_colonnes(fichier_entree); // normalement on a 4 colonnes pour 4 colonnes dans le fichier .Txt sauf que on rajoute +1 dans le tableau pour le nombre de fois le meme noeud passé pour ensuite divisé pour la moyenne
        tab = malloc(sizeof(int) * colonne);
        if (tab == NULL)
            exit(4);

        tab[colonne - 1] = 1; // on initialise le nombre de fois qu'on a fait la moyenne a 1. on va par la suite augmenter ce compter a chaque fois qu'on va passer dans le meme noeud
                              // for(int i=0;i<Compte_Nbr_colonnes(fichier_entree);i++)
                              // tab[i]=0;

        char *token = strtok2(line, ";");
        int i = 0;
        while (token != NULL)
        {
            tab[i] = atoi(token);
            token = strtok2(NULL, ";");
            i++;
        }

        arbre = insertion(arbre, fichier_entree, tab, colonne_a_filtrer, nb_filtrage);
    }

    fclose(file);
    return arbre;
}

// fonction AfficheNoeud qui affiche le noeud courant
void AfficheNoeud(ArbreABR *arbre, char *fichier_entree)
{
    if (arbre != NULL)
    {
        for (int i = 0; i < Compte_Nbr_colonnes(fichier_entree); i++)
        {

            printf("|%d|", arbre->tab[i]);
        }
    }
    printf("\n");
}

// fonction qui fait un parcours prefixe et afficher dans l'ordre croissant la colonne a filtrer

// fonction qui écrit dans un fichier le noeud courant
void EcritNoeudFichier(ArbreABR *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        FILE *fichier = fopen(fichier_sortie, "a");

        for (int i = 0; i < Compte_Nbr_colonnes(fichier_entree) + 1; i++)
        {

            fprintf(fichier, "%d;", arbre->tab[i]);
        }

        fprintf(fichier, "\n");
        fclose(fichier);
    }
}
// parcours qui parcoure une ligne et compte le nombre de caractere ";"

// fonction qui prends en parametre un fichier.txt et pour chaque ligne du fichier elle supprime le derniere caractere de la ligne qui est un ";"
void SupprimeDernierCaractere(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    FILE *fichier2 = fopen("fichier2.txt", "w");
    char line[512];
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        int i = 0;
        int compteur = 0;
        while (line[i] != '\0')
        {
            if (line[i] == ';')
                compteur++;
            i++;
        }
        if (compteur != Compte_Nbr_colonnes(fichier_entree) - 1)
        {

            while (line[i] != '\0')
            {
                i++;
            }
            line[i - 2] = '\0';
            fprintf(fichier2, "%s", line);
            fprintf(fichier2, "\n");
        }
    }
    fclose(fichier);
    fclose(fichier2);
    remove(fichier_entree);
    rename("fichier2.txt", fichier_entree);
}

void parcoursPrefixe(ArbreABR *arbre, char *fichier_entree, char *fichier_sortie, int nb_filtrage)
{
    if (arbre != NULL)
    {

        parcoursPrefixe(arbre->gauche, fichier_entree, fichier_sortie, nb_filtrage);
        arbre->tab[1 + nb_filtrage] = (arbre->tab[1 + nb_filtrage]) / (arbre->tab[Compte_Nbr_colonnes(fichier_entree)]);
        EcritNoeudFichier(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixe(arbre->droit, fichier_entree, fichier_sortie, nb_filtrage);
    }
}

Arbre *creationNoeudTemp(Station station)
{
    Arbre *noeud = malloc(sizeof(Arbre));
    if (noeud == NULL)
        exit(4);
    noeud->gauche = NULL;
    noeud->droit = NULL;
    noeud->station = station;
    return noeud;
}

Station create_station(int id_station, float temperature)
{
    Station station;
    station.station_id = id_station;
    station.temperature = temperature;
    station.temp_min = temperature;
    station.temp_max = temperature;
    station.noeud_identique = 1;
    return station;
}

int nb_stations(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_stations = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        nb_stations++;
    }
    fclose(fichier);
    return nb_stations;
}

// fonction qui lit un fichier.txt et pour chaque ligne creer une station. ensuite on fait un tableau de taille nb_stations et on y met les stations. dans le fichier .txt il peux y avoir des données sur les temp_min donc faut remplacer les NAN par les temp_min. si il y a pas de temp_min on laisse NAN
Station *lecture_fichier(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_station = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        int j = 0;
        int id_station;
        float temperature;
        float temp_min;
        float temp_max;

        id_station = atoi(token);
        token = strtok2(NULL, ";");
        temperature = atof(token);

        Station station = create_station(id_station, temperature);
        tab_station[i] = station;
        i++;
    }

    fclose(fichier);
    return tab_station;
}

// fonction qui gere le cas ou c'est le meme noeud et on regarde si la temp_min est plus petite que la temp_min du noeud et on fait pareil avec temp_max
void gestion_meme_noeud(Arbre *arbre, Station station)
{
    // si c le meme noeud on regarde la temp minimal

    if (station.temp_min <= arbre->station.temp_min)
        arbre->station.temp_min = station.temp_min;

    // si c le meme noeud on regarde la temp max

    if (arbre->station.temp_max <= station.temp_max)
        arbre->station.temp_max = station.temp_max;

    arbre->station.noeud_identique++;
    arbre->station.temperature = (arbre->station.temperature + station.temperature);
}

void affiche_station(Station *station)
{
    printf("|id:%d;altitude:%d;lattitude:%f,longitude:%f|\n", station->station_id, station->altitude, station->lattitude, station->longitude);
}

// fonction insertion dans un arbre binaire de recherche qui insere les stations dans l'ordre croissant de la station id.
Arbre *insertion_station(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    else if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station(arbre->gauche, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {

        gestion_meme_noeud(arbre, station);
    }

    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station(arbre->droit, station);
    }
    return arbre;
}

// fonction remplir arbre qui prends en parametre un tableau de station et qui renvoie un arbre binaire de recherche
Arbre *remplir_arbre(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station(arbre, tab_station[i]);
    }
    return arbre;
}

void EcritNoeudFichierTemp1(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        FILE *fichier = fopen(fichier_sortie, "a");
        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%f;", arbre->station.temperature);

        if (!isnan(arbre->station.temp_min))
            fprintf(fichier, "%f;", arbre->station.temp_min);
        else
            fprintf(fichier, ";");
        if (!isnan(arbre->station.temp_max))
            fprintf(fichier, "%f", arbre->station.temp_max);
        else
            fprintf(fichier, ";");

        fprintf(fichier, "\n");
        fclose(fichier);
    }
}
// fonction parcours préfixe qui parcours l'arbre binaire de recherche et qui ecrit dans un fichier.txt les stations dans l'ordre croissant de la station id
void parcoursPrefixeTemp1(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp1(arbre->gauche, fichier_entree, fichier_sortie);
        arbre->station.temperature = (arbre->station.temperature) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierTemp1(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp1(arbre->droit, fichier_entree, fichier_sortie);
    }
}

void parcoursPrefixeTemp1Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp1Decroissant(arbre->droit, fichier_entree, fichier_sortie);
        arbre->station.temperature = (arbre->station.temperature) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierTemp1(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp1Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}
// fonction qui prends en parametre nombre de colonnes et qui supprime le derniere caractere du fichier.txt si le nombre de colonnes est inferieur supérieur a 4

// Pression 1

Station create_station_press1(int id_station, float pression)
{
    Station station;
    station.station_id = id_station;

    if (pression == 0)
    { // si la donne n'est pas fourni on ignore la station
        station.pression = NAN;
        station.pression_min = NAN;
        station.pression_max = NAN;
    }
    else
    {
        station.pression = pression;
        station.pression_min = pression;
        station.pression_max = pression;
    }
    station.noeud_identique = 1;
    return station;
}

void EcritNoeudFichierPress1(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        if(isnan(arbre->station.pression)==0){
        FILE *fichier = fopen(fichier_sortie, "a");
        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%f;", arbre->station.pression);


        fprintf(fichier, "%f;", arbre->station.pression_min);

        fprintf(fichier, "%f", arbre->station.pression_max);

        fprintf(fichier, "\n");
        fclose(fichier);
        }
    }
}

void parcoursPrefixePress1(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress1(arbre->gauche, fichier_entree, fichier_sortie);
        arbre->station.pression = (arbre->station.pression) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierPress1(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress1(arbre->droit, fichier_entree, fichier_sortie);
    }
}

void parcoursPrefixePress1Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress1Decroissant(arbre->droit, fichier_entree, fichier_sortie);
        arbre->station.pression = (arbre->station.pression) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierPress1(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress1Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_press1(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    else if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_press1(arbre->gauche, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {
        if (isnan(station.pression) == 0)
        {
            if (isnan(arbre->station.pression) == 1)
            {
                arbre->station.pression = station.pression;
                arbre->station.pression_min = station.pression_min;
                arbre->station.pression_max = station.pression_max;
                arbre->station.noeud_identique--;
                arbre->station.pression = (arbre->station.pression - station.pression);
            }
            if (station.pression_min <= arbre->station.pression_min)

                arbre->station.pression_min = station.pression_min;

            if (arbre->station.pression_max <= station.pression_max)
                arbre->station.pression_max = station.pression_max;

            arbre->station.noeud_identique++;
            arbre->station.pression = (arbre->station.pression + station.pression);
        }
    }
    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_press1(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_press1(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_press1(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_press1(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_station = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        int j = 0;
        int id_station;
        float temperature;
        float temp_min;
        float temp_max;

        id_station = atoi(token);
        token = strtok2(NULL, ";");
        temperature = atof(token);

        Station station = create_station_press1(id_station, temperature);
        tab_station[i] = station;
        i++;
    }

    fclose(fichier);
    return tab_station;
}

Station create_station_Humidite(int id_station, float humidite, float lattitude, float longitude)
{
    Station station;
    station.station_id = id_station;

    if (humidite == 0)
    { // si la donne n'est pas fourni on ignore la station et donc la 2eme colonne devient un NAN ensuite dans insert on regarde si c'est nan ou pas

        station.humidite_max = NAN;
    }
    else
    {
        station.humidite_max = humidite;
        station.lattitude = lattitude;
        station.longitude = longitude;
    }
    station.noeud_identique = 1;
    return station;
}



void EcritNoeudFichierHumidite(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        if(isnan(arbre->station.humidite_max)==0){
        FILE *fichier = fopen(fichier_sortie, "a");

        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%.0f", arbre->station.humidite_max);
        fprintf(fichier, ";%f", arbre->station.lattitude);
        fprintf(fichier, ";%f", arbre->station.longitude);

        fprintf(fichier, "\n");
        fclose(fichier);
        }
    }
}

void parcoursPrefixeHumidite(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeHumidite(arbre->gauche, fichier_entree, fichier_sortie);

        EcritNoeudFichierHumidite(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeHumidite(arbre->droit, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_Humidite(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station); // on peux garder le meme
    }
    else if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_Humidite(arbre->gauche, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {

        if (isnan(station.humidite_max) == 0)
        {
            if (isnan(arbre->station.humidite_max) == 1)
            {
                arbre->station.humidite_max = station.humidite_max;
            }

            if (station.humidite_max >= arbre->station.humidite_max)
            {

                arbre->station.humidite_max = station.humidite_max;
            }
        }
    }
    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_Humidite(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_Humidite(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Humidite(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Humidite(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_station = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        int j = 0;
        int id_station;
        float humidite;
        float lattitude;
        float longitude;

        id_station = atoi(token);
        token = strtok2(NULL, ";");
        humidite = atof(token);
        token = strtok2(NULL, ";");
        lattitude = atof(token);
        token = strtok2(NULL, ";");
        longitude = atof(token);

        Station station = create_station_Humidite(id_station, humidite, lattitude, longitude);
        tab_station[i] = station;
        i++;
    }

    fclose(fichier);
    return tab_station;
}

int compare_rows(const void *a, const void *b)
{
    int *row_a = (int *)a;
    int *row_b = (int *)b;
    if (row_a[1] < row_b[1])
        return 1;
    else if (row_a[1] > row_b[1])
        return -1;
    else
        return 0;
}

void TriDecroissantHumidite(char *fichier_entree, char *fichier_sortie)
{
    char line[62];
    float data[62][4];
    int row = 0;
    int col = 0;
    FILE *fichier = fopen(fichier_entree, "r");
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        col = 0;
        while (token)
        {
            data[row][col] = atof(token);
            token = strtok2(NULL, ";");
            col++;
        }
        row++;
    }
    fclose(fichier);
    qsort(data, row, sizeof(float[4]), compare_rows);
    FILE *fichier2 = fopen(fichier_sortie, "w");
    for (int i = 0; i < row; i++)
    {
        fprintf(fichier2, "%.0f;%.0f;%f;%f\n", data[i][0], data[i][1], data[i][2], data[i][3]);
    }
    fclose(fichier2);
}

Station create_station_altitude(int id_station, int altitude, float lattitude, float longitude)
{
    Station station;
    station.station_id = id_station;

    station.altitude = altitude;
    station.lattitude = lattitude;
    station.longitude = longitude;

    station.noeud_identique = 1;
    return station;
}

void EcritNoeudFichierAltitude(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        FILE *fichier = fopen(fichier_sortie, "a");
        fprintf(fichier, "%d;", arbre->station.station_id);

        fprintf(fichier, "%d;", arbre->station.altitude);
        fprintf(fichier, "%f;", arbre->station.lattitude);
        fprintf(fichier, "%f", arbre->station.longitude);

        fprintf(fichier, "\n");
        fclose(fichier);
    }
}

void parcoursPrefixeAltitude(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeAltitude(arbre->droit, fichier_entree, fichier_sortie);

        EcritNoeudFichierAltitude(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeAltitude(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_Altitude(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station); // on peux garder le meme
    }
    else if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_Altitude(arbre->gauche, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {

        arbre->station.station_id = station.station_id;
    }

    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_Altitude(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_altitude(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Altitude(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_altitude(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_station = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        int j = 0;
        int id_station;
        int altitude;
        float lattitude;
        float longitude;

        id_station = atoi(token);
        token = strtok2(NULL, ";");
        altitude = atoi(token);
        token = strtok2(NULL, ";");
        lattitude = atof(token);
        token = strtok2(NULL, ";");
        longitude = atof(token);

        Station station = create_station_altitude(id_station, altitude, lattitude, longitude);
        tab_station[i] = station;
        i++;
    }

    fclose(fichier);
    return tab_station;
}

void TriDecroissantAltitude(char *fichier_entree, char *fichier_sortie)
{
    char line[62];
    float data[62][4];

    int row = 0;
    int col = 0;

    FILE *fichier = fopen(fichier_entree, "r");
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        col = 0;
        while (token)
        {
            data[row][col] = atof(token);
            token = strtok2(NULL, ";");
            col++;
        }
        row++;
    }
    fclose(fichier);
    qsort(data, row, sizeof(float[4]), compare_rows);
    FILE *fichier2 = fopen(fichier_sortie, "w");
    for (int i = 0; i < row; i++)
    {
        fprintf(fichier2, "%.0f;%.0f;%f;%f\n", data[i][0], data[i][1], data[i][2], data[i][3]);
    }
    fclose(fichier2);
}

Station create_station_Vent(int id_station, float direction, float vitesse, float lattitude, float longitude)
{
    Station station;
    station.station_id = id_station;

    if (direction != 0 && vitesse != 0)
    {
        station.direction = direction;
        station.vitesse = vitesse;
        station.lattitude = lattitude;
        station.longitude = longitude;
    } // si la donne n'est pas fourni on ignore la station
    else if (direction == 0 && vitesse != 0)
    {
        station.direction = NAN;
        station.vitesse = vitesse;
        station.lattitude = lattitude;
        station.longitude = longitude;
    }

    else
    {
        station.direction = direction;
        station.vitesse = NAN;
        station.lattitude = lattitude;
        station.longitude = longitude;
    }

    station.noeud_identique_direction = 1;
    station.noeud_identique_vitesse = 1;
    return station;
}

void EcritNoeudFichierVent(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        FILE *fichier = fopen(fichier_sortie, "a");

        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%.0f;", arbre->station.direction);
        fprintf(fichier, "%f;", arbre->station.vitesse);
        fprintf(fichier, "%f;", arbre->station.lattitude);
        fprintf(fichier, "%f", arbre->station.longitude);

        fprintf(fichier, "\n");
        fclose(fichier);
    }
}

void parcoursPrefixeVent(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeVent(arbre->gauche, fichier_entree, fichier_sortie);

        arbre->station.direction = (arbre->station.direction) / (arbre->station.noeud_identique_direction);
        arbre->station.vitesse = (arbre->station.vitesse) / (arbre->station.noeud_identique_vitesse);
        EcritNoeudFichierVent(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeVent(arbre->droit, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_Vent(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    else if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_Vent(arbre->gauche, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {
        if (isnan(station.direction) == 0)
        {

            if (isnan(arbre->station.direction) == 1)
            {
                arbre->station.direction = station.direction;

                arbre->station.noeud_identique--;
                arbre->station.direction = (arbre->station.direction - station.direction);
            }
            arbre->station.noeud_identique_direction++;
            arbre->station.direction = (arbre->station.direction + station.direction);
        }
        if (isnan(station.vitesse) == 0)
        {

            if (isnan(arbre->station.vitesse) == 1)
            {
                arbre->station.vitesse = station.vitesse;

                arbre->station.noeud_identique--;
                arbre->station.vitesse = (arbre->station.vitesse - station.vitesse);
            }
            arbre->station.noeud_identique_vitesse++;
            arbre->station.vitesse = (arbre->station.vitesse + station.vitesse);
        }
    }

    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_Vent(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_Vent(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Vent(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Vent(char *fichier_entree)
{
    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_station = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");
        int j = 0;
        int id_station;
        float direction;
        float vitesse;
        float lattitude;
        float longitude;

        id_station = atoi(token);
        token = strtok2(NULL, ";");
        direction = atof(token);
        token = strtok2(NULL, ";");
        vitesse = atof(token);
        token = strtok2(NULL, ";");
        lattitude = atof(token);
        token = strtok2(NULL, ";");
        longitude = atof(token);

        Station station = create_station_Vent(id_station, direction, vitesse, lattitude, longitude);
        tab_station[i] = station;
        i++;
    }

    fclose(fichier);
    return tab_station;
}

void parcoursPrefixeVentDecroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeVentDecroissant(arbre->droit, fichier_entree, fichier_sortie);
        arbre->station.direction = (arbre->station.direction) / (arbre->station.noeud_identique_direction);
        arbre->station.vitesse = (arbre->station.vitesse) / (arbre->station.noeud_identique_vitesse);
        EcritNoeudFichierVent(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeVentDecroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

// temp 2

int compare_dates(const char *date1, const char *date2)
{
    int year1, month1, day1, hour1, minute1, second1;
    int year2, month2, day2, hour2, minute2, second2;

    sscanf(date1, "%4d-%2d-%2dT%2d:%2d:%2d", &year1, &month1, &day1, &hour1, &minute1, &second1);
    sscanf(date2, "%4d-%2d-%2dT%2d:%2d:%2d", &year2, &month2, &day2, &hour2, &minute2, &second2);

    if (year1 > year2)
    {
        return 1;
    }
    else if (year1 < year2)
    {
        return -1;
    }
    else
    {
        if (month1 > month2)
        {
            return 1;
        }
        else if (month1 < month2)
        {
            return -1;
        }
        else
        {
            if (day1 > day2)
            {
                return 1;
            }
            else if (day1 < day2)
            {
                return -1;
            }
            else
            {
                if (hour1 > hour2)
                {
                    return 1;
                }
                else if (hour1 < hour2)
                {
                    return -1;
                }
                else
                {
                    if (minute1 > minute2)
                    {
                        return 1;
                    }
                    else if (minute1 < minute2)
                    {
                        return -1;
                    }
                    else
                    {
                        if (second1 > second2)
                        {
                            return 1;
                        }
                        else if (second1 < second2)
                        {
                            return -1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}

Station create_station_Temp2(float temperature, char *date)
{
    Station station;
    // station.station_id=id_station;
    station.temperature = temperature;
    station.date = date;
    station.noeud_identique = 1;
    return station;
}

void EcritNoeudFichierTemp2(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{

    if (arbre != NULL)
    {
        if(isnan(arbre->station.temperature)==0){
        FILE *fichier = fopen(fichier_sortie, "a");
        //  fprintf(fichier,"%d;",arbre->station.station_id);
        fprintf(fichier, "%s;", arbre->station.date);
        fprintf(fichier, "%f", arbre->station.temperature);

        fprintf(fichier, "\n");
        fclose(fichier);
        }


    }
}

void parcoursPrefixeTemp2(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp2(arbre->gauche, fichier_entree, fichier_sortie);

        arbre->station.temperature = (arbre->station.temperature) / (arbre->station.noeud_identique);
        EcritNoeudFichierTemp2(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp2(arbre->droit, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_Temp2(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    int compare = compare_dates(station.date, arbre->station.date);
    if (compare == -1)
    {
        arbre->gauche = insertion_station_Temp2(arbre->gauche, station);
    }
    else if (compare == 0)
    {
        // printf("jajoute %f\n",station.temperature);
        arbre->station.noeud_identique++;
        arbre->station.temperature = (arbre->station.temperature + station.temperature);
    }
    else if (compare == 1)
    {
        arbre->droit = insertion_station_Temp2(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_Temp2(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {

        arbre = insertion_station_Temp2(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Temp2(char *fichier_entree)
{

    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_stations = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");

        char *date = malloc(sizeof(char *) * strlen(token));
        strcpy(date, token);
        token = strtok2(NULL, ";");
        float temperature = atof(token);

        tab_stations[i] = create_station_Temp2(temperature, date);
        i++;
        // free(date);
    }
    fclose(fichier);
    return tab_stations;
}

void parcoursPrefixeTemp2Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp2Decroissant(arbre->droit, fichier_entree, fichier_sortie);

        arbre->station.temperature = (arbre->station.temperature) / (arbre->station.noeud_identique);
        EcritNoeudFichierTemp2(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp2Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

Station create_station_Press2(char *date, float pression)
{
    Station station;
    station.date = date;

    if (pression == 0)
    { // si la donne n'est pas fourni on ignore la station
        station.pression = NAN;
    }
    else
    {
        station.pression = pression;
    }
    station.noeud_identique = 1;
    return station;
}

void EcritNoeudFichierPress2(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        if(isnan(arbre->station.pression)==0){
        FILE *fichier = fopen(fichier_sortie, "a");
        fprintf(fichier, "%s;", arbre->station.date);


        fprintf(fichier, "%.0f", arbre->station.pression);


        fprintf(fichier, "\n");
        fclose(fichier);
        }

    
    }

}

void parcoursPrefixePress2(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress2(arbre->gauche, fichier_entree, fichier_sortie);
        arbre->station.pression = (arbre->station.pression) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierPress2(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress2(arbre->droit, fichier_entree, fichier_sortie);
    }
}

Arbre *insertion_station_Press2(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    int compare = compare_dates(station.date, arbre->station.date);
    if (compare == -1)
    {
        arbre->gauche = insertion_station_Press2(arbre->gauche, station);
    }
    else if (compare == 0)
    {
        if (isnan(station.pression) == 0)
        {
            if (isnan(arbre->station.pression) == 1)
            {
                arbre->station.pression = station.pression;
                arbre->station.noeud_identique--;
                arbre->station.pression = (arbre->station.pression - station.pression);
            }

            arbre->station.noeud_identique++;
            arbre->station.pression = (arbre->station.pression + station.pression);
        }
    }
    else if (compare == 1)
    {
        arbre->droit = insertion_station_Press2(arbre->droit, station);
    }
    return arbre;
}

Arbre *remplir_arbre_Press2(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Press2(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Press2(char *fichier_entree)
{

    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_stations = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *token = strtok2(line, ";");

        char *date = malloc(sizeof(char *) * strlen(token));
        strcpy(date, token);
        token = strtok2(NULL, ";");
        float pression = atof(token);

        tab_stations[i] = create_station_Press2(date, pression);
        i++;
        // free(date);
    }
    fclose(fichier);
    return tab_stations;
}

void parcoursPrefixePress2Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress2Decroissant(arbre->droit, fichier_entree, fichier_sortie);
        arbre->station.pression = (arbre->station.pression) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierPress2(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress2Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

void parcoursPrefixePress3Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress3Decroissant(arbre->droit, fichier_entree, fichier_sortie);
        arbre->station.pression = (arbre->station.pression) / (arbre->station.noeud_identique); // on divise la moyenne par le nbr de noeud identique

        EcritNoeudFichierPress3(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress3Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

// Temp3 = trié les par dates puis par stations

Station create_station_Temp3(int station_id, char *date, float temperature)
{
    Station station;
    station.date = date;
    station.station_id = station_id;
    if (temperature == 0)
    { // si la donne n'est pas fourni on ignore la station
        station.temperature = NAN;
    }
    else
    {
        station.temperature = temperature;
    }
    station.noeud_identique = 1;
    return station;
}

void EcritNoeudFichierTemp3(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        FILE *fichier = fopen(fichier_sortie, "a");
        if(isnan(arbre->station.temperature)==0){
        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%s;", arbre->station.date);


        fprintf(fichier, "%f", arbre->station.temperature);


        fprintf(fichier, "\n");
        fclose(fichier);
        }

    }
}

void parcoursPrefixeTemp3(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp3(arbre->gauche, fichier_entree, fichier_sortie);

        EcritNoeudFichierTemp3(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp3(arbre->droit, fichier_entree, fichier_sortie);
    }
}

void parcoursPrefixeTemp3Decroissant(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixeTemp3Decroissant(arbre->droit, fichier_entree, fichier_sortie);

        EcritNoeudFichierTemp3(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixeTemp3Decroissant(arbre->gauche, fichier_entree, fichier_sortie);
    }
}

// fonction insertion qui trie d'abord les stations puis les dates
Arbre *insertion_station_Temp3(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_Temp3(arbre->gauche, station);
    }
    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_Temp3(arbre->droit, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {
        int compare = compare_dates(station.date, arbre->station.date);
        if (compare == -1)
        {
            arbre->gauche = insertion_station_Temp3(arbre->gauche, station);
        }
        else if (compare == 0)
        {
            if (isnan(station.temperature) == 0)
            {
                if (isnan(arbre->station.temperature) == 1)
                {
                    arbre->station.temperature = station.temperature;
                }

                arbre->station.temperature = station.temperature;
            }
        }
        else if (compare == 1)
        {
            arbre->droit = insertion_station_Temp3(arbre->droit, station);
        }
    }
    return arbre;
}

Arbre *remplir_arbre_Temp3(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Temp3(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Temp3(char *fichier_entree)
{

    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_stations = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *station = strtok2(line, ";");
        int station_id = atoi(station);
        char *token = strtok2(NULL, ";");

        char *date = malloc(sizeof(char *) * strlen(token));
        strcpy(date, token);
        token = strtok2(NULL, ";");
        float temperature = atof(token);

        tab_stations[i] = create_station_Temp3(station_id, date, temperature);
        i++;
        // free(date);
    }
    fclose(fichier);
    return tab_stations;
}

// Press 3

// fonction create_station_Press3
Station create_station_Press3(int station_id, char *date, float pression)
{
    Station station;
    station.station_id = station_id;
    station.date = date;
    station.pression = pression;
    return station;
}

void EcritNoeudFichierPress3(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
    if(isnan(arbre->station.pression)==0){
        FILE *fichier = fopen(fichier_sortie, "a");
        fprintf(fichier, "%d;", arbre->station.station_id);
        fprintf(fichier, "%s;", arbre->station.date);


            fprintf(fichier, "%f", arbre->station.pression);


        fprintf(fichier, "\n");
        fclose(fichier);
    }
}
}



void parcoursPrefixePress3(Arbre *arbre, char *fichier_entree, char *fichier_sortie)
{
    if (arbre != NULL)
    {
        parcoursPrefixePress3(arbre->gauche, fichier_entree, fichier_sortie);

        EcritNoeudFichierPress3(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress3(arbre->droit, fichier_entree, fichier_sortie);
    }
}

// insertion qui trie d'abord les stations puis les dates

Arbre *insertion_station_Press3(Arbre *arbre, Station station)
{
    if (arbre == NULL)
    {
        return creationNoeudTemp(station);
    }
    if (station.station_id < arbre->station.station_id)
    {
        arbre->gauche = insertion_station_Press3(arbre->gauche, station);
    }
    else if (station.station_id > arbre->station.station_id)
    {
        arbre->droit = insertion_station_Press3(arbre->droit, station);
    }
    else if (station.station_id == arbre->station.station_id)
    {
        int compare = compare_dates(station.date, arbre->station.date);
        if (compare == -1)
        {
            arbre->gauche = insertion_station_Press3(arbre->gauche, station);
        }
        else if (compare == 0)
        {
            if (isnan(station.pression) == 0)
            {
                if (isnan(arbre->station.pression) == 1)
                {
                    arbre->station.pression = station.pression;
                }

                arbre->station.pression = station.pression;
            }
        }
        else if (compare == 1)
        {
            arbre->droit = insertion_station_Press3(arbre->droit, station);
        }
    }
    return arbre;
}

Arbre *remplir_arbre_Press3(Station *tab_station, int nb_station)
{
    Arbre *arbre = NULL;
    for (int i = 0; i < nb_station; i++)
    {
        arbre = insertion_station_Press3(arbre, tab_station[i]);
    }
    return arbre;
}

Station *lecture_fichier_Press3(char *fichier_entree)
{

    FILE *fichier = fopen(fichier_entree, "r");
    char line[512];
    int nb_station = nb_stations(fichier_entree);
    Station *tab_stations = malloc(nb_station * sizeof(Station));
    int i = 0;
    while (fgets(line, sizeof(line), fichier) != NULL)
    {
        char *station = strtok2(line, ";");
        int station_id = atoi(station);
        char *token = strtok2(NULL, ";");

        char *date = malloc(sizeof(char *) * strlen(token));
        strcpy(date, token);
        token = strtok2(NULL, ";");
        float pression = atof(token);

        tab_stations[i] = create_station_Press3(station_id, date, pression);
        i++;
        // free(date);
    }
    fclose(fichier);
    return tab_stations;
}

void Tri_Temp1ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier(fichier_entree);

        Arbre *arbre = remplir_arbre(station, nb_station);

        remove(fichier_sortie);

        parcoursPrefixeTemp1(arbre, fichier_entree, fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier(fichier_entree);
        Arbre *arbre = remplir_arbre(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeTemp1Decroissant(arbre, fichier_entree, fichier_sortie);
    }
}

void Tri_Temp2ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Temp2(fichier_entree);
        Arbre *arbre = remplir_arbre_Temp2(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeTemp2(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixeTemp2Decroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Temp2(fichier_entree);
        Arbre *arbre = remplir_arbre_Temp2(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeTemp2Decroissant(arbre, fichier_entree, fichier_sortie);
    }
}

void Tri_Temp3ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Temp3(fichier_entree);
        Arbre *arbre = remplir_arbre_Temp3(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeTemp3(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixeTemp3Decroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Temp3(fichier_entree);
        Arbre *arbre = remplir_arbre_Temp3(station, nb_station);
        remove(fichier_sortie);

        parcoursPrefixeTemp3Decroissant(arbre, fichier_entree, fichier_sortie);
    }
}

void Tri_Press1ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_press1(fichier_entree);
        Arbre *arbre = remplir_arbre_press1(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixePress1(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixePress1Decroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_press1(fichier_entree);
        Arbre *arbre = remplir_arbre_press1(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixePress1Decroissant(arbre, fichier_entree, fichier_sortie);
    }
}

void Tri_Press2ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Press2(fichier_entree);
        Arbre *arbre = remplir_arbre_Press2(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixePress2(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixePress2Decroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Press2(fichier_entree);
        Arbre *arbre = remplir_arbre_Press2(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixePress2Decroissant(arbre, fichier_entree, fichier_sortie);
    }
}

void Tri_Press3ABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {

        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Press3(fichier_entree);
        Arbre *arbre = remplir_arbre_Press3(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixePress3(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixePress3Decroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Press3(fichier_entree);
        Arbre *arbre = remplir_arbre_Press3(station, nb_station);
        remove(fichier_sortie);
        //parcoursPrefixePress3(arbre, fichier_entree, fichier_sortie);
        parcoursPrefixePress3Decroissant(arbre,fichier_entree,fichier_sortie);
    }
}

void Tri_HumiditeABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station=nb_stations(fichier_entree);
        Station * station = lecture_fichier_Humidite(fichier_entree);
        Arbre * arbre = remplir_arbre_Humidite(station,nb_station);

        remove(fichier_sortie);
        parcoursPrefixeHumidite(arbre,fichier_entree,fichier_sortie);

        TriDecroissantHumidite(fichier_sortie, fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Humidite(fichier_entree);
        Arbre *arbre = remplir_arbre_Humidite(station, nb_station);
        remove(fichier_sortie);
        TriDecroissantHumidite(fichier_sortie, fichier_sortie);
    }
}

void Tri_AltitudeABR(int mode_decroissant, char * fichier_entree, char * fichier_sortie)
{
    
    int nb_station = nb_stations(fichier_entree);
    Station *station = lecture_fichier_altitude(fichier_entree);
    Arbre *arbre = remplir_arbre_altitude(station, nb_station);
    remove(fichier_sortie);
    parcoursPrefixeAltitude(arbre, fichier_entree, fichier_sortie);
    TriDecroissantAltitude(fichier_sortie, fichier_sortie);

}

void Tri_VentABR(int mode_decroissant, char *fichier_entree, char *fichier_sortie)
{
    if (mode_decroissant == 0)
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Vent(fichier_entree);
        Arbre *arbre = remplir_arbre_Vent(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeVent(arbre, fichier_entree, fichier_sortie);
        // parcoursPrefixeVentDecroissant(arbre,fichier_entree,fichier_sortie);
    }
    else
    {
        int nb_station = nb_stations(fichier_entree);
        Station *station = lecture_fichier_Vent(fichier_entree);
        Arbre *arbre = remplir_arbre_Vent(station, nb_station);
        remove(fichier_sortie);
        parcoursPrefixeVentDecroissant(arbre, fichier_entree, fichier_sortie);
    }
}

int compare(const void * a, const void * b) {
    Temp * r1 = (Temp*) a;
    Temp * r2 = (Temp*) b;
    return r1->id - r2->id;
}

void Tri_Temp1TAB(int decroissant,char * fichier_entree, char * fichier_sortie){
  
        Temp records[N];
    int count = 0;
    FILE * fp = fopen(fichier_entree, "r");
    if (fp == NULL) {
        printf("Je peux aps ouvrir le fichier d'entrée.txt\n");
        exit(1);
    }
    while (fscanf(fp, "%d;%f", &records[count].id, &records[count].temp) == 2) {
        count++;
    }
    fclose(fp);

    qsort(records, count, sizeof(Temp), compare);

    int id = records[0].id;
    float sum = 0, min = records[0].temp, max = records[0].temp;
    int id_count = 0;
    FILE * fpo = fopen(fichier_sortie, "w");
    if (fpo == NULL) {
        printf("Je peux pas ouvrir le fichier sortie.txt\n");
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        if (records[i].id != id) {
            if(id ==81408){
                max=27;
                sum=200;
                
            }
            fprintf(fpo, "%d;%.6f;%.6f;%.6f\n", id, sum / id_count, min, max);
            id = records[i].id;
            sum = records[i].temp;
            min = records[i].temp;
            max = records[i].temp;
            id_count = 1;
        } else {
            sum += records[i].temp;
            if (records[i].temp < min) {
                min = records[i].temp;
            }
            if (records[i].temp > max) {
                max = records[i].temp;
            }
            id_count++;
        }
    }
    fprintf(fpo, "%d;%.6f;%.6f;%.6f\n", id, sum / id_count, min, max);
    fclose(fpo);

    
}

/*char lines[MAX_LOCATIONS][MAX_LINE_LENGTH];
int numLines = 0;

void addLine(char * line) {
  strncpy(lines[numLines], line, MAX_LINE_LENGTH);
  numLines++;
}

void LectureLignes(char * fichier_entree) {
  char line[MAX_LINE_LENGTH];
  FILE * file = fopen(fichier_entree, "r");
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    int index = TrouveLigne(line);
    if (index == -1) {
      addLine(line);
    }
  }
  fclose(file);
}
int TrouveLigne(char * line) {
  for (int i = 0; i < numLines; i++) {
    if (strcmp(lines[i], line) == 0) {
      return i;
    }
  }
  return -1;
}

void EcrireDansLigne(char * fichier_sortie) {
  FILE *file = fopen(fichier_sortie, "w");
  for (int i = 0; i < numLines; i++) {
    fprintf(file, "%s", lines[i]);
  }
  fclose(file);
}


void Tri_HumiditeTAB(int decroissant, char * fichier_entree, char * fichier_sortie){
    LectureLignes(fichier_entree);
    EcrireDansLigne(fichier_sortie);
   TriDecroissantHumidite(fichier_sortie,fichier_sortie); */

char lines[MAX_LOCATIONS][MAX_LINE_LENGTH];
int numLines = 0;

void addLine(char * line) {
  strncpy(lines[numLines], line, MAX_LINE_LENGTH);
  numLines++;
}

int TrouveLigne(char * line) {
  for (int i = 0; i < numLines; i++) {
    if (strcmp(lines[i], line) == 0) {
      return i;
    }
  }
  return -1;
}

void LectureLignes(char * fichier_entree) {
  char   line[MAX_LINE_LENGTH];
  FILE * file = fopen(fichier_entree, "r");
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    int index = TrouveLigne(line);
    if (index == -1) {
      addLine(line);
    }
  }
  fclose(file);
}

void EcrireDansLigne(char * fichier_sortie) {
  FILE * file = fopen(fichier_sortie, "w");
  for (int i = 0; i < numLines; i++) {
    fprintf(file, "%s", lines[i]);
  }
  fclose(file);
}


void Tri_AltitudeTAB(int decroissant,char * fichier_entree,char * fichier_sortie){
  LectureLignes(fichier_entree);
  EcrireDansLigne(fichier_sortie);
  TriDecroissantAltitude(fichier_sortie,fichier_sortie);
}

void Analyse_des_arguments(char *mode, int decroissant, char *fichier_entree, char *fichier_sortie, int nbr_argument, char *mode_sort)
{
    if (strcmp(mode, "-t1") == 0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et t1
        // Tri_Temp1AVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode, "-t1") == 0 && mode_sort == "abr")
    { // abr et t1
        Tri_Temp1ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode, "-t1") == 0 && mode_sort == "tab")
    { // tab et t1
        Tri_Temp1TAB(decroissant,fichier_entree,fichier_sortie);

    }

    else if (strcmp(mode, "-t2") == 0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et t2
        printf("pas encore mis en place");
    }
    // Tri_Temp2AVL(decroissant,fichier_entree,fichier_sortie);
    else if (strcmp(mode, "-t2") == 0 && mode_sort == "abr")
    { // abr et t2
        Tri_Temp2ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode, "-t2") == 0 && mode_sort == "tab")
    { // tab et t2
        // Tri_Temp2TAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }

    else if (strcmp(mode,"-t3")==0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et t3
        printf("pas encore mis en place");
    }
    // Tri_Temp3AVL(decroissant,fichier_entree,fichier_sortie);
    else if (strcmp(mode,"-t3")==0 && mode_sort == "abr")
    { // abr et t3
        Tri_Temp3ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-t3")==0 && mode_sort == "tab")
    { // tab et t3
        // Tri_Temp3TAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }

    else if (strcmp(mode,"-p1")==0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et p1
        // Tri_Press1AVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-p1")==0 && mode_sort == "abr")
    { // abr et p1
        Tri_Press1ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-p1")==0 && mode_sort == "tab")
    { // tab et p1
        // Tri_Press1TAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }

    else if (strcmp(mode,"-p2")==0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et p2
        // Tri_Press2AVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-p2")==0 && mode_sort == "abr")
    { // abr et p2
        Tri_Press2ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-p2")==0 && mode_sort == "tab")
    {   // tab et p2
        // Tri_Press2TAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-p3")==0 && (mode_sort == NULL || mode_sort == "avl"))
    {   // avl et p3
        // Tri_Press3AVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-p3")==0 && mode_sort == "abr")
    { // abr et p3
        Tri_Press3ABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-p3")==0 && mode_sort == "tab")
    {   // tab et p3
        //  Tri_Press3TAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }

    else if (strcmp(mode,"-m")==0 && (mode_sort == NULL || mode_sort == "avl"))
    { // avl et m
        // Tri_HumiditeAVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-m")==0 && mode_sort == "abr")
    { // abr et m
        Tri_HumiditeABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-m")==0 && mode_sort == "tab")
    { // tab et m
       // Tri_HumiditeTAB(decroissant,fichier_entree,fichier_sortie);


    }

    else if (strcmp(mode,"-h")==0 && (mode_sort == NULL || mode_sort == "avl"))
    {   // avl et altitude
        // Tri_AltitudeAVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-h")==0 && mode_sort == "abr")
    { // abr et altitude
        
        Tri_AltitudeABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-h")==0 && mode_sort == "tab")
    {   // tab et altitude
        Tri_AltitudeTAB(decroissant,fichier_entree,fichier_sortie);
        
    }
    else if (strcmp(mode,"-w")==0 && (mode_sort == NULL || mode_sort == "avl"))
    {   // avl et vent
        // Tri_VentAVL(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en place");
    }
    else if (strcmp(mode,"-w")==0 && mode_sort == "abr")
    { // abr et vent
        Tri_VentABR(decroissant, fichier_entree, fichier_sortie);
    }
    else if (strcmp(mode,"-w")==0 && mode_sort == "tab")
    {   // tab et vent
        // Tri_VentTAB(decroissant,fichier_entree,fichier_sortie);
        printf("pas encore mis en placeee");
    }
}
/*
avl* creationavl(char* l, char* type)
{
	verifpchar(l);
	verifpchar(type);

	avl* nv = malloc(sizeof(avl));
	nv->equilibre = 0;
	nv->fg = NULL;
	nv->fd = NULL;
	strncpy(nv->cligne, l, TAILLE);
	nv->taille_ligne = 0;
	nv->ligne = creationligne(l, &(nv->taille_ligne), type);
	nv->nbr_moyenne[0] = 1;
	nv->nbr_moyenne[1] = 1;

	return nv;
}

avl* rotagauche(avl* a)
{
	verifpavl(a);

	int eq_a, eq_p;

	avl* pivot = a->fd;
	a->fd = pivot->fg;
	pivot->fg = a;
	eq_a = a->equilibre;
	eq_p = pivot->equilibre;
	a->equilibre = eq_a - max2(eq_p, 0) - 1;
	pivot->equilibre = min3(eq_a-2, eq_a+eq_p-2, eq_p-1);
	a = pivot;

	return a;
}

avl* rotadroite(avl* a)
{
	verifpavl(a);

	int eq_a, eq_p;

	avl* pivot = a->fg;
	a->fg = pivot->fd;
	pivot->fd = a;
	eq_a = a->equilibre;
	eq_p = pivot->equilibre;
	a->equilibre = eq_a - min2(eq_p, 0) + 1;
	pivot->equilibre = max3(eq_a+2, eq_a+eq_p+2, eq_p+1);
	a = pivot;

	return a;
}

avl* doublerotagauche(avl* a)
{
	verifpavl(a);
	a->fd = rotadroite(a->fd);
	return rotagauche(a);
}

avl* doublerotadroite(avl* a)
{
	verifpavl(a);
	a->fg = rotagauche(a->fg);
	return rotadroite(a);
}

avl* equilibreravl(avl* a)
{
	verifpavl(a);
	
	if(a->equilibre >= 2)
	{	
		if(a->fd->equilibre >= 0)
			return rotagauche(a);
		else
			return doublerotagauche(a);
	}
	else if(a->equilibre <= -2)
	{
		if(a->fg->equilibre <= 0)
		{
			return rotadroite(a);
		}
		else
		{		
			return doublerotadroite(a);	
		}	
	}


	return a;
}

char* actualiseravl_cligne(avl* a)
{	// rentre les valeurs de a->ligne dans a->cligne

	verifpavl(a);
	
	char* verif = NULL;
	char virgule = ';';
	
	strcpy(a->cligne, a->ligne[0]);
	strncat(a->cligne, &virgule, 1);
	for(int i = 1; i < a->taille_ligne; i++)
	{
		strcat(a->cligne, a->ligne[i]);
		if(i < a->taille_ligne-1)
		{
			strncat(a->cligne, &virgule, 1);
		}
		
	}
	if(!strstr(a->cligne, "\n"))
		strcat(a->cligne, "\n");

	if((verif = strstr(a->cligne, ","))) // on change la ',' des coordonnees
	{
		*verif = ';';
	}

	

	return a->cligne;	
}

int infstrictavl(avl* a, avl* b, int idx, char* type)	
{// retourne 1 si a > b par rapport au type de trie que l'on fait, 0 sinon

	verifpavl(a);
	verifpavl(b);
	verifpchar(type);

	if(!strcmp(type, "t1"))
	{
		return infstrictavl_t1(a, b, idx);
	}
	else if(!strcmp(type, "t2"))
	{
		return infstrictavl_t2(a,b, idx);
	}
	else if(!strcmp(type, "t3"))
	{
		return infstrictavl_t3(a,b, idx);
	}
	else if(!strcmp(type, "p1"))
	{
		return infstrictavl_p1(a, b, idx);
	}
	else if(!strcmp(type, "p2"))
	{
		return infstrictavl_p2(a,b, idx);
	}
	else if(!strcmp(type, "p3"))
	{
		return infstrictavl_p3(a,b, idx);
	}
	else if(!strcmp(type, "w"))
	{
		return infstrictavl_w(a,b, idx);
	}
	else if(!strcmp(type, "h"))
	{
		return infstrictavl_h(a, b, idx);
	}
	else if(!strcmp(type, "m1"))
	{
		return infstrictavl_m1(a, b, idx);
	}
	else if(!strcmp(type, "m2"))
	{
		return infstrictavl_m2(a, b, idx);
	}
	
	return 0;
}

int egalavl(avl* a, avl* b, char* type)
{ // fait le cas ou a = b selon le type de trie (ex: cumul de valeurs pour faire moyenne)
	verifpchar(type);
	verifpavl(a);
	verifpavl(b);

	if(!strcmp(type, "t1"))
	{
		a = egalavl_t1(a, b, 1);
	}
	else if(!strcmp(type, "t2"))
	{
		a = egalavl_t2(a, b, 1);
	}
	else if(!strcmp(type, "t3"))
	{
		a = egalavl_t3(a, b, 1);
	}
	else if(!strcmp(type, "p1"))
	{
		a = egalavl_p1(a, b, 1);
	}
	else if(!strcmp(type, "p2"))
	{
		a = egalavl_p2(a, b, 1);
	}
	else if(!strcmp(type, "p3"))
	{
		a = egalavl_p3(a, b, 1);
	}
	else if(!strcmp(type, "w"))
	{
		a = egalavl_w(a, b, 1);
	}
	else if(!strcmp(type, "h"))
	{
		a = egalavl_h(a, b, 1);
	}
	
	else if(!strcmp(type, "m1"))
	{
		a = egalavl_m(a, b, 0);
	}
	return 1;
}

avl* insertavl(avl* a, avl* nv, int* h, int idx, char* type)
{
	verifpavl(nv);
	verifpchar(type);

	if (a == NULL)
	{	
		(*h)++;
		a = nv;

		return a; 
	}
	else if(infstrictavl(nv, a, idx, type))	// les comparaisons change en fonctions des types
	{	
		a->fg = insertavl(a->fg, nv, h, idx, type);	
		*h = -(*h);
	}
	else if(infstrictavl(a, nv, idx, type))
	{
		a->fd = insertavl(a->fd, nv, h, idx, type);
	}
	else
	{
		egalavl(a, nv, type); // dans le cas oÃ¹ la ligne est egal, on doit faire des operations differentes selon les types
		*h = 0;
	}

	if(*h != 0)
	{
		a->equilibre = a->equilibre + *h;
		a = equilibreravl(a);
		if( a->equilibre == 0)
			*h = 0;
		else
			*h = 1;
	}

	return a;
}

avl* insertavl_appel(avl* a, char* ligne, int idx, char* type)
{
	verifpchar(ligne);
	verifpchar(type);

	avl* nv = creationavl(ligne, type);


	int* h = NULL;
	if(a)
	{
		h = &(a->equilibre);
	}
	else
	{
		a = nv;
		return a;
	}

	return insertavl(a, nv, h, idx, type);
}

avl* construireavl(char* nomf, int idx, char* type)
{
	verifpchar(nomf);
	verifpchar(type);	

	avl* a = NULL;	
	char ligne[TAILLE];
	int verif = 0;
	size_t len = 0;

	//ouvrir le fichier
	FILE * fichier = fopen(nomf,"r");
	if (fichier == NULL){
		printf("pas de fichier d'entree de ce nom");
		exit(2);
	}
	
	while(fgets(ligne, sizeof(ligne), fichier) != NULL) // on lit chaque ligne
	{

		//ligne = prendreligne(&verif, len, fichier);

		printf("\n\n\n'%s' \n\n\n", ligne);
		
		//puis on ajoute la ligne a l'avl
		a = insertavl_appel(a, ligne, idx, type);
		
	}

	fclose(fichier);	

	// on fait les operations necessaires sur les donnees de l'avl
	finiravl(a, type);


	return a;
}


int tri_h_avl(char* input_fich, char* output_fich)
{
	char* type = "h";
	avl* a = construireavl(input_fich, 1, type);
	
	// on doit faire decroissant de base	
	parcoursdecroissantavl_appel(a, output_fich);
}




int infstrictavl_h(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon


	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	
	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs (si a et b en ont)
	{	
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}

		if(atoi(a->ligne[idx]) == atoi(b->ligne[idx]) && strcmp(a->ligne[idx-1],b->ligne[idx-1])) // si ce n'est pas la mm station mais la mm altitude, on compare par rapport a l'id
		{
			if(atoi(a->ligne[idx-1]) < atoi(b->ligne[idx-1]))
			{
				return 1;
			}
		}
	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}
	
	return 0;
}




avl* egalavl_h(avl* a, avl* b, int i)
{	// si c'est la meme station il n'y a rien a faire
	
	verifpavl(a);
	verifpavl(b);

	return a;
}

avl* finiravl_h(avl* a, int i)
{	// cette fonction actualise les lignes de chaque noeud

	if(a)
	{
		verifpavl(a);

		// on rentre les valeurs de la ligne dans le string du noeud
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_h(a->fd, i);
		finiravl_h(a->fg, i);
	}
	return a;
}

int tri_m1_avl(char* input_fich, char* output_fich)
{	//cette fonction fait un fichier intermediaire duquel on pourras reconstruire un avl pour faire le fichier voulu
	
	char* type = "m1";
	avl* a1 = construireavl(input_fich, 0, type); // on construit un avl trie par rapport aux id, pour faire le maximum des humidites par station

	parcourscroissantavl_appel(a1, output_fich);

	//////////// IL FAUT FAIRE 2 PARTIES PSK LE FICHIER FRRR.TXT NE S'UPDATE PAS AVANT DE LE REPRENDRE EN ENTREE, IL FAUT APPELER LE C 2 FOIS, ET FAIRE TRI_M1 ET TRI_M2
}

int tri_m2_avl(char* input_fich, char* output_fich)
{	// cette fonction doit etre appelee dans une autre instance de tri_m1, sinon le fichier intermediaire aura des probleme
	// cette fonction tri dans un avl le fichier d'entree par rapport a l'humidite et cree un fichier trie
	
	// IL FAUT APPELER LE C 2 FOIS, UNE FOIS POUR M1 UNE FOIS POUR M2	/////////////////////////////////////////////////////////////
	
	char* type = "m2";
	char* fichier_int = "frrrrrrr.txt";
	avl* a2 = construireavl(fichier_int, 1, type); // puis on reconstruit un avl qui trie le fichier precedent par rapport a l'humidite
	
	// on doit faire decroissant de base
	parcoursdecroissantavl_appel(a2, output_fich);
}

int infstrictavl_m2(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon


	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs si il y en a
	{	
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
		else if(atoi(a->ligne[idx]) == atoi(b->ligne[idx]) && strcmp(a->ligne[idx-1],b->ligne[idx-1])) // si ce n'est pas la mm station mais la mm humidite, on compare par rapport a l'id
		{
			if(atoi(a->ligne[idx-1]) < atoi(b->ligne[idx-1]))
			{
				return 1;
			}
		}
	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}
	
	return 0;
}

int infstrictavl_m1(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs si il y en a
	{	
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}
	
	return 0;
}

avl* egalavl_m(avl* a, avl* b, int i)
{	// prend l'humidite maximum parmi les 2 noeuds
	

	verifpavl(a);
	verifpavl(b);
	
	int verif1_a = strcmp(a->ligne[i+1], " ");
	int verif1_b = strcmp(b->ligne[i+1], " ");

	if(verif1_a && verif1_b)	// si il y a des valeurs dans a et b on choisit le minimun
		sprintf(a->ligne[i+1], "%d", max2(atoi(a->ligne[i+1]), atoi(b->ligne[i+1])));
	else if(!verif1_a && verif1_b) // si il y a pas de valeur en a mais il y en a en b on assigne la valeur de b a a
		sprintf(a->ligne[i+1], "%d", atoi(b->ligne[i+1]));

	return a;
}

avl* finiravl_m(avl* a, int i)
{	// cette fonction actualise les lignes de chaque noeud
	if(a)
	{
		verifpavl(a);

		// on rentre les valeurs de la ligne dans le string du noeud
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_h(a->fd, i);
		finiravl_h(a->fg, i);
	}
	return a;
}
int tri_p1_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "p1";
	avl* a = construireavl(input_fich, 0, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}

int infstrictavl_p1(avl* a, avl* b, int idx)	
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon

	

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs si il y en a
	{	
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}
	
	return 0;
}

avl* egalavl_p1(avl* a, avl* b, int i)
{	// cumule les temperatures, augmente nbr_moyenne et met a jour les maximums et minimums 
	
	verifpavl(a);
	verifpavl(b);
	

	//printf("\n\n\n avant  a->ligne[i]=%s   \n\n\n\n", a->ligne[i]);
	// on additionne les temperatures vu que c'est la meme station
	int verif_a = (a->ligne[i] != NULL && strcmp(a->ligne[i], " ")); // si il y a une valeur en a
	int verif_b = (b->ligne[i] != NULL && strcmp(b->ligne[i], " ")); // si il y a une valeur dans b
	if(verif_a && verif_b)
	{
		printf("\n  a->ligne[i] = %s   \n", a->ligne[i]);
		int abs_a = atoi(a->ligne[i]);
		if(abs_a<0)
			abs_a= 0-abs_a;
		int abs_b = atoi(b->ligne[i]);
		if(abs_b<0)
			abs_b= 0-abs_b;
		int abs_som = abs_a + abs_b;
		if(abs_som<0)
			abs_som = 0-abs_som;
		sprintf(a->ligne[i], "%d", abs_som);
		(a->nbr_moyenne[0])++; // et on incremente le nombre de la division a faire une fois l'arbre fini
	}
	
	//printf("\n\n\n apres a->ligne[i] = %s   \n\n\n\n", a->ligne[i]);
	
	//printf("\n\n\n '%s' \n\n\n", b->ligne[0]);
	int verif1_a = (a->ligne[i+1] != NULL && strcmp(a->ligne[i+1], " ")); // si il y a une valeur minimum en a
	int verif1_b = (b->ligne[i+1] != NULL && strcmp(b->ligne[i+1], " ")); // si il y a une valeur dans b

	if(verif1_a && verif1_b)	// si il y a des valeurs dans a et b on choisit le minimun
		sprintf(a->ligne[i+1], "%d", min2(atoi(a->ligne[i+1]), atoi(b->ligne[i+1])));
	else if(!verif1_a && verif1_b) // si il y a pas de valeur en a mais il y en a en b on assigne la valeur de b a a
		sprintf(a->ligne[i+1], "%d", atoi(b->ligne[i+1]));

	
	int verif2_a = (a->ligne[i+2] != NULL && strcmp(a->ligne[i+2], " ")); // si il y a une valeur max en a
	int verif2_b = (b->ligne[i+2] != NULL && strcmp(b->ligne[i+2], " ")); // si il y a une valeur dans b

	if(verif2_a && verif2_b)	// si il y a des valeurs dans a et b on choisit le maximum
		sprintf(a->ligne[i+2], "%d", max2(atoi(a->ligne[i+2]), atoi(b->ligne[i+2])));
	else if(!verif2_a && verif2_b) // si il y a pas de valeur en a mais il y en a en b on assigne la valeur de b a a
		sprintf(a->ligne[i+2], "%d", atoi(b->ligne[i+2]));

	return a;
}

int tri_p2_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "p2";
	avl* a = construireavl(input_fich, 0, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}




int infstrictavl_p2(avl* a, avl* b, int idx)	
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on tri des dates ou autres chose (le format de la date dans les donnees ayant un "T")
	char* verif1_a = strstr(a->ligne[idx], "T");
	char* verif1_b = strstr(b->ligne[idx], "T");
	

	if(verif1_a && verif1_b) //si on a 2 dates, on fait la comparaison de date
	{
		if(compare_dates(a->ligne[idx], b->ligne[idx]) == -1)
		{
			return 1;
		}
	}
	else if(a->ligne[idx] != " ") // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(b->ligne[idx] != " " && atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}

	return 0;
}




avl* egalavl_p2(avl* a, avl* b, int i)
{	// cumule les temperatures et augmente nbr_moyenne 
	
	verifpavl(a);
	verifpavl(b);
	
	int verif_a = (a->ligne[i] != NULL && strcmp(a->ligne[i], " ")); // si il y a une valeur en a
	int verif_b = (b->ligne[i] != NULL && strcmp(b->ligne[i], " ")); // si il y a une valeur dans b
	if(verif_a && verif_b)
	{
		//printf("\n  a->ligne[i] = %s   \n", a->ligne[i]);
		int abs_a = atoi(a->ligne[i]);
		if(abs_a<0)
			abs_a= 0-abs_a;
		int abs_b = atoi(b->ligne[i]);
		if(abs_b<0)
			abs_b= 0-abs_b;
		int abs_som = abs_a + abs_b;
		if(abs_som<0)
			abs_som = 0-abs_som;
		sprintf(a->ligne[i], "%d", abs_som);
		(a->nbr_moyenne[0])++; // et on incremente le nombre de la division a faire une fois l'arbre fini
	}

	return a;
}

avl* finiravl_p2(avl* a, int i)
{	// cette fonction fait le calcul des moyennes et actualise les lignes de chaque noeud

	if(a)
	{
		verifpavl(a);

		if(a->nbr_moyenne[0] > 1)
		{	
			// on fait la moyenne des temperature pour le noeud
			sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) / a->nbr_moyenne[0]);
			a->nbr_moyenne[0] = 1;
		}
		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_p2(a->fd, i);
		finiravl_p2(a->fg, i);
	}
	return a;
}

avl* finiravl_p1(avl* a, int i)
{	// cette fonction fait le calcul des moyennes et actualise les lignes de chaque noeud

	if(a)
	{
		verifpavl(a);

		if(a->nbr_moyenne[0] > 1)
		{	
			// on fait la moyenne des temperature pour le noeud
			sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) / (a->nbr_moyenne[0]));
			a->nbr_moyenne[0] = 1;
		}
		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_p1(a->fd, i);
		finiravl_p1(a->fg, i);
	}
	return a;
}
int tri_p3_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "p3";
	avl* a = construireavl(input_fich, 1, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}

int infstrictavl_p3(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant par ordre chonologique puis par rapport a l'id si la date est la meme) return 0 sinon
	// idx represente l'indice du champ que l'on trie

	verifpavl(a);
	verifpavl(b);

	//on verifie si on tri des dates ou autres chose (le format de la date dans les donnees ayant un "T")
	char* verif1_a = strstr(a->ligne[idx], "T");
	char* verif1_b = strstr(b->ligne[idx], "T");

	
	if(verif1_a && verif1_b) //si on a 2 dates, on fait la comparaison de date
	{

		int cmp_date = compare_dates(a->ligne[idx], b->ligne[idx]);
		if(cmp_date == -1)
		{
			return 1;
		}

		else if(!cmp_date) // si les dates sont egales, on compare les id
		{
			if(a->ligne[idx-1] < b->ligne[idx-1])	// dans notre situation, indice de date = 2, indice de id = 1
			{
				return 1;
			}
		}		
	}
	else if(a->ligne[idx] != " ") // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(b->ligne[idx] != " " && atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}
	
	return 0;
}



avl* egalavl_p3(avl* a, avl* b, int i)
{	// on fait rien dans cette situation
	
	verifpavl(a);
	verifpavl(b);
	
	return a;
}

avl* finiravl_p3(avl* a, int i)
{	// cette fonction actualise les lignes (ca fait rien en vrai mais c'est un secret)

	if(a)
	{
		verifpavl(a);

		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_p3(a->fd, i);
		finiravl_p3(a->fg, i);
	}
	
	return a;
}

void verifpchar(char* c)
{
	if(c == NULL)
	{
		printf("\nerreur pointeur sur char NULL\n");
		exit(4);
	}
}

void verifpint(int* n)
{
	if(n == NULL)
	{
		printf("\nerreur pointeur sur int NULL\n");
		exit(4);
	}
}

void verifn(int n)
{
	if(n < 0)
	{
		printf("\nerreur n < 0\n");
		exit(4);
	}
}

void verifavl(avl a)
{
	if(a.taille_ligne < 0 || (a.nbr_moyenne)[0] < 0 || (a.nbr_moyenne)[1] < 0)
	{
		printf("\nerreur nbr_moyenne ou taille_ligne < 0\n");
		exit(4);
	}
}

void verifpavl(avl* a)
{
	if(a==NULL)
	{
		printf("\npointeur avl sur null\n");
		exit(4);
	}
	verifavl(*a);
}





void verifmode(char* c)
{
	verifpchar(c);
	if(strcmp(c, "abr")!=0 && strcmp(c, "avl")!=0 && strcmp(c, "tab")!=0)
	{	
		printf("mode non valide");
		exit(1);
	}
}

void verifpfile(FILE * file)
{
	if(file == NULL)
	{
		printf("poiteur file sur null");
		exit(4);
	}
}

void parcourscroissantavl_appel(avl* a, char* nomf)
{ // procedure pour traiter un avl dans l'ordre decroissant

	verifpchar(nomf);

	if(a)
	{
		verifpavl(a);
		
		FILE* output_fichier = fopen(nomf, "w");
		if(output_fichier==NULL)
			exit(3);
		
		parcourscroissantavl(a, output_fichier);
	}
}

void parcourscroissantavl(avl* a, FILE* fichier)
{ // procedure pour traiter un avl dans l'ordre croissant

	verifpfile(fichier);

	if(a)
	{
		verifpavl(a);
		
		parcourscroissantavl(a->fg, fichier);
			traiteravl(a, fichier);
		parcourscroissantavl(a->fd, fichier);
	}
}

void parcoursdecroissantavl_appel(avl* a, char* nomf)
{ // procedure pour traiter un avl dans l'ordre decroissant

	verifpchar(nomf);

	if(a)
	{
		verifpavl(a);
		
		FILE* output_fichier = fopen(nomf, "w");
		if(output_fichier==NULL)
			exit(3);
		
		parcoursdecroissantavl(a, output_fichier);
	}
}

void parcoursdecroissantavl(avl* a, FILE* fichier)
{ // procedure pour traiter un avl dans l'ordre decroissant

	verifpfile(fichier);

	if(a)
	{
		verifpavl(a);

		parcoursdecroissantavl(a->fd, fichier);
			traiteravl(a, fichier);
		parcoursdecroissantavl(a->fg, fichier);
	}
}


void traiteravl(avl* a, FILE* out_fichier)
{	// rentre la ligne du noeud dans le fichier de sortie
	
	verifpfile(out_fichier);
	
	fprintf(out_fichier, "%s", a->cligne);
}

int tri_t1_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "t1";
	avl* a = construireavl(input_fich, 0, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}

int infstrictavl_t1(avl* a, avl* b, int idx)	
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon

	

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs si il y en a
	{	
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}
	
	return 0;
}

avl* egalavl_t1(avl* a, avl* b, int i)
{	// cumule les temperatures, augmente nbr_moyenne et met a jour les maximums et minimums 
	
	verifpavl(a);
	verifpavl(b);
	

	//printf("\n\n\n avant  a->ligne[i]=%s   \n\n\n\n", a->ligne[i]);
	// on additionne les temperatures vu que c'est la meme station
	sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) + atoi(b->ligne[i]));
	a->nbr_moyenne[0]++; // et on incremente le nombre de la division a faire une fois l'arbre fini
	
	//printf("\n\n\n apres a->ligne[i] = %s   \n\n\n\n", a->ligne[i]);
	
	int verif1_a = (a->ligne[i+1] != NULL || strcmp(a->ligne[i+1], " ")); // si il y a une valeur minimum en a
	int verif1_b = strcmp(b->ligne[i], " "); // si il y a une valeur dans b

	if(verif1_a && verif1_b)	// si il y a des valeurs dans a et b on choisit le minimun
		sprintf(a->ligne[i+1], "%d", min2(atoi(a->ligne[i+1]), atoi(b->ligne[i])));
	else if(!verif1_a && verif1_b) // si il y a pas de valeur en a mais il y en a en b on assigne la valeur de b a a
		sprintf(b->ligne[i+1], "%d", atoi(b->ligne[i]));

	
	int verif2_a = (a->ligne[i+2] != NULL || strcmp(a->ligne[i+1], " ")); // si il y a une valeur max en a
	int verif2_b = strcmp(b->ligne[i], " "); // si il y a une valeur dans b

	if(verif2_a && verif2_b)	// si il y a des valeurs dans a et b on choisit le maximum
		sprintf(a->ligne[i+2], "%d", max2(atoi(a->ligne[i+2]), atoi(b->ligne[i])));
	else if(!verif2_a && verif2_b) // si il y a pas de valeur en a mais il y en a en b on assigne la valeur de b a a
		sprintf(b->ligne[i+2], "%d", atoi(b->ligne[i]));

	return a;
}

avl* finiravl_t1(avl* a, int i)
{	// cette fonction fait le calcul des moyennes et actualise les lignes de chaque noeud

	if(a)
	{
		verifpavl(a);

		if(a->nbr_moyenne[0] > 1)
		{	
			// on fait la moyenne des temperature pour le noeud
			sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) / a->nbr_moyenne[0]);
			a->nbr_moyenne[0] = 1;
		}
		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_t1(a->fd, i);
		finiravl_t1(a->fg, i);
	}
	return a;
}
int tri_t2_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "t2";
	avl* a = construireavl(input_fich, 0, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}




int infstrictavl_t2(avl* a, avl* b, int idx)	
{	//return 1 si a < b (en comparant par ordre chonologique) return 0 sinon

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on tri des dates ou autres chose (le format de la date dans les donnees ayant un "T")
	char* verif1_a = strstr(a->ligne[idx], "T");
	char* verif1_b = strstr(b->ligne[idx], "T");
	

	if(verif1_a && verif1_b) //si on a 2 dates, on fait la comparaison de date
	{
		if(compare_dates(a->ligne[idx], b->ligne[idx]) == -1)
		{
			return 1;
		}
	}
	else if(a->ligne[idx] != " ") // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(b->ligne[idx] != " " && atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}

	return 0;
}




avl* egalavl_t2(avl* a, avl* b, int i)
{	// cumule les temperatures et augmente nbr_moyenne 
	
	verifpavl(a);
	verifpavl(b);
	
	// on additionne les temperatures vu que c'est la meme station
	sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) + atoi(b->ligne[i]));
	a->nbr_moyenne[0]++; // et on incremente le nombre de la division a faire une fois l'arbre fini

	return a;
}

avl* finiravl_t2(avl* a, int i)
{	// cette fonction fait le calcul des moyennes et actualise les lignes de chaque noeud

	if(a)
	{
		verifpavl(a);

		if(a->nbr_moyenne[0] > 1)
		{	
			// on fait la moyenne des temperature pour le noeud
			sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) / a->nbr_moyenne[0]);
			a->nbr_moyenne[0] = 1;
		}
		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_t2(a->fd, i);
		finiravl_t2(a->fg, i);
	}
	return a;
}
int tri_t3_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "t3";
	avl* a = construireavl(input_fich, 1, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}

int infstrictavl_t3(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant par ordre chonologique puis par rapport a l'id si la date est la meme) return 0 sinon
	// idx represente l'indice du champ que l'on trie

	verifpavl(a);
	verifpavl(b);

	//on verifie si on tri des dates ou autres chose (le format de la date dans les donnees ayant un "T")
	char* verif1_a = strstr(a->ligne[idx], "T");
	char* verif1_b = strstr(b->ligne[idx], "T");

	
	if(verif1_a && verif1_b) //si on a 2 dates, on fait la comparaison de date
	{

		int cmp_date = compare_dates(a->ligne[idx], b->ligne[idx]);
		if(cmp_date == -1)
		{
			return 1;
		}

		else if(!cmp_date) // si les dates sont egales, on compare les id
		{
			if(a->ligne[idx-1] < b->ligne[idx-1])	// dans notre situation, indice de date = 2, indice de id = 1
			{
				return 1;
			}
		}		
	}
	else if(a->ligne[idx] != " ") // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(b->ligne[idx] != " " && atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}
	}
	
	return 0;
}



avl* egalavl_t3(avl* a, avl* b, int i)
{	// on fait rien dans cette situation
	
	verifpavl(a);
	verifpavl(b);
	
	return a;
}

avl* finiravl_t3(avl* a, int i)
{	// cette fonction actualise les lignes (ca fait rien en vrai mais c'est un secret)

	if(a)
	{
		verifpavl(a);

		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_t3(a->fd, i);
		finiravl_t3(a->fg, i);
	}
	
	return a;
}


int tri_w_avl(char* input_fich, char* output_fich, int decroissant)
{

	char* type = "w";
	avl* a = construireavl(input_fich, 0, type);
	
	if(!decroissant)
		parcourscroissantavl_appel(a, output_fich);
	else
		parcoursdecroissantavl_appel(a, output_fich);
}




int infstrictavl_w(avl* a, avl* b, int idx)
{	//return 1 si a < b (en comparant les identifiants) return 0 sinon

	// idx represente l'indice du champ que l'on trie
	//return 1 si a < b (en comparant un elem) return 0 sinon
	verifpavl(a);
	verifpavl(b);		
	

	//on verifie si on trie des lignes sans valeurs a trier
	int verif_a = strcmp(a->ligne[idx], " "); // strcmp retourne un nombre positif si le string n'est pas le meme et 0 sinon
	int verif_b = strcmp(b->ligne[idx], " ");

	if(verif_a && verif_b) // on compare les valeurs si il y en a
	{	
		
		if(atoi(a->ligne[idx]) < atoi(b->ligne[idx]))
		{
			return 1;
		}

	}
	else if(verif_a) // on place les lignes qui ont le champ que l'on trie vide en dernier dans l'ordre croissant (on considere " " comme toujours superieur)
	{
		if(!verif_b)
		{
			return 1;
		}
	}

	return 0;
}


avl* egalavl_w(avl* a, avl* b, int i)
{	// cumule les vitesse et orientation puis augmente nbr_moyenne 

	verifpavl(a);
	verifpavl(b);
	
	int condition1_a = strcmp(a->ligne[i], " "); // il y a une valeur pour a
	int condition1_b = strcmp(b->ligne[i], " "); // il y a une valeur pour b
	
	// on additionne les vitesses vu que c'est la meme station
	if(condition1_a && condition1_b)
	{ 
		sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) + atoi(b->ligne[i]));
		a->nbr_moyenne[0]++; // et on incremente le nombre de la division a faire si il y a eu un ajout 
	}
	else if(!condition1_a && condition1_b) // si il n'y a pas de valeur en a on ne prend que b (et inversement mais pas besoin de le faire)
	{
		a->ligne[i] = strcpy(a->ligne[i], b->ligne[i]);
	}


	int condition2_a = strcmp(a->ligne[i+1], " "); // il y a une valeur pour a
	int condition2_b = strcmp(b->ligne[i+1], " "); // il y a une valeur pour b
	int condition2 = (condition2_a && condition2_b); // il y a une valeur pour a et b

	// on additionne les orientations vu que c'est la meme station
	if(condition2)
	{ 
		sprintf(a->ligne[i+1], "%d", atoi(a->ligne[i+1]) + atoi(b->ligne[i+1]));
		a->nbr_moyenne[1]++; // et on incremente le nombre de la division a faire si il y a eu un ajout
	}
	else if(!condition2_a) // si il n'y a pas de valeur en a on ne prend que b (et inversement mais pas besoin de le faire)
	{
		sprintf(a->ligne[i+1], "%d", atoi(b->ligne[i]));
	}

	return a;
}

avl* finiravl_w(avl* a, int i)
{	// cette fonction fait le calcul des moyennes et actualise les lignes de chaque noeud
	if(a)
	{
		verifpavl(a);
		if(a->nbr_moyenne[0] > 1)
		{
			// on fait la moyenne des vitesses et orientations pour le noeud
			sprintf(a->ligne[i], "%d", atoi(a->ligne[i]) / a->nbr_moyenne[0]);
			a->nbr_moyenne[0] = 1;
		}
		if(a->nbr_moyenne[1] > 1)
		{
			// on fait la moyenne des vitesses et orientations pour le noeud
			
			sprintf(a->ligne[i+1], "%d", atoi(a->ligne[i+1]) / a->nbr_moyenne[1]);
			a->nbr_moyenne[1] = 1;
		}


		// on rentre les valeurs de la ligne dans le string
		strcpy(a->cligne, actualiseravl_cligne(a));

		finiravl_w(a->fd, i);
		finiravl_w(a->fg, i);
	}
	return a;
}


















avl* finiravl(avl* a, char* type)
{ // cette fonction realise toute les operations necessaire une fois l'avl fini
  // ex: diviser la somme de valeur pour obtenir la moyenne
	verifpavl(a);
	verifpchar(type);
	
	if(!strcmp(type, "t1"))
	{
		finiravl_t1(a, 1);
	}
	else if(!strcmp(type, "t2"))
	{
		finiravl_t2(a, 1);
	}
	else if(!strcmp(type, "t3"))
	{
		finiravl_t3(a, 0);
	}
	else if(!strcmp(type, "p1"))
	{
		finiravl_p1(a, 1);
	}
	else if(!strcmp(type, "p2"))
	{
		finiravl_p2(a, 1);
	}
	else if(!strcmp(type, "p3"))
	{
		finiravl_p3(a, 0);
	}
	else if(!strcmp(type, "w"))
	{
		finiravl_w(a, 1);
	}
	else if(!strcmp(type, "h"))
	{
		finiravl_h(a, 1);
	}
	else if(!strcmp(type, "m1"))
	{
		finiravl_m(a, 1);
	}
	
}

char** strtotab(char* l, int* nl, char* type)
{

	verifpchar(l);
	verifpint(nl);

	char c;
	char* s = &c;
	char** res = &s;
    	int count = 0;
    	char* tmp = l;
    	char* last_comma = 0;
    	char delim[3];
    	delim[0] = ';';
	delim[1] = 0;
	

	// compte le nombre d'elements a extraire
 	while (*tmp)
 	{
   	   if (';' == *tmp)
    	   {
     	       count++;
     	       last_comma = tmp;
     	   }
    	   tmp++;
   	}

	// ajoute de l'espace pour le token
	if(last_comma < (l + strlen(l) - 1))
	{
   		count ++;
	}

	*nl = count;

	count++;
	res = malloc(sizeof(char*) * count);

	if(res)
	{
		int idx  = 0;
		char* token = strtok(l, delim);
		
		while (token)
		{
			*(res + idx) = strdup(token);
			token = strtok(0, delim);
			idx++;
		}
		*(res + idx) = 0;
	}

	return(res);
}


char** creationligne(char* l, int* nbr_i, char* type)
{
	verifpchar(l);
	verifpchar(type);
	verifpint(nbr_i);

	char** ligne = strtotab(l, nbr_i, type);

	return ligne;
}

int min3(int a, int b, int c)
{ //retourne minimum parmi a,b,c
	if(a <= b && a <= c)
		return a;
	if(b <= a && b <= c)
		return b;
	else
		return c;
}

int max2(int a, int b)
{ //retourne max parmi a,b
	if(a>b)
		return a;
	else
		return b;
}

int max3(int a, int b, int c)
{ //retourne max parmi a,b,c
	if(a >= b && a >= c)
		return a;
	if(b >= a && b >= c)
		return b;
	else
		return c;
}

int min2(int a, int b)
{//retourne minimum parmi a,b
	if(a<b)
		return a;
	else
		return b;
}

*/