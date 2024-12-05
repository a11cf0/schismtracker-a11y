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

#ifndef SCHISM_KEYBOARD_H_
#define SCHISM_KEYBOARD_H_

#include "headers.h"

/* locale-independent keyboard positions */
enum {
	SCHISM_SCANCODE_UNKNOWN = 0,

	/* USB keyboard page... */
	SCHISM_SCANCODE_A = 4,
	SCHISM_SCANCODE_B = 5,
	SCHISM_SCANCODE_C = 6,
	SCHISM_SCANCODE_D = 7,
	SCHISM_SCANCODE_E = 8,
	SCHISM_SCANCODE_F = 9,
	SCHISM_SCANCODE_G = 10,
	SCHISM_SCANCODE_H = 11,
	SCHISM_SCANCODE_I = 12,
	SCHISM_SCANCODE_J = 13,
	SCHISM_SCANCODE_K = 14,
	SCHISM_SCANCODE_L = 15,
	SCHISM_SCANCODE_M = 16,
	SCHISM_SCANCODE_N = 17,
	SCHISM_SCANCODE_O = 18,
	SCHISM_SCANCODE_P = 19,
	SCHISM_SCANCODE_Q = 20,
	SCHISM_SCANCODE_R = 21,
	SCHISM_SCANCODE_S = 22,
	SCHISM_SCANCODE_T = 23,
	SCHISM_SCANCODE_U = 24,
	SCHISM_SCANCODE_V = 25,
	SCHISM_SCANCODE_W = 26,
	SCHISM_SCANCODE_X = 27,
	SCHISM_SCANCODE_Y = 28,
	SCHISM_SCANCODE_Z = 29,

	SCHISM_SCANCODE_1 = 30,
	SCHISM_SCANCODE_2 = 31,
	SCHISM_SCANCODE_3 = 32,
	SCHISM_SCANCODE_4 = 33,
	SCHISM_SCANCODE_5 = 34,
	SCHISM_SCANCODE_6 = 35,
	SCHISM_SCANCODE_7 = 36,
	SCHISM_SCANCODE_8 = 37,
	SCHISM_SCANCODE_9 = 38,
	SCHISM_SCANCODE_0 = 39,

	SCHISM_SCANCODE_RETURN = 40,
	SCHISM_SCANCODE_ESCAPE = 41,
	SCHISM_SCANCODE_BACKSPACE = 42,
	SCHISM_SCANCODE_TAB = 43,
	SCHISM_SCANCODE_SPACE = 44,

	SCHISM_SCANCODE_MINUS = 45,
	SCHISM_SCANCODE_EQUALS = 46,
	SCHISM_SCANCODE_LEFTBRACKET = 47,
	SCHISM_SCANCODE_RIGHTBRACKET = 48,
	SCHISM_SCANCODE_BACKSLASH = 49,
	SCHISM_SCANCODE_NONUSHASH = 50,
	SCHISM_SCANCODE_SEMICOLON = 51,
	SCHISM_SCANCODE_APOSTROPHE = 52,
	SCHISM_SCANCODE_GRAVE = 53,
	SCHISM_SCANCODE_COMMA = 54,
	SCHISM_SCANCODE_PERIOD = 55,
	SCHISM_SCANCODE_SLASH = 56,

	SCHISM_SCANCODE_CAPSLOCK = 57,

	SCHISM_SCANCODE_F1 = 58,
	SCHISM_SCANCODE_F2 = 59,
	SCHISM_SCANCODE_F3 = 60,
	SCHISM_SCANCODE_F4 = 61,
	SCHISM_SCANCODE_F5 = 62,
	SCHISM_SCANCODE_F6 = 63,
	SCHISM_SCANCODE_F7 = 64,
	SCHISM_SCANCODE_F8 = 65,
	SCHISM_SCANCODE_F9 = 66,
	SCHISM_SCANCODE_F10 = 67,
	SCHISM_SCANCODE_F11 = 68,
	SCHISM_SCANCODE_F12 = 69,

