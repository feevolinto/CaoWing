#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "CMSC 18 - Final Project");

    Font sfFont = LoadFontEx("sf-pro-display/SFPRODISPLAYBOLD.otf", 96, 0, 0); 

    SetTextureFilter(sfFont.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(sfFont, "NA SET UP DIN SA WAKAS!", (Vector2){20, 100}, 56, 2, BLACK);
        DrawTextEx(sfFont, "Lorem ipsum", (Vector2){25, 200}, 32, 1, DARKGRAY);

        EndDrawing();
    }

    UnloadFont(sfFont);
    CloseWindow();

    return 0;
}