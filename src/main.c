#include "core.c"
#include "glyph.c"

int main(void) {
    InitWindow(800, 800, "SCRAWL");

    GlyphMap glyph_map = {0};

    // NOTE: Temporary
    i32 num_verts = 6;
    i32 num_edges = 5;
    Vector2 verts[] = {
        {0.7f, 0.2f}, {0.5f, 0.1f}, {0.3f, 0.2f},
        {0.7f, 0.8f}, {0.5f, 0.9f}, {0.3f, 0.8f}
    };
    IntVector2 edges[] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}
    };

    Glyph *glyph = GlyphAdd(&glyph_map, verts, edges, num_verts, num_edges);

    // Transform
    Vector2 glyph_offset = {100.0f, 100.0f};
    f32 glyph_vert_travel = 0.06f;
    f32 glyph_size = 200.0f;
    f32 glyph_thickness = 3.0f;

    for (i32 i = 0; i < num_verts; i++) {
        verts[i] = Vector2Add(verts[i], RandomCircle(glyph_vert_travel));
        verts[i] = Vector2Scale(verts[i], glyph_size);
        verts[i] = Vector2Add(verts[i], glyph_offset);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (i32 i = 0; i < num_edges; i++) {
            IntVector2 edge = edges[i];
            Vector2 startPos = verts[edge.x];
            Vector2 endPos = verts[edge.y];

            DrawLineEx(startPos, endPos, glyph_thickness, WHITE);
        }

        for (i32 i = 0; i < num_verts; i++) {
            Vector2 pos = verts[i];

            // DrawCircleV(pos, glyph_thickness, MAGENTA);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
