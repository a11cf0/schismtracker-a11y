/*
 * Schism Tracker - a cross-platform Impulse Tracker clone
 * copyright (c) 2003-2005 Storlek <storlek@rigelseven.com>
 * copyright (c) 2005-2008 Mrs. Brisby <mrs.brisby@nimh.org>
 * copyright (c) 2009 Storlek & Mrs. Brisby
 * copyright (c) 2010-2012 Storlek
 * URL: http://schismtracker.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "headers.h"

#include "charset.h"
#include "it.h"
#include "vgamem.h"
#include "fonts.h"
#include "song.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>

#define SAMPLE_DATA_COLOR 13 /* Sample data */
#define SAMPLE_LOOP_COLOR 3 /* Sample loop marks */
#define SAMPLE_MARK_COLOR 6 /* Play mark color */
#define SAMPLE_BGMARK_COLOR 7 /* Play mark color after note fade / NNA */

/* internal vgamem data structures */
enum vgamem_font {
	VGAMEM_FONT_ITF,       // ASCII with weird cool box characters
	VGAMEM_FONT_BIOS,      // codepage 437
	VGAMEM_FONT_HALFWIDTH, // ASCII - half-width; used in the info page and pattern editor
	VGAMEM_FONT_OVERLAY,   // none - draws from the overlay buffer
	VGAMEM_FONT_UNICODE,   // UCS-4 - any unicode codepoint
};

struct vgamem_colors {
	uint_fast8_t fg; /* 0...15 */
	uint_fast8_t bg; /* 0...15 */
};

/* contains all the needed information to draw many
 * types of characters onto the screen. historically,
 * all of this information was stored in a single 32-bit
 * unsigned integer */
struct vgamem_char {
	/* which font method to use */
	enum vgamem_font font;

	union {
		struct {
			/* two chars here */
			struct {
				uint_fast8_t c; /* 0...255 */

				struct vgamem_colors colors;
			} c1, c2;
		} halfwidth;

		struct {
			uint_fast8_t c; /* 0...255 */

			struct vgamem_colors colors;
		} cp437, itf; /* cp437 and itf have the same size */

		struct {
			/* can be any Unicode codepoint; realistically
			 * only a very small subset of characters can be
			 * supported though */
			uint_fast32_t c;

			struct vgamem_colors colors;
		} unicode;
	} character;
};

static struct vgamem_char vgamem[4000] = {0};
static struct vgamem_char vgamem_read[4000] = {0};

#ifdef USE_ACCESSIBILITY
// I know this is horrible!
typedef uint32_t acbuf_line[80];
static acbuf_line acbuf[50] = { 0 };

uint32_t* acbuf_get_ptr_to(int x, int y)
{
	assert(x >= 0 && y >= 0 && x < 80 && y < 50);

	return &acbuf[y][x];
}
#endif

static uint8_t ovl[640*400] = {0}; /* 256K */

#define CHECK_INVERT(tl,br,n) \
do {                                            \
	if (status.flags & INVERTED_PALETTE) {  \
		n = tl;                         \
		tl = br;                        \
		br = n;                         \
	}                                       \
} while(0)

void vgamem_flip(void)
{
	memcpy(vgamem_read, vgamem, sizeof(vgamem));
}

void vgamem_clear(void)
{
	memset(vgamem,0,sizeof(vgamem));
}

void vgamem_ovl_alloc(struct vgamem_overlay *n)
{
	n->q = &ovl[ (n->x1*8) + (n->y1 * 5120) ];
	n->width = 8 * ((n->x2 - n->x1) + 1);
	n->height = 8 * ((n->y2 - n->y1) + 1);
	n->skip = (640 - n->width);
}

void vgamem_ovl_apply(struct vgamem_overlay *n)
{
	unsigned int x, y;

	for (y = n->y1; y <= n->y2; y++)
		for (x = n->x1; x <= n->x2; x++)
			vgamem[x + (y*80)].font = VGAMEM_FONT_OVERLAY;
}

void vgamem_ovl_clear(struct vgamem_overlay *n, int color)
{
	int i, j;
	unsigned char *q = n->q;
	for (j = 0; j < n->height; j++) {
		for (i = 0; i < n->width; i++) {
			*q = color;
			q++;
		}
		q += n->skip;
	}
}

void vgamem_ovl_drawpixel(struct vgamem_overlay *n, int x, int y, int color)
{
	n->q[ (640*y) + x ] = color;
}

