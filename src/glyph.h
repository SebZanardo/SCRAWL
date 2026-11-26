#ifndef GLYPH_H
#define GLYPH_H

#include "core.h"

#define GLYPH_MAX_VERTS 1024
#define GLYPH_MAX_EDGES 1024
#define GLYPH_MAX_GLYPHS 128
#define GLYPH_MAX_SCRATCHES 4096

#define GLYPH_VERT_TRAVEL 0.03f
#define GLYPH_OVERRUN 0.1f
#define GLYPH_SIZE 100.0f
#define GLYPH_THICKNESS 2.0f

typedef struct {
    Vector2 start[GLYPH_MAX_SCRATCHES];
    Vector2 end[GLYPH_MAX_SCRATCHES];
    i32 num_scratches;
} GlyphScratches;

typedef struct {
    Vector2 *verts;
    IntVector2 *edges;
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

bool GlyphAddScratch(
    GlyphScratches *scratches,
    Glyph *glyph,
    Vector2 *cursor
);

#endif // GLYPH_H
