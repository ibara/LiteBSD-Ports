#ifndef _ASTRO_H_
#define _ASTRO_H_

/*
 * UNIX_TO_JULIAN  --  Convert internal Unix date/time to astronomical
 *             Julian time (i.e. Julian date plus day fraction, expressed as
 *	       a double).
 */
double unix_to_julian( time_t t );

/*
 * PHASEHUNT5  --  Find time of phases of the moon which surround
 *		the current date.  Five phases are found, starting
 *		and ending with the new moons which bound the
 *		current lunation.
 */
void phasehunt5( double sdate, double phases[5] );

/*
 * PHASEHUNT2  --  Find time of phases of the moon which surround
 *		the current date.  Two phases are found.
 */
void phasehunt2( double sdate, double phases[2], double which[2] );

/*
 * PHASE  --  Calculate phase of moon as a fraction:
 *
 *	The argument is the time for which the phase is requested,
 *	expressed as a Julian date and fraction.  Returns the terminator
 *	phase angle as a percentage of a full circle (i.e., 0 to 1),
 *	and stores into pointer arguments the illuminated fraction of
 *      the Moon's disc, the Moon's age in days and fraction, the
 *	distance of the Moon from the centre of the Earth, and the
 *	angular diameter subtended by the Moon as seen by an observer
 *	at the centre of the Earth.
 *
 * pphase:		Illuminated fraction
 * mage:		Age of moon in days
 * dist:		Distance in kilometres
 * angdia:		Angular diameter in degrees
 * sudist:		Distance to Sun
 * suangdia:            Sun's angular diameter
 */
double phase( double pdate, double* pphase, double* mage, double* dist, double* angdia, double* sudist, double* suangdia );

#endif /* _ASTRO_H_ */