static inline void _draw_line_v(struct vgamem_overlay *n, int x,
		int ys, int ye, int color)
{
	unsigned char *q = n->q + x;
	int y;

	if (ys < ye) {
		q += (ys * 640);
		for (y = ys; y <= ye; y++) {
			*q = color;
			q += 640;
		}
	} else {
		q += (ye * 640);
		for (y = ye; y <= ys; y++) {
			*q = color;
			q += 640;
		}
	}
}

static inline void _draw_line_h(struct vgamem_overlay *n, int xs,
		int xe, int y, int color)
{
	unsigned char *q = n->q + (y * 640);
	int x;
	if (xs < xe) {
		q += xs;
		for (x = xs; x <= xe; x++) {
			*q = color;
			q++;
		}
	} else {
		q += xe;
		for (x = xe; x <= xs; x++) {
			*q = color;
			q++;
		}
	}
}

#ifndef ABS
# define ABS(x) ((x) < 0 ? -(x) : (x))
#endif
#ifndef SGN
# define SGN(x) ((x) < 0 ? -1 : 1)      /* hey, what about zero? */
#endif

void vgamem_ovl_drawline(struct vgamem_overlay *n, int xs,
	int ys, int xe, int ye, int color)
{
	int d, x, y, ax, ay, sx, sy, dx, dy;

	dx = xe - xs;
	if (dx == 0) {
		_draw_line_v(n, xs, ys, ye, color);
		return;
	}

	dy = ye - ys;
	if (dy == 0) {
		_draw_line_h(n, xs, xe, ys, color);
		return;
	}

	ax = ABS(dx) << 1;
	sx = SGN(dx);
	ay = ABS(dy) << 1;
	sy = SGN(dy);

	x = xs;
	y = ys;
	if (ax > ay) {
		/* x dominant */
		d = ay - (ax >> 1);
		for (;;) {
			vgamem_ovl_drawpixel(n, x, y, color);
			if (x == xe) break;
			if (d >= 0) {
				y += sy;
				d -= ax;
			}
			x += sx;
			d += ay;
		}
	} else {
		/* y dominant */
		d = ax - (ay >> 1);
		for (;;) {
			vgamem_ovl_drawpixel(n, x, y, color);
			if (y == ye) break;
			if (d >= 0) {
				x += sx;
				d -= ay;
			}
			y += sy;
			d += ax;
		}
	}
}

/* generic scanner; BITS must be one of 8, 16, 32, 64
 *
 * I've tried to make this code as small and predictable
 * as possible in an effort to make it fast as I can.
 *
 * Older versions prioritized memory efficiency over speed,
 * as in every character was packed into a 32-bit integer.
 * Arguably this is a bad choice, especially considering
 * that this is the most taxing function to call in the
 * whole program (the audio crap doesn't even come close)
 * In a normal session, this function will probably amount
 * for ~80% of all processing that Schism does. */
