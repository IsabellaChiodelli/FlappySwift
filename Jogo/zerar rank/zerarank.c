#include <stdio.h>
#include <string.h>

typedef struct Rank {
    char nome[41];
    int score;
}Rank;


void salva_arquivo(char nome_arq[], Rank top5[5]){
    FILE *fp;

    fp = fopen(nome_arq, "wb");
    fwrite(top5, sizeof(Rank), 5, fp);
    fclose(fp);


}
int main(void){
    Rank top5[5];

    top5[0].score = 0;
    strcpy(top5[0].nome, "Marcelo Walter");
    top5[1].score = 0;
    strcpy(top5[1].nome, "Travis Kelce");
    top5[2].score = 0;
    strcpy(top5[2].nome, "Selena Gomez");
    top5[3].score = 0;
    strcpy(top5[3].nome, "Sabrina Carpenter");
    top5[4].score = 0;
    strcpy(top5[4].nome, "Lana del Rey");



    salva_arquivo("ranking.bin", top5);
}
