/*
 * (C) Copyright 2009, Libertron Corporation <www.libertron.com>
 * JhoonKim <jhkim@libertron.com>
 *
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, ARM Ltd. <philippe.robin@arm.com>
 *
 * (C) Copyright 2009
 * Jhoon Kim, Libertron Ltd. <jhkim@libertron.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <config.h>
#include <asm/spr_defs.h>



static ulong g_timer_tick_time;
static ulong g_u32TimerStamp;
static ulong g_u32LastTickCount;


/*
 * timer without interrupts
 */

int timer_init(void)
{
    g_timer_tick_time = (CPU_CLOCK_RATE/DIVISOR_FOR_TIMER);

    //printf ("timer_init >> %d\r\n", g_timer_tick_time);

	mtspr(SPR_TTMR, SPR_TTMR_RT | (g_timer_tick_time & SPR_TTMR_PERIOD));
    g_u32TimerStamp = 0;
    reset_timer();

	return (0);
}

void reset_timer(void)
{
	/* reset time */
  	mtspr(SPR_TTCR, 0);     // we reset TTCR to 0 and thus g_u32LastTickCount will be reset
	g_u32LastTickCount = 0;
	g_u32TimerStamp = 0;
}

ulong get_timer_stamp(void)
{
	ulong current_tick_count = mfspr(SPR_TTCR);

	if (current_tick_count >= g_u32LastTickCount)
    {
		g_u32TimerStamp += current_tick_count - g_u32LastTickCount;
	}
	else
    {
        g_u32TimerStamp +=  (g_timer_tick_time - g_u32LastTickCount) + current_tick_count;
	}
	g_u32LastTickCount = current_tick_count;

	return g_u32TimerStamp;
}

// Note: timer unit is cycle which makes the it limit to 13.76 sec
ulong get_timer(ulong base)
{
	return get_timer_stamp() - base;
}

void set_timer(ulong t)
{
	g_u32TimerStamp = t;
}

// Note: unit: usec. Input should be smaller than 0xFFFF FFFF / 336,000,000 ~= 13.76 sec
void udelay(unsigned long usec)
{
	ulong tmo, end, start;

	tmo = usec * (CPU_CLOCK_RATE/1000000);

	start = get_timer(0);               /* get current g_u32TimerStamp */
	if ((tmo + start + 1) < start)      /* if setting this fordward will roll time stamp */
	{
		reset_timer();                  /* reset "advancing" g_u32TimerStamp to 0, set g_u32LastTickCount value */
      	start = 0;
	}

    end = tmo + start;

	while (get_timer_stamp()<end)       /* loop till event */
	{
		 /*NOP*/
	}
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return (CPU_CLOCK_RATE);
}

