#include "engine.h"
#include <raylib.h>

void engine_init_graphics(int width, int height) {
    InitWindow(width, height, "mini golf");
}

void engine_deinit_graphics() {
    CloseWindow();
}

void engine_begin_drawing() {
    BeginDrawing();
}

void engine_end_drawing() {
    EndDrawing();
}

int engine_is_button_pressed(int button) {
    return IsKeyDown(button);
}

float engine_get_shot_power(int screen_height) {
    float power = (screen_height - GetMouseY() - (screen_height / 6.0)) / (float)(screen_height - (2 * screen_height / 6.0)) ;
    if (power > 1.0) {
        power = 1.0;
    }
    else if (power < 0.0) {
        power = 0.0;
    }

    return power;
}

void engine_clear_background() {
    ClearBackground((Color){ 67, 84, 60 });
}

void engine_draw_fps() {
    DrawFPS(5, 5);
}

void engine_draw_circle(int center_x, int center_y, float radius) {
    DrawCircle(center_x, center_y, radius, BLACK);
}

void engine_draw_circle_lines(int center_x, int center_y, float radius) {
    DrawCircleLines(center_x, center_y, radius, BLACK);
    DrawCircleLines(center_x, center_y, radius - 1, BLACK);
}

void engine_draw_rectangle(int pos_x, int pos_y, int width, int height) {
    DrawRectangle(pos_x, pos_y, width, height, BLACK);
}

void engine_draw_rectangle_rect(Engine_rectangle rect) {
    Rectangle raylib_rect = {
        .x = rect.x,
        .y = rect.y,
        .width = rect.width,
        .height = rect.height,
    };

    DrawRectangleRec(raylib_rect, BLACK);
}

void engine_draw_rectangle_rect_lines(Engine_rectangle rect) {
    Rectangle raylib_rect = {
        .x = rect.x,
        .y = rect.y,
        .width = rect.width,
        .height = rect.height,
    };

    DrawRectangleLinesEx(raylib_rect, 4, BLACK);
}

void engine_draw_line(Engine_vector2f start_pos, Engine_vector2f end_pos) {
    DrawLineEx((Vector2){ start_pos.x, start_pos.y }, (Vector2){ end_pos.x, end_pos.y }, 2, BLACK);
}