	SCHISM_SCANCODE_PRINTSCREEN = 70,
	SCHISM_SCANCODE_SCROLLLOCK = 71,
	SCHISM_SCANCODE_PAUSE = 72,
	SCHISM_SCANCODE_INSERT = 73,
	SCHISM_SCANCODE_HOME = 74,
	SCHISM_SCANCODE_PAGEUP = 75,
	SCHISM_SCANCODE_DELETE = 76,
	SCHISM_SCANCODE_END = 77,
	SCHISM_SCANCODE_PAGEDOWN = 78,
	SCHISM_SCANCODE_RIGHT = 79,
	SCHISM_SCANCODE_LEFT = 80,
	SCHISM_SCANCODE_DOWN = 81,
	SCHISM_SCANCODE_UP = 82,

	SCHISM_SCANCODE_NUMLOCKCLEAR = 83,
	SCHISM_SCANCODE_KP_DIVIDE = 84,
	SCHISM_SCANCODE_KP_MULTIPLY = 85,
	SCHISM_SCANCODE_KP_MINUS = 86,
	SCHISM_SCANCODE_KP_PLUS = 87,
	SCHISM_SCANCODE_KP_ENTER = 88,
	SCHISM_SCANCODE_KP_1 = 89,
	SCHISM_SCANCODE_KP_2 = 90,
	SCHISM_SCANCODE_KP_3 = 91,
	SCHISM_SCANCODE_KP_4 = 92,
	SCHISM_SCANCODE_KP_5 = 93,
	SCHISM_SCANCODE_KP_6 = 94,
	SCHISM_SCANCODE_KP_7 = 95,
	SCHISM_SCANCODE_KP_8 = 96,
	SCHISM_SCANCODE_KP_9 = 97,
	SCHISM_SCANCODE_KP_0 = 98,
	SCHISM_SCANCODE_KP_PERIOD = 99,

	SCHISM_SCANCODE_NONUSBACKSLASH = 100,
	SCHISM_SCANCODE_APPLICATION = 101, /* windows contextual menu, compose */
	SCHISM_SCANCODE_POWER = 102,
	SCHISM_SCANCODE_KP_EQUALS = 103,
	SCHISM_SCANCODE_F13 = 104,
	SCHISM_SCANCODE_F14 = 105,
	SCHISM_SCANCODE_F15 = 106,
	SCHISM_SCANCODE_F16 = 107,
	SCHISM_SCANCODE_F17 = 108,
	SCHISM_SCANCODE_F18 = 109,
	SCHISM_SCANCODE_F19 = 110,
	SCHISM_SCANCODE_F20 = 111,
	SCHISM_SCANCODE_F21 = 112,
	SCHISM_SCANCODE_F22 = 113,
	SCHISM_SCANCODE_F23 = 114,
	SCHISM_SCANCODE_F24 = 115,
	SCHISM_SCANCODE_EXECUTE = 116,
	SCHISM_SCANCODE_HELP = 117,    /* AL Integrated Help Center */
	SCHISM_SCANCODE_MENU = 118,    /* Menu (show menu) */
	SCHISM_SCANCODE_SELECT = 119,
	SCHISM_SCANCODE_STOP = 120,    /* AC Stop */
	SCHISM_SCANCODE_AGAIN = 121,   /* AC Redo/Repeat */
	SCHISM_SCANCODE_UNDO = 122,    /* AC Undo */
	SCHISM_SCANCODE_CUT = 123,     /* AC Cut */
	SCHISM_SCANCODE_COPY = 124,    /* AC Copy */
	SCHISM_SCANCODE_PASTE = 125,   /* AC Paste */
	SCHISM_SCANCODE_FIND = 126,    /* AC Find */
	SCHISM_SCANCODE_MUTE = 127,
	SCHISM_SCANCODE_VOLUMEUP = 128,
	SCHISM_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     SCHISM_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     SCHISM_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     SCHISM_SCANCODE_LOCKINGSCROLLLOCK = 132, */
	SCHISM_SCANCODE_KP_COMMA = 133,
	SCHISM_SCANCODE_KP_EQUALSAS400 = 134,

#if 0 // don't care
	SCHISM_SCANCODE_INTERNATIONAL1 = 135, /* used on Asian keyboards */
	SCHISM_SCANCODE_INTERNATIONAL2 = 136,
	SCHISM_SCANCODE_INTERNATIONAL3 = 137, /* Yen */
	SCHISM_SCANCODE_INTERNATIONAL4 = 138,
	SCHISM_SCANCODE_INTERNATIONAL5 = 139,
	SCHISM_SCANCODE_INTERNATIONAL6 = 140,
	SCHISM_SCANCODE_INTERNATIONAL7 = 141,
	SCHISM_SCANCODE_INTERNATIONAL8 = 142,
	SCHISM_SCANCODE_INTERNATIONAL9 = 143,
	SCHISM_SCANCODE_LANG1 = 144, /* Hangul/English toggle */
	SCHISM_SCANCODE_LANG2 = 145, /* Hanja conversion */
	SCHISM_SCANCODE_LANG3 = 146, /* Katakana */
	SCHISM_SCANCODE_LANG4 = 147, /* Hiragana */
	SCHISM_SCANCODE_LANG5 = 148, /* Zenkaku/Hankaku */
	SCHISM_SCANCODE_LANG6 = 149, /* reserved */
	SCHISM_SCANCODE_LANG7 = 150, /* reserved */
	SCHISM_SCANCODE_LANG8 = 151, /* reserved */
	SCHISM_SCANCODE_LANG9 = 152, /* reserved */
#endif

