#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>

#define GRID 4
#define EMPTY 0
#define X_PIECE 1
#define O_PIECE 2

typedef enum { STATE_MENU, STATE_RULES, STATE_GAME, STATE_WIN } GameState;
typedef enum { MODE_PVP, MODE_PVB, MODE_BVB } GameMode;
typedef enum { BOT_EASY, BOT_MEDIUM, BOT_HARD } BotDifficulty;

int board[GRID][GRID][GRID];
int currentPlayer = X_PIECE;
int winner = EMPTY;

GameState state = STATE_MENU;
GameMode mode = MODE_PVP;
BotDifficulty botDiff = BOT_EASY;

float botTimer = 0.0f;
float botDelay = 0.8f;

void ResetBoard(void)
{
    for (int x=0;x<GRID;x++)
        for (int y=0;y<GRID;y++)
            for (int z=0;z<GRID;z++)
                board[x][y][z] = EMPTY;

    currentPlayer = X_PIECE;
    winner = EMPTY;
}
int CheckWin(void)
{

    const int directions[13][3] = {
        {1, 0, 0},  
        {0, 1, 0},  
        {0, 0, 1},  

        {1, 1, 0}, 
        {1, -1, 0},

        {1, 0, 1}, 
        {1, 0, -1},

        {0, 1, 1},  
        {0, 1, -1},

        {1, 1, 1},  
        {1, 1, -1},
        {1, -1, 1},
        {1, -1, -1}
    };

    for (int x = 0; x < GRID; x++)
    for (int y = 0; y < GRID; y++)
    for (int z = 0; z < GRID; z++)
    {
        int start = board[x][y][z];
        if (start == EMPTY) continue;

        for (int d = 0; d < 13; d++)
        {
            int dx = directions[d][0];
            int dy = directions[d][1];
            int dz = directions[d][2];

            int count = 1;

            for (int step = 1; step < GRID; step++)
            {
                int nx = x + dx * step;
                int ny = y + dy * step;
                int nz = z + dz * step;

                if (nx < 0 || nx >= GRID ||
                    ny < 0 || ny >= GRID ||
                    nz < 0 || nz >= GRID)
                    break;

                if (board[nx][ny][nz] == start)
                    count++;
                else
                    break;
            }

            if (count == GRID)
                return start;
        }
    }

    return EMPTY;
}

void BotMove(BotDifficulty diff)
{
    int bx=-1, by=-1, bz=-1;

    if (diff != BOT_EASY)
    {
        for (int pass=0; pass<2; pass++)
        {
            int target = (pass==0)? currentPlayer : (currentPlayer==X_PIECE?O_PIECE:X_PIECE);

            for (int x=0;x<GRID;x++)
            for (int y=0;y<GRID;y++)
            for (int z=0;z<GRID;z++)
            {
                if (board[x][y][z] != EMPTY) continue;
                board[x][y][z] = target;
                if (CheckWin() == target)
                {
                    board[x][y][z] = EMPTY;
                    bx=x; by=y; bz=z;
                    goto PLACE;
                }
                board[x][y][z] = EMPTY;
            }
        }
    }

    do {
        bx = rand()%GRID;
        by = rand()%GRID;
        bz = rand()%GRID;
    } while (board[bx][by][bz] != EMPTY);

PLACE:
    board[bx][by][bz] = currentPlayer;
    currentPlayer = (currentPlayer==X_PIECE)?O_PIECE:X_PIECE;
}

