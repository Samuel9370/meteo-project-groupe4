#!/bin/bash
#verifie l executable C qu'il existe
#command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,2,7 > stationid.txt`
#echo $command

#idstation 1
#date 2
#pression 3 7 8
#temp 11 12 13
for file in *.txt; do
  [ "$file" == "README.txt" ] && continue
  rm "$file"
done

rm *.png
rm *.o

for argument in $@ # on verifie -r et les filtrages
do
case $argument in
    "--help")
echo "Pour faire fonctionner le script, vous devez obligatoirement avoir un -f suivis du nom du fichier contenant les données triés ( ex: -f data.csv ) et un -o suivis du nom du fichier de sortie. Vous devez aussi obligatoirement entrer au moins une option selon votre/vos choix parmis -h ( pour avoir une carte interpolé et coloré de l'altitude), -m ( pour avoir la même chose mais de l'humidité), -w (pour avoir un diagramme de type vecteur) et -p ou -t suivis de 1,2 ou 3 (ex:-p3)( t pour température et p pour pression, 1 pour avoir un diagramme de type barres d'erreur, 2 pour un diagramme de type ligne simple et 3 pour un diagramme de type multi-ligne). En option vous pouvez préciser une zone en ajoutant - suivis de A pour Antilles, G pour Guyane, F pour France, S pour St-Pierre et Miquellon, O pour Océan Indien et Q pour l'océan Antartique. Si vous voulez trier par date ajouter -d suivis de la date minimum et maximum au format AAAA-MM-DD( par exemple -d 2010-01-01 2010-05-06). L'ordre de tout ces options n'est pas important mais ne doivent pas être collée (ex: -A-p1 n'est pas bon mais -A -p1 l'est). Si rien ou --avl est ajouté, les données seront triés en AVL, si vous voulez trier les données en ABR ou en tableau, ajoutez --abr ou --tab. Pour trier les donner de façon décroissante, ajoutez -r."



    esac
done

declare -i plusieurs_modes=0
for argument in $@ # on verifie -r et les filtrages
do
case $argument in
    "-t1")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-t2")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-t3")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-p1")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-p2")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-p3")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-h")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-w")
        plusieurs_modes=plusieurs_modes+1
    ;;
    "-m")
        plusieurs_modes=plusieurs_modes+1
    ;;
    esac
done


for ((j=0; j<plusieurs_modes; j++)); do
date_bool=0 # booleen si 0 l'utilsateur n'a pas choisie de date si == 1 alors ya une date
nb_arg=$#
arr=($@)
for ((i=0; i<nb_arg; i++)); do
   
   if [[ ${arr[i]} == "-d" ]]; then
        if [ $date_bool == 1 ]; then
            echo "trop d'arguments de dates on sort du programme"
            exit 
        else
            date_bool=1
            date_min=${arr[i+1]} # on recup la date minimun
            date_max=${arr[i+2]} #date max 
            if [[ $date_min > $date_max ]]; then 
                echo "la date minimum est plus grande que la maximum veuillez recommencer"
                exit
            fi

            # on aura besoins des dates min et max afin de filtré plus tard dans le code
        fi 
    fi
   
    
done

lieu=0
decroissant=0
mode_trie="avl" #de base on trie en avl

fichier=0
mode=0
for argument in $@ # on verifie -r et les filtrages
do
case $argument in
    "-r")
        decroissant=1
    ;;
    "--abr")
        mode_trie="abr"
    ;;
    "--tab")
        mode_trie="tab"
    ;;
    "--avl")
        mode_trie="avl"
    ;;
    esac
