#include "engine.h"

#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 360

#define POWER_BAR_HEIGHT (4 * SCREEN_HEIGHT / 6.0)
#define POWER_BAR_WIDTH  (SCREEN_HEIGHT / 12.0)

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
        .pos = { 150.0, (SCREEN_HEIGHT / 2.0) },
        .radius = 10.0,
        .aim_direction = { 1.0, 0.0 },
        .aim_theta = 0.0,
        .power = 0.0,
    };

    Entity hole = {
        .pos  = { 600.0, (SCREEN_HEIGHT / 2.0) },
        .size = { 30.0, 30.0 },
    };

    engine_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!WindowShouldClose()) {
        float dt = 1000.0 / GetFPS();
        
        float delta_theta = 0.005 * dt;
        if (engine_is_button_pressed(KEY_J)) {
            ball.aim_theta += delta_theta;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }
        if (engine_is_button_pressed(KEY_K)) {
            ball.aim_theta -= delta_theta;
            ball.aim_direction.x = (cos(ball.aim_theta));
            ball.aim_direction.y = (sin(ball.aim_theta));
        }

        ball.power = engine_get_shot_power(SCREEN_HEIGHT);

        Engine_vector2f tracer_start_pos = {
            .x = ball.pos.x + (20 * ball.aim_direction.x),
            .y = ball.pos.y + (20 * (-ball.aim_direction.y)),
        };

        Engine_vector2f tracer_end_pos = {
            .x = tracer_start_pos.x + (60 * ball.aim_direction.x),
            .y = tracer_start_pos.y + (60 * (-ball.aim_direction.y)),
        };

        engine_begin_drawing();
        engine_clear_background();

        engine_draw_fps();

        { // Draw entities
            engine_draw_circle(ball.pos.x, ball.pos.y, ball.radius, RAYWHITE);
            engine_draw_circle(hole.pos.x, hole.pos.y, hole.size.x / 2.0, BLACK);
        }

        { // Draw UI elements
            engine_draw_line(tracer_start_pos, tracer_end_pos, RAYWHITE);

            Engine_rectangle power_background_rect = {
                .x = 30,
                .y = (SCREEN_HEIGHT / 6.0),
                . width = POWER_BAR_WIDTH,
                .height = POWER_BAR_HEIGHT,
            };
            engine_draw_rectangle_rect(power_background_rect, BLACK);

            Engine_rectangle power_rect = {
                .x = 30.0,
                .y = (SCREEN_HEIGHT - (SCREEN_HEIGHT / 6.0) - (int)(POWER_BAR_HEIGHT * ball.power)),
                .width = POWER_BAR_WIDTH,
                .height = (int)(POWER_BAR_HEIGHT * ball.power),
            };
            engine_draw_rectangle_rect(power_rect, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
