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

/* Well, this page is just a big hack factory, but it's at least an
 * improvement over the message editor :P */

#include "headers.h"

#include "it.h"
#include "page.h"
#include "widget.h"
#include "vgamem.h"
#include "accessibility.h"
#include "charset.h"
#include "keyboard.h"
#include "mem.h"
#include "str.h"

/* --------------------------------------------------------------------- */

/* Line type characters (the marker at the start of each line) */
enum {
	LTYPE_NORMAL = '|',
	LTYPE_BIOS = '+',
	LTYPE_SCHISM = ':',
	LTYPE_SCHISM_BIOS = ';',
	LTYPE_CLASSIC = '!',
	LTYPE_SEPARATOR = '%',
	LTYPE_DISABLED = '#',
	LTYPE_GRAPHIC = '=',
};

/* Types that should be hidden from view in classic/non-classic mode */
#define LINE_SCHISM_HIDDEN(p) (0[p] == LTYPE_CLASSIC)
#define LINE_CLASSIC_HIDDEN(p) (0[p] == LTYPE_SCHISM || 0[p] == LTYPE_SCHISM_BIOS)

/* Types that should be rendered with the standard font */
#define LINE_BIOS(p) (0[p] == LTYPE_BIOS || 0[p] == LTYPE_SCHISM_BIOS)

static struct widget widgets_help[2];

/*
Pointers to the start of each line, and total line counts,
for each help text, for both classic and "normal" mode.

For example,
	help_cache[HELP_PATTERN_EDITOR][0].lines[3][5]
is the fifth character of the third line of the non-classic-mode help for the
pattern editor.

Each line is terminated by some combination of \r and \n, or \0.
*/
static struct {
	const char **lines;
	int num_lines;
} help_cache[HELP_NUM_ITEMS][2] = {{{NULL, 0}}};

/* Shortcuts for sanity -- this will point to the currently applicable help. */
#define CURRENT_HELP_LINECACHE (help_cache[status.current_help_index][!!(status.flags & CLASSIC_MODE)].lines)
#define CURRENT_HELP_LINECOUNT (help_cache[status.current_help_index][!!(status.flags & CLASSIC_MODE)].num_lines)

/* should always point to the currently applicable help text -- cached
to prevent repetitively checking things that aren't going to change */
static const char **lines = NULL;

static int num_lines = 0;
static int top_line = 0;
static int current_line = 0; // Virtual accessibility cursor for lines
static int current_char = 0; // Same thing for chars

static const char blank_line[] = {LTYPE_NORMAL, '\0'};
static const char separator_line[] = {LTYPE_SEPARATOR, '\0'};

static int help_text_lastpos[HELP_NUM_ITEMS] = {0};

/* This isn't defined in an .h file since it's only used here. */
extern const char *help_text[];

/* --------------------------------------------------------------------- */

static void help_draw_const(void)
{
	draw_box(1, 12, 78, 45, BOX_THICK | BOX_INNER | BOX_INSET);

	if (status.dialog_type == DIALOG_NONE) widget_change_focus_to(1);
}

static void help_redraw(void)
{
	int n, pos, x;
	int lp;
	const char **ptr;
	const uint8_t graphic_chars[] = {0, 0x89, 0x8f, 0x96, 0x84, 0, 0x91, 0x8b, 0x86, 0x8a};
	char ch;

	draw_fill_chars(2, 13, 77, 44, DEFAULT_FG, 0);

	ptr = lines + top_line;
	for (pos = 13, n = top_line; pos < 45; pos++, n++) {
		switch (**ptr) {
		default:
			lp = strcspn(*ptr+1, "\015\012");
			if (LINE_BIOS(*ptr)) {
				draw_text_bios_len(*ptr + 1, lp, 2, pos, 6, 0);
			} else {
				draw_text_len(*ptr + 1, lp, 2, pos, **ptr == LTYPE_DISABLED ? 7 : 6, 0);
			}
			break;
		case LTYPE_GRAPHIC:
			lp = strcspn(*ptr + 1, "\015\012");
			for (x = 1; x <= lp; x++) {
				ch = ptr[0][x];
				if (ch >= '1' && ch <= '9')
					ch = graphic_chars[ch - '0'];
				draw_char(ch, x + 1, pos, 6, 0);
			}
			break;
		case LTYPE_SEPARATOR:
			for (x = 2; x < 78; x++)
				draw_char(154, x, pos, 6, 0);
			break;
		}
		ptr++;
	}
}