done
min_lattitude=0
max_lattitude=0
min_longitude=0
max_longitude=0
for argument in $@ # on verifie si ya un lieu qui a été précisé
do
case $argument in
    "-F")
        if [ $lieu == 0 ];
        then
            lieu="France"
            min_lattitude=40
            max_lattitude=52
            min_longitude=-10
            max_longitude=12
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    "-A")
        if [ $lieu == 0 ];
        then
            lieu="Antille"
            min_lattitude=12
            max_lattitude=19
            min_longitude=-65
            max_longitude=-58
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    "-G")
        if [ $lieu == 0 ];
        then
            lieu="Guyane"
            min_lattitude=-1
            max_lattitude=10
            min_longitude=-58
            max_longitude=-48
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    "-S")
        if [ $lieu == 0 ];
        then
            lieu="Saint_pierre_et_miquelon"
            min_lattitude=44
            max_lattitude=53
            min_longitude=-64
            max_longitude=-49
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    "-O")
        if [ $lieu == 0 ];
        then
            lieu="Ocean_indien"
            min_lattitude=-60
            max_lattitude=3
            min_longitude=26
            max_longitude=100
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    "-Q")
        if [ $lieu == 0 ];
        then
            lieu="Antartique"
            min_lattitude=40
            max_lattitude=52
            min_longitude=-10
            max_longitude=12
        else
            echo "erreur trop d'argument de lieu"
            exit
        fi
    ;;
    esac
done

 

