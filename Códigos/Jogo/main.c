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
#define POWER_UP_SIZE 33

//Numeração de Estados da Tela
typedef enum GameScreen { MENU, START, GAME, PAUSE, OVER, RANK, DIFF, SCORE, CREDITS } GameScreen;
//Estrutura do Player
typedef struct Player {
    Rectangle rec;
    Color cor;

}Player;
//Estrutura do Tubo
typedef struct Tube {
    Rectangle rec;

} Tube;
//Estrutura de dois tubos conjuntos
typedef struct Tubes {
    Tube tubeUp;
    Tube tubeDown;
    int gap;
    bool valid;
    bool respawn;

} Tubes;
//Estrutura do powerup
typedef struct PowerUp {
    Vector2 posit;
    float radius;
    bool mode;
    bool valid;
    bool respawn;
    int type;

} PowerUp;
//Estrutura dos Tiros
typedef struct Shot {
    Vector2 posit;
    float radius;
} Shot;
//Guarda as informações do rank
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
//Gera o posicionamento dentro dos limites da tela
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
//A partir da quantidade de Tubos predefinida, pega como referência um loop (se o máximo é 4, ent 4 pega 0 como referência, se nn é quatro, pega i-1)
int retorno(int i){
    int out;
    if(i == 0){
        out=(N_BARRA-1);
    }else{
        out = i-1;
    }return out;
}
//Guarda o valor da dificuldade em uma string
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
//Aplica as dificuldades.
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
//Gera os Fundos dependendo da dificuldade.
void desenha_fundo(int diff,  Texture2D t1, Texture2D t2, Texture2D t3, Texture2D t4, Texture2D t5){
    if(diff == 1) DrawTextureRec(t1, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 2) DrawTextureRec(t2, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 3) DrawTextureRec(t3, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 4) DrawTextureRec(t4, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
    if(diff == 5) DrawTextureRec(t5, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
}

int main(void){

    //Declaração e Inicialização das vaiáveis para leitura do documento de Dificuldade.
    int score_threshold,gap,inc_gap,dif_max_altura,inc_dif_max_altura,vel_obstaculos,inc_vel_obstaculos, diff = 1;
    le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
    int var_gap, var_alt;

    //Declaração de variáveis para manipulação de Rank.
    char out[270], rank1_nome[42], rank1_ponto[12], rank2_nome[42], rank2_ponto[12], rank3_nome[42], rank3_ponto[12], rank4_nome[42], rank4_ponto[12], rank5_nome[42], rank5_ponto[12], nova_pont[12], test5[3];
    int p1, p2, p3, p4, p5;
    char backspc = '\n';
    FILE *write_ptr;
    FILE *ptr;

    //Inicialização do conteúdo do arquivo de Rank.
    ptr = fopen("ranking.bin","rb");
    fread(out,270,1,ptr);
    fclose(ptr);

    //Passa as informações do documento de Rank para String
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

    //Passando os pontos do Rank para Int
    p1 = atoi(rank1_ponto);
    p2 = atoi(rank2_ponto);
    p3 = atoi(rank3_ponto);
    p4 = atoi(rank4_ponto);
    p5 = atoi(rank5_ponto);

    //Inicialização da Janela
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Teste Joguinho");

    // Inicialização das Imagens
    Image FundoFolkloreTxt = LoadImage("resources/FundoFolklore.png");
    ImageResize(&FundoFolkloreTxt, 1200, 800);
    Image FundoLoverTxt = LoadImage("resources/FundoLover.png");
    ImageResize(&FundoLoverTxt, 1200, 800);
    Image Fundo1989Txt = LoadImage("resources/Fundo1989.png");
    ImageResize(&Fundo1989Txt, 1200, 800);
    Image FundoFearlessTxt = LoadImage("resources/FundoFearless.png");
    ImageResize(&FundoFearlessTxt, 1200, 800);
    Image FundoReputationTxt = LoadImage("resources/FundoReputation.png");
    ImageResize(&FundoReputationTxt, 1200, 800);

    // Inicialização das Texturas
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
    Texture2D FundoFolkloreTx = LoadTextureFromImage(FundoFolkloreTxt);
    Texture2D Fundo1989Tx = LoadTextureFromImage(Fundo1989Txt);
    Texture2D FundoLoverTx = LoadTextureFromImage(FundoLoverTxt);
    Texture2D FundoFearlessTx = LoadTextureFromImage(FundoFearlessTxt);
    Texture2D FundoReputationTx = LoadTextureFromImage(FundoReputationTxt);
    Texture2D GameOverTx = LoadTexture("resources/Game_Over.png");
    Texture2D DiffTitleTx = LoadTexture("resources/Diff_titles.png");
    Texture2D RankTitleTx = LoadTexture("resources/Rank_Title.png");
    Texture2D TaylorTx = LoadTexture("resources/Taylor2.png");
    Texture2D UpBarFolkTx = LoadTexture("resources/UpBarFolklore.png");
    Texture2D DownBarFolkTx = LoadTexture("resources/DownBarFolklore.png");
    Texture2D UpBar1989Tx = LoadTexture("resources/UpBar1989.png");
    Texture2D DownBar1989Tx = LoadTexture("resources/DownBar1989.png");
    Texture2D UpBarLoverTx = LoadTexture("resources/UpBarLover.png");
    Texture2D DownBarLoverTx = LoadTexture("resources/DownBarLover.png");
    Texture2D Base_Rank = LoadTexture("resources/Base_Ranks.png");
    Texture2D DoneTx = LoadTexture("resources/Done.png");
    Texture2D ScrBackTx = LoadTexture("resources/Score_Back.png");
    Texture2D BordaTx = LoadTexture("resources/Borda.png");
    Texture2D PUONTx = LoadTexture("resources/powerupon.png");
    Texture2D PUOFFTx = LoadTexture("resources/powerupoff.png");
    Texture2D CreditsTx = LoadTexture("resources/Creditos.png");
    Texture2D KelceTx = LoadTexture("resources/Kelce.png");
    Texture2D BadBTX = LoadTexture("resources/BadBlood.png");

    //Inicialização das entidades do jogo
    Player player;
    Tubes tubes[N_BARRA] = {0};
    Shot shot[10] = {0};
    PowerUp powerUp;
    powerUp.mode = false;
    powerUp.type = 0;
    Rectangle BadB;
    Rectangle kelce;
    player.rec = (Rectangle){ 200, 200, 33, 87 };
    player.cor = RAYWHITE;

    //Declaração de Inteiros e Strings referência
    int tubeSpeed = 0;
    int score = 0;
    int aug_vel = 0;
    int q_rcrs = 0;
    char string_score[8] = {'0'};
    int grav = 15;
    int letterCount = 0;
    int framesCounter = 0;
    tubeSpeed = vel_obstaculos;
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int pup_spwn = 0;

    //Inicialização da Janela em modo Menu.
    GameScreen currentScreen = MENU;

    // Declaração dos botões do Menu.
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
    Rectangle textBox = { screenWidth/2 - 500 , screenHeight/2 -20 , 1000, 40 };

    //Declaração da inicialização do Mouse
    Vector2 mousePoint = { 0.0f, 0.0f };

    //Declaração de Booleanos de modo de jogo
    bool rodando = true;
    bool mouseOnText = false;
    bool powerup = false;
    bool invisible_string = false;
    bool golfe = false;
    bool bad_blood = false;
    bool kelce_pup = false;

    //Definição do FPS.
    SetTargetFPS(60);

    //Enquanto o Botão Exit ou X não forem apertados, o jogo roda.
    while ((rodando == true) && (!WindowShouldClose())){
        //Passa a posição do mouse como vetor
        mousePoint = GetMousePosition();
        //Dependendo do modo de tela, seleciona a lógica que será aplicada.
        switch(currentScreen)
        {
            //Se estiver no menu:
            case MENU:
            {
                //Checa a colisão entre o botão de Jogo e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, playBtn)){
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
                        break;

                    }
                }
                //Checa a colisão entre o botão de Dificuldade e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, diffBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = DIFF;
                        break;

                    }
                }
                //Checa a colisão entre o botão de Rank e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, rankBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = RANK;
                        break;

                    }
                }
                //Checa a colisão entre o botão de Exit e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, exitBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        rodando = false;
                    }
                }
                //Checa a colisão entre o botão de PowerUp e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, btnPU)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        //Se os powerups estiverem ligados, desliga ao clicar
                        if(powerUp.mode == true) {
                            powerUp.mode = false;
                        //Se estiverem desligados, liga ao clicar
                        }else powerUp.mode = true;
                    }
                }
                //Checa a colisão entre o botão de Créditos e o mouse e caso clicar
                if (CheckCollisionPointRec(mousePoint, btnCredits)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            currentScreen = CREDITS;
                    }
                }

            }break;
            case START:
            {
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
                    break;
                }
                //Converte os pontos para string
                sprintf(string_score, "%d", 0);
                //Inicialização do modo powerup
                if(powerUp.mode){
                    //Contador de Spawn
                    int pup_spwn = 0;
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

            }break;
            case GAME:
            {
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
                            if(score > p5){
                                currentScreen = SCORE;
                                break;
                            //Se não, Game Over
                            }else{
                                currentScreen = OVER;
                                break;

                            }
                        }
                    }
                    //Contagem de pontos. Se passou dos tubos, a pontuação soma (e alguns outros contadores que dependem da pontuação)
                    if (player.rec.x > tubes[i].tubeUp.rec.x +20 && tubes[i].valid == true){
                        score++;
                        aug_vel++;
                        q_rcrs = 0;
                        tubes[i].valid = false;
                        sprintf(string_score, "%d", score);
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
                        if(score > p5){
                            currentScreen = SCORE;
                            break;

                        }else{
                            currentScreen = OVER;
                            break;

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

            }break;
            //Se for a tela de Game Over
            case OVER:
            {
                //Se apertar o botão de restart
                if (CheckCollisionPointRec(mousePoint, restartBtn)){
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
                        break;

                    }
                }
                //Se o botão do Menu for clicado, vai pro Menu
                if (CheckCollisionPointRec(mousePoint, menuBtn)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;
                    }
                }



            }break;
            //Se for a tela de Dificuldades
            case DIFF:
            {
                //Se a dificuldade 1 for selecionada, atualiza os parâmetros do jogo
                if (CheckCollisionPointRec(mousePoint, btn1Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 1;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                //Se a dificuldade 2 for selecionada, atualiza os parâmetros do jogo
                if (CheckCollisionPointRec(mousePoint, btn2Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 2;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                //Se a dificuldade 3 for selecionada, atualiza os parâmetros do jogo
                if (CheckCollisionPointRec(mousePoint, btn3Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 3;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                //Se a dificuldade 4 for selecionada, atualiza os parâmetros do jogo
                if (CheckCollisionPointRec(mousePoint, btn4Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 4;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                //Se a dificuldade 5 for selecionada, atualiza os parâmetros do jogo
                if (CheckCollisionPointRec(mousePoint, btn5Diff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        diff = 5;
                        le_dificuldade(&score_threshold,&gap,&inc_gap,&dif_max_altura,&inc_dif_max_altura,&vel_obstaculos,&inc_vel_obstaculos, diff);
                    }
                }
                //Se o botão de Voltar for selecionado, volta pro Menu.
                if (CheckCollisionPointRec(mousePoint, backDiff)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;

                    }
                }
            }break;
            //Se for a tela de Rank
            case RANK:
            {
                //Se clicar no botão de voltar, volta pro Menu
                if (CheckCollisionPointRec(mousePoint, backRank)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;

                    }
                }

            }break;
            //Se for a Tela de Adicionar o Nome Ao Rank
            case SCORE:
            {
                //Se o mouse estiver em cima da barra, está ativo
                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                //se não não tá
                else mouseOnText = false;
                //Se o mouse está em cima da barra, então
                if (mouseOnText){
                    //O mouse fica modo "Escrever"
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    //Loop padrão da Raylib pra evitar erros
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
                if (CheckCollisionPointRec(mousePoint, btnDone)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            //Limpa a String inicial
                            memset(out,0,strlen(out));
                            //Manipula os dados do rank para organizar em uma string inteira
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
                            //Se o score for maior que o do quarto lugar
                            if(score > p4){
                                //Se o score for maior que o do terceiro lugar
                                if(score > p3){
                                    //Se o score for maior que o do segundo lugar
                                    if(score>p2){
                                        //Se o score for maior que o do primeiro lugar
                                        if(score>p1){
                                            //Reescreve o conteúdo do documento e o atualiza  colocando o player em primeiro lugar
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
                                        //Reescreve o conteúdo do documento e o atualiza  colocando o player em segundo lugar
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
                                    //Reescreve o conteúdo do documento e o atualiza  colocando o player em terceiro lugar
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
                                //Reescreve o conteúdo do documento e o atualiza  colocando o player em quarto lugar
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
                            //Reescreve o conteúdo do documento e o atualiza colocando o player em quinto lugar
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
                        //Abre o Documento Atualizado e coloca em o conteúdo em uma string padrão
                        ptr = fopen("ranking.bin","rb");
                        fread(out,270,1,ptr);
                        fclose(ptr);
                        //Atualiza os parâmetros do jogo a partir dessa atualização
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
                        //Zera o nome
                        name[0] = '\0';
                        letterCount = 0;
                        //Vai pra tela de Game Over
                        currentScreen = OVER;
                        break;
                    }
                }

            }break;
            case CREDITS:
            {
                //Se o Player apertar em Voltar, volta pro Menu
                if (CheckCollisionPointRec(mousePoint, backRank)){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                        break;
                    }
                }
            }break;
            //Se por algum motivo sair das telas padrão do jogo, fechar.
            default: break;

        }
    //Começa a renderização
    BeginDrawing();

        switch(currentScreen){
            //Desenha os botões do Menu
            case MENU:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(Name_Game, (Rectangle){0,0,800,200}, (Vector2){screenWidth/2 - 400, 50}, RAYWHITE);
                DrawTextureRec(PlayTx, (Rectangle){0,0,playBtn.width,playBtn.height}, (Vector2){playBtn.x, playBtn.y}, RAYWHITE);
                DrawTextureRec(DiffTx, (Rectangle){0,0,diffBtn.width,diffBtn.height}, (Vector2){diffBtn.x, diffBtn.y}, RAYWHITE);
                DrawTextureRec(RankTx, (Rectangle){0,0,rankBtn.width,rankBtn.height}, (Vector2){rankBtn.x, rankBtn.y}, RAYWHITE);
                DrawTextureRec(ExitTx, (Rectangle){0,0,exitBtn.width,exitBtn.height}, (Vector2){exitBtn.x, exitBtn.y}, RAYWHITE);
                if(powerUp.mode) DrawTextureRec(PUONTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
                else DrawTextureRec(PUOFFTx, (Rectangle){0,0,100,75}, (Vector2){50, 688}, RAYWHITE);
                DrawTextureRec(CreditsTx, (Rectangle){0,0,100,75}, (Vector2){1050, 688}, RAYWHITE);

            } break;
            //Desenha os componentes do Start
            case START:{
                ClearBackground(RAYWHITE);
                //Desenha os fundos de acordo com a dificuldade
                desenha_fundo(diff,FundoFolkloreTx,Fundo1989Tx,FundoLoverTx,FundoFearlessTx,FundoReputationTx);
                //Desenha a Taylor
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                int i =0;
                //Desenha as barras de acordo com a dificuldade
                while(i<N_BARRA){
                    if(diff == 1) {
                        DrawTextureRec(UpBarFolkTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarFolkTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 2) {
                        DrawTextureRec(UpBar1989Tx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBar1989Tx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }

                    if(diff == 3){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 4){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 5){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }

                    i++;
                }
                i=0;
                ClearBackground(RAYWHITE);
                //Desenha o Aviso para iniciar o jogo
                DrawText("Press Space", 480, 380, 20, RED);

            }break;
            //Desenha os componentes do Jogo
            case GAME:
            {
                ClearBackground(RAYWHITE);
                //Desenha o fundo de acordo com a dificuldade
                desenha_fundo(diff,FundoFolkloreTx,Fundo1989Tx,FundoLoverTx,FundoFearlessTx,FundoReputationTx);
                //Desenha as barras de acordo com a dificuldade
                int i =0;
                while(i<N_BARRA){
                    if(diff == 1) {
                        DrawTextureRec(UpBarFolkTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarFolkTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 2) {
                        DrawTextureRec(UpBar1989Tx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBar1989Tx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }

                    if(diff == 3){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 4){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 5){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    i++;
                }
                i=0;
                //Desenha os PowerUps
                if(powerUp.mode){
                    DrawCircleV(powerUp.posit, POWER_UP_SIZE, GREEN);
                    DrawText(itoa(powerUp.type,test5,5),powerUp.posit.x,powerUp.posit.y, 30, BLUE);
                }
                //Desenha a Taylor
                if(invisible_string) DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3},  CLITERAL(Color){ 255, 255, 255, 128 });
                else DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                //Desenha elementos BadBlood.
                for(int j = 0;j<10;j++){
                    DrawCircleV(shot[j].posit, shot[j].radius, YELLOW);
                }
                DrawTextureRec(BadBTX, (Rectangle){0,0,BadB.width,BadB.height},(Vector2){BadB.x,BadB.y},RAYWHITE);
                //Desenha elementos Kelce.
                DrawTextureRec(KelceTx, (Rectangle){0,0,kelce.width,kelce.height},(Vector2){kelce.x,kelce.y},RAYWHITE);
                //Desenha a pontuação
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);

            } break;
            case OVER:
            {
                ClearBackground(RAYWHITE);
                //Desenha o fundo de acordo com a dificuldade
                desenha_fundo(diff,FundoFolkloreTx,Fundo1989Tx,FundoLoverTx,FundoFearlessTx,FundoReputationTx);
                //Desenha a Taylor
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                //Desenha as barras de acordo com a dificuldade
                int i =0;
                while(i<N_BARRA){
                    if(diff == 1) {
                        DrawTextureRec(UpBarFolkTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarFolkTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 2) {
                        DrawTextureRec(UpBar1989Tx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBar1989Tx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }

                    if(diff == 3){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 4){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 5){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    i++;
                }
                i=0;
                //Desenha a pontuação
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);

                ClearBackground(RAYWHITE);
                //Desenha a barra de Game Over
                DrawTextureRec(GameOverTx, (Rectangle){0,0,400, 200}, (Vector2){screenWidth/2.0f - 200,screenHeight/2.0f - 100}, RAYWHITE);
                //Desenha o botão de Restart
                DrawTextureRec(RestartEGTx, (Rectangle){0,0,restartBtn.width,restartBtn.height}, (Vector2){restartBtn.x, restartBtn.y}, RAYWHITE);
                //Desenha o Botão de Menu
                DrawTextureRec(MenuEGTx, (Rectangle){0,0,menuBtn.width,menuBtn.height}, (Vector2){menuBtn.x, menuBtn.y}, RAYWHITE);
            }break;
            //Desenha os componentes das dificuldades
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
                //Desenha a barra dependendo da seleção
                if(diff == 1) DrawTextureRec(BordaTx, (Rectangle){0,0,510,110}, (Vector2){screenWidth/2 - 255, screenHeight/2 - 230}, RAYWHITE);
                if(diff == 2) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 - 120}, RAYWHITE);
                if(diff == 3) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 - 10}, RAYWHITE);
                if(diff == 4) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 + 100}, RAYWHITE);
                if(diff == 5) DrawTextureRec(BordaTx, (Rectangle){0,0,510, 110}, (Vector2){screenWidth/2 - 255,screenHeight/2 + 210}, RAYWHITE);

            }break;
            //Desenha os componentes do Ranking
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
            //Desenha os elementos da tela de Score
            case SCORE:
            {
                ClearBackground(RAYWHITE);
                //Desenha o Fundo de acordo com a dificuldade
                desenha_fundo(diff,FundoFolkloreTx,Fundo1989Tx,FundoLoverTx,FundoFearlessTx,FundoReputationTx);
                //Desenha a Taylor
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width+63,player.rec.height+3}, (Vector2){player.rec.x-45, player.rec.y-3}, RAYWHITE);
                //Desenha as Barras de acordo com a dificuldade
                int i =0;
                while(i<N_BARRA){
                    if(diff == 1) {
                        DrawTextureRec(UpBarFolkTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarFolkTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 2) {
                        DrawTextureRec(UpBar1989Tx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBar1989Tx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 3){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);

                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 4){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    if(diff == 5){
                        DrawTextureRec(UpBarLoverTx, (Rectangle){0,0,tubes[i].tubeUp.rec.width,tubes[i].tubeUp.rec.height}, (Vector2){tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y}, RAYWHITE);
                        DrawTextureRec(DownBarLoverTx, (Rectangle){0,0,tubes[i].tubeDown.rec.width,tubes[i].tubeDown.rec.height}, (Vector2){tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y}, RAYWHITE);
                    }
                    i++;
                }
                i=0;
                //Desenha a pontuaçãoo
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);
                //Desenhha o fundo de score
                DrawTextureRec(ScrBackTx, (Rectangle){0,0,1100, 500}, (Vector2){screenWidth/2 - 550,screenHeight/2 - 300}, RAYWHITE);
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
                DrawTextureRec(DoneTx, (Rectangle){0,0,btnDone.width,btnDone.height}, (Vector2){btnDone.x, btnDone.y}, RAYWHITE);
            }break;
            //Desenha os componentes da tela créditos
            case CREDITS:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(VoltarTx, (Rectangle){0,0,backRank.width,backRank.height}, (Vector2){backRank.x, backRank.y}, RAYWHITE);
            }break;
            //Se der algum problema e ir para uma tela inexistente, fecha.
            default: break;
        }
        //Finaliza a Renderização
        EndDrawing();
    }
    //Finaliza a janela
    CloseWindow();
    //Termina a execução
    return 0;
}
