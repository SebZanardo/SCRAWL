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
    glyph->edges_index = glyph_map->num_edges;
    glyph->verts_index = glyph_map->num_verts;
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
