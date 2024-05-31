#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "amp.h"

// La fonction initAMP initialise un pointeur de fichier FILE* pour un fichier donné.
// Elle prend en paramètre un fichier (filename).
// Elle vérifie si le fichier est NULL ou s'il est vide, auquel cas elle renvoie NULL.
// Ensuite, elle ouvre le fichier en mode lecture et renvoie le pointeur du fichier.
FILE* initAMP(char* filename){
    FILE * playlist = NULL;
    if ((filename == NULL) || (strcmp(filename,"")==0)){// Verfie que le fichier est ni null, ni vide
        return NULL;
    }
    playlist = fopen(filename,"r");
    return playlist;
}


// La fonction readAMP lit une ligne de chaîne de caractères à partir du fichier pointé par pf.
// Elle convertit les majuscules en minuscules et remplace les espaces, les apostrophes et les caractères spéciaux par des underscores.
// Elle prend en paramètre un pointeur vers un fichier (pf) et une chaîne de caractères (song_name) pour stocker la ligne lue.
void readAMP (FILE* pf, char* song_name){
   

    int j=0;
    if(fgets(song_name, 200,pf)!=NULL){
        song_name[strcspn(song_name, "\n")] = '\0'; // Permet d'ajouter un marqueur de fin de ligne, à la fin de la chaine de caractères.
        for (int i = 0; i<strlen(song_name);i++){

                if(song_name[i]>='A' && song_name[i]<='Z'){

                    song_name[j]=(song_name[i]-'A'+'a'); // Conversion des majuscules en minuscules
                    j++;
                }
                else if ((song_name[i]>='a' && song_name[i]<='z')){
                    song_name[j]=song_name[i];
                    j++;
                }

                else {

                        if (j>0){
                            if(song_name[j-1] != '_'){
                                song_name[j] = '_';// Supprime les underscores consécutifs
                                j++;
                            }  
                        }else{
                            song_name[j] = '_';
                            j++;
                        }
                }
        }
        song_name[j]='\0';
        strcat(song_name,".ams");
    }

}

// La fonction closeAMP permet de fermer le fichier pris en paramètre (pf), le fichier est de type pointeur.
void closeAMP(FILE* pf){

    if (pf!=NULL){
       fclose(pf);
    }
}