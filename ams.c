
#include "define.h"
#include "ams.h"
#define MAX_NUMBER_NOTES 4
#define MAX_NOTE_LENGTH 5

// La fonction readAMS permet d'ouvrir un fichier, lire son contenu ligne par ligne
// pour cela elle prend en argument le fichier à lire, de type pointeur
// elle permet de retourner la structure de la chanson contenant le titre la tempo en bpm et les ticks.
s_song readAMS(char *fileName) {
    char line[MAX_SIZE_LINE];
    int ticks = 0;
    int i=0;
    s_song mySong;
    memset(&mySong, 0, sizeof(mySong));// Permet d'initialiser le pointeur mySong avec des 0.
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        strcpy(mySong.title, "");// Copie d'une chaine vide dans le titre si le fichier est nul.
        return mySong;
    }
    if (fgets(line, sizeof(line), file) != NULL) {
        int i = strlen(line) - 1;
        while (i >= 0 && (line[i] == ' ' || line[i] == '\n' || line[i] == '\r')) {// Parcours la chaine de caractère à partir de la fin et suppression des espaces à la fin de la ligne.
            line[i] = '\0'; // Permet de marquer la fin de la chaine de caractère.
            i--;
        }
        strcpy(mySong.title, line);
    }
    fscanf(file, "%d\n", &mySong.tpm);
    mySong.tpm = mySong.tpm * 2;
    while (fgets(line, MAX_SIZE_LINE, file) != NULL) {
        if (i>0){
            getIndices(line, &mySong);
            ticks++;
            mySong.nTicks = ticks;
        }
        i++;
    }
    fclose(file);
    return mySong;
}


// La fonction getIndices analyse une ligne de texte pour connaitre les indices
// des notes de musiques accentuées ou simplement jouée.
// pour cela elle prend en argument les lignes du fichier ainsi que le fichier lui-même de type pointeur.
void getIndices(char *ligne, s_song *mySong) {
    int noteIndex = 0;
    int index = 0;
    char caractere = ligne[index];
    while (caractere != '\0') { // Parcours la chaine de caractère
        if (caractere == 'x' || caractere == '^') {
             mySong->tickTab[mySong->nTicks].note[noteIndex] = (((int)(index)-4)/3)+1; // Calcul de l'indice de la note musicale et stockage dans la structure de la chanson.
             noteIndex++;
             if (caractere == '^'){
                mySong->tickTab[mySong->nTicks].accent = 1; // Indique qu'un accent est présent dans la structure de la chanson.
             }
        }
        index++;
        caractere = ligne[index];    
    }
}



/*
 Fonction initAMSFile : Initialise le fichier AMS en mettant le nom, le bpm et la ligne avec l'indice des notes
 */
void initAMSFile( s_song *newSong, FILE *amsFile){
    fprintf(amsFile, "%s\n", newSong->title);
    fprintf(amsFile, "%d\n\n", newSong->tpm);
    fprintf(amsFile, "    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 \n");
}

/*
 * Fonction createAMS : Convertit un fichier de musique texte en format AMS.
 * Elle lit les informations musicales depuis le fichier texte spécifié, puis les transforme en un fichier au format AMS.
 * Chaque ligne du fichier texte est analysée pour extraire les informations sur les notes et leurs durées, puis elles sont stockées.
 * Ensuite, un fichier au format AMS est créé et les informations de chaque tick musical sont écrites.
 */
