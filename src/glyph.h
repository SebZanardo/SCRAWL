#ifndef GLYPH_H
#define GLYPH_H

#include "core.h"

#define GLYPH_MAX_VERTS 1024
#define GLYPH_MAX_EDGES 1024
#define GLYPH_MAX_GLYPHS 128

typedef struct {
    i32 verts_index;
    i32 edges_index;
    i32 num_verts;
    i32 num_edges;
} Glyph;

typedef struct {
    Glyph glyphs[GLYPH_MAX_GLYPHS];
    Vector2 verts[GLYPH_MAX_VERTS];
    IntVector2 edges[GLYPH_MAX_EDGES];
    i32 num_glyphs;
    i32 num_verts;
    i32 num_edges;
} GlyphMap;

Glyph *GlyphAdd(
    GlyphMap *glyph_map,
    Vector2 *verts,
    IntVector2 *edges,
    i32 num_verts,
    i32 num_edges
);

#endif // GLYPH_H
