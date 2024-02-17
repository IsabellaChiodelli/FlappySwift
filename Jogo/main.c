#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUBE_WIDTH 80
#define TUBE_HEIGHT 800
#define N_BARRA 4
#define DIST_X_TUBES 300
#define MAX_INPUT_CHARS 40
#define POWER_UP_SPACE 20
#define POWER_UP_SIZE 30
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define MAXSCORES 5

typedef enum GameScreen { MENU, START, GAME, PAUSE, OVER, RANK, DIFF, SCORE, CREDITS } GameScreen;

typedef struct Player {
    Rectangle rec;
    Color cor;

}Player;

typedef struct Tube {
    Rectangle rec;

} Tube;

typedef struct Tubes {
    Tube tubeUp;
    Tube tubeDown;
    int gap;
    bool valid;
    bool respawn;

} Tubes;

typedef struct PowerUp {
    Vector2 posit;
    float radius;
    bool mode;
    bool valid;
    bool respawn;
    int type;

} PowerUp;

typedef struct Shot {
    Vector2 posit;
    float radius;
} Shot;

typedef struct Rank {
    char nome[41];
    int score;
}Rank;

typedef struct TexturePack {
    Image FundoFolkloreTxt;
    Image FundoLoverTxt;
    Image Fundo1989Txt;
    Image FundoFearlessTxt;
    Image FundoReputationTxt;

    Texture2D Name_Game;
    Texture2D PlayTx;
    Texture2D RankTx;
    Texture2D DiffTx;
    Texture2D ExitTx;
    Texture2D Diff1Tx;
    Texture2D Diff2Tx;
    Texture2D Diff3Tx;
    Texture2D Diff4Tx;
    Texture2D Diff5Tx;
    Texture2D RestartEGTx;
    Texture2D MenuEGTx;
    Texture2D VoltarTx;
    Texture2D FundoTx;
    Texture2D FundoFolkloreTx;
    Texture2D Fundo1989Tx ;
    Texture2D FundoLoverTx;
    Texture2D FundoFearlessTx;
    Texture2D FundoReputationTx;
    Texture2D GameOverTx;
    Texture2D DiffTitleTx;
    Texture2D RankTitleTx;
    Texture2D TaylorTx;
    Texture2D TaylorGolfeTx;
    Texture2D UpBarFolkTx ;
    Texture2D DownBarFolkTx;
    Texture2D UpBar1989Tx;
    Texture2D DownBar1989Tx;
    Texture2D UpBarLoverTx;
    Texture2D DownBarLoverTx;
    Texture2D FearlessBarTx;
    Texture2D RepBarTx;
    Texture2D Base_Rank;
    Texture2D DoneTx ;
    Texture2D ScrBackTx;
    Texture2D BordaTx;
    Texture2D PUONTx;
    Texture2D PUOFFTx;
    Texture2D CreditsTx;
    Texture2D KelceTx;
    Texture2D BadBTX;
    Texture2D TacoTx;
    Texture2D ArminhaTx;
    Texture2D NoveloTx;
    Texture2D CapaceteTx;


}TexturePack;

typedef struct Mouse {
    Vector2 mousePoint;
}Mouse;

static Rank top5[MAXSCORES];
static int score_threshold,gap,inc_gap,dif_max_altura,inc_dif_max_altura,vel_obstaculos,inc_vel_obstaculos, diff = 1;
static int var_gap, var_alt;
static const int screenWidth = SCREEN_WIDTH;
static const int screenHeight = SCREEN_HEIGHT;
static Player player;
static Tubes tubes[N_BARRA] = {0};
static Shot shot[10] = {0};
static PowerUp powerUp;
static Rectangle BadB;
static Rectangle kelce;
static int tubeSpeed = 0;
static int score = 0;
static int aug_vel = 0;
static int q_rcrs = 0;
static int grav = 15;
static int letterCount = 0;
static int framesCounter = 0;
static char name[MAX_INPUT_CHARS + 1] = "\0";
static GameScreen currentScreen = MENU;

static Rectangle playBtn = { screenWidth/2 - 150, screenHeight/2 - 110, 300, 100 };
static Rectangle diffBtn = { screenWidth/2 - 150, screenHeight/2, 300, 100 };
static Rectangle rankBtn = { screenWidth/2 - 150, screenHeight/2 + 110, 300, 100 };
static Rectangle exitBtn = { screenWidth/2 - 150, screenHeight/2 + 220, 300, 100 };
static Rectangle menuBtn = { screenWidth/2.0f +70, screenHeight/2.0f + 20, 80, 40 };
static Rectangle restartBtn = { screenWidth/2.0f - 140, screenHeight/2.0f + 20, 80, 40 };
static Rectangle backRank = {screenWidth/2 - 20, 720 , 40, 40};
static Rectangle backDiff = {screenWidth/2 - 20, 740 , 40, 40};
static Rectangle btn1Diff = {screenWidth/2 - 250, screenHeight/2 - 225 , 500, 100};
static Rectangle btn2Diff = {screenWidth/2 - 250, screenHeight/2 - 115 , 500, 100};
static Rectangle btn3Diff = {screenWidth/2 - 250, screenHeight/2 - 5, 500, 100};
static Rectangle btn4Diff = {screenWidth/2 - 250, screenHeight/2 + 105 , 500, 100};
static Rectangle btn5Diff = {screenWidth/2 - 250, screenHeight/2 + 215 , 500, 100};
static Rectangle btnDone = {screenWidth/2 - 40, screenHeight/2 + 100 , 80, 40};
static Rectangle btnPU = {50, 688 , 100, 75};
static Rectangle btnCredits = {1050, 688 , 100, 75};
static Rectangle textBox = { screenWidth/2 - 500 , screenHeight/2 -20 , 1000, 40 };

