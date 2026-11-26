#include "glyph.h"

Glyph *GlyphAdd(
    GlyphMap *glyph_map,
    Vector2 *verts,
    IntVector2 *edges,
    i32 num_verts,
    i32 num_edges
) {
    // Invalid
    if (
        glyph_map->num_glyphs >= GLYPH_MAX_GLYPHS ||
        glyph_map->num_verts + num_verts >= GLYPH_MAX_VERTS ||
        glyph_map->num_edges + num_edges >= GLYPH_MAX_EDGES
    ) {
        return NULL;
    }

    // Add glyph
    Glyph *glyph = &glyph_map->glyphs[glyph_map->num_glyphs++];
    glyph->verts = &glyph_map->verts[glyph_map->num_verts];
    glyph->edges = &glyph_map->edges[glyph_map->num_edges];
    glyph->num_verts = num_verts;
    glyph->num_edges = num_edges;

    // Add vertices
    for (i32 i = 0; i < num_verts; i++) {
        glyph_map->verts[glyph_map->num_verts++] = verts[i];
    }

    // Add edges
    for (i32 i = 0; i < num_edges; i++) {
        glyph_map->edges[glyph_map->num_edges++] = edges[i];
    }

    return glyph;
}

bool GlyphAddScratch(
    GlyphScratches *scratches,
    Glyph *glyph,
    Vector2 *cursor
) {
    // Invalid
    if (scratches->num_scratches + glyph->num_edges > GLYPH_MAX_SCRATCHES) {
        return false;
    }

    // Walk vertices & overrun edges
    for (i32 i = 0; i < glyph->num_edges; i++) {
        IntVector2 edge = glyph->edges[i];

        Vector2 a_walk = RandomCircle(GLYPH_VERT_TRAVEL);
        Vector2 a = Vector2Add(glyph->verts[edge.x], a_walk);

        Vector2 b_walk = RandomCircle(GLYPH_VERT_TRAVEL);
        Vector2 b = Vector2Add(glyph->verts[edge.y], b_walk);

        Vector2 diff = Vector2Subtract(b, a);

        f32 a_overrun = -(RandomF32() * GLYPH_OVERRUN);
        f32 b_overrun = RandomF32() * GLYPH_OVERRUN;

        Vector2 *start = &scratches->start[scratches->num_scratches + i];
        *start = Vector2Add(a, Vector2Scale(diff, a_overrun));

        Vector2 *end = &scratches->end[scratches->num_scratches + i];
        *end = Vector2Add(b, Vector2Scale(diff, b_overrun));
    }

    // Scale and offset
    for (i32 i = 0; i < glyph->num_edges; i++) {
        Vector2 *start = &scratches->start[scratches->num_scratches + i];
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