int main(void)
{
    InitWindow(1000, 700, "4x4x4 Tic Tac Toe");
    SetTargetFPS(60);
    DisableCursor();
    srand(time(NULL));

    Camera3D cam = {0};
    cam.position = (Vector3){10,10,10};
    cam.target   = (Vector3){0,0,0};
    cam.up       = (Vector3){0,1,0};
    cam.fovy     = 60;
    cam.projection = CAMERA_PERSPECTIVE;

    float yaw=-135, pitch=-30;
    float mouseSensitivity = 0.1f;
    float moveSpeed = 0.15f;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (IsKeyPressed(KEY_Q) && state != STATE_GAME) state = STATE_MENU;

        BeginDrawing();
        ClearBackground((Color){20,20,30,255});

        if (state == STATE_MENU)
        {
            DrawText("4x4x4 TIC TAC TOE", 340, 80, 32, RAYWHITE);
            DrawText("1 - Player vs Player", 380, 180, 20, GRAY);
            DrawText("2 - Player vs Bot",    380, 220, 20, GRAY);
            DrawText("3 - Bot vs Bot",       380, 260, 20, GRAY);
            DrawText("E / M / H - Bot Difficulty", 380, 320, 20, GRAY);
            DrawText("R - Rules", 380, 360, 20, GRAY);

            if (IsKeyPressed(KEY_ONE))   { mode=MODE_PVP; ResetBoard(); state=STATE_GAME; }
            if (IsKeyPressed(KEY_TWO))   { mode=MODE_PVB; ResetBoard(); state=STATE_GAME; }
            if (IsKeyPressed(KEY_THREE)) { mode=MODE_BVB; ResetBoard(); state=STATE_GAME; }

            if (IsKeyPressed(KEY_E)) botDiff = BOT_EASY;
            if (IsKeyPressed(KEY_M)) botDiff = BOT_MEDIUM;
            if (IsKeyPressed(KEY_H)) botDiff = BOT_HARD;

            if (IsKeyPressed(KEY_R)) state = STATE_RULES;
        }

        else if (state == STATE_RULES)
        {
            DrawText("RULES", 460, 80, 32, RAYWHITE);
            DrawText("Align 4 cubes in a straight or diagonal line.", 200, 200, 20, GRAY);
            DrawText("First player to do so wins.", 200, 240, 20, GRAY);
            DrawText("Q - Back to menu", 200, 320, 20, DARKGRAY);
        }

        else if (state == STATE_GAME)
        {
            Vector2 md = GetMouseDelta();
            yaw += md.x * mouseSensitivity;
            pitch -= md.y * mouseSensitivity;
            pitch = Clamp(pitch, -89, 89);

            Vector3 forward = {
                cosf(DEG2RAD*pitch)*cosf(DEG2RAD*yaw),
                sinf(DEG2RAD*pitch),
                cosf(DEG2RAD*pitch)*sinf(DEG2RAD*yaw)
            };
            Vector3 right = {
                sinf(DEG2RAD*yaw - PI/2),
                0,
                cosf(DEG2RAD*yaw - PI/2)
            };

            if (IsKeyDown(KEY_W)) cam.position = Vector3Add(cam.position, Vector3Scale(forward, moveSpeed));
            if (IsKeyDown(KEY_S)) cam.position = Vector3Subtract(cam.position, Vector3Scale(forward, moveSpeed));
            if (IsKeyDown(KEY_A)) cam.position = Vector3Subtract(cam.position, Vector3Scale(right, moveSpeed));
            if (IsKeyDown(KEY_D)) cam.position = Vector3Add(cam.position, Vector3Scale(right, moveSpeed));

            cam.target = Vector3Add(cam.position, forward);

            Ray ray = GetMouseRay((Vector2){GetScreenWidth()/2, GetScreenHeight()/2}, cam);

            BeginMode3D(cam);

            int hx=-1,hy=-1,hz=-1;
            float closest = 10000.0f;

            for (int x=0;x<GRID;x++)
            for (int y=0;y<GRID;y++)
            for (int z=0;z<GRID;z++)
            {
                Vector3 pos = {(x-1.5f)*3,(y-1.5f)*3,(z-1.5f)*3};
                BoundingBox box = {
                    Vector3Subtract(pos,(Vector3){0.75f,0.75f,0.75f}),
                    Vector3Add(pos,(Vector3){0.75f,0.75f,0.75f})
                };

                RayCollision col = GetRayCollisionBox(ray, box);
                if (col.hit && col.distance < closest)
                {
                    closest = col.distance;
                    hx=x; hy=y; hz=z;
                }
            }

            for (int x=0;x<GRID;x++)
            for (int y=0;y<GRID;y++)
            for (int z=0;z<GRID;z++)
            {
                Vector3 pos = {(x-1.5f)*3,(y-1.5f)*3,(z-1.5f)*3};
                Color c = (Color){100,180,255,120};

                if (board[x][y][z]==X_PIECE) c=(Color){255,80,80,200};
                if (board[x][y][z]==O_PIECE) c=(Color){80,255,120,200};
                if (x==hx && y==hy && z==hz) c.a = 255;

                DrawCube(pos,1.5f,1.5f,1.5f,c);
                DrawCubeWires(pos,1.5f,1.5f,1.5f,BLUE);
            }

            EndMode3D();

            bool botTurn =
                (mode==MODE_PVB && currentPlayer==O_PIECE) ||
                (mode==MODE_BVB);

            if (!botTurn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (hx!=-1 && board[hx][hy][hz]==EMPTY)
                {
                    board[hx][hy][hz] = currentPlayer;
                    currentPlayer = (currentPlayer==X_PIECE)?O_PIECE:X_PIECE;
                }
            }

            if (botTurn)
            {
                botTimer += GetFrameTime();
                if (botTimer >= botDelay)
                {
                    BotMove(botDiff);
                    botTimer = 0;
                }
            }

            winner = CheckWin();
            if (winner != EMPTY) state = STATE_WIN;

            int cx=GetScreenWidth()/2, cy=GetScreenHeight()/2;
            DrawLine(cx-10,cy,cx+10,cy,WHITE);
            DrawLine(cx,cy-10,cx,cy+10,WHITE);
            DrawCircle(cx,cy,2,WHITE);
        }

        else if (state == STATE_WIN)
        {
            DrawText(winner==X_PIECE?"PLAYER X WINS":"PLAYER O WINS", 360, 300, 30, RAYWHITE);
            DrawText("Q - Back to menu", 380, 350, 20, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