static bool rodando = true;
static bool mouseOnText = false;
static bool invisible_string = false;
static bool golfe = false;
static bool bad_blood = false;
static bool kelce_pup = false;

static TexturePack texturas;
static Mouse mouse;

void le_arquivo(char nome_arq[], Rank top5[MAXSCORES]){
    FILE *fp;

    fp = fopen(nome_arq, "rb");
    fread(top5, sizeof(Rank), MAXSCORES, fp);
    fclose(fp);

}

void salva_arquivo(char nome_arq[], Rank top5[MAXSCORES]){
    FILE *fp;

    fp = fopen(nome_arq, "wb");
    fwrite(top5, sizeof(Rank), MAXSCORES, fp);
    fclose(fp);


}

int cabe_tela(int pos_in, int gap, int dist_alt){

    int diff;
    int pos_out;
    diff = GetRandomValue(-dist_alt, dist_alt);
    pos_out = pos_in + diff;
    if (pos_out > -50 - gap){
        return cabe_tela(pos_in, gap,dist_alt);
    }else if(pos_out < -750){
        return cabe_tela(pos_in, gap,dist_alt);
    }else{
        return pos_out;
    }

}

int retorno(int i){
    int out;
    if(i == 0){
        out=(N_BARRA-1);
    }else{
        out = i-1;
    }
    return out;
}

int guarda_valor(char* string, char *restante){

    char val[10], rest[100];
    int i=0,j=0,valor;
    while(string[i] != ' '){
        val[i] = string[i];
        i++;
    }
    val[i] = '\0';
    i++;
    while(string[i] != '\0'){
        rest[j] = string[i];
        i++;
        j++;
    }
    strcpy(restante, rest);
    valor = atoi(val);
    return valor;
}

void le_dificuldade(int *um, int *dois, int *tres, int *quatro, int *cinco, int *seis, int *sete, int diff){
    FILE *fptr;

    if(diff == 1) fptr = fopen("dificuldade1.txt", "r");
    if(diff == 2) fptr = fopen("dificuldade2.txt", "r");
    if(diff == 3) fptr = fopen("dificuldade3.txt", "r");
    if(diff == 4) fptr = fopen("dificuldade4.txt", "r");
    if(diff == 5) fptr = fopen("dificuldade5.txt", "r");

    char myString[100];
    fgets(myString, 100, fptr);
    fclose(fptr);

    *um = guarda_valor(myString, myString);
    *dois = guarda_valor(myString, myString);
    *tres = guarda_valor(myString, myString);
    *quatro = guarda_valor(myString, myString);
    *cinco = guarda_valor(myString, myString);
    *seis = guarda_valor(myString, myString);
    *sete = atoi(myString);
}