for argument in $@ #for qui lit tous les arguments passé par l'utilisateur
do

    case $argument in # switch

        "-t1") # on doit mtn vérifié si on prends en compte le parametre date (-d min max)
                #on regarde si on doit filtré par date ou par un lieu précisé par l'utilisateur
                mode="-t1"
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $11 ";" $2}' > tempmode1_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$3 >= date_min && $3 <= date_max' tempmode1_no_filtrage.txt > tempmode1_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="tempmode1_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $11 ";" $10}' > tempmode1_no_filtrage.txt
                        
                        cat tempmode1_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > tempmode1_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="tempmode1_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,11 > tempmode1_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="tempmode1_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $11 ";" $10 ";" $2}' > tempmode1_no_filtrage.txt
                    cat tempmode1_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > tempmode1_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' tempmode1_need_date_filter.txt > tempmode1_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="tempmode1_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi
        ;;
        "-t2")
            mode="-t2"
          #  command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 2,11 > tempmode2_no_triage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $11}' > tempmode2_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$1 >= date_min && $1 <= date_max' tempmode2_no_filtrage.txt > tempmode2_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="tempmode2_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $11 ";" $10}' > tempmode2_no_filtrage.txt
                        
                        cat tempmode2_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > tempmode2_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="tempmode2_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 2,11 > tempmode2_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="tempmode2_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $11 ";" $10}' > tempmode2_no_filtrage.txt
                    cat tempmode2_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > tempmode2_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$1 >= date_min && $1 <= date_max' tempmode2_need_date_filter.txt > tempmode2_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="tempmode2_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi

        ;;
        "-t3")
            mode="-t3"
            #command=`cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $11}' > tempmode3_no_triage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $11}' > tempmode3_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$2 >= date_min && $2 <= date_max' tempmode3_no_filtrage.txt > tempmode3_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="tempmode3_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $11 ";" $10}' > tempmode3_no_filtrage.txt
                        
                        cat tempmode3_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > tempmode3_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="tempmode3_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,2,11 > tempmode3_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="tempmode3_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $11 ";" $10}' > tempmode3_no_filtrage.txt
                    cat tempmode3_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > tempmode3_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$2 >= date_min && $2 <= date_max' tempmode3_need_date_filter.txt > tempmode3_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="tempmode3_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi
        ;;
        "-p1")
            mode="-p1"
            #command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,7 > pressmode1_no_filtrage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $7 ";" $2}' > press1_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$3 >= date_min && $3 <= date_max' press1_no_filtrage.txt > press1_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="press1_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $7 ";" $10}' > press1_no_filtrage.txt
                        
                        cat press1_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > press1_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="press1_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,7 > press1_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="press1_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $7 ";" $10 ";" $2}' > press1_no_filtrage.txt
                    cat press1_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > press1_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' press1_need_date_filter.txt > press1_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="press1_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi
        ;;
        "-p3")
            mode="-p3"
            #command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,2,7 > pressmode3_no_triage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $7}' > press3_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$2 >= date_min && $2 <= date_max' press3_no_filtrage.txt > press3_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="press3_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $7 ";" $10}' > press3_no_filtrage.txt
                        
                        cat press3_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > press3_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="press3_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 1,2,7 > press3_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="press3_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $2 ";" $7 ";" $10}' > press3_no_filtrage.txt
                    cat press3_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > press3_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$2 >= date_min && $2 <= date_max' press3_need_date_filter.txt > press3_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="press3_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi
        ;;
        "-p2")
            mode="-p2"
          #  command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 2,7 > pressmode2_no_triage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $7}' > press2_no_filtrage.txt
                    
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$1 >= date_min && $1 <= date_max' press2_no_filtrage.txt > press2_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="press2_filtre.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $7 ";" $10}' > press2_no_filtrage.txt
                        
                        cat press2_no_filtrage.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > press2_filtre.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="press2_filtre.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    command=`cat meteo.csv | tail -n+2 |cut -d ';' -f 2,7 > press2_filtre.txt`
                    echo $command
                    echo "Pas de filtrage, On passe au C"
                    fichier="press2_filtre.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $2 ";" $7 ";" $10}' > press2_no_filtrage.txt
                    cat press2_no_filtrage.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > press2_need_date_filter.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$1 >= date_min && $1 <= date_max' press2_need_date_filter.txt > press2_filtre.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="press2_filtre.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 
                fi
        ;;
        "-m") #colonne 10 c'est les coordonnées
            mode="-m"
            #command=`cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $6 ";" $10}' > humidite_no_filtrage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $6 ";" $10 ";" $2}' > humidite_filtre.txt
                    sed -i -e "s/,/;/g" humidite_filtre.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' humidite_filtre.txt > humidite_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="humidite_pas_trie.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $6 ";" $10}' > humidite_filtre.txt
                        sed -i -e "s/,/;/g" humidite_filtre.txt
                        cat humidite_filtre.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > humidite_pas_trie.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="humidite_pas_trie.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $6 ";" $10}' > humidite_pas_trie.txt
                    sed -i -e "s/,/;/g" humidite_pas_trie.txt
                    echo "Pas de filtrage, On passe au C"
                    fichier="humidite_pas_trie.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $6 ";" $10 ";" $2}' > humidite_filtre.txt
                    sed -i -e "s/,/;/g" humidite_filtre.txt
                    cat humidite_filtre.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > humidite_filtre_need_date.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' humidite_filtre_need_date.txt > humidite_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="humidite_pas_trie.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 

                fi
        ;;
        "-h") # on doit mtn vérifié si on prends en compte le parametre date (-d min max)
            mode="-h"
            #command=`cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $14 ";" $10}' > altitude_pas_trie.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $14 ";" $10 ";" $2}' > altitude_filtre.txt
                    sed -i -e "s/,/;/g" altitude_filtre.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' altitude_filtre.txt > altitude_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="altitude_pas_trie.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $14 ";" $10}' > altitude_filtre.txt
                        sed -i -e "s/,/;/g" altitude_filtre.txt
                        cat altitude_filtre.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > altitude_pas_trie.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="altitude_pas_trie.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $14 ";" $10}' > altitude_pas_trie.txt
                    sed -i -e "s/,/;/g" altitude_pas_trie.txt
                    echo "Pas de filtrage, On passe au C"
                    fichier="altitude_pas_trie.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $14 ";" $10 ";" $2}' > altitude_filtre.txt
                    sed -i -e "s/,/;/g" altitude_filtre.txt
                    cat altitude_filtre.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($3>min_lattitude && $3<max_lattitude && $4>min_longitude && $4<max_longitude) {print $0}}' > altitude_filtre_need_date.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$5 >= date_min && $5 <= date_max' altitude_filtre_need_date.txt > altitude_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="altitude_pas_trie.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 

                fi


   

        ;;
        "-w")
            mode="-w"
            #`cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $4 ";"$5 ";" $10}' > vent_no_triage.txt`
                if [ $date_bool == 1 ] && [ $lieu == 0 ]; then
                    # commande qui recupere tous les donnes pour le mode température 1 + la date
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $4 ";" $5 ";" $10 ";" $2}' > vent_filtre.txt
                    sed -i -e "s/,/;/g" vent_filtre.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$6 >= date_min && $6 <= date_max' vent_filtre.txt > vent_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    echo "Donnée Filtré par date, On passe au C"
                    fichier="vent_pas_trie.txt"
                    
                elif [ $date_bool == 0 ] && [ $lieu != 0 ]; then

                        #on rajoute les coordonnes en dernier au fichier .txt
                        cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $4 ";" $5 ";" $10}' > vent_filtre.txt
                        sed -i -e "s/,/;/g" vent_filtre.txt
                        cat vent_filtre.txt  | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > vent_pas_trie.txt
                        echo "Donnée Filtré par lieu, On passe au C"
                        fichier="vent_pas_trie.txt"


                    #filtre par longitude et latitude
                elif [ $date_bool == 0 ] && [ $lieu == 0 ]; then
                    # pas de filtrage
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $4 ";" $5 ";" $10}' > vent_pas_trie.txt
                    sed -i -e "s/,/;/g" vent_pas_trie.txt
                    echo "Pas de filtrage, On passe au C"
                    fichier="vent_pas_trie.txt"

                elif [ $date_bool == 1 ] && [ $lieu != 0 ]; then
                    cat meteo.csv | tail -n+2 | awk -F ';' '{print $1 ";" $4 ";" $5 ";" $10 ";" $2}' > vent_filtre.txt
                    sed -i -e "s/,/;/g" vent_filtre.txt
                    cat vent_filtre.txt | tr ',' ';' | awk -v min_lattitude="$min_lattitude" -v max_lattitude="$max_lattitude" -v min_longitude="$min_longitude" -v max_longitude="$max_longitude" -F ';' '{if($4>min_lattitude && $4<max_lattitude && $5>min_longitude && $5<max_longitude) {print $0}}' > vent_filtre_need_date.txt
                    #date_max represente la date maximum de l'intervalle a filtré fourni par l'utilisateur et on rajoute 1 jour a la date que nous a fourni l'utilisateur car pour l'inclure dans l'intevalle 
                    date_max=$(date -d "$date_max + 1 day" +"%Y-%m-%d")
                    #la commande awk va filtré le fichier .txt créer par la commande cat (ligne 121). elle filtre par l'intervalle de date et garde que les lignes compris dans l'intervalle de dates
                    awk -v date_min="$date_min" -v date_max="$date_max" -F ";" '$6 >= date_min && $6 <= date_max' vent_filtre_need_date.txt > vent_pas_trie.txt
                    # on supprime l'ancien fichier qui n'était pas filtré.
                    fichier="vent_pas_trie.txt"
                    echo "Donnée Filtrée par date et par lieu, On passe au C ! " 

                fi
        ;;

    esac
done


make
if [ -x "exec" ]; then
    if [ $decroissant == 1 ]; then
        ./exec -f "$fichier" -o sortie.txt --"$mode_trie" -r "$mode"
    else
        ./exec -f "$fichier" -o sortie.txt --"$mode_trie" "$mode"
    fi
else
echo "exec n'est pas un fichier exécutable ou n'existe pas"
exit 1
fi
echo "On passe maintenant à GNUPLOT !"
echo "On va créer un fichier graphique.png, si vous avez choississez plusieurs modes alors il yaura graphique1.png et graphique2.png etc..."


#GNUPLOT
if [ $mode == "-h" ]; then
    gnuplot gnuplot_h.gnu
elif [ $mode == "-m" ]; then
    gnuplot gnuplot_m.gnu
elif [ $mode == "-p1" ]; then
    gnuplot gnuplot_p1.gnu
elif [ $mode == "-p2" ]; then
    gnuplot gnuplot_p2.gnu
elif [ $mode == "-t1" ]; then
    gnuplot gnuplot_t1.gnu
elif [ $mode == "-t2" ]; then
    gnuplot gnuplot_t2.gnu
fi

if [ $plusieurs_modes > 1 ]; then
   
    mv graphique.png graphique$j.png
fi

done