	SCHISM_SCANCODE_ALTERASE = 153,    /* Erase-Eaze */
	SCHISM_SCANCODE_SYSREQ = 154,
	SCHISM_SCANCODE_CANCEL = 155,      /* AC Cancel */
	SCHISM_SCANCODE_CLEAR = 156,
	SCHISM_SCANCODE_PRIOR = 157,
	SCHISM_SCANCODE_RETURN2 = 158,
	SCHISM_SCANCODE_SEPARATOR = 159,
	SCHISM_SCANCODE_OUT = 160,
	SCHISM_SCANCODE_OPER = 161,
	SCHISM_SCANCODE_CLEARAGAIN = 162,
	SCHISM_SCANCODE_CRSEL = 163,
	SCHISM_SCANCODE_EXSEL = 164,

	SCHISM_SCANCODE_KP_00 = 176,
	SCHISM_SCANCODE_KP_000 = 177,
	SCHISM_SCANCODE_THOUSANDSSEPARATOR = 178,
	SCHISM_SCANCODE_DECIMALSEPARATOR = 179,
	SCHISM_SCANCODE_CURRENCYUNIT = 180,
	SCHISM_SCANCODE_CURRENCYSUBUNIT = 181,
	SCHISM_SCANCODE_KP_LEFTPAREN = 182,
	SCHISM_SCANCODE_KP_RIGHTPAREN = 183,
	SCHISM_SCANCODE_KP_LEFTBRACE = 184,
	SCHISM_SCANCODE_KP_RIGHTBRACE = 185,
	SCHISM_SCANCODE_KP_TAB = 186,
	SCHISM_SCANCODE_KP_BACKSPACE = 187,
	SCHISM_SCANCODE_KP_A = 188,
	SCHISM_SCANCODE_KP_B = 189,
	SCHISM_SCANCODE_KP_C = 190,
	SCHISM_SCANCODE_KP_D = 191,
	SCHISM_SCANCODE_KP_E = 192,
	SCHISM_SCANCODE_KP_F = 193,
	SCHISM_SCANCODE_KP_XOR = 194,
	SCHISM_SCANCODE_KP_POWER = 195,
	SCHISM_SCANCODE_KP_PERCENT = 196,
	SCHISM_SCANCODE_KP_LESS = 197,
	SCHISM_SCANCODE_KP_GREATER = 198,
	SCHISM_SCANCODE_KP_AMPERSAND = 199,
	SCHISM_SCANCODE_KP_DBLAMPERSAND = 200,
	SCHISM_SCANCODE_KP_VERTICALBAR = 201,
	SCHISM_SCANCODE_KP_DBLVERTICALBAR = 202,
	SCHISM_SCANCODE_KP_COLON = 203,
	SCHISM_SCANCODE_KP_HASH = 204,
	SCHISM_SCANCODE_KP_SPACE = 205,
	SCHISM_SCANCODE_KP_AT = 206,
	SCHISM_SCANCODE_KP_EXCLAM = 207,
	SCHISM_SCANCODE_KP_MEMSTORE = 208,
	SCHISM_SCANCODE_KP_MEMRECALL = 209,
	SCHISM_SCANCODE_KP_MEMCLEAR = 210,
	SCHISM_SCANCODE_KP_MEMADD = 211,
	SCHISM_SCANCODE_KP_MEMSUBTRACT = 212,
	SCHISM_SCANCODE_KP_MEMMULTIPLY = 213,
	SCHISM_SCANCODE_KP_MEMDIVIDE = 214,
	SCHISM_SCANCODE_KP_PLUSMINUS = 215,
	SCHISM_SCANCODE_KP_CLEAR = 216,
	SCHISM_SCANCODE_KP_CLEARENTRY = 217,
	SCHISM_SCANCODE_KP_BINARY = 218,
	SCHISM_SCANCODE_KP_OCTAL = 219,
	SCHISM_SCANCODE_KP_DECIMAL = 220,
	SCHISM_SCANCODE_KP_HEXADECIMAL = 221,