void desenha_fundo(void){
    if(diff == 1) DrawTextureRec(texturas.FundoFolkloreTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 2) DrawTextureRec(texturas.Fundo1989Tx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 3) DrawTextureRec(texturas.FundoLoverTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 4) DrawTextureRec(texturas.FundoFearlessTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 5) DrawTextureRec(texturas.FundoReputationTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
}

void desenha_tubos(void){
    int i =0;
    while(i<N_BARRA){
        if(diff == 1) {
            DrawTextureRec(texturas.UpBarFolkTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
            DrawTextureRec(texturas.DownBarFolkTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
        }
        if(diff == 2) {
            DrawTextureRec(texturas.UpBar1989Tx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
            DrawTextureRec(texturas.DownBar1989Tx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
        }
        if(diff == 3){
            DrawTextureRec(texturas.UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
            DrawTextureRec(texturas.DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
        }
        if(diff == 4){
            DrawTextureRec(texturas.FearlessBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
            DrawTextureRec(texturas.FearlessBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
        }
        if(diff == 5){
            DrawTextureRec(texturas.RepBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
            DrawTextureRec(texturas.RepBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
            }
        i++;
        }
    i=0;
}

void desenha_borda(void){
    if(diff == 1) DrawTextureRec(texturas.BordaTx, (Rectangle){0,0,510,110}, (Vector2){SCREEN_WIDTH/2 - 255, SCREEN_HEIGHT/2 - 230}, RAYWHITE);
    if(diff == 2) DrawTextureRec(texturas.BordaTx, (Rectangle){0,0,510, 110}, (Vector2){SCREEN_WIDTH/2 - 255,SCREEN_HEIGHT/2 - 120}, RAYWHITE);
    if(diff == 3) DrawTextureRec(texturas.BordaTx, (Rectangle){0,0,510, 110}, (Vector2){SCREEN_WIDTH/2 - 255,SCREEN_HEIGHT/2 - 10}, RAYWHITE);
    if(diff == 4) DrawTextureRec(texturas.BordaTx, (Rectangle){0,0,510, 110}, (Vector2){SCREEN_WIDTH/2 - 255,SCREEN_HEIGHT/2 + 100}, RAYWHITE);
    if(diff == 5) DrawTextureRec(texturas.BordaTx, (Rectangle){0,0,510, 110}, (Vector2){SCREEN_WIDTH/2 - 255,SCREEN_HEIGHT/2 + 210}, RAYWHITE);
}

void desenha_menu(void){
    ClearBackground(RAYWHITE);
    DrawTextureRec(texturas.FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    DrawTextureRec(texturas.Name_Game, (Rectangle){0,0,800,200}, (Vector2){screenWidth/2 - 400, 50}, RAYWHITE);
    DrawTextureRec(texturas.PlayTx, (Rectangle){0,0,playBtn.width,playBtn.height}, (Vector2){playBtn.x, playBtn.y}, RAYWHITE);
    DrawTextureRec(texturas.DiffTx, (Rectangle){0,0,diffBtn.width,diffBtn.height}, (Vector2){diffBtn.x, diffBtn.y}, RAYWHITE);
    DrawTextureRec(texturas.RankTx, (Rectangle){0,0,rankBtn.width,rankBtn.height}, (Vector2){rankBtn.x, rankBtn.y}, RAYWHITE);
    DrawTextureRec(texturas.ExitTx, (Rectangle){0,0,exitBtn.width,exitBtn.height}, (Vector2){exitBtn.x, exitBtn.y}, RAYWHITE);
    if(powerUp.mode) DrawTextureRec(texturas.PUONTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
    else DrawTextureRec(texturas.PUOFFTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
    //DrawTextureRec(texturas.CreditsTx, (Rectangle){0,0,100,75}, (Vector2){1050, 688}, RAYWHITE);
}

void desenha_creditos(void){
    ClearBackground(RAYWHITE);
    DrawTextureRec(texturas.VoltarTx, (Rectangle){0,0,backRank.width,backRank.height}, (Vector2){backRank.x, backRank.y}, RAYWHITE);
}

void desenha_score(void){
    ClearBackground(RAYWHITE);
    //Desenha o Fundo de acordo com a dificuldade
    desenha_fundo();
    //Desenha a Taylor
    DrawTextureRec(texturas.TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
    //Desenha as Barras de acordo com a dificuldade
    desenha_tubos();
    //Desenha a pontuação
    DrawText(TextFormat("%d", score), screenWidth - 200, 20, 50, GREEN);
    //Desenhha o fundo de score
    DrawTextureRec(texturas.ScrBackTx, (Rectangle){0,0,1100, 500}, (Vector2){screenWidth/2 - 550,screenHeight/2 - 300}, RAYWHITE);
    //Desenha a caixa de texto
    DrawRectangleRec(textBox, LIGHTGRAY);
    //Muda a cor da borda se o cursor estiver em cima da caixa de texto
    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, (Color){ 255, 109, 194, 255 });
    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
    //Desenha o Texto que é escrito
    DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 30, (Color){ 255, 109, 194, 255 });
    //Desenha uma barrinha piscando se pode escrever mais caracteres.
    if (mouseOnText){
        if (letterCount < MAX_INPUT_CHARS){
            if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 30), (int)textBox.y + 12, 30, (Color){ 255, 109, 194, 255 });
        }
    }
    //Desenha o botão de Ok
    DrawTextureRec(texturas.DoneTx, (Rectangle){0,0,btnDone.width,btnDone.height}, (Vector2){btnDone.x, btnDone.y}, RAYWHITE);
}

void desenha_rank(void){
    ClearBackground(RAYWHITE);
    DrawTextureRec(texturas.FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    DrawTextureRec(texturas.VoltarTx, (Rectangle){0,0,backRank.width,backRank.height}, (Vector2){backRank.x, backRank.y}, RAYWHITE);
    DrawTextureRec(texturas.RankTitleTx, (Rectangle){0,0,500, 100}, (Vector2){screenWidth/2.0f - 250,50}, RAYWHITE);

    DrawTextureRec(texturas.Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 180}, RAYWHITE);
    DrawText(top5[0].nome, screenWidth/2 - 490, screenHeight/2 - 170, 30, BLACK);
    DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 135, 30, BLACK);
    DrawText(TextFormat("%d", top5[0].score), screenWidth/2 - 370, screenHeight/2 - 135, 30, BLACK);

    DrawTextureRec(texturas.Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 90}, RAYWHITE);
    DrawText(top5[1].nome, screenWidth/2 - 490, screenHeight/2 - 80, 30, BLACK);
    DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 45, 30, BLACK);
    DrawText(TextFormat("%d", top5[1].score), screenWidth/2 - 370, screenHeight/2 - 45, 30, BLACK);

    DrawTextureRec(texturas.Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2}, RAYWHITE);
    DrawText(top5[2].nome, screenWidth/2 - 490, screenHeight/2 + 10, 30, BLACK);
    DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 45, 30, BLACK);
    DrawText(TextFormat("%d", top5[2].score), screenWidth/2 - 370, screenHeight/2 + 45, 30, BLACK);

    DrawTextureRec(texturas.Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 90}, RAYWHITE);
    DrawText(top5[3].nome, screenWidth/2 - 490, screenHeight/2 + 100, 30, BLACK);
    DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 135, 30, BLACK);
    DrawText(TextFormat("%d", top5[3].score), screenWidth/2 - 370, screenHeight/2 + 135, 30, BLACK);

    DrawTextureRec(texturas.Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 180}, RAYWHITE);
    DrawText(top5[4].nome, screenWidth/2 - 490, screenHeight/2 + 190, 30, BLACK);
    DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 225, 30, BLACK);
    DrawText(TextFormat("%d", top5[4].score), screenWidth/2 - 370, screenHeight/2 + 225, 30, BLACK);
}

void desenha_dificuldades(void){
    ClearBackground(RAYWHITE);
    DrawTextureRec(texturas.FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    DrawTextureRec(texturas.VoltarTx, (Rectangle){0,0,backDiff.width,backDiff.height}, (Vector2){backDiff.x, backDiff.y}, RAYWHITE);
    DrawTextureRec(texturas.DiffTitleTx, (Rectangle){0,0,600, 100}, (Vector2){screenWidth/2.0f - 300,50}, RAYWHITE);
    DrawTextureRec(texturas.Diff1Tx, (Rectangle){0,0,btn1Diff.width,btn1Diff.height}, (Vector2){btn1Diff.x, btn1Diff.y}, RAYWHITE);
    DrawTextureRec(texturas.Diff2Tx, (Rectangle){0,0,btn2Diff.width,btn2Diff.height}, (Vector2){btn2Diff.x, btn2Diff.y}, RAYWHITE);
    DrawTextureRec(texturas.Diff3Tx, (Rectangle){0,0,btn3Diff.width,btn3Diff.height}, (Vector2){btn3Diff.x, btn3Diff.y}, RAYWHITE);
    DrawTextureRec(texturas.Diff4Tx, (Rectangle){0,0,btn4Diff.width,btn4Diff.height}, (Vector2){btn4Diff.x, btn4Diff.y}, RAYWHITE);
    DrawTextureRec(texturas.Diff5Tx, (Rectangle){0,0,btn5Diff.width,btn5Diff.height}, (Vector2){btn5Diff.x, btn5Diff.y}, RAYWHITE);
    //Desenha a barra dependendo da seleção
    desenha_borda();
}

void desenha_over(void){
    ClearBackground(RAYWHITE);
    //Desenha o fundo de acordo com a dificuldade
    desenha_fundo();
    //Desenha a Taylor
    DrawTextureRec(texturas.TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
    //Desenha as barras de acordo com a dificuldade
    desenha_tubos();
    //Desenha a pontuação
    DrawText(TextFormat("%d", score), screenWidth - 200, 20, 50, GREEN);
    //Desenha a barra de Game Over
    DrawTextureRec(texturas.GameOverTx, (Rectangle){0,0,400, 200}, (Vector2){screenWidth/2.0f - 200,screenHeight/2.0f - 100}, RAYWHITE);
    //Desenha o botão de Restart
    DrawTextureRec(texturas.RestartEGTx, (Rectangle){0,0,restartBtn.width,restartBtn.height}, (Vector2){restartBtn.x, restartBtn.y}, RAYWHITE);
    //Desenha o Botão de Menu
    DrawTextureRec(texturas.MenuEGTx, (Rectangle){0,0,menuBtn.width,menuBtn.height}, (Vector2){menuBtn.x, menuBtn.y}, RAYWHITE);
}

void desenha_jogo(void){
    ClearBackground(RAYWHITE);
    //Desenha o fundo de acordo com a dificuldade
    desenha_fundo();
    //Desenha as barras de acordo com a dificuldade
    desenha_tubos();
    //Desenha os PowerUps
    if(powerUp.mode){
        //DrawCircleV(powerUp.posit, POWER_UP_SIZE, GREEN);
        if(powerUp.type == 1) DrawTextureRec(texturas.NoveloTx, (Rectangle){0,0,60,60}, (Vector2){powerUp.posit.x-30, powerUp.posit.y-30}, RAYWHITE);
        if(powerUp.type == 2) DrawTextureRec(texturas.TacoTx, (Rectangle){0,0,60,60}, (Vector2){powerUp.posit.x-30, powerUp.posit.y-30}, RAYWHITE);
        if(powerUp.type == 3) DrawTextureRec(texturas.ArminhaTx, (Rectangle){0,0,60,60}, (Vector2){powerUp.posit.x-30, powerUp.posit.y-30}, RAYWHITE);
        if(powerUp.type == 4) DrawTextureRec(texturas.CapaceteTx, (Rectangle){0,0,60,60}, (Vector2){powerUp.posit.x-30, powerUp.posit.y-30}, RAYWHITE);
    }
    //Desenha elementos BadBlood.
    for(int j = 0;j<10;j++){
        DrawCircleV(shot[j].posit, shot[j].radius, YELLOW);
    }
    DrawTextureRec(texturas.BadBTX, (Rectangle){0,0,BadB.width,BadB.height},(Vector2){BadB.x,BadB.y},RAYWHITE);
    //Desenha elementos Kelce.
    DrawTextureRec(texturas.KelceTx, (Rectangle){0,0,kelce.width,kelce.height},(Vector2){kelce.x,kelce.y},RAYWHITE);
    //Desenha a Taylor
    if(invisible_string) DrawTextureRec(texturas.TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3},  CLITERAL(Color){ 255, 255, 255, 128 });
    else if(golfe) DrawTextureRec(texturas.TaylorGolfeTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
    else DrawTextureRec(texturas.TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);

    //Desenha a pontuação
    DrawText(TextFormat("%d", score), screenWidth - 200, 20, 50, GREEN);

}

void desenha_start(void){
    ClearBackground(RAYWHITE);
    //Desenha os fundos de acordo com a dificuldade
    desenha_fundo();
    //Desenha a Taylor
    DrawTextureRec(texturas.TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
    //Desenha as Barras de acordo com a dificuldade
    desenha_tubos();
    ClearBackground(RAYWHITE);
    //Desenha o Aviso para iniciar o jogo
    DrawText("Aperte SPACE", 400, 380, 50, GREEN);
}

void render(void){
    BeginDrawing();

    switch(currentScreen){
        //Desenha os botões do Menu
        case MENU:
        {
            desenha_menu();

        } break;
        //Desenha os componentes do Start
        case START:{
            desenha_start();

        }break;
        //Desenha os componentes do Jogo
        case GAME:
        {
            desenha_jogo();

        } break;
        case OVER:
        {
            desenha_over();

        }break;
        //Desenha os componentes das dificuldades
        case DIFF:
        {
            desenha_dificuldades();

        }break;
        //Desenha os componentes do Ranking
        case RANK:
        {
            desenha_rank();

        }break;
        //Desenha os elementos da tela de Score
        case SCORE:
        {
            desenha_score();

        }break;
        //Desenha os componentes da tela créditos
        case CREDITS:
        {
            desenha_creditos();

        }break;
        default: break;
        }
        //Finaliza a Renderização
        EndDrawing();
}

void game_logic(void){
    //Quando o Player pula, o contador de gravidade começa.
    if (IsKeyPressed(KEY_SPACE) && player.rec.y > 0) {
           grav = 50;
    }
    //Se o contador for maior que 0.
    if(grav>0){
        //Se o contador for maior que 40, sobe com uma velocidade.
        if(grav > 40){
            grav--;
            player.rec.y -= 9;
        }else{
            //Se o contador for maior que 35 e menor que 40, diminui essa velocidade.
            if(grav > 35){
                grav--;
                player.rec.y -= 4;
            }else{
                //Se o contador for maior que 30 e menor que 35, fica no ar.
                if(grav > 30){
                    grav--;
                }else{
                    //Se o contador for maior que 23 e menor que 30, desce com uma velocidade.
                    if(grav > 23){
                         player.rec.y += 3;
                         grav--;
                    //Se for menor que 23, desce mais rápido.
                    }else{
                        grav--;
                        player.rec.y += 7;
                    }

                }

            }

        }
    // Se o contador de velocidade chegar a menos que 50, desce ainda mais rápido
    }else{
        player.rec.y += 10;
    }
    //Coisas que dependem dos tubos
    for(int i=0;i<N_BARRA;i++){

        //Movimentação dos Tubos
        tubes[i].tubeUp.rec.x -= tubeSpeed ;
        tubes[i].tubeDown.rec.x -= tubeSpeed ;
        //Regeneração dos Tubos (dependem da posição do tubo anterior)
        if(tubes[i].tubeUp.rec.x < -TUBE_WIDTH && tubes[i].respawn == true){
            tubes[i].respawn = false;
            tubes[i].gap = var_gap;
            tubes[i].tubeUp.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
            tubes[i].tubeDown.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
            tubes[i].tubeUp.rec.y = cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap, var_alt);
            tubes[i].tubeDown.rec.y = tubes[i].tubeUp.rec.y + TUBE_HEIGHT + tubes[i].gap ;
            tubes[i].valid = true;
        }
        //Se o tubo sair da tela, ele respawna
        if(tubes[i].tubeUp.rec.x > -40 && tubes[i].respawn == false){
            tubes[i].respawn = true;

        }
        //Colisão entre player e tubos.
        if(CheckCollisionRecs(player.rec, tubes[i].tubeUp.rec) || CheckCollisionRecs(player.rec, tubes[i].tubeDown.rec)){
            //Se for invisible String, desliga a colisão
            if(invisible_string){
            //Se for golfe, bate nos tubos e eles vão pra cima.
            }else if(golfe){
                if(CheckCollisionRecs(player.rec, tubes[i].tubeUp.rec)){
                    tubes[i].tubeUp.rec.y += -50;
                }
                if(CheckCollisionRecs(player.rec, tubes[i].tubeDown.rec)){
                    tubes[i].tubeDown.rec.y += 50;
                }
            //Se não for nem golfe nem invisible string, acaba o jogo
            }else{
                //Se a pontuação estiver no top5, abre a tela para colocar o nome
                if(score > top5[4].score){
                    currentScreen = SCORE;

                //Se não, Game Over
                }else{
                    currentScreen = OVER;

                }
            }
        }
        //Contagem de pontos. Se passou dos tubos, a pontuação soma (e alguns outros contadores que dependem da pontuação)
        if (player.rec.x > tubes[i].tubeUp.rec.x +20 && tubes[i].valid == true){
            score++;
            aug_vel++;
            q_rcrs = 0;
            tubes[i].valid = false;

        }

        //Lógica dos Tiros e do Helicóptero
        if(bad_blood){
            for(int j=0;j<10; j++){
                //Movimentação do tiro
                shot[0].posit.x += 5;
                if(shot[j-1].posit.x > 1200){
                    shot[j].posit.x += 6;
                    shot[j].posit.y += GetRandomValue(-4,7);
                }
                //Se o tiro pegar um dos tubos, ele sai da tela
                if(CheckCollisionCircleRec(shot[j].posit, shot[j].radius,tubes[i].tubeUp.rec) || CheckCollisionCircleRec(shot[j].posit, shot[j].radius, tubes[i].tubeDown.rec)){
                    if(CheckCollisionCircleRec(shot[j].posit, shot[j].radius,tubes[i].tubeUp.rec))tubes[i].tubeUp.rec.y = -801;
                    if(CheckCollisionCircleRec(shot[j].posit, shot[j].radius, tubes[i].tubeDown.rec))tubes[i].tubeDown.rec.y = 801;
                }
            }
            //Se o helicóptero acertar o tubo, ele sai da tela
            if(CheckCollisionRecs(BadB,tubes[i].tubeUp.rec)) tubes[i].tubeUp.rec.y = -801;
        }
        //Lógica do Kelce
        if(kelce_pup){
            //Se colide, o tubo sai da tela
            if(CheckCollisionRecs(kelce, tubes[i].tubeDown.rec)) tubes[i].tubeDown.rec.y = 801;
        }

    }

    //Incrementação de efeitos prévios de aumento de dificuldade
    if(aug_vel == (score_threshold - N_BARRA)) {
        //Contador para aplicar só uma vez.
        if(q_rcrs == 0){
            var_gap -= inc_gap;
            var_alt += inc_dif_max_altura;
            q_rcrs++;
        }
    }
    //Incrementação de efeitos diretttos de aumento de dificuldade
    if(aug_vel == score_threshold ) {
        tubeSpeed += inc_vel_obstaculos;
        aug_vel = 0;
    }

    //Colisão com o chão
    if (player.rec.y > 724){
        if(player.rec.y > 724) player.rec.y = 720;
        if(score > top5[4].score){
            currentScreen = SCORE;

        }else{
            currentScreen = OVER;

        }
    }
    // Colisão com o teto
    if (player.rec.y < 0)player.rec.y = 0;
    // Configuração do PowerUp
    if(powerUp.mode){
        //Se o score for 15, 35, 55, e assim por diante
        if(score % POWER_UP_SPACE == 15) {
            //Spawna o PowerUp
            powerUp.posit = (Vector2){389*5, GetRandomValue(150, 650)};
            powerUp.radius = POWER_UP_SIZE;
            //Sorteia o PowerUp utilizado
            powerUp.type = GetRandomValue(1,4);
        }
        //Power Up mexe com a mesma velocidade que os tubos
        powerUp.posit.x -= tubeSpeed;
        //Se o player encostar no PowerUp
        if(CheckCollisionCircleRec(powerUp.posit, powerUp.radius, player.rec)){
            //O power up sai da tela
            powerUp.posit.x = -100;
            //Dependendo do tipo do Power Up:
            //se for 1, ativa invisible string
            if(powerUp.type == 1) invisible_string = true;
            //se for 2, ativa golfe
            if(powerUp.type == 2) golfe = true;
            //Se for 3, ativa bad_blood
            if(powerUp.type == 3){
                bad_blood = true;
                //Posiciona o helicoptero
                BadB.x = player.rec.x - 112;
                BadB.y = 150;
                //Posiciona os tiros
                for(int j=0;j<10; j++){
                    shot[j].radius = 8;
                    shot[j].posit.x = player.rec.x + 5;
                    shot[j].posit.y = 200;
                }
            }
            //Se for 4, ativa Kelce
            if(powerUp.type == 4){
                kelce_pup = true ;
                //Posiciona o Kelce
                kelce = (Rectangle){200,screenHeight-175,175,175};
            }
        }
        //Se o último tiro sair da tela, o Helicóptero começa a sair
        if (shot[9].posit.x > 1200) BadB.x -= tubeSpeed;
        //Se a pontuação for igual a 10, 30, 50, assim por diante
        if (score % POWER_UP_SPACE == 10){
            //Desativa os powerUps
            invisible_string = false;
            golfe = false;
            bad_blood = false;
            kelce_pup = false;
            //Tira o helicóptero, os tiros e o Kelce da tela
            BadB.x -= tubeSpeed;
            for(int j=0;j<10; j++){
                    shot[j].posit.x = -20;
                }
            kelce.x -= tubeSpeed;
        }
    }
}

void over_logic(void){
    //Se apertar o botão de restart
    if (CheckCollisionPointRec(mouse.mousePoint, restartBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //Inicializa o Primeiro Tubo.
            var_gap = gap;
            var_alt = dif_max_altura;
            tubes[0].gap = var_gap;
            tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap, var_alt), TUBE_WIDTH, TUBE_HEIGHT};
            tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
            tubes[0].valid = true;
            tubes[0].respawn = false;
            //Muda pra tela Start
            currentScreen = START;

        }
    }
    //Se o botão do Menu for clicado, vai pro Menu
    if (CheckCollisionPointRec(mouse.mousePoint, menuBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = MENU;

        }
    }
}

void menu_logic(void){
    //Checa a colisão entre o botão de Jogo e o mouse e caso clicar
    if (CheckCollisionPointRec(mouse.mousePoint, playBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //Inicializa o primeiro tubo
            var_gap = gap;
            var_alt = dif_max_altura;
            tubes[0].gap = var_gap;
            tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap, var_alt), TUBE_WIDTH, TUBE_HEIGHT};
            tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
            tubes[0].valid = true;
            tubes[0].respawn = true;
            //Muda pra tela start
            currentScreen = START;

        }
    }
    //Checa a colisão entre o botão de Dificuldade e o mouse e caso clicar
    if (CheckCollisionPointRec(mouse.mousePoint, diffBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = DIFF;


        }
    }
    //Checa a colisão entre o botão de Rank e o mouse e caso clicar
    if (CheckCollisionPointRec(mouse.mousePoint, rankBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = RANK;


        }
    }
    //Checa a colisão entre o botão de Exit e o mouse e caso clicar
    if (CheckCollisionPointRec(mouse.mousePoint, exitBtn)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            rodando = false;
        }
    }
    //Checa a colisão entre o botão de PowerUp e o mouse e caso clicar
    if (CheckCollisionPointRec(mouse.mousePoint, btnPU)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //Se os powerups estiverem ligados, desliga ao clicar
            if(powerUp.mode == true) {
                powerUp.mode = false;
            //Se estiverem desligados, liga ao clicar
            }else powerUp.mode = true;
        }
    }
    //Checa a colisão entre o botão de Créditos e o mouse e caso clicar
    //if (CheckCollisionPointRec(mouse.mousePoint, btnCredits)){
    //    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    //            currentScreen = CREDITS;
    //    }
    //}
}

