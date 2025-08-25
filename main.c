#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

typedef struct {
    Vector2 pos;
    Vector2 size;
} Entity;

typedef struct {
    Vector2 pos;
    float   radius;
    Vector2 aim_direction;
    float   aim_theta;
    float   power;
} Ball;

int main() {

    Ball ball = {
        .pos = { 150.0, 230.0 },
        .radius = 10.0,
        .aim_direction = { 1.0, 0.0 },
        .aim_theta = 0.0,
        .power = 0.0,
    };

    Entity hole = {
        .pos  = { 600.0, 230.0 },
        .size = { 30.0, 30.0 },
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mini golf");
    while (!WindowShouldClose()) {
        float dt = 1000.0 / GetFPS();
        
        float delta_theta = 0.005 * dt;
        if (IsKeyDown(KEY_J)) {
            ball.aim_theta += delta_theta;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }
        if (IsKeyDown(KEY_K)) {
            ball.aim_theta -= delta_theta;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }

        { // Update power
            ball.power = (SCREEN_HEIGHT - GetMouseY() - 90) / (float)(SCREEN_HEIGHT - 180);
            if (ball.power > 1) {
                ball.power = 1.0;
            }
            else if (ball.power < 0) {
                ball.power = 0.0;
            }
        }

        Vector2 tracer_start_pos = {
            .x = ball.pos.x + (20 * ball.aim_direction.x),
            .y = ball.pos.y + (20 * (-ball.aim_direction.y)),
        };

        Vector2 tracer_end_pos = {
            .x = tracer_start_pos.x + (60 * ball.aim_direction.x),
            .y = tracer_start_pos.y + (60 * (-ball.aim_direction.y)),
        };

        BeginDrawing();
        ClearBackground(GREEN);

        DrawFPS(0, 0);

        { // Draw entities
            DrawCircle(ball.pos.x, ball.pos.y, ball.radius, RAYWHITE);
            DrawCircle(hole.pos.x, hole.pos.y, hole.size.x / 2.0, BLACK);
        }

        { // Draw UI elements
            DrawLineV(tracer_start_pos, tracer_end_pos, RAYWHITE);

            Rectangle power_background_rect = {
                .x = 30,
                .y = 90,
                . width = 40,
                .height = 300,
            };
            DrawRectangleRec(power_background_rect, BLACK);

            Rectangle power_rect = {
                .x = 30.0,
                .y = (SCREEN_HEIGHT - 90 - (int)(300.0 * ball.power)),
                .width = 40.0,
                .height = (int)(300.0 * ball.power),
            };
            DrawRectangleRec(power_rect, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