	SCHISM_SCANCODE_LCTRL = 224,
	SCHISM_SCANCODE_LSHIFT = 225,
	SCHISM_SCANCODE_LALT = 226, /* alt, option */
	SCHISM_SCANCODE_LGUI = 227, /* windows, command (apple), meta */
	SCHISM_SCANCODE_RCTRL = 228,
	SCHISM_SCANCODE_RSHIFT = 229,
	SCHISM_SCANCODE_RALT = 230, /* alt gr, option */
	SCHISM_SCANCODE_RGUI = 231, /* windows, command (apple), meta */
};

typedef uint32_t schism_scancode_t;

#define SCHISM_KEYSYM_SCANCODE_MASK (1<<30)
#define SCHISM_SCANCODE_TO_KEYSYM(X)  (X | SCHISM_KEYSYM_SCANCODE_MASK)

enum {
	SCHISM_KEYSYM_UNKNOWN = 0,

	SCHISM_KEYSYM_RETURN = '\r',
	SCHISM_KEYSYM_ESCAPE = '\x1B',
	SCHISM_KEYSYM_BACKSPACE = '\b',
	SCHISM_KEYSYM_TAB = '\t',
	SCHISM_KEYSYM_SPACE = ' ',
	SCHISM_KEYSYM_EXCLAIM = '!',
	SCHISM_KEYSYM_QUOTEDBL = '"',
	SCHISM_KEYSYM_HASH = '#',
	SCHISM_KEYSYM_PERCENT = '%',
	SCHISM_KEYSYM_DOLLAR = '$',
	SCHISM_KEYSYM_AMPERSAND = '&',
	SCHISM_KEYSYM_QUOTE = '\'',
	SCHISM_KEYSYM_LEFTPAREN = '(',
	SCHISM_KEYSYM_RIGHTPAREN = ')',
	SCHISM_KEYSYM_ASTERISK = '*',
	SCHISM_KEYSYM_PLUS = '+',
	SCHISM_KEYSYM_COMMA = ',',
	SCHISM_KEYSYM_MINUS = '-',
	SCHISM_KEYSYM_PERIOD = '.',
	SCHISM_KEYSYM_SLASH = '/',
	SCHISM_KEYSYM_0 = '0',
	SCHISM_KEYSYM_1 = '1',
	SCHISM_KEYSYM_2 = '2',
	SCHISM_KEYSYM_3 = '3',
	SCHISM_KEYSYM_4 = '4',
	SCHISM_KEYSYM_5 = '5',
	SCHISM_KEYSYM_6 = '6',
	SCHISM_KEYSYM_7 = '7',
	SCHISM_KEYSYM_8 = '8',
	SCHISM_KEYSYM_9 = '9',
	SCHISM_KEYSYM_COLON = ':',
	SCHISM_KEYSYM_SEMICOLON = ';',
	SCHISM_KEYSYM_LESS = '<',
	SCHISM_KEYSYM_EQUALS = '=',
	SCHISM_KEYSYM_GREATER = '>',
	SCHISM_KEYSYM_QUESTION = '?',
	SCHISM_KEYSYM_AT = '@',

