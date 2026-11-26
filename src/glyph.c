#include "glyph.h"

void GlyphAtlasAdd(
    GlyphAtlas* atlas,
    Vector2* verts,
    IntVector2* edges,
    i32 num_verts,
    i32 num_edges
) {
    Assert(atlas->num_glyphs < GLYPH_ATLAS_GLYPHS_LEN, "Glyph atlas full");
    Assert(num_verts <= GLYPH_MAX_VERTS, "Too many glyph verts %d", num_verts);
    Assert(num_edges <= GLYPH_MAX_EDGES, "Too many glyph edges %d", num_edges);

    // Add glyph
    Glyph* glyph = &atlas->glyphs[atlas->num_glyphs++];
    glyph->verts = &atlas->verts[atlas->num_verts];
    glyph->edges = &atlas->edges[atlas->num_edges];
    glyph->num_verts = num_verts;
    glyph->num_edges = num_edges;

    // Add vertices
    for (i32 i = 0; i < num_verts; i++) {
        atlas->verts[atlas->num_verts++] = verts[i];
    }

    // Add edges
    for (i32 i = 0; i < num_edges; i++) {
        atlas->edges[atlas->num_edges++] = edges[i];
    }
}

void GlyphScratchesAdd(
    GlyphScratches* scratches,
    Glyph* glyph,
    Vector2* cursor
) {
    Assert(
        scratches->num_scratches + glyph->num_edges <= GLYPH_SCRATCHES_LEN,
        "Glyph scratches full"
    );

    // Jitter vertices and find undistorted width
    static Vector2 verts[GLYPH_MAX_VERTS];
    f32 min_x = glyph->num_verts > 0 ? MAX_F32 : 0.0f;
    f32 max_x = glyph->num_verts > 0 ? -MAX_F32 : 0.0f;
    for (i32 v = 0; v < glyph->num_verts; v++) {
        verts[v] = Vector2Add(glyph->verts[v], RandomCircle(GLYPH_VERT_TRAVEL));
        min_x = Min(min_x, glyph->verts[v].x);
        max_x = Max(max_x, glyph->verts[v].x);
    }

    // Narrow glyphs are centered in min width
    f32 width = (max_x - min_x) * GLYPH_SIZE;
    f32 advance = Max(width, GLYPH_MIN_WIDTH);
    Vector2 origin = {
        cursor->x + (advance - width) * 0.5f - min_x * GLYPH_SIZE,
        cursor->y
    };

    // Edges rotate around midpoint and overrun vertices
    for (i32 i = 0; i < glyph->num_edges; i++) {
        IntVector2 edge = glyph->edges[i];
        Vector2 a = verts[edge.x];
        Vector2 b = verts[edge.y];

        Vector2 mid = Vector2Scale(Vector2Add(a, b), 0.5f);
        f32 rot = GLYPH_EDGE_ROT * RandomSignedF32();
        a = Vector2Add(mid, Vector2Rotate(Vector2Subtract(a, mid), rot));
        b = Vector2Add(mid, Vector2Rotate(Vector2Subtract(b, mid), rot));

        Vector2 diff = Vector2Subtract(b, a);

        f32 a_overrun = -(RandomF32() * GLYPH_OVERRUN);
        f32 b_overrun = RandomF32() * GLYPH_OVERRUN;

        a = Vector2Add(a, Vector2Scale(diff, a_overrun));
        b = Vector2Add(b, Vector2Scale(diff, b_overrun));

        // Scale and offset
        i32 n = scratches->num_scratches++;
        scratches->start[n] = Vector2Add(Vector2Scale(a, GLYPH_SIZE), origin);
        scratches->end[n] = Vector2Add(Vector2Scale(b, GLYPH_SIZE), origin);
    }

    // Move cursor
    cursor->x += advance + GLYPH_SEPARATION + GLYPH_EXTRA * RandomF32();
}

void GlyphScratchesAddString(
    GlyphScratches* scratches,
    GlyphAtlas* atlas,
    const char* text,
    Vector2* cursor
) {
    f32 line_start = cursor->x;

    for (i32 i = 0; text[i] != '\0'; i++) {
        u8 code = (u8)text[i];

        // Space and newline have no glyph, they only move the cursor
        if (code == ' ') {
            cursor->x += GLYPH_SPACE_WIDTH;
            continue;
        }
        if (code == '\n') {
            cursor->x = line_start;
            cursor->y += GLYPH_LINE_HEIGHT;
            continue;
        }

        Assert(code < atlas->num_glyphs, "No glyph for code %d", code);
        GlyphScratchesAdd(scratches, &atlas->glyphs[code], cursor);
    }
}