void start_logic(void){
    //Inicializa posições do jogo
    player.rec.y = 200;
    score = 0;
    aug_vel = 0;
    tubeSpeed = vel_obstaculos;
    for(int i=1;i<N_BARRA;i++){
        tubes[i].gap = var_gap;
        tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*(DIST_X_TUBES + TUBE_WIDTH),cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap, var_alt), TUBE_WIDTH , TUBE_HEIGHT };
        tubes[i].tubeDown.rec = (Rectangle){1160 + i*(DIST_X_TUBES + TUBE_WIDTH), tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };
        tubes[i].valid = true;
        tubes[i].respawn = true;
    }
    //Se apertar "SPACE" inicia o jogo
    if (IsKeyPressed(KEY_SPACE)){
        currentScreen = GAME;

    }

    //Inicialização do modo powerup
    if(powerUp.mode){
        //Inicializa os modos como falsos
        invisible_string = false;
        golfe = false;
        bad_blood = false;
        kelce_pup = false;
        powerUp.type = 0;
        //Inicializa o PowerUp
        powerUp.posit.x = -500;
        powerUp.posit.y = -500;
        //Inicializa os Tiros
        for(int j=0;j<10;j++){
            shot[j].posit.x = -20;
            shot[j].posit.y = -20;
        }
        //inicializa o helicóptero
        BadB = (Rectangle){-132,0,132,72};
        //inicializa o Kelce
        kelce = (Rectangle){-175,screenHeight-175,175,175};
    }
}

