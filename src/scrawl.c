#include "scrawl.h"

// Parses number in 0..MAX_U8 from start of str, returns where it ended
char* ScrawlParseInt(char* str, i32* out) {
    char* end = NULL;
    long value = strtol(str, &end, 10);
    Assert(end != str && value >= 0 && value <= MAX_U8, "Invalid number \"%s\"", str);

    *out = (i32)value;
    return end;
}

void ScrawlLoad(GlyphAtlas* atlas, char* path) {
    char* text = LoadFileText(path);
    Assert(text != NULL, "Unable to load %s", path);

    // Header
    char* delims = " \t\r\n";
    char* token = strtok(text, delims);
    Assert(token != NULL && strcmp(token, "SCRAWL") == 0, "%s: Missing header", path);

    i32 version = 0;
    token = strtok(NULL, delims);
    Assert(token != NULL && *ScrawlParseInt(token, &version) == '\0', "%s: Missing version", path);
    Assert(version == SCRAWL_VERSION, "%s: Unsupported version %d", path, version);

    // Glyph index is ASCII code
    atlas->num_glyphs = 0;
    atlas->num_verts = 0;
    atlas->num_edges = 0;

    // Tokens are code, x,y or a-b
    i32 code = -1;
    Glyph* glyph = NULL;
    while ((token = strtok(NULL, delims)) != NULL) {
        i32 a = 0;
        i32 b = 0;
        char* end = ScrawlParseInt(token, &a);
        char separator = *end;
        if (separator != '\0') {
            end = ScrawlParseInt(end + 1, &b);
            Assert(*end == '\0', "%s: Invalid token \"%s\"", path, token);
        }

        // Code, skipped codes are empty glyphs
        if (separator == '\0') {
            Assert(a >= atlas->num_glyphs && a < GLYPH_ATLAS_GLYPHS_LEN, "%s: Glyph %d out of order", path, a);
            while (atlas->num_glyphs <= a) {
                GlyphAtlasAdd(atlas, NULL, NULL, 0, 0);
            }
            code = a;
            glyph = &atlas->glyphs[code];
            continue;
        }

        Assert(glyph != NULL, "%s: \"%s\" before first glyph code", path, token);

        // Vert x,y, must come before edges
        if (separator == ',') {
            Assert(
                a <= SCRAWL_GRID_MAX && b <= SCRAWL_GRID_MAX &&
                glyph->num_edges == 0 && glyph->num_verts < GLYPH_MAX_VERTS,
                "%s: Glyph %d invalid vert \"%s\"", path, code, token
            );
            atlas->verts[atlas->num_verts++] = (Vector2) {
                (f32)a / SCRAWL_GRID_MAX,
                (f32)b / SCRAWL_GRID_MAX
            };
            glyph->num_verts++;
        }

        // Edge a-b
        else if (separator == '-') {
            Assert(
                a < glyph->num_verts && b < glyph->num_verts &&
                glyph->num_edges < GLYPH_MAX_EDGES,
                "%s: Glyph %d invalid edge \"%s\"", path, code, token
            );
            atlas->edges[atlas->num_edges++] = (IntVector2) { a, b };
            glyph->num_edges++;
        }

        else {
            Assert(false, "%s: Invalid token \"%s\"", path, token);
        }
    }

    // Trailing codes are empty glyphs
    while (atlas->num_glyphs < GLYPH_ATLAS_GLYPHS_LEN) {
        GlyphAtlasAdd(atlas, NULL, NULL, 0, 0);
    }

    UnloadFileText(text);
}

void ScrawlSave(GlyphAtlas* atlas, char* path) {
    // NOTE: Sized for every glyph at max verts and edges, cannot overflow
    static char text[SCRAWL_TEXT_LEN];
    i32 len = snprintf(text, SCRAWL_TEXT_LEN, "SCRAWL %d\n", SCRAWL_VERSION);

    for (i32 i = 0; i < atlas->num_glyphs; i++) {
        Glyph* glyph = &atlas->glyphs[i];

        // Empty glyphs are left out
        if (glyph->num_verts == 0 && glyph->num_edges == 0) { continue; }

        len += snprintf(text + len, SCRAWL_TEXT_LEN - len, "%d", i);

        for (i32 v = 0; v < glyph->num_verts; v++) {
            Vector2 vert = Vector2Clamp(glyph->verts[v], Vector2Zero(), Vector2One());
            i32 x = (i32)round_f32(vert.x * SCRAWL_GRID_MAX);
            i32 y = (i32)round_f32(vert.y * SCRAWL_GRID_MAX);
            len += snprintf(text + len, SCRAWL_TEXT_LEN - len, " %d,%d", x, y);
        }

        for (i32 e = 0; e < glyph->num_edges; e++) {
            IntVector2 edge = glyph->edges[e];
            len += snprintf(text + len, SCRAWL_TEXT_LEN - len, " %d-%d", edge.x, edge.y);
        }

        len += snprintf(text + len, SCRAWL_TEXT_LEN - len, "\n");
    }

    bool is_saved = SaveFileText(path, text);
    Assert(is_saved, "Unable to save %s", path);
}