/* --------------------------------------------------------------------- */

static void _help_close(void)
{
	set_page(status.previous_page);
}

static int _get_line_length(int line)
{
	const char **ptr = lines + line;
	return strcspn(*ptr+1, "\015\012");
}

static const char* help_a11y_get_value(char *buf)
{
	const char **ptr = lines + current_line;
	int lp = _get_line_length(current_line);
	const char *type;
	switch (**ptr) {
	case LTYPE_GRAPHIC:
		type = "Graphic ";
		break;
	case LTYPE_SEPARATOR:
		type = "Separator";
		break;
	case LTYPE_DISABLED:
		type = "Disabled ";
		break;
	default:
		type = "";
		break;
	}
	strcpy(buf, type);
	if (!*type || **ptr == LTYPE_DISABLED) {
		int len = strlen(buf);
		strncat(buf, *ptr + 2, lp);
		CHARSET_EASY_MODE(&buf[len], CHARSET_CP437, CHARSET_CHAR, {
			strcpy(buf, out);
		});
	}
	return buf;
}

static char help_a11y_get_char_at(int pos)
{
	const char **ptr = lines + current_line;
	if (**ptr == LTYPE_SEPARATOR || **ptr == LTYPE_GRAPHIC)
		return '\0';
	return (*ptr + 1)[pos];
}

static int help_handle_key(struct key_event * k)
{
	int new_line = top_line;
	int new_cur_line = current_line;
	char buf[100];
	char ch;
	int last_char = _get_line_length(new_cur_line) - 2;

	if (status.dialog_type != DIALOG_NONE) return 0;

	if (k->mouse == MOUSE_SCROLL_UP) {
		new_line -= MOUSE_SCROLL_LINES;
		new_cur_line -= MOUSE_SCROLL_LINES;
	} else if (k->mouse == MOUSE_SCROLL_DOWN) {
		new_line += MOUSE_SCROLL_LINES;
		new_cur_line += MOUSE_SCROLL_LINES;
	} else if (k->mouse != MOUSE_NONE) {
		return 0;
	}
	switch (k->sym) {
	case SCHISM_KEYSYM_ESCAPE:
		if (k->state == KEY_RELEASE)
			return 1;
		set_page(status.previous_page);
		return 1;
	case SCHISM_KEYSYM_UP:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line--;
		new_cur_line--;
		break;
	case SCHISM_KEYSYM_DOWN:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line++;
		new_cur_line++;
		break;
	case SCHISM_KEYSYM_PAGEUP:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line -= 32;
		new_cur_line -= 32;
		break;
	case SCHISM_KEYSYM_PAGEDOWN:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line += 32;
		new_cur_line += 32;
		break;
	case SCHISM_KEYSYM_HOME:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line = 0;
		new_cur_line = 0;
		break;
	case SCHISM_KEYSYM_END:
		if (k->state == KEY_RELEASE)
			return 1;
		new_line = num_lines - 32;
		new_cur_line = num_lines - 1;
		break;
	case SCHISM_KEYSYM_LEFT:
		if (k->state == KEY_RELEASE)
			return 1;
		current_char--;
		if (current_char < 0) current_char = 0;
		ch = help_a11y_get_char_at(current_char);
		if (ch) a11y_output_char(ch, 0);
		return 1;
	case SCHISM_KEYSYM_RIGHT:
		if (k->state == KEY_RELEASE)
			return 1;
		current_char++;
		if (current_char > last_char) current_char = last_char;
		ch = help_a11y_get_char_at(current_char);
		if (ch) a11y_output_char(ch, 0);
		return 1;
	default:
		if (k->mouse != MOUSE_NONE) {
			if (k->state == KEY_RELEASE)
				return 1;
		} else {
			return 0;
		}
	}

	new_line = CLAMP(new_line, 0, num_lines - 32);
	new_cur_line = CLAMP(new_cur_line, 0, num_lines - 1);
	if (new_line != top_line) {
		top_line = new_line;
		help_text_lastpos[status.current_help_index] = top_line;
		status.flags |= NEED_UPDATE;
	}
	if (new_cur_line != current_line) {
		current_line = new_cur_line;
		current_char = 0;
	}
	help_a11y_get_value(buf);
	a11y_output(*buf ? buf : "Blank", 0);

	return 1;
}