void difficulty_logic(void){
    //Se a dificuldade 1 for selecionada, atualiza os parâmetros do jogo
    if (CheckCollisionPointRec(mouse.mousePoint, btn1Diff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            diff = 1;
            le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
        }
    }
    //Se a dificuldade 2 for selecionada, atualiza os parâmetros do jogo
    if (CheckCollisionPointRec(mouse.mousePoint, btn2Diff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            diff = 2;
            le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
        }
    }
    //Se a dificuldade 3 for selecionada, atualiza os parâmetros do jogo
    if (CheckCollisionPointRec(mouse.mousePoint, btn3Diff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            diff = 3;
            le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
        }
    }
    //Se a dificuldade 4 for selecionada, atualiza os parâmetros do jogo
    if (CheckCollisionPointRec(mouse.mousePoint, btn4Diff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            diff = 4;
            le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
        }
    }
    //Se a dificuldade 5 for selecionada, atualiza os parâmetros do jogo
    if (CheckCollisionPointRec(mouse.mousePoint, btn5Diff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            diff = 5;
            le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
        }
    }
    //Se o botão de Voltar for selecionado, volta pro Menu.
    if (CheckCollisionPointRec(mouse.mousePoint, backDiff)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = MENU;


        }
    }
}

void rank_logic(void){
    //Se clicar no botão de voltar, volta pro Menu
    if (CheckCollisionPointRec(mouse.mousePoint, backRank)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = MENU;


        }
    }
}

