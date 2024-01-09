#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUBE_WIDTH 80
#define TUBE_HEIGHT 800
#define N_BARRA 4
#define DIST_X_TUBES 300
#define MAX_INPUT_CHARS 40

typedef enum GameScreen { MENU, START, GAME, PAUSE, OVER, RANK, DIFF, SCORE, CREDITS } GameScreen;

typedef struct Player {
    Rectangle rec;
    Color cor;
    //Texture2D taylor;


}Player;

typedef struct Tube {
    Rectangle rec;
    //Texture2D pilar;

} Tube;

typedef struct Tubes {
    Tube tubeUp;
    Tube tubeDown;
    int gap;
    bool valid;
    bool respawn;

} Tubes;

char* guarda_valor_rank(char* string, char *restante){

    char val[41], rest[260];
    int i=0,j=0;
    while(string[i] != '\n'){
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
    char *retorno = val;

    return retorno;
}

//static int dist_alt = 100;

int cabe_tela(int pos_in, int gap, int dist_alt){
    int diff;
    int pos_out;
    diff = GetRandomValue(-dist_alt, dist_alt);
    pos_out = pos_in + diff;
    if (pos_out > -50 - gap){
        cabe_tela(pos_in, gap,dist_alt);
    }else{
        if(pos_out < -750){
        cabe_tela(pos_in, gap,dist_alt);
        }else{
            return pos_out;
            }
    }
}

int retorno(int i){
    int out;
    if(i == 0){
        out=(N_BARRA-1);
    }else{
        out = i-1;
    }return out;
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

    *um = guarda_valor(myString, &myString);
    *dois = guarda_valor(myString, &myString);
    *tres = guarda_valor(myString, &myString);
    *quatro = guarda_valor(myString, &myString);
    *cinco = guarda_valor(myString, &myString);
    *seis = guarda_valor(myString, &myString);
    *sete = atoi(myString);
}

int main(void){

    int score_threshold,gap,inc_gap,dif_max_altura,inc_dif_max_altura,vel_obstaculos,inc_vel_obstaculos, diff = 1;
    le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
    int var_gap, var_alt;

    char out[270], rank1_nome[42], rank1_ponto[12], rank2_nome[42], rank2_ponto[12], rank3_nome[42], rank3_ponto[12], rank4_nome[42], rank4_ponto[12], rank5_nome[42], rank5_ponto[12], nova_pont[12];
    int p1, p2, p3, p4, p5;
    char backspc = '\n';
    FILE *write_ptr;
    FILE *ptr;

    ptr = fopen("ranking.bin","rb");
    fread(out,270,1,ptr);
    fclose(ptr);

    strcpy(rank1_nome, guarda_valor_rank(out, &out));
    strcpy(rank1_ponto, guarda_valor_rank(out, &out));
    strcpy(rank2_nome, guarda_valor_rank(out, &out));
    strcpy(rank2_ponto, guarda_valor_rank(out, &out));
    strcpy(rank3_nome, guarda_valor_rank(out, &out));
    strcpy(rank3_ponto, guarda_valor_rank(out, &out));
    strcpy(rank4_nome, guarda_valor_rank(out, &out));
    strcpy(rank4_ponto, guarda_valor_rank(out, &out));
    strcpy(rank5_nome, guarda_valor_rank(out, &out));
    strcpy(rank5_ponto, guarda_valor_rank(out, &out));

    p1 = atoi(rank1_ponto);
    p2 = atoi(rank2_ponto);
    p3 = atoi(rank3_ponto);
    p4 = atoi(rank4_ponto);
    p5 = atoi(rank5_ponto);

    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Teste Joguinho");

    Texture2D Name_Game = LoadTexture("resources/nome_jogo.png");
    Texture2D PlayTx = LoadTexture("resources/Play.png");
    Texture2D RankTx = LoadTexture("resources/Rank.png");
    Texture2D DiffTx = LoadTexture("resources/Difficulty.png");
    Texture2D ExitTx = LoadTexture("resources/Exit.png");
    Texture2D Diff1Tx = LoadTexture("resources/Diff_1.png");
    Texture2D Diff2Tx = LoadTexture("resources/Diff_2.png");
    Texture2D Diff3Tx = LoadTexture("resources/Diff_3.png");
    Texture2D Diff4Tx = LoadTexture("resources/Diff_4.png");
    Texture2D Diff5Tx = LoadTexture("resources/Diff_5.png");
    Texture2D RestartEGTx = LoadTexture("resources/Restart_EG.png");
    Texture2D MenuEGTx = LoadTexture("resources/Menu_EG.png");
    Texture2D VoltarTx = LoadTexture("resources/Voltar.png");
    Texture2D FundoTx = LoadTexture("resources/Fundo.png");
    Texture2D FundoFolkloreTx = LoadTexture("resources/Folklore_Background.png");
    Texture2D Fundo1989Tx = LoadTexture("resources/Fundo1989.png");
    Texture2D FundoLoverTx = LoadTexture("resources/FundoLover.png");
    Texture2D FundoFearlessTx = LoadTexture("resources/FundoFearless.png");
    Texture2D FundoReputationTx = LoadTexture("resources/FundoRep.png");
    Texture2D GameOverTx = LoadTexture("resources/Game_Over.png");
    Texture2D DiffTitleTx = LoadTexture("resources/Diff_titles.png");
    Texture2D RankTitleTx = LoadTexture("resources/Rank_Title.png");
    Texture2D TaylorTx = LoadTexture("resources/Taylor2.png");
    Texture2D UpBarTx = LoadTexture("resources/Up_Bar.png");
    Texture2D DownBarTx = LoadTexture("resources/Down_Bar.png");
    Texture2D Base_Rank = LoadTexture("resources/Base_Ranks.png");
    Texture2D DoneTx = LoadTexture("resources/Done.png");
    Texture2D ScrBackTx = LoadTexture("resources/Score_Back.png");
    Texture2D BordaTx = LoadTexture("resources/Borda.png");
    Texture2D PUONTx = LoadTexture("resources/powerupon.png");
    Texture2D PUOFFTx = LoadTexture("resources/powerupoff.png");
    Texture2D CreditsTx = LoadTexture("resources/Creditos.png");

    Player player;
    Tubes tubes[N_BARRA] = {0};
    int tubeSpeed = 0;
    int score = 0;
    int aug_vel = 0;
    int q_rcrs = 0;
    char string_score[8] = {'0'};
    int grav = 15;

    GameScreen currentScreen = MENU;

    Rectangle backMenu = { 0, 0, screenWidth, screenHeight };
    Rectangle playBtn = { screenWidth/2 - 150, screenHeight/2 - 110, 300, 100 };
    Rectangle diffBtn = { screenWidth/2 - 150, screenHeight/2, 300, 100 };
    Rectangle rankBtn = { screenWidth/2 - 150, screenHeight/2 + 110, 300, 100 };
    Rectangle exitBtn = { screenWidth/2 - 150, screenHeight/2 + 220, 300, 100 };
    Rectangle menuBtn = { screenWidth/2.0f +70, screenHeight/2.0f + 20, 80, 40 };
    Rectangle restartBtn = { screenWidth/2.0f - 140, screenHeight/2.0f + 20, 80, 40 };
    Rectangle backRank = {screenWidth/2 - 20, 720 , 40, 40};
    Rectangle backDiff = {screenWidth/2 - 20, 740 , 40, 40};
    Rectangle btn1Diff = {screenWidth/2 - 250, screenHeight/2 - 225 , 500, 100};
    Rectangle btn2Diff = {screenWidth/2 - 250, screenHeight/2 - 115 , 500, 100};
    Rectangle btn3Diff = {screenWidth/2 - 250, screenHeight/2 - 5, 500, 100};
    Rectangle btn4Diff = {screenWidth/2 - 250, screenHeight/2 + 105 , 500, 100};
    Rectangle btn5Diff = {screenWidth/2 - 250, screenHeight/2 + 215 , 500, 100};
    Rectangle btnDone = {screenWidth/2 - 40, screenHeight/2 + 100 , 80, 40};
    Rectangle btnPU = {50, 688 , 100, 75};
    Rectangle btnCredits = {1050, 688 , 100, 75};

    bool rodando = true;

    Vector2 mousePoint = { 0.0f, 0.0f };

    player.rec = (Rectangle){ 200, 200, 33, 87 };
    player.cor = RAYWHITE;
    tubeSpeed = vel_obstaculos;

    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    Rectangle textBox = { screenWidth/2 - 500 , screenHeight/2 -20 , 1000, 40 };
    bool mouseOnText = false;

    int framesCounter = 0;

    bool powerup = false;

    SetTargetFPS(60);


    while (rodando == true){

        mousePoint = GetMousePosition();

        switch(currentScreen)
        {
            case MENU:
            {


                if (CheckCollisionPointRec(mousePoint, playBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        player.rec.y = 200;
                        score = 0;
                        aug_vel = 0;
                        var_gap = gap;
                        var_alt = dif_max_altura;
                        player.rec.y = 200;
                        tubes[0].gap = var_gap;
                        tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap, var_alt), TUBE_WIDTH, TUBE_HEIGHT};
                        tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
                        tubes[0].valid = true;
                        tubes[0].respawn = true;
                        for(int i=1;i<N_BARRA;i++){
                            tubes[i].gap = var_gap;
                            tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*(DIST_X_TUBES + TUBE_WIDTH),cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap, var_alt), TUBE_WIDTH , TUBE_HEIGHT };

                            tubes[i].tubeDown.rec = (Rectangle){1160 + i*(DIST_X_TUBES + TUBE_WIDTH), tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };

                            tubes[i].valid = true;
                            tubes[i].respawn = true;
                        }
                        currentScreen = START;
                        break;

                    }
                }
                if (CheckCollisionPointRec(mousePoint, diffBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

                        currentScreen = DIFF;
                        break;

                    }
                }
                if (CheckCollisionPointRec(mousePoint, rankBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

                        currentScreen = RANK;
                        break;

                    }
                }
                if (CheckCollisionPointRec(mousePoint, exitBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        rodando = false;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, btnPU)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        if(powerup) powerup = false;
                        else powerup = true;

                    }
                }

                //if (CheckCollisionPointRec(mousePoint, btnCredits)){
                    //if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    //}
                //}

                score = 0;
                aug_vel = 0;
                sprintf(string_score, "%d", 0);


            }break;
            case START:
            {
                player.rec.y = 200;
                score = 0;
                aug_vel = 0;
                var_gap = gap;
                var_alt = dif_max_altura;
                tubeSpeed = vel_obstaculos;
                if (IsKeyPressed(KEY_SPACE)){
                    currentScreen = GAME;
                    break;
                }
                sprintf(string_score, "%d", 0);



            }break;
            case GAME:
            {
                if (IsKeyPressed(KEY_SPACE) && player.rec.y > 0) {
                       grav = 50;
                }
                if(grav>0){
                    if(grav > 40){
                        grav--;
                        player.rec.y -= 9;
                    }else{
                        if(grav > 35){
                            grav--;
                            player.rec.y -= 4;
                        }else{
                            if(grav > 30){
                                grav--;
                            }else{
                                if(grav > 23){
                                     player.rec.y += 3;
                                     grav--;
                                }else{
                                    grav--;
                                    player.rec.y += 7;
                                }

                            }

                        }

                    }

                }else{
                    player.rec.y += 10;
                }

                for(int i=0;i<N_BARRA;i++){

                    tubes[i].tubeUp.rec.x -= tubeSpeed ;
                    tubes[i].tubeDown.rec.x -= tubeSpeed ;

                    if(tubes[i].tubeUp.rec.x < -TUBE_WIDTH && tubes[i].respawn == true){
                        tubes[i].respawn = false;
                        tubes[i].gap = var_gap;
                        tubes[i].tubeUp.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
                        tubes[i].tubeDown.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
                        tubes[i].tubeUp.rec.y = cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap, var_alt);
                        tubes[i].tubeDown.rec.y = tubes[i].tubeUp.rec.y + TUBE_HEIGHT + tubes[i].gap ;
                        tubes[i].valid = true;

                    }
                    if(tubes[i].tubeUp.rec.x > -40 && tubes[i].respawn == false){
                        tubes[i].respawn = true;

                    }
                    if(CheckCollisionRecs(player.rec, tubes[i].tubeUp.rec) || CheckCollisionRecs(player.rec, tubes[i].tubeDown.rec)){
                        if(score > p5){
                            currentScreen = SCORE;
                            break;

                        }else{
                            currentScreen = OVER;
                            break;

                        }
                    }
                    if (player.rec.x > tubes[i].tubeUp.rec.x +20 && tubes[i].valid == true){
                        score++;
                        aug_vel++;
                        q_rcrs = 0;
                        tubes[i].valid = false;
                        sprintf(string_score, "%d", score);
                    }
                }

                    if(aug_vel == (score_threshold - N_BARRA)) {
                        if(q_rcrs == 0){
                            var_gap -= inc_gap;
                            var_alt += inc_dif_max_altura;
                            q_rcrs++;
                        }
                    }
                    if(aug_vel == score_threshold ) {
                        tubeSpeed += inc_vel_obstaculos;
                        aug_vel = 0;
                    }

                    if (player.rec.y > 724){
                        if(player.rec.y > 724) player.rec.y = 720;
                        if(score > p5){
                            currentScreen = SCORE;
                            break;

                        }else{
                            currentScreen = OVER;
                            break;

                        }
                    }
                    if (player.rec.y < 0)player.rec.y = 0;

            }break;
            case OVER:
            {

                if (CheckCollisionPointRec(mousePoint, restartBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        var_gap = gap;
                        var_alt = dif_max_altura;
                        tubeSpeed = vel_obstaculos;
                        tubes[0].gap = var_gap;
                        tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap, var_alt), TUBE_WIDTH, TUBE_HEIGHT};
                        tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
                        tubes[0].valid = true;
                        tubes[0].respawn = false;
                        for(int i=1;i<N_BARRA;i++){
                            tubes[i].gap = var_gap;
                            tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*(DIST_X_TUBES + TUBE_WIDTH), cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap, var_alt), TUBE_WIDTH , TUBE_HEIGHT };
                            tubes[i].tubeDown.rec = (Rectangle){1160 + i*(DIST_X_TUBES + TUBE_WIDTH), tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };
                            tubes[i].valid = true;
                            tubes[i].respawn = false;
                        }
                        currentScreen = START;
                        break;

                    }
                }
                if (CheckCollisionPointRec(mousePoint, menuBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        player.rec.y = 200;
                        currentScreen = MENU;
                        break;

                    }
                }


            }break;
            case DIFF:
            {
                if (CheckCollisionPointRec(mousePoint, btn1Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 1;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                if (CheckCollisionPointRec(mousePoint, btn2Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 2;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                if (CheckCollisionPointRec(mousePoint, btn3Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 3;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                if (CheckCollisionPointRec(mousePoint, btn4Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 4;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                if (CheckCollisionPointRec(mousePoint, btn5Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 5;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                if (CheckCollisionPointRec(mousePoint, backDiff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;

                    }
                }
            }break;
            case RANK:
            {
                if (CheckCollisionPointRec(mousePoint, backRank)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;

                    }
                }

            }break;

            case SCORE:
            {
                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                else mouseOnText = false;



                if (mouseOnText){
                    // Set the window's cursor to the I-Beam
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);

                    // Get char pressed (unicode character) on the queue
                    int key = GetCharPressed();

                    // Check if more characters have been pressed on the same frame
                    while (key > 0){
                        // NOTE: Only allow keys in range [32..125]
                        if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)){
                            name[letterCount] = (char)key;
                            name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                            letterCount++;
                        }

                        key = GetCharPressed();  // Check next character in the queue
                    }

                    if (IsKeyPressed(KEY_BACKSPACE)){
                        letterCount--;
                        if (letterCount < 0) letterCount = 0;
                        name[letterCount] = '\0';
                    }
                }else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnText) framesCounter++;
                else framesCounter = 0;

                if (CheckCollisionPointRec(mousePoint, btnDone)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            memset(out,0,strlen(out));
                            itoa(p1, rank1_ponto, 10);
                            strncat(rank1_ponto, &backspc, 1);
                            itoa(p2, rank2_ponto, 10);
                            strncat(rank2_ponto, &backspc, 1);
                            itoa(p3, rank3_ponto, 10);
                            strncat(rank3_ponto, &backspc, 1);
                            itoa(p4, rank4_ponto, 10);
                            strncat(rank4_ponto, &backspc, 1);
                            strncat(rank1_nome, &backspc, 1);
                            strncat(rank2_nome, &backspc, 1);
                            strncat(rank3_nome, &backspc, 1);
                            strncat(rank4_nome, &backspc, 1);
                            if(score > p4){
                                if(score > p3){
                                    if(score>p2){
                                        if(score>p1){
                                            strcat(out, name);
                                            strncat(out, &backspc, 1);
                                            strcat(out, itoa(score, nova_pont, 10));
                                            strncat(out, &backspc, 1);
                                            strcat(out, rank1_nome);
                                            strcat(out, rank1_ponto);
                                            strcat(out, rank2_nome);
                                            strcat(out, rank2_ponto);
                                            strcat(out, rank3_nome);
                                            strcat(out, rank3_ponto);
                                            strcat(out, rank4_nome);
                                            strcat(out, rank4_ponto);
                                            write_ptr = fopen("ranking.bin","wb");
                                            fwrite(out,sizeof(out),1,write_ptr);
                                            fclose(write_ptr);
                                        }else{
                                            strcat(out, rank1_nome);
                                            strcat(out, rank1_ponto);
                                            strcat(out, name);
                                            strncat(out, &backspc, 1);
                                            strcat(out, itoa(score, nova_pont, 10));
                                            strncat(out, &backspc, 1);
                                            strcat(out, rank2_nome);
                                            strcat(out, rank2_ponto);
                                            strcat(out, rank3_nome);
                                            strcat(out, rank3_ponto);
                                            strcat(out, rank4_nome);
                                            strcat(out, rank4_ponto);
                                            write_ptr = fopen("ranking.bin","wb");
                                            fwrite(out,sizeof(out),1,write_ptr);
                                            fclose(write_ptr);
                                        }
                                    }else{

                                        strcat(out, rank1_nome);
                                        strcat(out, rank1_ponto);
                                        strcat(out, rank2_nome);
                                        strcat(out, rank2_ponto);
                                        strcat(out, name);
                                        strncat(out, &backspc, 1);
                                        strcat(out, itoa(score, nova_pont, 10));
                                        strncat(out, &backspc, 1);
                                        strcat(out, rank3_nome);
                                        strcat(out, rank3_ponto);
                                        strcat(out, rank4_nome);
                                        strcat(out, rank4_ponto);
                                        write_ptr = fopen("ranking.bin","wb");
                                        fwrite(out,sizeof(out),1,write_ptr);
                                        fclose(write_ptr);
                                    }
                                }else{

                                    strcat(out, rank1_nome);
                                    strcat(out, rank1_ponto);
                                    strcat(out, rank2_nome);
                                    strcat(out, rank2_ponto);
                                    strcat(out, rank3_nome);
                                    strcat(out, rank3_ponto);
                                    strcat(out, name);
                                    strncat(out, &backspc, 1);
                                    strcat(out, itoa(score, nova_pont, 10));
                                    strncat(out, &backspc, 1);
                                    strcat(out, rank4_nome);
                                    strcat(out, rank4_ponto);
                                    write_ptr = fopen("ranking.bin","wb");
                                    fwrite(out,sizeof(out),1,write_ptr);
                                    fclose(write_ptr);
                                }
                            }else{
                                strcat(out, rank1_nome);
                                strcat(out, rank1_ponto);
                                strcat(out, rank2_nome);
                                strcat(out, rank2_ponto);
                                strcat(out, rank3_nome);
                                strcat(out, rank3_ponto);
                                strcat(out, rank4_nome);
                                strcat(out, rank4_ponto);
                                strcat(out, name);
                                strncat(out, &backspc, 1);
                                strcat(out, itoa(score, nova_pont, 10));
                                strncat(out, &backspc, 1);
                                write_ptr = fopen("ranking.bin","wb");
                                fwrite(out,sizeof(out),1,write_ptr);
                                fclose(write_ptr);
                            }


                        ptr = fopen("ranking.bin","rb");
                        fread(out,270,1,ptr);
                        fclose(ptr);

                        strcpy(rank1_nome, guarda_valor_rank(out, &out));
                        strcpy(rank1_ponto, guarda_valor_rank(out, &out));
                        strcpy(rank2_nome, guarda_valor_rank(out, &out));
                        strcpy(rank2_ponto, guarda_valor_rank(out, &out));
                        strcpy(rank3_nome, guarda_valor_rank(out, &out));
                        strcpy(rank3_ponto, guarda_valor_rank(out, &out));
                        strcpy(rank4_nome, guarda_valor_rank(out, &out));
                        strcpy(rank4_ponto, guarda_valor_rank(out, &out));
                        strcpy(rank5_nome, guarda_valor_rank(out, &out));
                        strcpy(rank5_ponto, guarda_valor_rank(out, &out));

                        p1 = atoi(rank1_ponto);
                        p2 = atoi(rank2_ponto);
                        p3 = atoi(rank3_ponto);
                        p4 = atoi(rank4_ponto);
                        p5 = atoi(rank5_ponto);

                        letterCount = 0;
                        name[0] = '\0';

                        currentScreen = OVER;

                        break;

                    }
                }


            }break;
            default: break;

        }

    BeginDrawing();

        switch(currentScreen){

            case MENU:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(Name_Game, (Rectangle){0,0,800,200}, (Vector2){screenWidth/2 - 400, 50}, RAYWHITE);
                DrawTextureRec(PlayTx, (Rectangle){0,0,playBtn.width,playBtn.height}, (Vector2){playBtn.x, playBtn.y}, RAYWHITE);
                DrawTextureRec(DiffTx, (Rectangle){0,0,diffBtn.width,diffBtn.height}, (Vector2){diffBtn.x, diffBtn.y}, RAYWHITE);
                DrawTextureRec(RankTx, (Rectangle){0,0,rankBtn.width,rankBtn.height}, (Vector2){rankBtn.x, rankBtn.y}, RAYWHITE);
                DrawTextureRec(ExitTx, (Rectangle){0,0,exitBtn.width,exitBtn.height}, (Vector2){exitBtn.x, exitBtn.y}, RAYWHITE);

                if(powerup) DrawTextureRec(PUONTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
                else DrawTextureRec(PUOFFTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
                DrawTextureRec(CreditsTx, (Rectangle){0,0,100,75}, (Vector2){1050, 688}, RAYWHITE);



            } break;

            case START:{
                ClearBackground(RAYWHITE);
                if(diff == 1) DrawTextureRec(FundoFolkloreTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 2) DrawTextureRec(Fundo1989Tx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 3) DrawTextureRec(FundoLoverTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 4) DrawTextureRec(FundoFearlessTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 5) DrawTextureRec(FundoReputationTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                int i =0;
                while(i<N_BARRA){
                    DrawTextureRec(UpBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);

                    DrawTextureRec(DownBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    i++;
                }
                i=0;
                ClearBackground(RAYWHITE);
                DrawText("Press Space", 480, 380, 20, GRAY);
            }break;

            case GAME:
            {
                ClearBackground(RAYWHITE);
                if(diff == 1) DrawTextureRec(FundoFolkloreTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 2) DrawTextureRec(Fundo1989Tx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 3) DrawTextureRec(FundoLoverTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 4) DrawTextureRec(FundoFearlessTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 5) DrawTextureRec(FundoReputationTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);

                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                int i =0;
                while(i<N_BARRA){

                    DrawTextureRec(UpBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);

                    DrawTextureRec(DownBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    i++;
                }
                i=0;
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);


            } break;
            case OVER:
            {
                ClearBackground(RAYWHITE);
               if(diff == 1) DrawTextureRec(FundoFolkloreTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 2) DrawTextureRec(Fundo1989Tx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 3) DrawTextureRec(FundoLoverTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 4) DrawTextureRec(FundoFearlessTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 5) DrawTextureRec(FundoReputationTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);

                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);

                int i =0;
                while(i<N_BARRA){
                    DrawTextureRec(UpBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);

                    DrawTextureRec(DownBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    i++;
                }
                i=0;
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);

                ClearBackground(RAYWHITE);
                DrawTextureRec(GameOverTx, (Rectangle){0,0,400, 200}, (Vector2){screenWidth/2.0f - 200,screenHeight/2.0f - 100}, RAYWHITE);
                DrawTextureRec(RestartEGTx, (Rectangle){0,0,restartBtn.width,restartBtn.height}, (Vector2){restartBtn.x, restartBtn.y}, RAYWHITE);
                DrawTextureRec(MenuEGTx, (Rectangle){0,0,menuBtn.width,menuBtn.height}, (Vector2){menuBtn.x, menuBtn.y}, RAYWHITE);
            }break;
            case DIFF:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(VoltarTx, (Rectangle){0,0,backDiff.width,backDiff.height}, (Vector2){backDiff.x, backDiff.y}, RAYWHITE);
                DrawRectangle(screenWidth/2 - 300, 50, 600, 100 , RED);
                DrawTextureRec(DiffTitleTx, (Rectangle){0,0,600, 100}, (Vector2){screenWidth/2.0f - 300,50}, RAYWHITE);
                DrawTextureRec(Diff1Tx, (Rectangle){0,0,btn1Diff.width,btn1Diff.height}, (Vector2){btn1Diff.x, btn1Diff.y}, RAYWHITE);
                DrawTextureRec(Diff2Tx, (Rectangle){0,0,btn2Diff.width,btn2Diff.height}, (Vector2){btn2Diff.x, btn2Diff.y}, RAYWHITE);
                DrawTextureRec(Diff3Tx, (Rectangle){0,0,btn3Diff.width,btn3Diff.height}, (Vector2){btn3Diff.x, btn3Diff.y}, RAYWHITE);
                DrawTextureRec(Diff4Tx, (Rectangle){0,0,btn4Diff.width,btn4Diff.height}, (Vector2){btn4Diff.x, btn4Diff.y}, RAYWHITE);
                DrawTextureRec(Diff5Tx, (Rectangle){0,0,btn1Diff.width,btn5Diff.height}, (Vector2){btn5Diff.x, btn5Diff.y}, RAYWHITE);
                if(diff == 1) DrawTextureRec(BordaTx, (Rectangle){0,0,510,110}, (Vector2){screenWidth/2 - 255, screenHeight/2 - 230}, RAYWHITE);
                if(diff == 2) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 - 120}, RAYWHITE);
                if(diff == 3) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 - 10}, RAYWHITE);
                if(diff == 4) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 + 100}, RAYWHITE);
                if(diff == 5) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 + 210}, RAYWHITE);

            }break;
            case RANK:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(VoltarTx, (Rectangle){0,0,backRank.width,backRank.height}, (Vector2){backRank.x, backRank.y}, RAYWHITE);
                DrawTextureRec(RankTitleTx, (Rectangle){0,0,500, 100}, (Vector2){screenWidth/2.0f - 250,50}, RAYWHITE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 180}, RAYWHITE);
                DrawText(rank1_nome, screenWidth/2 - 490, screenHeight/2 - 170, 30, BLACK);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 135, 30, BLACK);
                DrawText(rank1_ponto, screenWidth/2 - 370, screenHeight/2 - 135, 30, BLACK);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 90}, RAYWHITE);
                DrawText(rank2_nome, screenWidth/2 - 490, screenHeight/2 - 80, 30, BLACK);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 45, 30, BLACK);
                DrawText(rank2_ponto, screenWidth/2 - 370, screenHeight/2 - 45, 30, BLACK);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2}, RAYWHITE);
                DrawText(rank3_nome, screenWidth/2 - 490, screenHeight/2 + 10, 30, BLACK);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 45, 30, BLACK);
                DrawText(rank3_ponto, screenWidth/2 - 370, screenHeight/2 + 45, 30, BLACK);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 90}, RAYWHITE);
                DrawText(rank4_nome, screenWidth/2 - 490, screenHeight/2 + 100, 30, BLACK);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 135, 30, BLACK);
                DrawText(rank4_ponto, screenWidth/2 - 370, screenHeight/2 + 135, 30, BLACK);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 180}, RAYWHITE);
                DrawText(rank5_nome, screenWidth/2 - 490, screenHeight/2 + 190, 30, BLACK);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 225, 30, BLACK);
                DrawText(rank5_ponto, screenWidth/2 - 370, screenHeight/2 + 225, 30, BLACK);


            }break;
            case SCORE:
            {
                ClearBackground(RAYWHITE);
                if(diff == 1) DrawTextureRec(FundoFolkloreTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 2) DrawTextureRec(Fundo1989Tx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 3) DrawTextureRec(FundoLoverTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 4) DrawTextureRec(FundoFearlessTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                if(diff == 5) DrawTextureRec(FundoReputationTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);

                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);

                int i =0;
                while(i<N_BARRA){
                    DrawTextureRec(UpBarTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);

                    DrawTextureRec(DownBarTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    i++;
                }
                i=0;
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);


                DrawTextureRec(ScrBackTx, (Rectangle){0,0,1100, 500}, (Vector2){screenWidth/2 - 550,screenHeight/2 - 300}, RAYWHITE);


                DrawRectangleRec(textBox, LIGHTGRAY);
                if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, (Color){ 255, 109, 194, 255 });
                else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

                DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 30, (Color){ 255, 109, 194, 255 });

                if (mouseOnText){
                    if (letterCount < MAX_INPUT_CHARS){
                        if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 30), (int)textBox.y + 12, 30, (Color){ 255, 109, 194, 255 });
                    }
                }
                DrawTextureRec(DoneTx, (Rectangle){0,0,btnDone.width,btnDone.height}, (Vector2){btnDone.x, btnDone.y}, RAYWHITE);
            }break;
            default: break;
        }


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