	/*
	   Skip uppercase letters
	 */

	SCHISM_KEYSYM_LEFTBRACKET = '[',
	SCHISM_KEYSYM_BACKSLASH = '\\',
	SCHISM_KEYSYM_RIGHTBRACKET = ']',
	SCHISM_KEYSYM_CARET = '^',
	SCHISM_KEYSYM_UNDERSCORE = '_',
	SCHISM_KEYSYM_BACKQUOTE = '`',
	SCHISM_KEYSYM_a = 'a',
	SCHISM_KEYSYM_b = 'b',
	SCHISM_KEYSYM_c = 'c',
	SCHISM_KEYSYM_d = 'd',
	SCHISM_KEYSYM_e = 'e',
	SCHISM_KEYSYM_f = 'f',
	SCHISM_KEYSYM_g = 'g',
	SCHISM_KEYSYM_h = 'h',
	SCHISM_KEYSYM_i = 'i',
	SCHISM_KEYSYM_j = 'j',
	SCHISM_KEYSYM_k = 'k',
	SCHISM_KEYSYM_l = 'l',
	SCHISM_KEYSYM_m = 'm',
	SCHISM_KEYSYM_n = 'n',
	SCHISM_KEYSYM_o = 'o',
	SCHISM_KEYSYM_p = 'p',
	SCHISM_KEYSYM_q = 'q',
	SCHISM_KEYSYM_r = 'r',
	SCHISM_KEYSYM_s = 's',
	SCHISM_KEYSYM_t = 't',
	SCHISM_KEYSYM_u = 'u',
	SCHISM_KEYSYM_v = 'v',
	SCHISM_KEYSYM_w = 'w',
	SCHISM_KEYSYM_x = 'x',
	SCHISM_KEYSYM_y = 'y',
	SCHISM_KEYSYM_z = 'z',

