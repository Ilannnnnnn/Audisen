#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "frame.h"
#include "define.h"



// La fonction createInitFrame créer une trame d'initialisation à partir des informations de la chanson.
// Elle prend en paramètre une structure s_song contenant les informations de la chanson
// et une chaîne de caractères (frame) pour stocker la trame créée.
void createInitFrame(s_song mySong, char* frame) {
    char checksum[3];
    sprintf(frame, "#%s,%d,%d*", mySong.title, mySong.tpm, mySong.nTicks);// Formatage de la trame avec le titre de la chanson, le tempo et le nombre de ticks.
    calculateChecksum(frame + 1, checksum);
   
    for (int i = 0; i < 2; i++) {// Conversion du checksum en minuscules
        if (isalpha(checksum[i])) {
            checksum[i] = tolower(checksum[i]);
        }
    }
    strcat(frame, checksum);
    strcat(frame, "\r\n");
}


// La fonction createTickFrame crée une trame pour chaque tick de la chanson.
// Elle prend en paramètre une structure s_tick contenant les informations du tick
// et une chaîne de caractères (frame) pour stocker la trame créée.
void createTickFrame(s_tick myTick, char* frame) {
    char checksum[3];
    sprintf(frame, "#0,%d,%02d,%02d,%02d,%02d*", myTick.accent, myTick.note[0], myTick.note[1], myTick.note[2], myTick.note[3]);// Formatage de la trame avec les notes accentuées et les tick correspondant.
    calculateChecksum(frame + 1, checksum);
    for (int i = 0; i < 2; i++) { // Permet de convertir la majuscule en minuscule lors de l'écriture hexadécimale du checksum.
        if (isalpha(checksum[i])) {
            checksum[i] = tolower(checksum[i]);
        }
    }
   
    strcat(frame, checksum); // Ajout de la checksum
    strcat(frame, "\r\n"); // Ajout de <CR><LF> à la trame
}

// La fonction calculateChecksum calcule le checksum d'une chaîne de caractères.
// Elle prend en paramètre un pointeur vers le contenu de la chaîne (content)
// et un pointeur vers la variable où stocker le checksum (checksum).
void calculateChecksum(char *content, char *checksum) {
    char result = 0;
    while (*content != '*') {
        result ^= *content++; // Opération de XOR entre le résultat actuel et le caractère suivant de la chaîne.
    }
    sprintf(checksum, "%02X", result); // Conversion du résultat en hexadécimal
}

