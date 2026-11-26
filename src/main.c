#include "raylib.h"

int main(void) {
    InitWindow(800, 800, "scrawl");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