	SCHISM_KEYSYM_CAPSLOCK = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CAPSLOCK),

	SCHISM_KEYSYM_F1 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F1),
	SCHISM_KEYSYM_F2 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F2),
	SCHISM_KEYSYM_F3 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F3),
	SCHISM_KEYSYM_F4 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F4),
	SCHISM_KEYSYM_F5 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F5),
	SCHISM_KEYSYM_F6 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F6),
	SCHISM_KEYSYM_F7 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F7),
	SCHISM_KEYSYM_F8 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F8),
	SCHISM_KEYSYM_F9 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F9),
	SCHISM_KEYSYM_F10 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F10),
	SCHISM_KEYSYM_F11 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F11),
	SCHISM_KEYSYM_F12 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F12),

	SCHISM_KEYSYM_PRINTSCREEN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PRINTSCREEN),
	SCHISM_KEYSYM_SCROLLLOCK = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_SCROLLLOCK),
	SCHISM_KEYSYM_PAUSE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PAUSE),
	SCHISM_KEYSYM_INSERT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_INSERT),
	SCHISM_KEYSYM_HOME = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_HOME),
	SCHISM_KEYSYM_PAGEUP = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PAGEUP),
	SCHISM_KEYSYM_DELETE = '\x7F',
	SCHISM_KEYSYM_END = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_END),
	SCHISM_KEYSYM_PAGEDOWN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PAGEDOWN),
	SCHISM_KEYSYM_RIGHT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RIGHT),
	SCHISM_KEYSYM_LEFT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_LEFT),
	SCHISM_KEYSYM_DOWN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_DOWN),
	SCHISM_KEYSYM_UP = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_UP),

	SCHISM_KEYSYM_NUMLOCKCLEAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_NUMLOCKCLEAR),
	SCHISM_KEYSYM_KP_DIVIDE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_DIVIDE),
	SCHISM_KEYSYM_KP_MULTIPLY = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MULTIPLY),
	SCHISM_KEYSYM_KP_MINUS = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MINUS),
	SCHISM_KEYSYM_KP_PLUS = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_PLUS),
	SCHISM_KEYSYM_KP_ENTER = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_ENTER),
	SCHISM_KEYSYM_KP_1 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_1),
	SCHISM_KEYSYM_KP_2 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_2),
	SCHISM_KEYSYM_KP_3 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_3),
	SCHISM_KEYSYM_KP_4 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_4),
	SCHISM_KEYSYM_KP_5 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_5),
	SCHISM_KEYSYM_KP_6 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_6),
	SCHISM_KEYSYM_KP_7 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_7),
	SCHISM_KEYSYM_KP_8 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_8),
	SCHISM_KEYSYM_KP_9 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_9),
	SCHISM_KEYSYM_KP_0 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_0),
	SCHISM_KEYSYM_KP_PERIOD = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_PERIOD),

	SCHISM_KEYSYM_APPLICATION = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_APPLICATION),
	SCHISM_KEYSYM_POWER = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_POWER),
	SCHISM_KEYSYM_KP_EQUALS = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_EQUALS),
	SCHISM_KEYSYM_F13 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F13),
	SCHISM_KEYSYM_F14 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F14),
	SCHISM_KEYSYM_F15 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F15),
	SCHISM_KEYSYM_F16 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F16),
	SCHISM_KEYSYM_F17 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F17),
	SCHISM_KEYSYM_F18 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F18),
	SCHISM_KEYSYM_F19 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F19),
	SCHISM_KEYSYM_F20 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F20),
	SCHISM_KEYSYM_F21 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F21),
	SCHISM_KEYSYM_F22 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F22),
	SCHISM_KEYSYM_F23 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F23),
	SCHISM_KEYSYM_F24 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_F24),
	SCHISM_KEYSYM_EXECUTE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_EXECUTE),
	SCHISM_KEYSYM_HELP = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_HELP),
	SCHISM_KEYSYM_MENU = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_MENU),
	SCHISM_KEYSYM_SELECT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_SELECT),
	SCHISM_KEYSYM_STOP = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_STOP),
	SCHISM_KEYSYM_AGAIN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_AGAIN),
	SCHISM_KEYSYM_UNDO = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_UNDO),
	SCHISM_KEYSYM_CUT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CUT),
	SCHISM_KEYSYM_COPY = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_COPY),
	SCHISM_KEYSYM_PASTE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PASTE),
	SCHISM_KEYSYM_FIND = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_FIND),
	SCHISM_KEYSYM_MUTE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_MUTE),
	SCHISM_KEYSYM_VOLUMEUP = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_VOLUMEUP),
	SCHISM_KEYSYM_VOLUMEDOWN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_VOLUMEDOWN),
	SCHISM_KEYSYM_KP_COMMA = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_COMMA),
	SCHISM_KEYSYM_KP_EQUALSAS400 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_EQUALSAS400),

	SCHISM_KEYSYM_ALTERASE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_ALTERASE),
	SCHISM_KEYSYM_SYSREQ = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_SYSREQ),
	SCHISM_KEYSYM_CANCEL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CANCEL),
	SCHISM_KEYSYM_CLEAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CLEAR),
	SCHISM_KEYSYM_PRIOR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_PRIOR),
	SCHISM_KEYSYM_RETURN2 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RETURN2),
	SCHISM_KEYSYM_SEPARATOR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_SEPARATOR),
	SCHISM_KEYSYM_OUT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_OUT),
	SCHISM_KEYSYM_OPER = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_OPER),
	SCHISM_KEYSYM_CLEARAGAIN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CLEARAGAIN),
	SCHISM_KEYSYM_CRSEL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CRSEL),
	SCHISM_KEYSYM_EXSEL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_EXSEL),

	SCHISM_KEYSYM_KP_00 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_00),
	SCHISM_KEYSYM_KP_000 = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_000),
	SCHISM_KEYSYM_THOUSANDSSEPARATOR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_THOUSANDSSEPARATOR),
	SCHISM_KEYSYM_DECIMALSEPARATOR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_DECIMALSEPARATOR),
	SCHISM_KEYSYM_CURRENCYUNIT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CURRENCYUNIT),
	SCHISM_KEYSYM_CURRENCYSUBUNIT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_CURRENCYSUBUNIT),
	SCHISM_KEYSYM_KP_LEFTPAREN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_LEFTPAREN),
	SCHISM_KEYSYM_KP_RIGHTPAREN = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_RIGHTPAREN),
	SCHISM_KEYSYM_KP_LEFTBRACE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_LEFTBRACE),
	SCHISM_KEYSYM_KP_RIGHTBRACE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_RIGHTBRACE),
	SCHISM_KEYSYM_KP_TAB = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_TAB),
	SCHISM_KEYSYM_KP_BACKSPACE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_BACKSPACE),
	SCHISM_KEYSYM_KP_A = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_A),
	SCHISM_KEYSYM_KP_B = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_B),
	SCHISM_KEYSYM_KP_C = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_C),
	SCHISM_KEYSYM_KP_D = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_D),
	SCHISM_KEYSYM_KP_E = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_E),
	SCHISM_KEYSYM_KP_F = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_F),
	SCHISM_KEYSYM_KP_XOR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_XOR),
	SCHISM_KEYSYM_KP_POWER = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_POWER),
	SCHISM_KEYSYM_KP_PERCENT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_PERCENT),
	SCHISM_KEYSYM_KP_LESS = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_LESS),
	SCHISM_KEYSYM_KP_GREATER = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_GREATER),
	SCHISM_KEYSYM_KP_AMPERSAND = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_AMPERSAND),
	SCHISM_KEYSYM_KP_DBLAMPERSAND = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_DBLAMPERSAND),
	SCHISM_KEYSYM_KP_VERTICALBAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_VERTICALBAR),
	SCHISM_KEYSYM_KP_DBLVERTICALBAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_DBLVERTICALBAR),
	SCHISM_KEYSYM_KP_COLON = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_COLON),
	SCHISM_KEYSYM_KP_HASH = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_HASH),
	SCHISM_KEYSYM_KP_SPACE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_SPACE),
	SCHISM_KEYSYM_KP_AT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_AT),
	SCHISM_KEYSYM_KP_EXCLAM = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_EXCLAM),
	SCHISM_KEYSYM_KP_MEMSTORE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMSTORE),
	SCHISM_KEYSYM_KP_MEMRECALL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMRECALL),
	SCHISM_KEYSYM_KP_MEMCLEAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMCLEAR),
	SCHISM_KEYSYM_KP_MEMADD = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMADD),
	SCHISM_KEYSYM_KP_MEMSUBTRACT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMSUBTRACT),
	SCHISM_KEYSYM_KP_MEMMULTIPLY = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMMULTIPLY),
	SCHISM_KEYSYM_KP_MEMDIVIDE = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_MEMDIVIDE),
	SCHISM_KEYSYM_KP_PLUSMINUS = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_PLUSMINUS),
	SCHISM_KEYSYM_KP_CLEAR = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_CLEAR),
	SCHISM_KEYSYM_KP_CLEARENTRY = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_CLEARENTRY),
	SCHISM_KEYSYM_KP_BINARY = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_BINARY),
	SCHISM_KEYSYM_KP_OCTAL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_OCTAL),
	SCHISM_KEYSYM_KP_DECIMAL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_DECIMAL),
	SCHISM_KEYSYM_KP_HEXADECIMAL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_KP_HEXADECIMAL),

	SCHISM_KEYSYM_LCTRL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_LCTRL),
	SCHISM_KEYSYM_LSHIFT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_LSHIFT),
	SCHISM_KEYSYM_LALT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_LALT),
	SCHISM_KEYSYM_LGUI = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_LGUI),
	SCHISM_KEYSYM_RCTRL = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RCTRL),
	SCHISM_KEYSYM_RSHIFT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RSHIFT),
	SCHISM_KEYSYM_RALT = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RALT),
	SCHISM_KEYSYM_RGUI = SCHISM_SCANCODE_TO_KEYSYM(SCHISM_SCANCODE_RGUI),
};

