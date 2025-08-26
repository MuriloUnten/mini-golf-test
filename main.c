#include <math.h>
#include <raylib.h>
#include <stdio.h>

typedef struct {
    Vector2 pos;
    Vector2 size;
} Entity;

typedef struct {
    Vector2 pos;
    float   radius;
    Vector2 aim_direction;
    float   aim_theta;
} Ball;

int main() {

    Ball ball = {
        .pos = { 150.0, 230.0 },
        .radius = 10.0,
        .aim_direction = { 1.0, 0.0 },
        .aim_theta = 0.0,
    };

    Entity hole = {
        .pos  = { 600.0, 230.0 },
        .size = { 30.0, 30.0 },
    };

    InitWindow(720, 480, "mini golf");
    while (!WindowShouldClose()) {
        float dt = GetFPS() / 1000000.0;
        
        // printf("%f\n", aim_theta);
        if (IsKeyDown(KEY_J)) {
            ball.aim_theta += 0.2 * dt;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }
        if (IsKeyDown(KEY_K)) {
            ball.aim_theta -= 0.2 * dt;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }

        BeginDrawing();
        ClearBackground(GREEN);

        DrawFPS(0, 0);
        Vector2 tracer_start_pos = {
            .x = ball.pos.x + (20 * ball.aim_direction.x),
            .y = ball.pos.y + (20 * (-ball.aim_direction.y)),
        };

        Vector2 tracer_end_pos = {
            .x = tracer_start_pos.x + (60 * ball.aim_direction.x),
            .y = tracer_start_pos.y + (60 * (-ball.aim_direction.y)),
        };

        DrawCircle(ball.pos.x, ball.pos.y, ball.radius, RAYWHITE);
        DrawLineV(tracer_start_pos, tracer_end_pos, RAYWHITE);

        DrawCircle(hole.pos.x, hole.pos.y, hole.size.x / 2.0, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
