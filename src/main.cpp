#include <raylib.h>
#include <iostream>

int player_score = 0;
int computer_score = 0;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

class Ball {
    public:
        float x, y;
        int speedx, speedy;
        int radius;

        Ball(float x, float y, int speedx, int speedy, int radius) {
            this->x = x;
            this->y = y;
            this->speedx = speedx;
            this->speedy = speedy;
            this->radius = radius;
        }

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        y += speedy;
        x += speedx;


        if ( y + radius >= GetScreenHeight() || y - radius <= 0 ) {
            speedy *= -1;
        }

        if ( x + radius >= GetScreenWidth() )
        {
            player_score ++;
            ResetBall();
        }

        if ( x - radius <= 0 )
        {
            computer_score ++;
            ResetBall();
        }

        if ( x + radius >= GetScreenWidth() || x - radius <= 0 ) {
            speedx *= -1;
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        int speed_choices[2] = {-1, 1};
        speedx *= speed_choices[GetRandomValue(0, 1)];
        speedy *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
    public:
        float x, y;
        float width, height;
        int speed;

    Paddle(float x, float y, float width, float height, int speed) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed = speed;
    }

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMovement();
    }

    protected:
    void LimitMovement() {
        if(y <= 0)
        {
            y = 0;
        }
        if(y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
};

class CpuPaddle: public Paddle {

    public:
    CpuPaddle(float x, float y, float width, float height, int speed):  Paddle(x, y, width, height, speed) {};

    void Update(int ball_y) {
        if(y + height/2 > ball_y) {
            y = y - speed;
        }
        if(y + height/2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

int main() 
{
    const int screen_width = 1280;
    const int screen_height = 800;
    const int fps = 60;
    const int paddle_height = 120;
    const int paddle_width = 25;
    const int paddle_speed = 6;


    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(fps);

    Ball ball(screen_width/2, screen_height/2, 7, 7, 20);
    Paddle player(10, screen_height/2 - paddle_height/2, paddle_width, paddle_height, paddle_speed);
    CpuPaddle computer(screen_width - paddle_width - 10, screen_height/2 - paddle_height/2, paddle_width, paddle_height, paddle_speed);

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // updating game objects
        ball.Update();
        player.Update();
        computer.Update(ball.y);

        // clear the screen
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);

        // check for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height} )) {
            ball.speedx *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{computer.x, computer.y, computer.width, computer.height} )) {
            ball.speedx *= -1;
        }

        // draw our objects
        ball.Draw();
        player.Draw();
        computer.Draw();

        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        // DrawText(text, xpos, ypos, font_size, color)
        DrawText(TextFormat("%i", player_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", computer_score),  3 * screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}