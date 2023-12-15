#include "raylib.h"

#define TUBE_WIDTH 80
#define TUBE_HEIGHT 800
#define N_BARRA 10
#define DIST_X_TUBES 300

typedef enum GameScreen { MENU, START, GAME, PAUSE, OVER, RANK, DIFF } GameScreen;

typedef struct Player {
    Rectangle rec;
    Color cor;
    //Texture2D taylor;


}Player;

typedef struct Tube {
    Rectangle rec;
    Color cor;
    //Texture2D pilar;

} Tube;

typedef struct Tubes {
    Tube tubeUp;
    Tube tubeDown;
    int gap;
    bool valid;
    bool respawn;

} Tubes;

static int dist_alt =  250;

int cabe_tela(int pos_in, int gap){
    int diff;
    int pos_out;
    diff = GetRandomValue(-dist_alt, dist_alt);
    pos_out = pos_in + diff;
    if (pos_out > -50 - gap){
        cabe_tela(pos_in, gap);
    }else{
        if(pos_out < -750){
        cabe_tela(pos_in, gap);
        }else{
            return pos_out;
            }
    }
}
int retorno(int i){
    int out;
    if(i == 0){
        out=5;
    }else{
        out = i-1;
    }return out;
}



int main(void){

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
    Texture2D GameOverTx = LoadTexture("resources/Game_Over.png");
    Texture2D DiffTitleTx = LoadTexture("resources/Diff_titles.png");
    Texture2D RankTitleTx = LoadTexture("resources/Rank_Title.png");
    Texture2D TaylorTx = LoadTexture("resources/Taylor.png");
    Texture2D UpBarTx = LoadTexture("resources/Up_Bar.png");
    Texture2D DownBarTx = LoadTexture("resources/Down_Bar.png");
    Texture2D Base_Rank = LoadTexture("resources/Base_Ranks.png");

    Player player;
    Tubes tubes[N_BARRA] = {0};
    int tubeSpeed = 0;
    int score = 0;
    int aug_vel = 0;
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
    Rectangle backDiff = {screenWidth/2 - 20, 720 , 40, 40};
    Rectangle btn1Diff = {screenWidth/2 - 150, screenHeight/2 - 180 , 300, 80};
    Rectangle btn2Diff = {screenWidth/2 - 150, screenHeight/2 - 90 , 300, 80};
    Rectangle btn3Diff = {screenWidth/2 - 150, screenHeight/2, 300, 80};
    Rectangle btn4Diff = {screenWidth/2 - 150, screenHeight/2 + 90 , 300, 80};
    Rectangle btn5Diff = {screenWidth/2 - 150, screenHeight/2 + 180 , 300, 80};




    bool rodando = true;


    Vector2 mousePoint = { 0.0f, 0.0f };

    player.rec = (Rectangle){ 200, 200, 80, 80 };
    player.cor = RED;
    tubeSpeed = 5;


    SetTargetFPS(60);

    while (rodando == true){

        mousePoint = GetMousePosition();

        switch(currentScreen)
        {
            case MENU:
            {
                player.rec.y = 200;
                tubes[0].gap = 300;
                tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap), TUBE_WIDTH, TUBE_HEIGHT};
                tubes[0].tubeUp.cor = RED;
                tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
                tubes[0].tubeDown.cor = RED;
                tubes[0].valid = true;
                tubes[0].respawn = true;
                for(int i=1;i<N_BARRA;i++){
                    tubes[i].gap = 300;
                    tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*(DIST_X_TUBES + TUBE_WIDTH), cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap), TUBE_WIDTH , TUBE_HEIGHT };
                    tubes[i].tubeUp.cor = RED;
                    tubes[i].tubeDown.rec = (Rectangle){1160 + i*(DIST_X_TUBES + TUBE_WIDTH), tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };
                    tubes[i].tubeDown.cor = RED;
                    tubes[i].valid = true;
                    tubes[i].respawn = true;
                }
                if (CheckCollisionPointRec(mousePoint, playBtn)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        currentScreen = START;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, diffBtn)){
                    if (IsKeyDown(KEY_SPACE)){

                        currentScreen = DIFF;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, rankBtn)){
                    if (IsKeyDown(KEY_SPACE)){

                        currentScreen = RANK;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, exitBtn)){
                    if (IsKeyDown(KEY_SPACE)){
                        rodando = false;
                    }
                }
                score = 0;
                aug_vel = 0;
                sprintf(string_score, "%d", 0);

            }break;
            case START:{
                if (IsKeyPressed(KEY_SPACE)){
                    currentScreen = GAME;
                }
                score = 0;
                aug_vel = 0;
                tubeSpeed = 5;
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
                        tubes[i].tubeUp.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
                        tubes[i].tubeDown.rec.x += (DIST_X_TUBES + TUBE_WIDTH)*N_BARRA;
                        tubes[i].tubeUp.rec.y = cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap);
                        tubes[i].tubeDown.rec.y = tubes[i].tubeUp.rec.y + TUBE_HEIGHT + tubes[i].gap ;
                        tubes[i].valid = true;

                    }
                    if(tubes[i].tubeUp.rec.x > -40 && tubes[i].respawn == false){
                        tubes[i].respawn = true;

                    }
                    if(CheckCollisionRecs(player.rec, tubes[i].tubeUp.rec) || CheckCollisionRecs(player.rec, tubes[i].tubeDown.rec)){
                        currentScreen = OVER;
                        break;
                    }
                    if (player.rec.x > tubes[i].tubeUp.rec.x +20 && tubes[i].valid == true){
                        score++;
                        aug_vel++;
                        tubes[i].valid = false;
                        sprintf(string_score, "%d", score);
                    }
                }
                    if(aug_vel == 10) {
                        tubeSpeed++;
                        aug_vel = 0;

                    }
                    if (player.rec.y > 724){
                        if(player.rec.y > 724) player.rec.y = 720;
                        currentScreen = OVER;
                    }
                    if (player.rec.y < 0)player.rec.y = 0;




            }break;
            case OVER:
            {
                if (CheckCollisionPointRec(mousePoint, restartBtn)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        tubes[0].gap = 300;
                        tubes[0].tubeUp.rec = (Rectangle){ 1160, cabe_tela(-400 - tubes[0].gap/2, tubes[0].gap), TUBE_WIDTH, TUBE_HEIGHT};
                        tubes[0].tubeUp.cor = RED;
                        tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap + TUBE_HEIGHT , TUBE_WIDTH, TUBE_HEIGHT };
                        tubes[0].tubeDown.cor = RED;
                        tubes[0].valid = true;
                        tubes[0].respawn = true;
                for(int i=1;i<N_BARRA;i++){
                    tubes[i].gap = 300;
                    tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*(DIST_X_TUBES + TUBE_WIDTH), cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap), TUBE_WIDTH , TUBE_HEIGHT };
                    tubes[i].tubeUp.cor = RED;
                    tubes[i].tubeDown.rec = (Rectangle){1160 + i*(DIST_X_TUBES + TUBE_WIDTH), tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };
                    tubes[i].tubeDown.cor = RED;
                    tubes[i].valid = true;
                    tubes[i].respawn = true;
                }
                        currentScreen = START;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, menuBtn)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        currentScreen = MENU;
                    }
                }


            }break;
            case DIFF:
            {
                if (CheckCollisionPointRec(mousePoint, backDiff)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        currentScreen = MENU;
                    }
                }
            }break;
            case RANK:
            {
                if (CheckCollisionPointRec(mousePoint, backRank)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        currentScreen = MENU;
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


            } break;

            case START:{
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width,player.rec.height}, (Vector2){player.rec.x, player.rec.y}, RAYWHITE);
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
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);

                DrawRectangleRec(player.rec, player.cor);
                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width,player.rec.height}, (Vector2){player.rec.x, player.rec.y}, RAYWHITE);
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
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);

                DrawTextureRec(TaylorTx, (Rectangle){0,0,player.rec.width,player.rec.height}, (Vector2){player.rec.x, player.rec.y}, RAYWHITE);

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
                DrawRectangle(screenWidth/2 - 250, 50, 500, 100 , RED);
                DrawTextureRec(DiffTitleTx, (Rectangle){0,0,500, 100}, (Vector2){screenWidth/2.0f - 250,50}, RAYWHITE);
                DrawTextureRec(Diff1Tx, (Rectangle){0,0,btn1Diff.width,btn1Diff.height}, (Vector2){btn1Diff.x, btn1Diff.y}, RAYWHITE);
                DrawTextureRec(Diff2Tx, (Rectangle){0,0,btn2Diff.width,btn2Diff.height}, (Vector2){btn2Diff.x, btn2Diff.y}, RAYWHITE);
                DrawTextureRec(Diff3Tx, (Rectangle){0,0,btn3Diff.width,btn3Diff.height}, (Vector2){btn3Diff.x, btn3Diff.y}, RAYWHITE);
                DrawTextureRec(Diff4Tx, (Rectangle){0,0,btn4Diff.width,btn4Diff.height}, (Vector2){btn4Diff.x, btn4Diff.y}, RAYWHITE);
                DrawTextureRec(Diff5Tx, (Rectangle){0,0,btn1Diff.width,btn5Diff.height}, (Vector2){btn5Diff.x, btn5Diff.y}, RAYWHITE);

            }break;
            case RANK:
            {
                ClearBackground(RAYWHITE);
                DrawTextureRec(FundoTx, (Rectangle){0,0,1200,800}, (Vector2){0,0}, RAYWHITE);
                DrawTextureRec(VoltarTx, (Rectangle){0,0,backRank.width,backRank.height}, (Vector2){backRank.x, backRank.y}, RAYWHITE);
                DrawTextureRec(RankTitleTx, (Rectangle){0,0,500, 100}, (Vector2){screenWidth/2.0f - 250,50}, RAYWHITE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 180}, RAYWHITE);
                DrawText("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", screenWidth/2 - 490, screenHeight/2 - 170, 30, BLUE);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 135, 30, BLUE);
                DrawText("1", screenWidth/2 - 370, screenHeight/2 - 135, 30, BLUE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 - 90}, RAYWHITE);
                DrawText("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", screenWidth/2 - 490, screenHeight/2 - 80, 30, BLUE);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 - 45, 30, BLUE);
                DrawText("100", screenWidth/2 - 370, screenHeight/2 - 45, 30, BLUE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2}, RAYWHITE);
                DrawText("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", screenWidth/2 - 490, screenHeight/2 + 10, 30, BLUE);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 45, 30, BLUE);
                DrawText("10000", screenWidth/2 - 370, screenHeight/2 + 45, 30, BLUE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 90}, RAYWHITE);
                DrawText("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", screenWidth/2 - 490, screenHeight/2 + 100, 30, BLUE);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 135, 30, BLUE);
                DrawText("1000000", screenWidth/2 - 370, screenHeight/2 + 135, 30, BLUE);


                DrawTextureRec(Base_Rank, (Rectangle){0,0,1000, 80}, (Vector2){screenWidth/2 - 500,screenHeight/2 + 180}, RAYWHITE);
                DrawText("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", screenWidth/2 - 490, screenHeight/2 + 190, 30, BLUE);
                DrawText("SCORE:", screenWidth/2 - 490, screenHeight/2 + 225, 30, BLUE);
                DrawText("100000000", screenWidth/2 - 370, screenHeight/2 + 225, 30, BLUE);


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