typedef uint32_t schism_keysym_t;

enum {
	SCHISM_KEYMOD_NONE = (0),
	SCHISM_KEYMOD_LCTRL = (1u << 0),
	SCHISM_KEYMOD_RCTRL = (1u << 1),
	SCHISM_KEYMOD_LSHIFT = (1u << 2),
	SCHISM_KEYMOD_RSHIFT = (1u << 3),
	SCHISM_KEYMOD_LALT = (1u << 4),
	SCHISM_KEYMOD_RALT = (1u << 5),
	SCHISM_KEYMOD_LGUI = (1u << 6),
	SCHISM_KEYMOD_RGUI = (1u << 7),
	SCHISM_KEYMOD_NUM = (1u << 8),
	SCHISM_KEYMOD_CAPS = (1u << 9),
	SCHISM_KEYMOD_MODE = (1u << 10),
	SCHISM_KEYMOD_SCROLL = (1u << 11),
	SCHISM_KEYMOD_CAPS_PRESSED = (1u << 12),

	SCHISM_KEYMOD_CTRL = (SCHISM_KEYMOD_LCTRL | SCHISM_KEYMOD_RCTRL),
	SCHISM_KEYMOD_SHIFT = (SCHISM_KEYMOD_LSHIFT | SCHISM_KEYMOD_RSHIFT),
	SCHISM_KEYMOD_ALT = (SCHISM_KEYMOD_LALT | SCHISM_KEYMOD_RALT),
	SCHISM_KEYMOD_GUI = (SCHISM_KEYMOD_LGUI | SCHISM_KEYMOD_RGUI),
};