void createAMS(char* txtFileName, char* amsFileName){

        typedef struct new_tick{
            int accent;//accentuation du new_tick (0=Non, 1:Oui)
            int note[4];// Tableau de 4 notes (0 à 60)
            int duree[4]; //Nombre de ticks restant pour chaque note jouée
        }s_tick;


        typedef struct song{
            int tpm;// new_tick par minutes
            int nTicks;// Nombre de ticks dans le morceau
            char title[MAX_SIZE_TITLE];// Titre du morceau
            struct new_tick tickTab[MAX_NUMBER_TICKS];// Tableau de ticks
        }s_song;

        typedef struct {
        char caractere_note[3]; // Tableau de caractères pour stocker les caracteres de la note
        } CaracterePosition;

        s_song newSong;
        FILE *amsFile = fopen(txtFileName, "r");
        if (amsFile == NULL) {
            printf("Erreur : Impossible d'ouvrir' le fichier %s\n", txtFileName);
        }
        else{

            typedef struct note_tab{
                char note[3]; // Nom de la note
                int numero; // numero associé à la note
            } n_note;

            // Tableau des notes 
            n_note noteTab[] = {
                {"C1", 1}, {"C1#", 2}, {"D1", 3}, {"D1#", 4}, {"E1", 5}, {"F1", 6}, {"F1#", 7},
                {"G1", 8}, {"G1#", 9}, {"A1", 10}, {"A1#", 11}, {"B1", 12},
                {"C2", 13}, {"C2#", 14}, {"D2", 15}, {"D2#", 16}, {"E2", 17}, {"F2", 18}, {"F2#", 19},
                {"G2", 20}, {"G2#", 21}, {"A2", 22}, {"A2#", 23}, {"B2", 24},
                {"C3", 25}, {"C3#", 26}, {"D3", 27}, {"D3#", 28}, {"E3", 29}, {"F3", 30}, {"F3#", 31},
                {"G3", 32}, {"G3#", 33}, {"A3", 34}, {"A3#", 35}, {"B3", 36},
                {"C4", 37}, {"C4#", 38}, {"D4", 39}, {"D4#", 40}, {"E4", 41}, {"F4", 42}, {"F4#", 43},
                {"G4", 44}, {"G4#", 45}, {"A4", 46}, {"A4#", 47}, {"B4", 48},
                {"C5", 49}, {"C5#", 50}, {"D5", 51}, {"D5#", 52}, {"E5", 53}, {"F5", 54}, {"F5#", 55},
                {"G5", 56}, {"G5#", 57}, {"A5", 58}, {"A5#", 59}, {"B5", 60}
            };

            char ligne[MAX_SIZE_LINE];
            int numero_ligne = 0;
            int nbtick = 0;
            memset(&newSong, 0, sizeof(newSong)); //On initialise la structure newSong à 0


            while (fgets(ligne, MAX_NUMBER_TICKS, amsFile) != NULL){
                if (numero_ligne==0){
                        ligne[strlen(ligne) - 1] = '\0';//On récupère le titre
                        strcpy(newSong.title, ligne);
                    }

                    else if (numero_ligne==1){
                        ligne[strlen(ligne) - 1] = '\0';//On récupère le bpm
                        sscanf(ligne, "%d", &newSong.tpm);
                    }
                    
                    else if (numero_ligne>2) {
                        
                        int numero_note = 0; //On peut jouer jusqu'à 4 notes en même temps donc numero_note va de 0 à 3
                        newSong.nTicks = numero_ligne-2;
                        for (int index=0; index != strlen(ligne)-1; index++){
                            char PositionNote[6]; //va de 0 à 60 pour la position de la note
                            char nom_note = "";
                            CaracterePosition caractPos = {0}; //On initialise la struct pour récuperer les caracteres
                            
                            caractPos.caractere_note[0] = ligne[index]; //On récupère le 1er caractere
                            caractPos.caractere_note[1] = ligne[index+1];//On récupère le 2nd caractere
                            if (ligne[index+2] != ' '){ //Si le caractère suivant n'est pas un espace, c'est un #. Il faut le récuperer
                                caractPos.caractere_note[2] = ligne[index+2];
                                ++index;//On augmente l'index pour être sur l'espace
                            }
                            char duree_note = ligne[index+3]; // Avec les conditions précédentes, le caractère en index+3 est forcément la lettre associée a la durée de la note
                            if (ligne[index+4] == ','){ // Si on a une vigule, c'est qu'il y a une autre note a récuperer 
                                index=index+5;
                            }
                            else{ //Sinon on se met juste avant la fin
                                index = strlen(ligne)-2; 
                                }
                            
                            

                            int j = 0;
                            while(strcmp(noteTab[j].note, caractPos.caractere_note) != 0 ){ //On cherche la note récupérer dans le tableau de notes
                                j++;
                            }

                            int k_tick = (duree_note == 'N') ? 1 : (duree_note == 'B') ? 3 : (duree_note == 'R') ? 7 : 0; // Condition pour retrouver la durée en ticks

                            
                            for(int sup_tick= 0; sup_tick <= k_tick; sup_tick++){ // On met la durée de chaque note dans les ticks correspondant
                                while (newSong.tickTab[nbtick+sup_tick].note[numero_note] != 0){
                                    numero_note++;
                                }
                                newSong.tickTab[nbtick+sup_tick].note[numero_note] = noteTab[j].numero;
                            }
                            newSong.tickTab[nbtick].duree[numero_note] = k_tick;
                        }



                        nbtick++;


                    }


                numero_ligne++;
            }

        }






        amsFile = fopen(amsFileName, "w");
        if (amsFile == NULL) {
            printf("Erreur : Impossible de créer le fichier %s\n", amsFileName);
        }

        else{
            initAMSFile(&newSong, amsFile);
            for (int tick =0; tick!=newSong.nTicks; tick++){ //On parcourt chaque tick de la chanson
                fprintf(amsFile, "%03d", tick+1);
                int numero_note = 0;
                for (int colonne=1; colonne!=61; colonne++){
                    if (colonne == newSong.tickTab[tick].note[numero_note]){// Vérifie si la colonne correspond à la note actuelle
                        if((tick==0) || (newSong.tickTab[tick-1].duree[numero_note] == 0) ){ // Vérifie si c'est le premier tick ou si la durée de la note précédente est 0
                            fprintf(amsFile, "|^ ");
                        }
                        else{
                            fprintf(amsFile, "|x ");// Réduit la durée de la note et écrit "x"
                            newSong.tickTab[tick].duree[numero_note] = newSong.tickTab[tick-1].duree[numero_note] - 1;
                        }
                        
                        numero_note++;
                        
                    }
                    else{
                        fprintf(amsFile, "|  ");
                    }
                
                    
                }
                if(tick!=newSong.nTicks-1){// Termine la ligne avec un "|" si ce n'est pas le dernier tick
                    fprintf(amsFile,"|\n");
                }
                else{
                    fprintf(amsFile,"|"); // Si c'est le dernier tick, termine la ligne sans saut de ligne supplémentaire
                }
                

            }
            //printf("%d", newSong.nTicks);
            
        }
    fclose(amsFile);
    }
