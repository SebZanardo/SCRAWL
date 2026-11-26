#ifndef SCRAWL_H
#define SCRAWL_H

#include "core.h"
#include "glyph.h"

// .scrawl
// Text font format. One row per glyph in ascending code order.
// Empty glyphs may be left out.
//
// Header format:
//      SCRAWL <version>
//
// Row format:
//      <code> <vert_pairs x,y> <edge_pairs a-b>
//
//      * code: ASCII code (0-31) free reign for fun symbols
//      * vert_pairs: pairs of coordinates in range 0..SCRAWL_GRID_MAX
//          comma separated and max 32 vert pairs
//      * edge_pairs: pairs of vert indexes in range 0..31
//          dash separated and max 32 edge pairs
//
// File example:
//      SCRAWL 1
//      83 44,13 32,6 19,13 44,50 32,57 19,50 0-1 1-2 2-3 3-4 4-5

#define SCRAWL_VERSION 1
#define SCRAWL_GRID_MAX 63

#define SCRAWL_ROW_LEN (3 + GLYPH_MAX_VERTS * 6 + GLYPH_MAX_EDGES * 6 + 1)
#define SCRAWL_TEXT_LEN (KiB(1) + GLYPH_ATLAS_GLYPHS_LEN * SCRAWL_ROW_LEN)

void ScrawlLoad(GlyphAtlas* atlas, char* path);
void ScrawlSave(GlyphAtlas* atlas, char* path);

#endif // SCRAWL_H