typedef uint16_t schism_keymod_t;

enum key_state {
	KEY_PRESS = 0,
	KEY_RELEASE,
};

enum mouse_state {
	MOUSE_NONE = 0,
	MOUSE_CLICK,
	MOUSE_SCROLL_UP,
	MOUSE_SCROLL_DOWN,
	MOUSE_DBLCLICK,
};

enum mouse_button {
	MOUSE_BUTTON_LEFT = 0,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT,
};

struct key_event {
	schism_keysym_t sym; /* A keycode, can be Unicode */
	schism_scancode_t scancode; /* Locale-independent key locations */
	schism_keymod_t mod; /* current key modifiers */
	const char* text; /* text input, if any. can be NULL */

	enum key_state state;
	enum mouse_state mouse;
	enum mouse_button mouse_button;

	int midi_note;
	int midi_channel;
	int midi_volume; /* -1 for not a midi key otherwise 0...128 */
	int midi_bend;  /* normally 0; -8192 to +8192  */
	unsigned int sx, sy; /* start x and y position (character) */
	unsigned int x, hx, fx; /* x position of mouse (character, halfcharacter, fine) */
	unsigned int y, fy; /* y position of mouse (character, fine) */

	unsigned int rx, ry; /* x/y resolution */

	int is_repeat;
	int on_target;
	int is_synthetic; /* 1 came from paste */
};

int numeric_key_event(struct key_event *k, int kponly);

char *get_note_string(int note, char *buf);     /* "C-5" or "G#4" */
char *get_note_string_short(int note, char *buf);       /* "c5" or "G4" */
char *get_volume_string(int volume, int volume_effect, char *buf);
char get_effect_char(int command);
int get_effect_number(char effect);
int get_ptm_effect_number(char effect);

/* values for kbd_sharp_flat_toggle */
typedef enum {
	KBD_SHARP_FLAT_TOGGLE = -1,
	KBD_SHARP_FLAT_SHARPS = 0,
	KBD_SHARP_FLAT_FLATS = 1,
} kbd_sharp_flat_t;

void kbd_init(void);
kbd_sharp_flat_t kbd_sharp_flat_state(void);
void kbd_sharp_flat_toggle(kbd_sharp_flat_t e);
int kbd_get_effect_number(struct key_event *k);
int kbd_char_to_hex(struct key_event *k);
int kbd_char_to_99(struct key_event *k);

int kbd_get_current_octave(void);
void kbd_set_current_octave(int new_octave);

int kbd_get_note(struct key_event *k);

int kbd_get_alnum(struct key_event *k);

void kbd_key_translate(struct key_event *k);

/* -------------------------------------------- */
/* key repeat */

int kbd_key_repeat_enabled(void);
void kbd_handle_key_repeat(void);
void kbd_cache_key_repeat(struct key_event* kk);
void kbd_empty_key_repeat(void);

/* use 0 for delay to (re)set the default rate. */
void kbd_set_key_repeat(int delay, int rate);

#endif /* SCHISM_KEYBOARD_H_ */
