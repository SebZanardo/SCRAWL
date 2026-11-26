#ifndef GLYPH_H
#define GLYPH_H

#include "core.h"

#define GLYPH_MAX_VERTS 32
#define GLYPH_MAX_EDGES 32
#define GLYPH_VERT_TRAVEL 0.1f
#define GLYPH_EDGE_ROT 0.08f
#define GLYPH_OVERRUN 0.2f
#define GLYPH_SIZE 48.0f
#define GLYPH_THICKNESS 2.0f
#define GLYPH_SEPARATION 6.0f
#define GLYPH_EXTRA 4.0f
#define GLYPH_MIN_WIDTH (GLYPH_SIZE * 0.25f)
#define GLYPH_SPACE_WIDTH (GLYPH_SIZE * 0.5f)
#define GLYPH_LINE_HEIGHT (GLYPH_SIZE * 1.25f)

#define GLYPH_ATLAS_GLYPHS_LEN 128
#define GLYPH_ATLAS_VERTS_LEN (GLYPH_ATLAS_GLYPHS_LEN * GLYPH_MAX_VERTS)
#define GLYPH_ATLAS_EDGES_LEN (GLYPH_ATLAS_GLYPHS_LEN * GLYPH_MAX_EDGES)

#define GLYPH_SCRATCHES_LEN 4096

typedef struct {
    Vector2 start[GLYPH_SCRATCHES_LEN];
    Vector2 end[GLYPH_SCRATCHES_LEN];
    i32 num_scratches;
} GlyphScratches;

typedef struct {
    Vector2* verts;
    IntVector2* edges;
    i32 num_verts;
    i32 num_edges;
} Glyph;

typedef struct {
    Glyph glyphs[GLYPH_ATLAS_GLYPHS_LEN];
    Vector2 verts[GLYPH_ATLAS_VERTS_LEN];
    IntVector2 edges[GLYPH_ATLAS_EDGES_LEN];
    i32 num_glyphs;
    i32 num_verts;
    i32 num_edges;
} GlyphAtlas;

void GlyphAtlasAdd(
    GlyphAtlas* atlas,
    Vector2* verts,
    IntVector2* edges,
    i32 num_verts,
    i32 num_edges
);

void GlyphScratchesAdd(
    GlyphScratches* scratches,
    Glyph* glyph,
    Vector2* cursor
);

void GlyphScratchesAddString(
    GlyphScratches* scratches,
    GlyphAtlas* atlas,
    const char* text,
    Vector2* cursor
);

#endif // GLYPH_H