void score_logic(void){
    //Se o mouse estiver em cima da barra, está ativo
    if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
    //se não não tá
    else mouseOnText = false;
    //Se o mouse está em cima da barra, então
    if (mouseOnText){
        //O mouse fica modo "Escrever"
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        int key = GetCharPressed();
        //Loop para pegar a proxima tecla
        while (key > 0){
            //Se a tecla pressionada for uma das aceitas e o tamanho estiver dentro dos conformes, guarda esse valor
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)){
                name[letterCount] = (char)key;
                name[letterCount+1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }
        //Se for apertado BackSpace, apaga a última char da string.
        if (IsKeyPressed(KEY_BACKSPACE)){
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }
    //O mouse fica normal
    }else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    //Contador de Frames pra uma barrinha q fica piscando quando o mouse tá em cima da caixa pra escrever
    if (mouseOnText) framesCounter++;
    else framesCounter = 0;
    //Quando o Player der ok, atualiza o rank dependendo da posição do player
    if (CheckCollisionPointRec(mouse.mousePoint, btnDone)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(score>top5[0].score){
                strcpy(top5[4].nome, top5[3].nome);
                top5[4].score = top5[3].score;
                strcpy(top5[3].nome, top5[2].nome);
                top5[3].score = top5[2].score;
                strcpy(top5[2].nome, top5[1].nome);
                top5[2].score = top5[1].score;
                strcpy(top5[1].nome, top5[0].nome);
                top5[1].score = top5[0].score;
                strcpy(top5[0].nome, name);
                top5[0].score = score;
            }else if(score>top5[1].score){
                strcpy(top5[4].nome, top5[3].nome);
                top5[4].score = top5[3].score;
                strcpy(top5[3].nome, top5[2].nome);
                top5[3].score = top5[2].score;
                strcpy(top5[2].nome, top5[1].nome);
                top5[2].score = top5[1].score;
                strcpy(top5[1].nome, name);
                top5[1].score = score;
            }else if(score>top5[2].score){
                strcpy(top5[4].nome, top5[3].nome);
                top5[4].score = top5[3].score;
                strcpy(top5[3].nome, top5[2].nome);
                top5[3].score = top5[2].score;
                strcpy(top5[2].nome, name);
                top5[2].score = score;
            }else if(score>top5[3].score){
                strcpy(top5[4].nome, top5[3].nome);
                top5[4].score = top5[3].score;
                strcpy(top5[3].nome, name);
                top5[3].score = score;
            }else if(score>top5[4].score){
                strcpy(top5[4].nome, name);
                top5[4].score = score;
            }
            salva_arquivo("ranking.bin", top5);
            name[0] = '\0';
            letterCount = 0;
            le_arquivo("ranking.bin", top5);
            //Vai pra tela de Game Over
            currentScreen = OVER;

        }
    }
}

