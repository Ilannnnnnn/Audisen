#include <stdio.h>
#include <string.h>
#include "ams.h"
#include "frame.h"
#include "amp.h"


/* Ouvre le fichier frm pour y ecrire les trames*/
FILE* initFRM(char* filename_frm){
    FILE * frm = NULL;
    frm = fopen(filename_frm, "w");
    if (frm==NULL){
        return NULL;
    }
    return frm;
}
/* Ouvre le fichier frm pour y ecrire les trames*/
void closeFRM(FILE* frm){
    fclose(frm);
}


int main() {
    printf("debut\n");
    char song_name[MAX_SIZE_TITLE];
    char* playlist="Playlist_autotest.amp";
    char filename_frm[MAX_SIZE_TITLE] = "frame_test.frm";
    char frame[MAX_NUMBER_TICKS];
    char frame_tick[MAX_NUMBER_TICKS];
    FILE* pf = initAMP(playlist);
    FILE* frm = initFRM(filename_frm);
    char txtFile[]="bohemian_rhapsody.txt";
    char amsFile[]="bohemian_rhapsody.ams";
    createAMS(txtFile, amsFile);
    while(!feof(pf)){
        readAMP(pf, song_name);
        s_song mySong = readAMS(song_name);
        createInitFrame(mySong, frame);
        fprintf(frm, "%s", frame); // Écrire frame initiale dans le fichier
        for (int i = 0; i<mySong.nTicks; i++){
            createTickFrame(mySong.tickTab[i], frame);
            fprintf(frm, "%s", frame); // Écrire les ticks dans le fichier
        }

    }
    
    printf("fin\n");
    return 0;
}
