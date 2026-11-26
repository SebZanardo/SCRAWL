#include "glyph.h"

Glyph* GlyphAtlasAdd(
    GlyphAtlas* atlas,
    Vector2* verts,
    IntVector2* edges,
    i32 num_verts,
    i32 num_edges
) {
    // Invalid
    if (
        atlas->num_glyphs >= GLYPH_ATLAS_GLYPHS_LEN ||
        atlas->num_verts + num_verts > GLYPH_ATLAS_VERTS_LEN ||
        atlas->num_edges + num_edges > GLYPH_ATLAS_EDGES_LEN ||
        num_verts > GLYPH_MAX_VERTS ||
        num_edges > GLYPH_MAX_EDGES
    ) {
        return NULL;
    }

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

    return glyph;
}

bool GlyphScratchesAdd(
    GlyphScratches* scratches,
    Glyph* glyph,
    Vector2* cursor
) {
    // Invalid
    if (scratches->num_scratches + glyph->num_edges > GLYPH_SCRATCHES_LEN) {
        return false;
    }

    static Vector2 verts[GLYPH_MAX_VERTS];
    for (i32 v = 0; v < glyph->num_verts; v++) {
        verts[v] = Vector2Add(glyph->verts[v], RandomCircle(GLYPH_VERT_TRAVEL));
    }

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

        scratches->start[scratches->num_scratches + i] = a;
        scratches->end[scratches->num_scratches + i] = b;
    }

    // Scale and offset
    for (i32 i = 0; i < glyph->num_edges; i++) {
        Vector2* start = &scratches->start[scratches->num_scratches + i];
        *start = Vector2Scale(*start, GLYPH_SIZE);
        *start = Vector2Add(*start, *cursor);

        Vector2 *end = &scratches->end[scratches->num_scratches + i];
        *end = Vector2Scale(*end, GLYPH_SIZE);
        *end = Vector2Add(*end, *cursor);
    }

    // Increment scratches count
    scratches->num_scratches += glyph->num_edges;

    // Move cursor
    cursor->x += GLYPH_SIZE;

    return true;
}