/* --------------------------------------------------------------------- */

static void help_set_page(void)
{
	const char *ptr;
	int local_lines = 0, global_lines = 0, cur_line = 0;
	int have_local_help = (status.current_help_index != HELP_GLOBAL);

	widget_change_focus_to(1);
	top_line = help_text_lastpos[status.current_help_index];

	lines = CURRENT_HELP_LINECACHE;
	if (lines) {
		num_lines = CURRENT_HELP_LINECOUNT;
		if (top_line > num_lines - 32)
			top_line = 0;
		return;
	}

	/* how many lines? */
	global_lines = str_get_num_lines(help_text[HELP_GLOBAL]);
	if (have_local_help) {
		local_lines = str_get_num_lines(help_text[status.current_help_index]);
		num_lines = local_lines + global_lines + 5;
	} else {
		num_lines = global_lines + 2;
	}

	/* allocate the array */
	lines = CURRENT_HELP_LINECACHE = mem_calloc(num_lines + 1, sizeof(char *));

	/* page help text */
	if (have_local_help) {
		ptr = help_text[status.current_help_index];
		while (local_lines--) {
			if (status.flags & CLASSIC_MODE) {
				if (!LINE_CLASSIC_HIDDEN(ptr))
					lines[cur_line++] = ptr;
			} else {
				if (!LINE_SCHISM_HIDDEN(ptr))
					lines[cur_line++] = ptr;
			}
			ptr = strpbrk(ptr, "\015\012");
			if (*ptr == 13)
				ptr++;
			if (*ptr == 10)
				ptr++;
		}
		lines[cur_line++] = blank_line;
		lines[cur_line++] = separator_line;
	}

	lines[cur_line++] = blank_line;

	/* global help text */
	ptr = help_text[HELP_GLOBAL];
	while (global_lines--) {
		if (status.flags & CLASSIC_MODE) {
			if (!LINE_CLASSIC_HIDDEN(ptr))
				lines[cur_line++] = ptr;
		} else {
			if (!LINE_SCHISM_HIDDEN(ptr))
				lines[cur_line++] = ptr;
		}
		ptr = strpbrk(ptr, "\015\012");
		if (*ptr == 13)
			ptr++;
		if (*ptr == 10)
			ptr++;
	}

	lines[cur_line++] = blank_line;
	if (have_local_help)
		lines[cur_line++] = separator_line;

	lines[cur_line] = NULL;
	CURRENT_HELP_LINECOUNT = num_lines = cur_line;
	if (top_line > num_lines - 32)
		top_line = 0;
	current_line = top_line;
}

/* --------------------------------------------------------------------- */

void help_load_page(struct page *page)
{
	page->title = "Help";
	page->draw_const = help_draw_const;
	page->set_page = help_set_page;
	page->total_widgets = 2;
	page->widgets = widgets_help;
	page->pre_handle_key = help_handle_key;

	widget_create_other(widgets_help + 0, 0, help_handle_key, NULL, help_redraw);
	widgets_help[0].d.other.a11y_type = "";
	widgets_help[0].d.other.a11y_get_value = help_a11y_get_value;
	widget_create_button(widgets_help + 1, 35,47,8, 0, 1, 1,1, 0,
			_help_close, "Done", 3);
}

