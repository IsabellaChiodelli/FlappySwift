#include "raylib.h"

#define TUBE_WIDTH 80
#define TUBE_HEIGHT 800
#define N_BARRA 10
#define DIST_X_TUBES 260

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

static int dist_alt =  40;

int cabe_tela(int pos_in, int gap){
    int diff;
    int pos_out;
    diff = GetRandomValue(-dist_alt, dist_alt);
    pos_out = pos_in + diff;
    if (pos_out > - 50 - gap){
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

    Player player;
    Tubes tubes[N_BARRA] = {0};
    int tubeSpeed = 0;
    int score = 0;
    int aug_vel = 0;
    char string_score[8] = {'0'};



    InitWindow(screenWidth, screenHeight, "Teste Joguinho");

    GameScreen currentScreen = MENU;

    Rectangle backMenu = { 0, 0, screenWidth, screenHeight };
    Rectangle gameName = { screenWidth/2 - 400, 50, 800, 200 };
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

    player.rec = (Rectangle){ 200, 200, 40, 80 };
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
                tubes[0].tubeUp.rec = (Rectangle){ 1160, GetRandomValue(-dist_alt, dist_alt) - tubes[0].gap/2 - 400, TUBE_WIDTH, TUBE_HEIGHT};
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
                if(!(IsKeyDown(KEY_SPACE) && player.rec.y == 0)) player.rec.y += 5;
                if (IsKeyDown(KEY_SPACE) && player.rec.y > 0) {
                        player.rec.y -= 10;
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
                    if (player.rec.y > 764){
                        if(player.rec.y > 764) player.rec.y = 760;
                        currentScreen = OVER;
                    }



            }break;
            case OVER:
            {
                if (CheckCollisionPointRec(mousePoint, restartBtn)){
                    if (IsKeyDown(KEY_SPACE)){
                        player.rec.y = 200;
                        tubes[0].gap = 300;
                        tubes[0].tubeUp.rec = (Rectangle){ 1160, -400 + GetRandomValue(-dist_alt, dist_alt) - tubes[0].gap/2, TUBE_WIDTH, TUBE_HEIGHT};
                        tubes[0].tubeUp.cor = RED;
                        tubes[0].tubeDown.rec = (Rectangle){1160, tubes[0].tubeUp.rec.y + tubes[0].gap , TUBE_WIDTH, TUBE_HEIGHT };
                        tubes[0].tubeDown.cor = RED;
                        tubes[0].valid = true;
                        tubes[0].respawn = true;
                for(int i=1;i<N_BARRA;i++){
                    tubes[i].gap = 300;
                    tubes[i].tubeUp.rec = (Rectangle){ 1160 + i*300, cabe_tela(tubes[retorno(i)].tubeUp.rec.y, tubes[i].gap), TUBE_WIDTH , TUBE_HEIGHT };
                    tubes[i].tubeUp.cor = RED;
                    tubes[i].tubeDown.rec = (Rectangle){1160 + i*300, tubes[i].tubeUp.rec.y + tubes[i].gap + TUBE_HEIGHT,  TUBE_WIDTH, TUBE_HEIGHT };
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
                DrawRectangleRec(backMenu, GREEN);
                DrawRectangleRec(gameName, RED);
                DrawRectangleRec(playBtn, RED);
                DrawRectangleRec(diffBtn, RED);
                DrawRectangleRec(rankBtn, RED);
                DrawRectangleRec(exitBtn, RED);

            } break;

            case START:{
                ClearBackground(RAYWHITE);
                DrawRectangleRec(backMenu, GREEN);
                DrawRectangleRec(player.rec, player.cor);
                int i =0;
                while(i<N_BARRA){
                    DrawRectangle(tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y, tubes[i].tubeUp.rec.width, tubes[i].tubeUp.rec.height, tubes[i].tubeUp.cor);
                    DrawRectangle(tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y, tubes[i].tubeDown.rec.width, tubes[i].tubeDown.rec.height, tubes[i].tubeDown.cor);
                    i++;
                }
                i=0;
                ClearBackground(RAYWHITE);
                DrawText("Press Space", 480, 380, 20, GRAY);
            }break;

            case GAME:
            {
                ClearBackground(RAYWHITE);
                DrawRectangleRec(backMenu, GREEN);

                DrawRectangleRec(player.rec, player.cor);
                int i =0;
                while(i<N_BARRA){
                    DrawRectangle(tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y, tubes[i].tubeUp.rec.width, tubes[i].tubeUp.rec.height, tubes[i].tubeUp.cor);
                    DrawRectangle(tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y, tubes[i].tubeDown.rec.width, tubes[i].tubeDown.rec.height, tubes[i].tubeDown.cor);
                    i++;
                }
                i=0;
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);


            } break;
            case OVER:
            {
                ClearBackground(RAYWHITE);
                DrawRectangleRec(backMenu, GREEN);

                DrawRectangleRec(player.rec, player.cor);

                int i =0;
                while(i<N_BARRA){
                    DrawRectangle(tubes[i].tubeUp.rec.x, tubes[i].tubeUp.rec.y, tubes[i].tubeUp.rec.width, tubes[i].tubeUp.rec.height, tubes[i].tubeUp.cor);
                    DrawRectangle(tubes[i].tubeDown.rec.x, tubes[i].tubeDown.rec.y, tubes[i].tubeDown.rec.width, tubes[i].tubeDown.rec.height, tubes[i].tubeDown.cor);
                    i++;
                }
                i=0;
                DrawText(string_score, screenWidth - 70, 20, 30, BLUE);

                ClearBackground(RAYWHITE);
                DrawRectangle(screenWidth/2.0f - 200, screenHeight/2.0f - 100, 400, 200, BLUE);
                DrawRectangleRec(restartBtn, RED);
                DrawRectangleRec(menuBtn, RED);
                DrawText("GAME OVER", screenWidth/2.0f - 123, screenHeight/2.0f - 80, 40, DARKBLUE);
            }break;
            case DIFF:
            {
                ClearBackground(RAYWHITE);
                DrawRectangleRec(backMenu, GREEN);
                DrawRectangleRec(backDiff, RED);
                DrawRectangle(screenWidth/2 - 250, 50, 500, 100 , RED);
                DrawRectangleRec(btn1Diff,RED);
                DrawRectangleRec(btn2Diff,RED);
                DrawRectangleRec(btn3Diff,RED);
                DrawRectangleRec(btn4Diff,RED);
                DrawRectangleRec(btn5Diff,RED);

            }break;
            case RANK:
            {
                ClearBackground(RAYWHITE);
                DrawRectangleRec(backMenu, GREEN);
                DrawRectangleRec(backRank, RED);
                DrawRectangle(screenWidth/2 - 250, 50, 500, 100 , RED);
                DrawRectangle(screenWidth/2 - 400, screenHeight/2 - 180 , 810, 80, RED);
                DrawRectangle(screenWidth/2 - 395, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 350, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 305, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 260, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 215, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 170, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 125, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 80, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 35, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 10, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 55, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 100, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 145, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 190, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 235, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 280, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 325, screenHeight/2 - 175 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 370, screenHeight/2 - 175 , 35, 70, BLUE);

                DrawRectangle(screenWidth/2 - 400, screenHeight/2 - 90 , 810, 80, RED);
                DrawRectangle(screenWidth/2 - 395, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 350, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 305, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 260, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 215, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 170, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 125, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 80, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 35, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 10, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 55, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 100, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 145, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 190, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 235, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 280, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 325, screenHeight/2 - 85 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 370, screenHeight/2 - 85 , 35, 70, BLUE);

                DrawRectangle(screenWidth/2 - 400, screenHeight/2 , 810, 80, RED);
                DrawRectangle(screenWidth/2 - 395, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 350, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 305, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 260, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 215, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 170, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 125, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 80, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 35, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 10, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 55, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 100, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 145, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 190, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 235, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 280, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 325, screenHeight/2 + 5 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 370, screenHeight/2 + 5 , 35, 70, BLUE);

                DrawRectangle(screenWidth/2 - 400, screenHeight/2 + 90 , 810, 80, RED);
                DrawRectangle(screenWidth/2 - 395, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 350, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 305, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 260, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 215, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 170, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 125, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 80, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 35, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 10, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 55, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 100, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 145, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 190, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 235, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 280, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 325, screenHeight/2 + 95 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 370, screenHeight/2 + 95 , 35, 70, BLUE);

                DrawRectangle(screenWidth/2 - 400, screenHeight/2 + 180 , 810, 80, RED);
                DrawRectangle(screenWidth/2 - 395, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 350, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 305, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 260, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 215, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 170, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 125, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 80, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 - 35, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 10, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 55, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 100, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 145, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 190, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 235, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 280, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 325, screenHeight/2 + 185 , 35, 70, BLUE);
                DrawRectangle(screenWidth/2 + 370, screenHeight/2 + 185 , 35, 70, BLUE);


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