#define VGAMEM_SCANNER_VARIANT(BITS) \
	void vgamem_scan##BITS(uint32_t ry, uint##BITS##_t *out, uint32_t tc[16], uint32_t mouseline[80], uint32_t mouseline_mask[80]) \
	{ \
		/* constants */ \
		const uint_fast32_t y = (ry >> 3), yl = (ry & 7); \
		const uint8_t *q = ovl + (ry * 640); \
		const uint8_t *const itf = font_data + yl, \
			*const bios = font_default_upper_alt + yl, \
			*const bioslow = font_default_lower + yl, \
			*const hf = font_half_data + (yl >> 1), \
			*const hiragana = font_hiragana + yl, \
			*const extlatin = font_extended_latin + yl, \
			*const greek = font_greek + yl; \
		const struct vgamem_char *bp = &vgamem_read[y * 80]; \
	\
		uint_fast32_t x; \
		for (x = 0; x < 80; x++, bp++, q += 8) { \
			uint_fast8_t fg, bg, fg2, bg2, dg; \
	\
			switch (bp->font) { \
			case VGAMEM_FONT_ITF: \
				/* regular character */ \
				fg = bp->character.itf.colors.fg; \
				bg = bp->character.itf.colors.bg; \
				fg2 = fg; \
				bg2 = bg; \
				dg = itf[bp->character.itf.c << 3]; \
				break; \
			case VGAMEM_FONT_BIOS: \
				/* VGA BIOS character */ \
				fg = bp->character.cp437.colors.fg; \
				bg = bp->character.cp437.colors.bg; \
				fg2 = fg; \
				bg2 = bg; \
				dg = (bp->character.cp437.c & 0x80) \
					? bios[(bp->character.cp437.c & 0x7F) << 3] \
					: bioslow[(bp->character.cp437.c & 0x7F) << 3]; \
				break; \
			case VGAMEM_FONT_HALFWIDTH: \
				/* halfwidth (used for patterns) */ \
				{ \
					const uint_fast8_t dg1 = hf[bp->character.halfwidth.c1.c << 2]; \
					const uint_fast8_t dg2 = hf[bp->character.halfwidth.c2.c << 2]; \
	\
					dg = (!(ry & 1)) \
						? ((dg1 & 0xF0) | dg2 >> 4) \
						: (dg1 << 4 | (dg2 & 0xF)); \
				} \
	\
				fg = bp->character.halfwidth.c1.colors.fg; \
				bg = bp->character.halfwidth.c1.colors.bg; \
				fg2 = bp->character.halfwidth.c2.colors.fg; \
				bg2 = bp->character.halfwidth.c2.colors.bg; \
				break; \
			case VGAMEM_FONT_OVERLAY: \
				/* raw pixel data, needs special code ;) */ \
				*out++ = tc[ (q[0]|((mouseline[x] & 0x80)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[1]|((mouseline[x] & 0x40)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[2]|((mouseline[x] & 0x20)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[3]|((mouseline[x] & 0x10)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[4]|((mouseline[x] & 0x08)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[5]|((mouseline[x] & 0x04)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[6]|((mouseline[x] & 0x02)?15:0)) & 0xFF]; \
				*out++ = tc[ (q[7]|((mouseline[x] & 0x01)?15:0)) & 0xFF]; \
				continue; \
			case VGAMEM_FONT_UNICODE: { \
				/* Any unicode character. */ \
				const uint_fast32_t c = bp->character.unicode.c; \
	\
				/* These are ordered by how often they will probably appear
				 * for an average user of Schism (i.e., English speakers). */ \
				if (c >= 0x20 && c <= 0x7F) { \
					/* ASCII */ \
					dg = itf[c << 3]; \
				} else if (c >= 0xA0 && c <= 0xFF) { \
					/* extended latin */ \
					dg = extlatin[(c - 0xA0) << 3]; \
				} else if (c >= 0x390 && c <= 0x3C9) { \
					/* greek */ \
					dg = greek[(c - 0x390) << 3]; \
				} else if (c >= 0x3040 && c <= 0x309F) { \
					/* japanese hiragana */ \
					dg = hiragana[(c - 0x3040) << 3]; \
				} else { \
					/* will display a ? if no cp437 equivalent found */ \
					dg = itf[char_unicode_to_cp437(c) << 3]; \
				} \
	\
				fg = bp->character.unicode.colors.fg; \
				bg = bp->character.unicode.colors.bg; \
				fg2 = fg; \
				bg2 = bg; \
	\
				break; \
			} \
			default: continue; \
			} \
	\
			dg |= mouseline[x]; \
			dg &= ~(mouseline_mask[x] ^ mouseline[x]); \
	\
			*out++ = tc[(dg & 0x80) ? fg : bg]; \
			*out++ = tc[(dg & 0x40) ? fg : bg]; \
			*out++ = tc[(dg & 0x20) ? fg : bg]; \
			*out++ = tc[(dg & 0x10) ? fg : bg]; \
			*out++ = tc[(dg & 0x8) ? fg2 : bg2]; \
			*out++ = tc[(dg & 0x4) ? fg2 : bg2]; \
			*out++ = tc[(dg & 0x2) ? fg2 : bg2]; \
			*out++ = tc[(dg & 0x1) ? fg2 : bg2]; \
		} \
	}

VGAMEM_SCANNER_VARIANT(8)
VGAMEM_SCANNER_VARIANT(16)
VGAMEM_SCANNER_VARIANT(32)

#undef VGAMEM_SCAN_VARIANT

void draw_char_unicode(uint32_t c, int x, int y, uint32_t fg, uint32_t bg)
{
	assert(x >= 0 && y >= 0 && x < 80 && y < 50);

	struct vgamem_char ch;
	ch.font = VGAMEM_FONT_UNICODE;
	ch.character.unicode.c = c;
	ch.character.unicode.colors.fg = fg;
	ch.character.unicode.colors.bg = bg;

	vgamem[x + (y*80)] = ch;
#ifdef USE_ACCESSIBILITY
	acbuf[y][x] = c;
#endif
}

void draw_char_bios(uint8_t c, int x, int y, uint32_t fg, uint32_t bg)
{
	assert(x >= 0 && y >= 0 && x < 80 && y < 50);

	struct vgamem_char ch;
	ch.font = VGAMEM_FONT_BIOS;
	ch.character.cp437.c = c;
	ch.character.cp437.colors.fg = fg;
	ch.character.cp437.colors.bg = bg;

	vgamem[x + (y*80)] = ch;

#ifdef USE_ACCESSIBILITY
	char text[2] = { c, '\0' };
	charset_decode_t decoder = {
		.in = text,
		.offset = 0,
		.size = SIZE_MAX,
	};
	for (int i = 0; decoder.state == DECODER_STATE_NEED_MORE && !charset_decode_next(&decoder, CHARSET_CP437) && decoder.state != DECODER_STATE_DONE; i++)
		acbuf[y][x + i] = decoder.codepoint;
#endif
}

void draw_char(uint8_t c, int x, int y, uint32_t fg, uint32_t bg)
{
	assert(x >= 0 && y >= 0 && x < 80 && y < 50);

	struct vgamem_char ch;

	ch.font = VGAMEM_FONT_ITF,
	ch.character.itf.c = c;
	ch.character.itf.colors.fg = fg;
	ch.character.itf.colors.bg = bg;

	vgamem[x + (y*80)] = ch;

#ifdef USE_ACCESSIBILITY
	if (isprint(c))
		acbuf[y][x] = c;
	else
		acbuf[y][x] = 0;
#endif
}

int draw_text(const char * text, int x, int y, uint32_t fg, uint32_t bg)
{
	int n = 0;

	while (*text) {
		draw_char(*text, x + n, y, fg, bg);
		n++;
		text++;
	}

	return n;
}

int draw_text_bios(const char * text, int x, int y, uint32_t fg, uint32_t bg)
{
	int n = 0;

	while (*text) {
		draw_char_bios(*text, x + n, y, fg, bg);
		n++;
		text++;
	}

	return n;
}

int draw_text_utf8(const char * text, int x, int y, uint32_t fg, uint32_t bg)
{
	uint8_t *composed = charset_compose_to_utf8(text, CHARSET_UTF8);
	if (!composed)
		return draw_text_bios(text, x, y, fg, bg);

	charset_decode_t decoder = {0};
	decoder.in = composed;
	decoder.offset = 0;
	decoder.size = SIZE_MAX;

	int n;
	for (n = 0; decoder.state == DECODER_STATE_NEED_MORE && !charset_decode_next(&decoder, CHARSET_UTF8) && decoder.state != DECODER_STATE_DONE; n++) {
		draw_char_unicode(decoder.codepoint, x + n, y, fg, bg);
	}

	return n;
}

void draw_fill_chars(int xs, int ys, int xe, int ye, uint32_t fg, uint32_t bg)
{
	struct vgamem_char *mm;
	int x, len;
	mm = &vgamem[(ys * 80) + xs];
	len = (xe - xs)+1;
	ye -= ys;

#ifdef USE_ACCESSIBILITY
	for (int y = ys; y < ys + ye; y++) {
		for (x = xs; x < xe; x++) {
			acbuf[y][x] = 0;
		}
	}
#endif

	do {
		for (x = 0; x < len; x++) {
			mm[x].font = VGAMEM_FONT_ITF;
			mm[x].character.itf.c = 0;
			mm[x].character.itf.colors.fg = fg;
			mm[x].character.itf.colors.bg = bg;
		}
		mm += 80;
		ye--;
	} while (ye >= 0);
}

int draw_text_len(const char * text, int len, int x, int y, uint32_t fg, uint32_t bg)
{
	int n = 0;

	while (*text && n < len) {
		draw_char(*text, x + n, y, fg, bg);
		n++;
		text++;
	}
	draw_fill_chars(x + n, y, x + len - 1, y, fg, bg);

	return n;
}

int draw_text_bios_len(const char * text, int len, int x, int y, uint32_t fg, uint32_t bg)
{
	int n = 0;

	while (*text && n < len) {
		draw_char_bios(*text, x + n, y, fg, bg);
		n++;
		text++;
	}
	draw_fill_chars(x + n, y, x + len - 1, y, fg, bg);

	return n;
}

int draw_text_utf8_len(const char * text, int len, int x, int y, uint32_t fg, uint32_t bg)
{
	uint8_t *composed = charset_compose_to_utf8(text, CHARSET_UTF8);
	if (!composed)
		return draw_text_bios_len(text, len, x, y, fg, bg);

	charset_decode_t decoder = {0};
	decoder.in = composed;
	decoder.offset = 0;
	decoder.size = SIZE_MAX;

	int n;
	for (n = 0; n < len && decoder.state == DECODER_STATE_NEED_MORE && !charset_decode_next(&decoder, CHARSET_UTF8) && decoder.state != DECODER_STATE_DONE; n++) {
		draw_char_unicode(decoder.codepoint, x + n, y, fg, bg);
	}

	draw_fill_chars(x + n, y, x + len - 1, y, fg, bg);

	return n;
}

/* --------------------------------------------------------------------- */

void draw_half_width_chars(uint8_t c1, uint8_t c2, int x, int y,
			   uint32_t fg1, uint32_t bg1, uint32_t fg2, uint32_t bg2)
{
	assert(x >= 0 && y >= 0 && x < 80 && y < 50);

	struct vgamem_char ch;
	ch.font = VGAMEM_FONT_HALFWIDTH,
	ch.character.halfwidth.c1.c = c1;
	ch.character.halfwidth.c1.colors.fg = fg1;
	ch.character.halfwidth.c1.colors.bg = bg1;
	ch.character.halfwidth.c2.c = c2;
	ch.character.halfwidth.c2.colors.fg = fg2;
	ch.character.halfwidth.c2.colors.bg = bg2;

	vgamem[x + (y*80)] = ch;
}
/* --------------------------------------------------------------------- */
/* boxes */

enum box_type {
	BOX_THIN_INNER = 0, BOX_THIN_OUTER, BOX_THICK_OUTER
};

static const uint8_t boxes[4][8] = {
	{139, 138, 137, 136, 134, 129, 132, 131},       /* thin inner */
	{128, 130, 133, 135, 129, 134, 131, 132},       /* thin outer */
	{142, 144, 147, 149, 143, 148, 145, 146},       /* thick outer */
};

static void _draw_box_internal(int xs, int ys, int xe, int ye, uint32_t tl, uint32_t br, const uint8_t ch[8])
{
	int n;

	CHECK_INVERT(tl, br, n);

	draw_char(ch[0], xs, ys, tl, 2);       /* TL corner */
	draw_char(ch[1], xe, ys, br, 2);       /* TR corner */
	draw_char(ch[2], xs, ye, br, 2);       /* BL corner */
	draw_char(ch[3], xe, ye, br, 2);       /* BR corner */

	for (n = xs + 1; n < xe; n++) {
		draw_char(ch[4], n, ys, tl, 2);        /* top */
		draw_char(ch[5], n, ye, br, 2);        /* bottom */
	}
	for (n = ys + 1; n < ye; n++) {
		draw_char(ch[6], xs, n, tl, 2);        /* left */
		draw_char(ch[7], xe, n, br, 2);        /* right */
	}
}

static void draw_thin_inner_box(int xs, int ys, int xe, int ye, uint32_t tl, uint32_t br)
{
	_draw_box_internal(xs, ys, xe, ye, tl, br, boxes[BOX_THIN_INNER]);
}

static void draw_thick_inner_box(int xs, int ys, int xe, int ye, uint32_t tl, uint32_t br)
{
	/* this one can't use _draw_box_internal because the corner
	 * colors are different */

	int n;

	CHECK_INVERT(tl, br, n);

	draw_char(153, xs, ys, tl, 2); /* TL corner */
	draw_char(152, xe, ys, tl, 2); /* TR corner */
	draw_char(151, xs, ye, tl, 2); /* BL corner */
	draw_char(150, xe, ye, br, 2); /* BR corner */

	for (n = xs + 1; n < xe; n++) {
		draw_char(148, n, ys, tl, 2);  /* top */
		draw_char(143, n, ye, br, 2);  /* bottom */
	}
	for (n = ys + 1; n < ye; n++) {
		draw_char(146, xs, n, tl, 2);  /* left */
		draw_char(145, xe, n, br, 2);  /* right */
	}
}

static void draw_thin_outer_box(int xs, int ys, int xe, int ye, uint32_t c)
{
	_draw_box_internal(xs, ys, xe, ye, c, c, boxes[BOX_THIN_OUTER]);
}

static void draw_thin_outer_cornered_box(int xs, int ys, int xe, int ye, int flags)
{
	const int colors[4][2] = { {3, 1}, {1, 3}, {3, 3}, {1, 1} };
	int tl = colors[flags & BOX_SHADE_MASK][0];
	int br = colors[flags & BOX_SHADE_MASK][1];
	int n;

	CHECK_INVERT(tl, br, n);

	draw_char(128, xs, ys, tl, 2); /* TL corner */
	draw_char(141, xe, ys, 1, 2);  /* TR corner */
	draw_char(140, xs, ye, 1, 2);  /* BL corner */
	draw_char(135, xe, ye, br, 2); /* BR corner */

	for (n = xs + 1; n < xe; n++) {
		draw_char(129, n, ys, tl, 2);  /* top */
		draw_char(134, n, ye, br, 2);  /* bottom */
	}

	for (n = ys + 1; n < ye; n++) {
		draw_char(131, xs, n, tl, 2);  /* left */
		draw_char(132, xe, n, br, 2);  /* right */
	}
}

static void draw_thick_outer_box(int xs, int ys, int xe, int ye, uint32_t c)
{
	_draw_box_internal(xs, ys, xe, ye, c, c, boxes[BOX_THICK_OUTER]);
}

void draw_box(int xs, int ys, int xe, int ye, int flags)
{
	const int colors[5][2] = { {3, 1}, {1, 3}, {3, 3}, {1, 1}, {0, 0} };
	int tl = colors[flags & BOX_SHADE_MASK][0];
	int br = colors[flags & BOX_SHADE_MASK][1];

	switch (flags & (BOX_TYPE_MASK | BOX_THICKNESS_MASK)) {
	case BOX_THIN | BOX_INNER:
		draw_thin_inner_box(xs, ys, xe, ye, tl, br);
		break;
	case BOX_THICK | BOX_INNER:
		draw_thick_inner_box(xs, ys, xe, ye, tl, br);
		break;
	case BOX_THIN | BOX_OUTER:
		draw_thin_outer_box(xs, ys, xe, ye, tl);
		break;
	case BOX_THICK | BOX_OUTER:
		draw_thick_outer_box(xs, ys, xe, ye, tl);
		break;
	case BOX_THIN | BOX_CORNER:
	case BOX_THICK | BOX_CORNER:
		draw_thin_outer_cornered_box(xs, ys, xe, ye, flags & BOX_SHADE_MASK);
		break;
	}
}

/* ----------------------------------------------------------------- */

static inline void _draw_thumb_bar_internal(int width, int x, int y,
						int val, uint32_t fg)
{
	const uint8_t thumb_chars[2][8] = {
		{155, 156, 157, 158, 159, 160, 161, 162},
		{0, 0, 0, 163, 164, 165, 166, 167}
	};
	int n = ++val >> 3;

	val %= 8;
	draw_fill_chars(x, y, x + n - 1, y, DEFAULT_FG, 0);
	draw_char(thumb_chars[0][val], x + n, y, fg, 0);
	if (++n < width)
		draw_char(thumb_chars[1][val], x + n, y, fg, 0);
	if (++n < width)
		draw_fill_chars(x + n, y, x + width - 1, y, DEFAULT_FG, 0);
}

void draw_thumb_bar(int x, int y, int width, int min, int max, int val,
			int selected)
{
	/* this wouldn't happen in a perfect world :P */
	if (val < min || val > max) {
		draw_fill_chars(x, y, x + width - 1, y, DEFAULT_FG,
				((status.flags & CLASSIC_MODE) ? 2 : 0));
		return;
	}

	/* fix the range so that it's 0->n */
	val -= min;
	max -= min;

	/* draw the bar */
	if (!max)
		_draw_thumb_bar_internal(width, x, y, 0,
				 selected ? 3 : 2);
	else
		_draw_thumb_bar_internal(width, x, y,
				val * (width - 1) * 8 / max,
				selected ? 3 : 2);
}

/* --------------------------------------------------------------------- */
/* VU meters */

void draw_vu_meter(int x, int y, int width, int val, int color, int peak)
{
	const uint8_t endtext[8][3] = {
		{174, 0, 0}, {175, 0, 0}, {176, 0, 0}, {176, 177, 0},
		{176, 178, 0}, {176, 179, 180}, {176, 179, 181},
		{176, 179, 182},
	};
	int leftover;
	int chunks = (width / 3);
	int maxval = width * 8 / 3;

	/* reduced from (val * maxval / 64) */
	val = CLAMP((val*width/24), 0, (maxval-1));
	if (!val)
		return;

	leftover = val & 7;
	val >>= 3;
	if ((val < chunks - 1) || (status.flags & CLASSIC_MODE))
		peak = color;

	draw_char(endtext[leftover][0], 3 * val + x + 0, y, peak, 0);
	draw_char(endtext[leftover][1], 3 * val + x + 1, y, peak, 0);
	draw_char(endtext[leftover][2], 3 * val + x + 2, y, peak, 0);
	while (val--) {
		draw_char(176, 3 * val + x + 0, y, color, 0);
		draw_char(179, 3 * val + x + 1, y, color, 0);
		draw_char(182, 3 * val + x + 2, y, color, 0);
	}
}

/* --------------------------------------------------------------------- */
/* sample drawing
 * 
 * output channels = number of oscis
 * input channels = number of channels in data
*/

/* somewhat heavily based on CViewSample::DrawSampleData2 in modplug */
#define DRAW_SAMPLE_DATA_VARIANT(bits, doublebits) \
	static void _draw_sample_data_##bits(struct vgamem_overlay *r, \
		int##bits##_t *data, uint32_t length, unsigned int inputchans, unsigned int outputchans) \
	{ \
		const int32_t nh = r->height / outputchans; \
		int32_t np = r->height - nh / 2; \
		uint32_t step, cc; \
	\
		length /= inputchans; \
		step = (length << 16) / r->width; \
	\
		for (cc = 0; cc < outputchans; cc++) { \
			uint32_t x, poshi = 0, poslo = 0; \
			int##bits##_t oldmin = *data, oldmax = *data; \
	\
			for (x = 0; x < r->width; x++) { \
				uint32_t scanlength, i; \
				int##bits##_t min = INT##bits##_MAX, max = INT##bits##_MIN; \
	\
				poslo += step; \
				scanlength = ((poslo + 0xFFFF) >> 16); \
				if (poshi >= length) poshi = length - 1; \
				if (poshi + scanlength > length) scanlength = length - poshi; \
				scanlength = MAX(scanlength, 1); \
	\
				for (i = 0; i < scanlength; i++) { \
					uint32_t co = 0; \
	\
					do { \
						int##bits##_t s = data[((poshi + i) * inputchans) + cc + co]; \
						if (s < min) min = s; \
						if (s > max) max = s; \
					} while (co++ < inputchans - outputchans); \
				} \
	\
				/* XXX is doing this with integers faster than say, floating point?
				 * I mean, it sure is a bit more ~accurate~ at least, and it'll work the same everywhere. */ \
				min = rshift_signed((int##doublebits##_t)min * nh, bits); \
				max = rshift_signed((int##doublebits##_t)max * nh, bits); \
	\
				vgamem_ovl_drawline(r, x, np - 1 - max, x, np - 1 - min, SAMPLE_DATA_COLOR); \
	\
				oldmin = min; \
				oldmax = max; \
				poshi += (poslo >> 16); \
				poslo &= 0xFFFF; \
			} \
	\
			np -= nh; \
		} \
	}

DRAW_SAMPLE_DATA_VARIANT(8, 16)
DRAW_SAMPLE_DATA_VARIANT(16, 32)
DRAW_SAMPLE_DATA_VARIANT(32, 64)

#undef DRAW_SAMPLE_DATA_VARIANT

/* --------------------------------------------------------------------- */
/* these functions assume the screen is locked! */

/* loop drawing */
static void _draw_sample_loop(struct vgamem_overlay *r, song_sample_t * sample)
{
	int loopstart, loopend, y;
	int c = ((status.flags & CLASSIC_MODE) ? SAMPLE_DATA_COLOR : SAMPLE_LOOP_COLOR);

	if (!(sample->flags & CHN_LOOP))
		return;

	loopstart = sample->loop_start * (r->width - 1) / sample->length;
	loopend = sample->loop_end * (r->width - 1) / sample->length;

	y = 0;
	do {
		vgamem_ovl_drawpixel(r, loopstart, y, 0); vgamem_ovl_drawpixel(r, loopend, y, 0); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, c); vgamem_ovl_drawpixel(r, loopend, y, c); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, c); vgamem_ovl_drawpixel(r, loopend, y, c); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, 0); vgamem_ovl_drawpixel(r, loopend, y, 0); y++;
	} while (y < r->height);
}

static void _draw_sample_susloop(struct vgamem_overlay *r, song_sample_t * sample)
{
	int loopstart, loopend, y;
	int c = ((status.flags & CLASSIC_MODE) ? SAMPLE_DATA_COLOR : SAMPLE_LOOP_COLOR);

	if (!(sample->flags & CHN_SUSTAINLOOP))
		return;

	loopstart = sample->sustain_start * (r->width - 1) / sample->length;
	loopend = sample->sustain_end * (r->width - 1) / sample->length;

	y = 0;
	do {
		vgamem_ovl_drawpixel(r, loopstart, y, c); vgamem_ovl_drawpixel(r, loopend, y, c); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, 0); vgamem_ovl_drawpixel(r, loopend, y, 0); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, c); vgamem_ovl_drawpixel(r, loopend, y, c); y++;
		vgamem_ovl_drawpixel(r, loopstart, y, 0); vgamem_ovl_drawpixel(r, loopend, y, 0); y++;
	} while (y < r->height);
}

/* this does the lines for playing samples */
static void _draw_sample_play_marks(struct vgamem_overlay *r, song_sample_t * sample)
{
	int n, x, y;
	int c;
	song_voice_t *channel;
	uint32_t *channel_list;

	if (song_get_mode() == MODE_STOPPED)
		return;

	song_lock_audio();

	n = song_get_mix_state(&channel_list);
	while (n--) {
		channel = song_get_mix_channel(channel_list[n]);
		if (channel->current_sample_data != sample->data)
			continue;
		if (!channel->final_volume) continue;
		c = (channel->flags & (CHN_KEYOFF | CHN_NOTEFADE)) ? SAMPLE_BGMARK_COLOR : SAMPLE_MARK_COLOR;
		x = channel->position * (r->width - 1) / sample->length;
		if (x >= r->width) {
			/* this does, in fact, happen :( */
			continue;
		}
		y = 0;
		do {
			/* unrolled 8 times */
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
			vgamem_ovl_drawpixel(r, x, y++, c);
		} while (y < r->height);
	}

	song_unlock_audio();
}

/* --------------------------------------------------------------------- */
/* meat! */

void draw_sample_data(struct vgamem_overlay *r, song_sample_t *sample)
{
	vgamem_ovl_clear(r, 0);

	if (sample->flags & CHN_ADLIB) {
		vgamem_ovl_clear(r, 2);
		vgamem_ovl_apply(r);
		char buf1[32], buf2[32];

		int y1 = r->y1, y2 = y1+3;

		draw_box(59,y1, 77,y2, BOX_THICK | BOX_INNER | BOX_INSET); // data
		draw_box(54,y1, 58,y2, BOX_THIN | BOX_INNER | BOX_OUTSET); // button
		draw_text_len("Mod", 3, 55,y1+1, 0,2);
		draw_text_len("Car", 3, 55,y1+2, 0,2);

		sprintf(buf1, "%02X %02X %02X %02X %02X %02X", // length:6*3-1=17
			sample->adlib_bytes[0],
			sample->adlib_bytes[2],
			sample->adlib_bytes[4],
			sample->adlib_bytes[6],
			sample->adlib_bytes[8],
			sample->adlib_bytes[10]);
		sprintf(buf2, "%02X %02X %02X %02X %02X",      // length: 5*3-1=14
			sample->adlib_bytes[1],
			sample->adlib_bytes[3],
			sample->adlib_bytes[5],
			sample->adlib_bytes[7],
			sample->adlib_bytes[9]);
		draw_text_len(buf1, 17, 60,y1+1, 2,0);
		draw_text_len(buf2, 17, 60,y1+2, 2,0);
		return;
	}

	if (!sample->length || !sample->data) {
		vgamem_ovl_apply(r);
		return;
	}

	/* do the actual drawing */
	int chans = sample->flags & CHN_STEREO ? 2 : 1;
	if (sample->flags & CHN_16BIT)
		_draw_sample_data_16(r, (signed short *) sample->data,
				sample->length * chans,
				chans, chans);
	else
		_draw_sample_data_8(r, sample->data,
				sample->length * chans,
				chans, chans);

	if ((status.flags & CLASSIC_MODE) == 0)
		_draw_sample_play_marks(r, sample);
	_draw_sample_loop(r, sample);
	_draw_sample_susloop(r, sample);
	vgamem_ovl_apply(r);
}

void draw_sample_data_rect_32(struct vgamem_overlay *r, int32_t *data,
	int length, unsigned int inputchans, unsigned int outputchans)
{
	vgamem_ovl_clear(r, 0);
	_draw_sample_data_32(r, data, length, inputchans, outputchans);
	vgamem_ovl_apply(r);
}

void draw_sample_data_rect_16(struct vgamem_overlay *r, int16_t *data,
	int length, unsigned int inputchans, unsigned int outputchans)
{
	vgamem_ovl_clear(r, 0);
	_draw_sample_data_16(r, data, length, inputchans, outputchans);
	vgamem_ovl_apply(r);
}

void draw_sample_data_rect_8(struct vgamem_overlay *r, int8_t *data,
	int length, unsigned int inputchans, unsigned int outputchans)
{
	vgamem_ovl_clear(r, 0);
	_draw_sample_data_8(r, data, length, inputchans, outputchans);
	vgamem_ovl_apply(r);
}
