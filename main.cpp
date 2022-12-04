#include <iostream>
#include "raylib.h"
using std::cout;
using std::string;
using std::to_string;

const int border = 35;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = SCREEN_WIDTH + border;
const int scl = 25;
const Color HEAD_COL = RED;
const Color TAIL_COL = DARKBLUE;
const Color BG = SKYBLUE;
const Color SC_BOARD = BLUE;
const float velocity = scl;
const int TAIL = (SCREEN_WIDTH / scl) * ((SCREEN_HEIGHT - border) / scl) - 1;

int sqr(int a)
{
    return a * a;
}

class food
{
public:
    int x, y;

    food()
    {
        x = GetRandomValue(1, (SCREEN_WIDTH - scl) / scl) * scl;
        y = border + GetRandomValue(1, (SCREEN_HEIGHT - border - scl) / scl) * scl;
    }

    void draw()
    {
        DrawRectangle(x, y, scl, scl, ORANGE);
        DrawRectangle(x + 2, y + 2, scl - 4, scl - 4, GOLD);
    }
};
class snake
{
    int cnt = 0;

public:
    int ax, ay;
    Vector2 tail[TAIL] = {0};
    bool ate = false;

    bool canMove = true;
    int len = 0;
    int x = 0, y = border;
    Vector2 vel = {0, 0};

    void update()
    {
        for (int i = 0; i < len - 1; i++)
        {

            tail[i].x = tail[i + 1].x;
            tail[i].y = tail[i + 1].y;
        }
        tail[len - 1].x = x;
        tail[len - 1].y = y;

        x += vel.x;
        y += vel.y;
        if (x < 0)
            canMove = false;
        else if (x > SCREEN_WIDTH - scl)
            canMove = false;
        if (y < border)
            canMove = false;
        else if (y > SCREEN_HEIGHT - scl)
            canMove = false;
    }
    void draw()
    {
        // Drawing Head
        DrawRectangle(x, y, scl, scl, HEAD_COL);
        DrawRectangleLines(x, y, scl, scl, Fade(BLACK, 0.1f));
        Color tail_col = {0, 153, 255, 255};

        for (int i = 0; i < len; i++)
        {
            float alpha = 1.0 - 0.01 * (i + 1);
            alpha = (alpha <= 0.4 ? 0.4 : alpha);

            tail_col.r = 5 * (i + 1);
            tail_col.r = (tail_col.r > 255) ? 255 : 2 * (i + 1);
            tail_col.g = (i + 1);
            tail_col.g = (tail_col.g > 255) ? 255 : (i + 1);
            tail_col.b = 103 + (i + 1) * (0.5);
            DrawRectangle(tail[len - i - 1].x, tail[len - i - 1].y, scl, scl, Fade(tail_col, alpha));
        }

        if (ate && cnt < len - 1)
        {
            DrawRectangle(ax, ay, scl, scl, Fade(YELLOW, 0.5f));
            cnt++;
        }
        else
        {
            ate = false;
            cnt = 0;
        }
    }
};

void pickLocation(food &fd, snake &sn)
{

    fd.x = GetRandomValue(0, (SCREEN_WIDTH - scl) / scl) * scl;
    fd.y = border + GetRandomValue(0, (SCREEN_HEIGHT - border - scl) / scl) * scl;
    if (fd.x == sn.x && fd.y == sn.y)
        pickLocation(fd, sn);
    else
        for (int i = 0; i < sn.len; i++)
        {
            if (fd.x == sn.tail[i].x && fd.y == sn.tail[i].y)
                pickLocation(fd, sn);
        }
}

bool eat(food &fd, snake &sn)
{
    if (fd.x == sn.x && fd.y == sn.y)
    {
        pickLocation(fd, sn);
        sn.ate = true;

        if (sn.ate)
        {
            sn.ax = sn.x, sn.ay = sn.y;
        }
        return true;
    }
    return false;
}

