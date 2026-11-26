#include "core.c"
#include "glyph.c"
#include "scrawl.c"

int main(void) {
    InitWindow(800, 800, "SCRAWL");

    static GlyphAtlas glyph_atlas = {0};
    static GlyphScratches glyph_scratches = {0};

    ScrawlLoad(&glyph_atlas, "data/scrawl/font.scrawl");

    // Cache scratches
    Vector2 cursor = {200.0f, 200.0f};
    GlyphScratchesAddString(
        &glyph_scratches,
        &glyph_atlas,
        "ABCDEFGHIJKLM\nNOPQRSTUVWXYZ\n"
        "abcdefghijklm\nnopqrstuvwxyz\n"
        "0123456789\n"
        "!\"#$%&'()*+,-./:;\n<=>?@[\\]^_`{|}~",
        &cursor
    );

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (i32 i = 0; i < glyph_scratches.num_scratches; i++) {
            Vector2 startPos = glyph_scratches.start[i];
            Vector2 endPos = glyph_scratches.end[i];

            DrawLineEx(startPos, endPos, GLYPH_THICKNESS, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
