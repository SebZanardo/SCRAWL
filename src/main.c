#include "core.c"
#include "glyph.c"

int main(void) {
    InitWindow(800, 800, "SCRAWL");

    GlyphAtlas glyph_atlas = {0};
    GlyphScratches glyph_scratches = {0};

    // NOTE: Temporary until file loading
    i32 num_verts = 6;
    i32 num_edges = 5;
    Vector2 verts[16] = {
        {0.7f, 0.2f}, {0.5f, 0.1f}, {0.3f, 0.2f},
        {0.7f, 0.8f}, {0.5f, 0.9f}, {0.3f, 0.8f}
    };
    IntVector2 edges[16] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}
    };
    Glyph* glyph = GlyphAtlasAdd(
        &glyph_atlas, verts, edges, num_verts, num_edges
    );

    // Cache scratches
    Vector2 cursor = {100.0f, 100.0f};
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);
    GlyphScratchesAdd(&glyph_scratches, glyph, &cursor);

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
