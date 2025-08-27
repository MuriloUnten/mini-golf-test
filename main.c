#include "engine.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 360

#define BALL_RADIUS 10.0
#define TRACER_LENGTH 60.0
#define TRACER_BALL_DISTANCE (2 * BALL_RADIUS)
#define HOLE_RADIUS 30.0
#define POWER_BAR_HEIGHT (4 * SCREEN_HEIGHT / 6.0)
#define POWER_BAR_WIDTH  (SCREEN_HEIGHT / 12.0)

typedef struct {
    Engine_vector2f pos;
    Engine_vector2f size;
} Entity;

typedef struct {
    Engine_vector2f pos;
    Engine_vector2f vel;
    float   radius;
    Engine_vector2f aim_direction;
    float   aim_theta;
    float   power;
} Ball;

typedef struct {
    Engine_vector2f start_pos;
    Engine_vector2f end_pos;
} Tracer;

typedef struct {
    Engine_rectangle background;
} Power_bar;

typedef struct {
    float dt;
    Ball ball;
    Tracer tracer;
    Engine_rectangle power_bar;
    Entity hole;
    Entity* obstacles;
} Game_state;

int ball_is_shooting(Game_state* state) {
    if (state->ball.vel.x == 0.0 && state->ball.vel.y == 0.0) {
        return 0;
    }

    return 1;
}

void init_game_state(Game_state* state) {
    state->dt = 0.0;

    Ball ball = {
        .pos = { 150.0, (SCREEN_HEIGHT / 2.0) },
        .vel = { 0.0, 0.0 },
        .radius = BALL_RADIUS,
        .aim_direction = { 1.0, 0.0 },
        .aim_theta = 0.0,
        .power = 0.0,
    };
    state->ball = ball;

    Tracer tracer = {
        .start_pos = (Engine_vector2f){
            state->ball.pos.x + (TRACER_BALL_DISTANCE * state->ball.aim_direction.x),
            state->ball.pos.y + (TRACER_BALL_DISTANCE * (-state->ball.aim_direction.y)),
        },
        .end_pos = {
            tracer.start_pos.x + (TRACER_LENGTH * state->ball.aim_direction.x),
            tracer.start_pos.y + (TRACER_LENGTH * (-state->ball.aim_direction.y)),
        },
    };
    state->tracer = tracer;

    Engine_rectangle power_bar = {
        .x = 30,
        .y = (SCREEN_HEIGHT / 6.0),
        . width = POWER_BAR_WIDTH,
        .height = POWER_BAR_HEIGHT,
    };
    state->power_bar = power_bar;

    Entity hole = {
        .pos  = { 600.0, (SCREEN_HEIGHT / 2.0) },
        .size = { HOLE_RADIUS, HOLE_RADIUS },
    };
    state->hole = hole;

    state->obstacles = malloc(10 * sizeof(Entity));
}

void shoot(Game_state* state) {
    state->ball.vel.x = state->ball.power * 350 * state->ball.aim_direction.x;
    state->ball.vel.y = state->ball.power * 350 * -state->ball.aim_direction.y;
}

void update_state(Game_state* state) {
    state->dt = engine_get_delta_time();

    if (!ball_is_shooting(state)) {
        if (engine_is_button_pressed(KEY_SHOOT)) {
            shoot(state);
        }
        else {
            float delta_theta = 2 * state->dt;
            if (engine_is_button_pressed(KEY_COUNTER_CLOCKWISE)) {
                state->ball.aim_theta += delta_theta;
            }
            if (engine_is_button_pressed(KEY_CLOCKWISE)) {
                state->ball.aim_theta -= delta_theta;
            }
            state->ball.aim_direction.x = (cos(state->ball.aim_theta));
            state->ball.aim_direction.y = (sin(state->ball.aim_theta));

            state->tracer.start_pos.x = state->ball.pos.x + (TRACER_BALL_DISTANCE * state->ball.aim_direction.x);
            state->tracer.start_pos.y = state->ball.pos.y + (TRACER_BALL_DISTANCE * (-state->ball.aim_direction.y));
            state->tracer.end_pos.x = state->tracer.start_pos.x + (TRACER_LENGTH * state->ball.aim_direction.x);
            state->tracer.end_pos.y = state->tracer.start_pos.y + (TRACER_LENGTH * (-state->ball.aim_direction.y));
        }
    }

    { // update ball position
        if (state->ball.vel.x != 0.0) {
            state->ball.vel.x -= 150 * state->dt * state->ball.aim_direction.x;
            if (fabs(state->ball.vel.x) < 0.1) {
                state->ball.vel.x = 0.0;
            }
        }

        if (state->ball.vel.y != 0.0) {
            state->ball.vel.y += 150 * state->dt * state->ball.aim_direction.y;
            if (fabs(state->ball.vel.y) < 0.1) {
                state->ball.vel.y = 0.0;
            }
        }

        state->ball.pos.x += state->dt * state->ball.vel.x;
        state->ball.pos.y += state->dt * state->ball.vel.y;
    }

    state->ball.power = engine_get_shot_power(SCREEN_HEIGHT);
}

void render(Game_state* state) {
    engine_begin_drawing();

    engine_clear_background();
    engine_draw_fps();

    { // Draw entities
        engine_draw_circle_lines(state->ball.pos.x, state->ball.pos.y, state->ball.radius);
        engine_draw_circle(state->hole.pos.x, state->hole.pos.y, state->hole.size.x / 2.0);
    }

    { // Draw UI elements
        if (!ball_is_shooting(state)) {
            engine_draw_line(state->tracer.start_pos, state->tracer.end_pos);
        }

        engine_draw_rectangle_rect_lines(state->power_bar);

        Engine_rectangle power_rect = {
            .x = 30.0,
            .y = (SCREEN_HEIGHT - (SCREEN_HEIGHT / 6.0) - (int)(POWER_BAR_HEIGHT * state->ball.power)),
            .width = POWER_BAR_WIDTH,
            .height = (int)(POWER_BAR_HEIGHT * state->ball.power),
        };
        engine_draw_rectangle_rect(power_rect);
    }

    engine_end_drawing();
}

int main() {

    Game_state state;
    init_game_state(&state);

    engine_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!WindowShouldClose()) {
        update_state(&state);
        render(&state);
    }

    CloseWindow();
    return 0;
}
