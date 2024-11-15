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
#include "sdlmain.h"

#ifdef SCHISM_WIN32
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_s(result, timep) ? NULL : result;
}
#else
static SDL_mutex *localtime_r_mutex = NULL;

static void localtime_r_atexit(void)
{
	SDL_DestroyMutex(localtime_r_mutex);
}

struct tm *localtime_r(const time_t *timep, struct tm *result)
{
	static struct tm *our_tm;
	static int initialized = 0;

	if (!initialized) {
		localtime_r_mutex = SDL_CreateMutex();
		if (!localtime_r_mutex)
			return NULL;

		initialized = 1;
	}

	SDL_LockMutex(localtime_r_mutex);

	our_tm = localtime(timep);
	memcpy(result, our_tm, sizeof(struct tm));

	SDL_UnlockMutex(localtime_r_mutex);

	return result;
}
#endif