void credits_logic(void){
    //Se o Player apertar em Voltar, volta pro Menu
    if (CheckCollisionPointRec(mouse.mousePoint, backRank)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentScreen = MENU;

        }
    }
}

void tick(void){
    mouse.mousePoint = GetMousePosition();
    //Dependendo do modo de tela, seleciona a lógica que será aplicada.
    switch(currentScreen)
    {
        //Se estiver no menu:
        case MENU:
        {
            menu_logic();

        }break;
        case START:
        {
            start_logic();

        }break;
        case GAME:
        {
            game_logic();

        }break;
        //Se for a tela de Game Over
        case OVER:
        {
            over_logic();

        }break;
        //Se for a tela de Dificuldades
        case DIFF:
        {
            difficulty_logic();

        }break;
        //Se for a tela de Rank
        case RANK:
        {
            rank_logic();

        }break;
        //Se for a Tela de Adicionar o Nome Ao Rank
        case SCORE:
        {
            score_logic();

        }break;
        case CREDITS:
        {
            credits_logic();

        }break;
        //Se por algum motivo sair das telas padrão do jogo, fechar.
        default: break;

    }
}

void init_program(void){

    le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);

    le_arquivo("ranking.bin", top5);

    texturas.FundoFolkloreTxt = LoadImage("resources/FundoFolklore.png");
    ImageResize(&texturas.FundoFolkloreTxt, screenWidth, screenHeight);
    texturas.FundoLoverTxt = LoadImage("resources/FundoLover.png");
    ImageResize(&texturas.FundoLoverTxt, screenWidth, screenHeight);
    texturas.Fundo1989Txt = LoadImage("resources/Fundo1989.png");
    ImageResize(&texturas.Fundo1989Txt, screenWidth, screenHeight);
    texturas.FundoFearlessTxt= LoadImage("resources/FundoFearless.png");
    ImageResize(&texturas.FundoFearlessTxt, screenWidth, screenHeight);
    texturas.FundoReputationTxt = LoadImage("resources/FundoReputation.png");
    ImageResize(&texturas.FundoReputationTxt, screenWidth, screenHeight);

    texturas.Name_Game = LoadTexture("resources/nome_jogo.png");
    texturas.PlayTx = LoadTexture("resources/Play.png");
    texturas.RankTx = LoadTexture("resources/Rank.png");
    texturas.DiffTx = LoadTexture("resources/Difficulty.png");
    texturas.ExitTx = LoadTexture("resources/Exit.png");
    texturas.Diff1Tx = LoadTexture("resources/Diff_1.png");
    texturas.Diff2Tx = LoadTexture("resources/Diff_2.png");
    texturas.Diff3Tx = LoadTexture("resources/Diff_3.png");
    texturas.Diff4Tx = LoadTexture("resources/Diff_4.png");
    texturas.Diff5Tx = LoadTexture("resources/Diff_5.png");
    texturas.RestartEGTx = LoadTexture("resources/Restart_EG.png");
    texturas.MenuEGTx = LoadTexture("resources/Menu_EG.png");
    texturas.VoltarTx = LoadTexture("resources/Voltar.png");
    texturas.FundoTx = LoadTexture("resources/Fundo.png");
    texturas.FundoFolkloreTx = LoadTextureFromImage(texturas.FundoFolkloreTxt);
    texturas.Fundo1989Tx = LoadTextureFromImage(texturas.Fundo1989Txt);
    texturas.FundoLoverTx = LoadTextureFromImage(texturas.FundoLoverTxt);
    texturas.FundoFearlessTx = LoadTextureFromImage(texturas.FundoFearlessTxt);
    texturas.FundoReputationTx = LoadTextureFromImage(texturas.FundoReputationTxt);
    texturas.GameOverTx = LoadTexture("resources/Game_Over.png");
    texturas.DiffTitleTx = LoadTexture("resources/Diff_titles.png");
    texturas.RankTitleTx = LoadTexture("resources/Rank_Title.png");
    texturas.TaylorTx = LoadTexture("resources/Taylor2.png");
    texturas.TaylorGolfeTx = LoadTexture("resources/TaylorGolfe.png");
    texturas.UpBarFolkTx = LoadTexture("resources/UpBarFolklore.png");
    texturas.DownBarFolkTx = LoadTexture("resources/DownBarFolklore.png");
    texturas.UpBar1989Tx = LoadTexture("resources/UpBar1989.png");
    texturas.DownBar1989Tx = LoadTexture("resources/DownBar1989.png");
    texturas.UpBarLoverTx = LoadTexture("resources/UpBarLover.png");
    texturas.DownBarLoverTx = LoadTexture("resources/DownBarLover.png");
    texturas.FearlessBarTx = LoadTexture("resources/BarFearless.png");
    texturas.RepBarTx = LoadTexture("resources/RepBar.png");
    texturas.Base_Rank = LoadTexture("resources/Base_Ranks.png");
    texturas.DoneTx = LoadTexture("resources/Done.png");
    texturas.ScrBackTx = LoadTexture("resources/Score_Back.png");
    texturas.BordaTx = LoadTexture("resources/Borda.png");
    texturas.PUONTx = LoadTexture("resources/powerupon.png");
    texturas.PUOFFTx = LoadTexture("resources/powerupoff.png");
    texturas.CreditsTx = LoadTexture("resources/Creditos.png");
    texturas.KelceTx = LoadTexture("resources/Kelce.png");
    texturas.BadBTX = LoadTexture("resources/BadBlood.png");
    texturas.CapaceteTx = LoadTexture("resources/Capacete.png");
    texturas.NoveloTx = LoadTexture("resources/Novelo.png");
    texturas.TacoTx = LoadTexture("resources/Taco.png");
    texturas.ArminhaTx = LoadTexture("resources/Arminha.png");

    mouse.mousePoint = (Vector2){0,0};

    powerUp.mode = false;
    powerUp.type = 0;

    player.rec = (Rectangle){ 200, 200, 33, 87 };
    player.cor = RAYWHITE;

}

int main(void){

    InitWindow(screenWidth, screenHeight, "Flappy Swift");

    InitAudioDevice();

    Music music = LoadMusicStream("resources/Music.mp3");
    music.looping = true;
    SetMusicVolume(music, 0.1);
    PlayMusicStream(music);


    init_program();

    SetTargetFPS(60);

    //Enquanto o Botão Exit ou X não forem apertados, o jogo roda.
    while ((rodando == true) && (!WindowShouldClose())){
        UpdateMusicStream(music);
        tick();
        render();
    }

    CloseWindow();

    return 0;
}