bool collide(snake &snk)
{
    if (snk.canMove)
    {
        for (int i = 4; i < snk.len; i++)
        {
            if (snk.tail[i].x == snk.x && snk.tail[i].y == snk.y)
            {
                snk.canMove = false;
                return true;
            }
        }
        return false;
    }
    else
        return true;
}

int main()
{
    snake gogo;
    food candy;
    bool gameOver = false;
    bool pause = false;
    int fcount = 0;
    int score = 0;
    InitAudioDevice();
    Sound bgMusic = LoadSound("src/bg.wav");
    Sound Eat = LoadSound("src/eat.wav");
    Sound dead = LoadSound("src/dead.wav");
    Sound move = LoadSound("src/move.wav");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D Snake - Game");
    SetTargetFPS(10);
    
    PlaySound(bgMusic);
    SetSoundVolume(bgMusic, 0.2f);
    SetSoundVolume(move,0.2f);
    int g = 1;
    while (!WindowShouldClose())
    {
        if(!IsSoundPlaying(bgMusic))
             PlaySound(bgMusic);
            
        // Actions
        if (IsKeyDown(KEY_UP) && gogo.vel.y == 0 && gogo.x % scl == 0)
            gogo.vel.y -= velocity, gogo.vel.x = 0, PlaySound(move);
        else if (IsKeyDown(KEY_DOWN) && gogo.vel.y == 0 && gogo.x % scl == 0)
            gogo.vel.y += velocity, gogo.vel.x = 0, PlaySound(move);
        else if (IsKeyDown(KEY_LEFT) && gogo.vel.x == 0 && (gogo.y - border) % scl == 0)
            gogo.vel.x -= velocity, gogo.vel.y = 0, PlaySound(move);
        else if (IsKeyDown(KEY_RIGHT) && gogo.vel.x == 0 && (gogo.y - border) % scl == 0)
            gogo.vel.x += velocity, gogo.vel.y = 0, PlaySound(move);
        if (IsKeyDown(KEY_R) && gameOver)
        {
            gogo.len = 0;
            gameOver = false;
            gogo.canMove = true;
            gogo.x = 0;
            gogo.y = border;
            gogo.vel.x = 0;
            gogo.vel.y = 0;
            score = 0;
            for (int i = 0; i < TAIL; i++)
                gogo.tail[i] = {0};
        }
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause, gogo.canMove = (pause) ? false : true;

        if (gogo.canMove)
            gogo.update(), collide(gogo);
        else if (!pause)
            gameOver = true;

        if (eat(candy, gogo))
        {
            PlaySound(Eat);
            score++;
            gogo.len++;
        }

        BeginDrawing();
        ClearBackground(BG); // Top border
        gogo.draw();
        DrawRectangle(0, 0, SCREEN_WIDTH, border, SC_BOARD);
        if (!gameOver)
        {

            DrawRectangle((SCREEN_WIDTH * 2) / 3, 0, SCREEN_WIDTH / 3, border, DARKBLUE);
            DrawText(("Score: " + to_string(score)).c_str(), SCREEN_WIDTH - 155, 3, 30, WHITE);
            candy.draw();
            if (pause)
            {
                PauseSound(bgMusic);
                DrawRectangle(5, border + 5, SCREEN_WIDTH - 10, SCREEN_HEIGHT - border - 10, Fade(BLACK, 0.3f));
            }
            else
                ResumeSound(bgMusic);
        }
        else
        {
            if(g)
             PlaySound(dead),g=0;
            DrawText("GAME OVER", SCREEN_WIDTH / 3, 5, 30, BLACK);
            DrawText(("FINAL SCORE: " + to_string(score)).c_str(), SCREEN_WIDTH / 12, SCREEN_HEIGHT / 3, 50, Fade(BLACK, 0.7f));
            DrawText("Press R to RESET", (SCREEN_WIDTH * 2) / 9, (SCREEN_HEIGHT) / 2, 30, DARKGRAY);
        }

        EndDrawing();
    }
    UnloadSound(Eat);
    UnloadSound(dead);
    UnloadSound(move);
    UnloadSound(bgMusic);
    CloseAudioDevice();
    return 0;
}