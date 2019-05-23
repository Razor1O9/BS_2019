/*
 * UNIX shell
 *
 * S. R. Bourne
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"

STKPTR		stakbot=nullstr;
STKPTR		stakbas;
BLKPTR		stakbsy;
STKPTR		staktop;

/* ========	storage allocation	======== */

STKPTR	getstak(INT asize)
{	/* allocate requested stack */
	REG STKPTR	oldstak;
	REG INT		size;

	size=round(asize,BYTESPERWORD);
	oldstak=stakbot;
	staktop = stakbot += size;
	return(oldstak);
}

STKPTR	locstak(VOID)
{	/* set up stack for local use
	 * should be followed by `endstak'
	 */
	IF brkend-stakbot<BRKINCR
	THEN	setbrk(brkincr);
		IF brkincr < BRKMAX
		THEN	brkincr += 256;
		FI
	FI
	return(stakbot);
}

STKPTR	savstak(VOID)
{
	assert(staktop==stakbot);
	return(stakbot);
}

STKPTR	endstak(STRING argp)
{	/* tidy up after `locstak' */
	REG STKPTR	oldstak;
	*argp++=0;
	oldstak=stakbot; stakbot=staktop=(STKPTR)round(argp,BYTESPERWORD);
	return(oldstak);
}

VOID	tdystak(STKPTR x)
{
	/* try to bring stack back to x */
	WHILE ADR(stakbsy)>ADR(x)
	DO freemem(stakbsy);
	   stakbsy = stakbsy->word;
	OD
	staktop=stakbot=max(ADR(x),ADR(stakbas));
	rmtemp((IOPTR) x);
}

VOID stakchk(VOID)
{
	IF (brkend-stakbas)>BRKINCR+BRKINCR
	THEN	setbrk(-BRKINCR);
	FI
}

STKPTR	cpystak(STKPTR x)
{
	return(endstak(movstr(x,locstak())));
}
