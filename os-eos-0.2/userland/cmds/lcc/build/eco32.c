
/*
 * eco32.md -- ECO32 back-end specification
 *
 * register usage:
 *   $0   always zero
 *   $1   reserved for assembler
 *   $2   func return value
 *   $3   func return value
 *   $4   proc/func argument
 *   $5   proc/func argument
 *   $6   proc/func argument
 *   $7   proc/func argument
 *   $8   temporary register (caller-save)
 *   $9   temporary register (caller-save)
 *   $10  temporary register (caller-save)
 *   $11  temporary register (caller-save)
 *   $12  temporary register (caller-save)
 *   $13  temporary register (caller-save)
 *   $14  temporary register (caller-save)
 *   $15  temporary register (caller-save)
 *   $16  register variable  (callee-save)
 *   $17  register variable  (callee-save)
 *   $18  register variable  (callee-save)
 *   $19  register variable  (callee-save)
 *   $20  register variable  (callee-save)
 *   $21  register variable  (callee-save)
 *   $22  register variable  (callee-save)
 *   $23  register variable  (callee-save)
 *   $24  temporary register (caller-save)
 *   $25  temporary register (caller-save)
 *   $26  reserved for OS kernel
 *   $27  reserved for OS kernel
 *   $28  reserved for OS kernel
 *   $29  stack pointer
 *   $30  interrupt return address
 *   $31  proc/func return address
 * caller-save registers are not preserved across procedure calls
 * callee-save registers are preserved across procedure calls
 *
 * tree grammar terminals produced by:
 *   ops c=1 s=2 i=4 l=4 h=4 f=4 d=8 x=8 p=4
 */

#include "c.h"

#define NODEPTR_TYPE	Node
#define OP_LABEL(p)	((p)->op)
#define LEFT_CHILD(p)	((p)->kids[0])
#define RIGHT_CHILD(p)	((p)->kids[1])
#define STATE_LABEL(p)	((p)->x.state)

static void address(Symbol, Symbol, long);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void export(Symbol);
static void function(Symbol, Symbol [], Symbol [], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char * []);
static void progend(void);
static void segment(int);
static void space(int);
static Symbol rmap(int);
static void blkfetch(int, int, int, int);
static void blkstore(int, int, int, int);
static void blkloop(int, int, int, int, int, int []);
static void emit2(Node);
static void doarg(Node);
static void target(Node);
static void clobber(Node);

#define INTTMP	0x0100FF00
#define INTVAR	0x00FF0000
#define INTRET	0x00000004
#define FLTTMP	0x000F0FF0
#define FLTVAR	0xFFF00000
#define FLTRET	0x00000003

static Symbol ireg[32];
static Symbol iregw;
static Symbol freg2[32];
static Symbol freg2w;
static Symbol blkreg;
static int tmpregs[] = { 3, 9, 10 };

/*
generated at Wed May 22 18:02:33 2019
by $Id: lburg.c,v 2.10 2002/03/08 18:45:21 drh Exp $
*/
static void _kids(NODEPTR_TYPE, int, NODEPTR_TYPE[]);
static void _label(NODEPTR_TYPE);
static int _rule(void*, int);

#define _stmt_NT 1
#define _reg_NT 2
#define _con_NT 3
#define _acon_NT 4
#define _addr_NT 5
#define _rc_NT 6
#define _rc5_NT 7
#define _ar_NT 8

static char *_ntname[] = {
	0,
	"stmt",
	"reg",
	"con",
	"acon",
	"addr",
	"rc",
	"rc5",
	"ar",
	0
};

struct _state {
	short cost[9];
	struct {
		unsigned int _stmt:6;
		unsigned int _reg:7;
		unsigned int _con:3;
		unsigned int _acon:2;
		unsigned int _addr:3;
		unsigned int _rc:2;
		unsigned int _rc5:2;
		unsigned int _ar:2;
	} rule;
};

static short _nts_0[] = { 0 };
static short _nts_1[] = { _reg_NT, 0 };
static short _nts_2[] = { _con_NT, 0 };
static short _nts_3[] = { _reg_NT, _acon_NT, 0 };
static short _nts_4[] = { _acon_NT, 0 };
static short _nts_5[] = { _addr_NT, 0 };
static short _nts_6[] = { _addr_NT, _reg_NT, 0 };
static short _nts_7[] = { _reg_NT, _rc_NT, 0 };
static short _nts_8[] = { _reg_NT, _rc5_NT, 0 };
static short _nts_9[] = { _reg_NT, _reg_NT, 0 };
static short _nts_10[] = { _ar_NT, 0 };

static short *_nts[] = {
	0,	/* 0 */
	_nts_0,	/* 1 */
	_nts_0,	/* 2 */
	_nts_0,	/* 3 */
	_nts_0,	/* 4 */
	_nts_0,	/* 5 */
	_nts_0,	/* 6 */
	_nts_0,	/* 7 */
	_nts_1,	/* 8 */
	_nts_1,	/* 9 */
	_nts_1,	/* 10 */
	_nts_1,	/* 11 */
	_nts_1,	/* 12 */
	_nts_1,	/* 13 */
	_nts_1,	/* 14 */
	_nts_0,	/* 15 */
	_nts_0,	/* 16 */
	_nts_0,	/* 17 */
	_nts_0,	/* 18 */
	_nts_0,	/* 19 */
	_nts_0,	/* 20 */
	_nts_0,	/* 21 */
	_nts_1,	/* 22 */
	_nts_2,	/* 23 */
	_nts_0,	/* 24 */
	_nts_3,	/* 25 */
	_nts_3,	/* 26 */
	_nts_3,	/* 27 */
	_nts_4,	/* 28 */
	_nts_1,	/* 29 */
	_nts_0,	/* 30 */
	_nts_0,	/* 31 */
	_nts_5,	/* 32 */
	_nts_0,	/* 33 */
	_nts_0,	/* 34 */
	_nts_0,	/* 35 */
	_nts_0,	/* 36 */
	_nts_0,	/* 37 */
	_nts_0,	/* 38 */
	_nts_0,	/* 39 */
	_nts_6,	/* 40 */
	_nts_6,	/* 41 */
	_nts_6,	/* 42 */
	_nts_6,	/* 43 */
	_nts_6,	/* 44 */
	_nts_6,	/* 45 */
	_nts_6,	/* 46 */
	_nts_5,	/* 47 */
	_nts_5,	/* 48 */
	_nts_5,	/* 49 */
	_nts_5,	/* 50 */
	_nts_5,	/* 51 */
	_nts_5,	/* 52 */
	_nts_5,	/* 53 */
	_nts_5,	/* 54 */
	_nts_5,	/* 55 */
	_nts_5,	/* 56 */
	_nts_5,	/* 57 */
	_nts_5,	/* 58 */
	_nts_5,	/* 59 */
	_nts_2,	/* 60 */
	_nts_1,	/* 61 */
	_nts_7,	/* 62 */
	_nts_7,	/* 63 */
	_nts_7,	/* 64 */
	_nts_7,	/* 65 */
	_nts_7,	/* 66 */
	_nts_7,	/* 67 */
	_nts_1,	/* 68 */
	_nts_7,	/* 69 */
	_nts_7,	/* 70 */
	_nts_7,	/* 71 */
	_nts_7,	/* 72 */
	_nts_7,	/* 73 */
	_nts_7,	/* 74 */
	_nts_7,	/* 75 */
	_nts_7,	/* 76 */
	_nts_7,	/* 77 */
	_nts_7,	/* 78 */
	_nts_7,	/* 79 */
	_nts_7,	/* 80 */
	_nts_1,	/* 81 */
	_nts_1,	/* 82 */
	_nts_0,	/* 83 */
	_nts_1,	/* 84 */
	_nts_8,	/* 85 */
	_nts_8,	/* 86 */
	_nts_8,	/* 87 */
	_nts_8,	/* 88 */
	_nts_1,	/* 89 */
	_nts_1,	/* 90 */
	_nts_1,	/* 91 */
	_nts_1,	/* 92 */
	_nts_1,	/* 93 */
	_nts_1,	/* 94 */
	_nts_1,	/* 95 */
	_nts_1,	/* 96 */
	_nts_1,	/* 97 */
	_nts_1,	/* 98 */
	_nts_0,	/* 99 */
	_nts_4,	/* 100 */
	_nts_1,	/* 101 */
	_nts_9,	/* 102 */
	_nts_9,	/* 103 */
	_nts_9,	/* 104 */
	_nts_9,	/* 105 */
	_nts_9,	/* 106 */
	_nts_9,	/* 107 */
	_nts_9,	/* 108 */
	_nts_9,	/* 109 */
	_nts_9,	/* 110 */
	_nts_9,	/* 111 */
	_nts_9,	/* 112 */
	_nts_9,	/* 113 */
	_nts_10,	/* 114 */
	_nts_10,	/* 115 */
	_nts_10,	/* 116 */
	_nts_10,	/* 117 */
	_nts_0,	/* 118 */
	_nts_1,	/* 119 */
	_nts_0,	/* 120 */
	_nts_1,	/* 121 */
	_nts_1,	/* 122 */
	_nts_1,	/* 123 */
	_nts_1,	/* 124 */
	_nts_1,	/* 125 */
	_nts_1,	/* 126 */
	_nts_1,	/* 127 */
	_nts_1,	/* 128 */
	_nts_9,	/* 129 */
	_nts_0,	/* 130 */
	_nts_1,	/* 131 */
	_nts_5,	/* 132 */
	_nts_6,	/* 133 */
	_nts_9,	/* 134 */
	_nts_9,	/* 135 */
	_nts_9,	/* 136 */
	_nts_9,	/* 137 */
	_nts_1,	/* 138 */
	_nts_1,	/* 139 */
	_nts_1,	/* 140 */
	_nts_1,	/* 141 */
	_nts_1,	/* 142 */
	_nts_9,	/* 143 */
	_nts_9,	/* 144 */
	_nts_9,	/* 145 */
	_nts_9,	/* 146 */
	_nts_9,	/* 147 */
	_nts_9,	/* 148 */
	_nts_10,	/* 149 */
	_nts_1,	/* 150 */
	_nts_1,	/* 151 */
	_nts_0,	/* 152 */
	_nts_1,	/* 153 */
	_nts_5,	/* 154 */
	_nts_6,	/* 155 */
	_nts_9,	/* 156 */
	_nts_9,	/* 157 */
	_nts_9,	/* 158 */
	_nts_9,	/* 159 */
	_nts_1,	/* 160 */
	_nts_1,	/* 161 */
	_nts_1,	/* 162 */
	_nts_1,	/* 163 */
	_nts_1,	/* 164 */
	_nts_9,	/* 165 */
	_nts_9,	/* 166 */
	_nts_9,	/* 167 */
	_nts_9,	/* 168 */
	_nts_9,	/* 169 */
	_nts_9,	/* 170 */
	_nts_10,	/* 171 */
	_nts_1,	/* 172 */
	_nts_1,	/* 173 */
};

static char *_templates[] = {
/* 0 */	0,
/* 1 */	"# read register\n",	/* reg: INDIRI1(VREGP) */
/* 2 */	"# read register\n",	/* reg: INDIRI2(VREGP) */
/* 3 */	"# read register\n",	/* reg: INDIRI4(VREGP) */
/* 4 */	"# read register\n",	/* reg: INDIRP4(VREGP) */
/* 5 */	"# read register\n",	/* reg: INDIRU1(VREGP) */
/* 6 */	"# read register\n",	/* reg: INDIRU2(VREGP) */
/* 7 */	"# read register\n",	/* reg: INDIRU4(VREGP) */
/* 8 */	"# write register\n",	/* stmt: ASGNI1(VREGP,reg) */
/* 9 */	"# write register\n",	/* stmt: ASGNI2(VREGP,reg) */
/* 10 */	"# write register\n",	/* stmt: ASGNI4(VREGP,reg) */
/* 11 */	"# write register\n",	/* stmt: ASGNP4(VREGP,reg) */
/* 12 */	"# write register\n",	/* stmt: ASGNU1(VREGP,reg) */
/* 13 */	"# write register\n",	/* stmt: ASGNU2(VREGP,reg) */
/* 14 */	"# write register\n",	/* stmt: ASGNU4(VREGP,reg) */
/* 15 */	"%a",	/* con: CNSTI1 */
/* 16 */	"%a",	/* con: CNSTI2 */
/* 17 */	"%a",	/* con: CNSTI4 */
/* 18 */	"%a",	/* con: CNSTP4 */
/* 19 */	"%a",	/* con: CNSTU1 */
/* 20 */	"%a",	/* con: CNSTU2 */
/* 21 */	"%a",	/* con: CNSTU4 */
/* 22 */	"",	/* stmt: reg */
/* 23 */	"%0",	/* acon: con */
/* 24 */	"%a",	/* acon: ADDRGP4 */
/* 25 */	"$%0,%1",	/* addr: ADDI4(reg,acon) */
/* 26 */	"$%0,%1",	/* addr: ADDP4(reg,acon) */
/* 27 */	"$%0,%1",	/* addr: ADDU4(reg,acon) */
/* 28 */	"$0,%0",	/* addr: acon */
/* 29 */	"$%0,0",	/* addr: reg */
/* 30 */	"$29,%a+%F",	/* addr: ADDRFP4 */
/* 31 */	"$29,%a+%F",	/* addr: ADDRLP4 */
/* 32 */	"\tadd\t$%c,%0\n",	/* reg: addr */
/* 33 */	"# reg\n",	/* reg: CNSTI1 */
/* 34 */	"# reg\n",	/* reg: CNSTI2 */
/* 35 */	"# reg\n",	/* reg: CNSTI4 */
/* 36 */	"# reg\n",	/* reg: CNSTP4 */
/* 37 */	"# reg\n",	/* reg: CNSTU1 */
/* 38 */	"# reg\n",	/* reg: CNSTU2 */
/* 39 */	"# reg\n",	/* reg: CNSTU4 */
/* 40 */	"\tstb\t$%1,%0\n",	/* stmt: ASGNI1(addr,reg) */
/* 41 */	"\tsth\t$%1,%0\n",	/* stmt: ASGNI2(addr,reg) */
/* 42 */	"\tstw\t$%1,%0\n",	/* stmt: ASGNI4(addr,reg) */
/* 43 */	"\tstw\t$%1,%0\n",	/* stmt: ASGNP4(addr,reg) */
/* 44 */	"\tstb\t$%1,%0\n",	/* stmt: ASGNU1(addr,reg) */
/* 45 */	"\tsth\t$%1,%0\n",	/* stmt: ASGNU2(addr,reg) */
/* 46 */	"\tstw\t$%1,%0\n",	/* stmt: ASGNU4(addr,reg) */
/* 47 */	"\tldb\t$%c,%0\n",	/* reg: INDIRI1(addr) */
/* 48 */	"\tldh\t$%c,%0\n",	/* reg: INDIRI2(addr) */
/* 49 */	"\tldw\t$%c,%0\n",	/* reg: INDIRI4(addr) */
/* 50 */	"\tldw\t$%c,%0\n",	/* reg: INDIRP4(addr) */
/* 51 */	"\tldbu\t$%c,%0\n",	/* reg: INDIRU1(addr) */
/* 52 */	"\tldhu\t$%c,%0\n",	/* reg: INDIRU2(addr) */
/* 53 */	"\tldw\t$%c,%0\n",	/* reg: INDIRU4(addr) */
/* 54 */	"\tldb\t$%c,%0\n",	/* reg: CVII4(INDIRI1(addr)) */
/* 55 */	"\tldh\t$%c,%0\n",	/* reg: CVII4(INDIRI2(addr)) */
/* 56 */	"\tldbu\t$%c,%0\n",	/* reg: CVUU4(INDIRU1(addr)) */
/* 57 */	"\tldhu\t$%c,%0\n",	/* reg: CVUU4(INDIRU2(addr)) */
/* 58 */	"\tldbu\t$%c,%0\n",	/* reg: CVUI4(INDIRU1(addr)) */
/* 59 */	"\tldhu\t$%c,%0\n",	/* reg: CVUI4(INDIRU2(addr)) */
/* 60 */	"%0",	/* rc: con */
/* 61 */	"$%0",	/* rc: reg */
/* 62 */	"\tadd\t$%c,$%0,%1\n",	/* reg: ADDI4(reg,rc) */
/* 63 */	"\tadd\t$%c,$%0,%1\n",	/* reg: ADDP4(reg,rc) */
/* 64 */	"\tadd\t$%c,$%0,%1\n",	/* reg: ADDU4(reg,rc) */
/* 65 */	"\tsub\t$%c,$%0,%1\n",	/* reg: SUBI4(reg,rc) */
/* 66 */	"\tsub\t$%c,$%0,%1\n",	/* reg: SUBP4(reg,rc) */
/* 67 */	"\tsub\t$%c,$%0,%1\n",	/* reg: SUBU4(reg,rc) */
/* 68 */	"\tsub\t$%c,$0,$%0\n",	/* reg: NEGI4(reg) */
/* 69 */	"\tmul\t$%c,$%0,%1\n",	/* reg: MULI4(reg,rc) */
/* 70 */	"\tmulu\t$%c,$%0,%1\n",	/* reg: MULU4(reg,rc) */
/* 71 */	"\tdiv\t$%c,$%0,%1\n",	/* reg: DIVI4(reg,rc) */
/* 72 */	"\tdivu\t$%c,$%0,%1\n",	/* reg: DIVU4(reg,rc) */
/* 73 */	"\trem\t$%c,$%0,%1\n",	/* reg: MODI4(reg,rc) */
/* 74 */	"\tremu\t$%c,$%0,%1\n",	/* reg: MODU4(reg,rc) */
/* 75 */	"\tand\t$%c,$%0,%1\n",	/* reg: BANDI4(reg,rc) */
/* 76 */	"\tand\t$%c,$%0,%1\n",	/* reg: BANDU4(reg,rc) */
/* 77 */	"\tor\t$%c,$%0,%1\n",	/* reg: BORI4(reg,rc) */
/* 78 */	"\tor\t$%c,$%0,%1\n",	/* reg: BORU4(reg,rc) */
/* 79 */	"\txor\t$%c,$%0,%1\n",	/* reg: BXORI4(reg,rc) */
/* 80 */	"\txor\t$%c,$%0,%1\n",	/* reg: BXORU4(reg,rc) */
/* 81 */	"\txnor\t$%c,$0,$%0\n",	/* reg: BCOMI4(reg) */
/* 82 */	"\txnor\t$%c,$0,$%0\n",	/* reg: BCOMU4(reg) */
/* 83 */	"%a",	/* rc5: CNSTI4 */
/* 84 */	"$%0",	/* rc5: reg */
/* 85 */	"\tsll\t$%c,$%0,%1\n",	/* reg: LSHI4(reg,rc5) */
/* 86 */	"\tsll\t$%c,$%0,%1\n",	/* reg: LSHU4(reg,rc5) */
/* 87 */	"\tsar\t$%c,$%0,%1\n",	/* reg: RSHI4(reg,rc5) */
/* 88 */	"\tslr\t$%c,$%0,%1\n",	/* reg: RSHU4(reg,rc5) */
/* 89 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADI1(reg) */
/* 90 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADI2(reg) */
/* 91 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADI4(reg) */
/* 92 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADP4(reg) */
/* 93 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADU1(reg) */
/* 94 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADU2(reg) */
/* 95 */	"\tadd\t$%c,$0,$%0\n",	/* reg: LOADU4(reg) */
/* 96 */	"\tsll\t$%c,$%0,8*(4-%a)\n\tsar\t$%c,$%c,8*(4-%a)\n",	/* reg: CVII4(reg) */
/* 97 */	"\tand\t$%c,$%0,(1<<(8*%a))-1\n",	/* reg: CVUI4(reg) */
/* 98 */	"\tand\t$%c,$%0,(1<<(8*%a))-1\n",	/* reg: CVUU4(reg) */
/* 99 */	"%a:\n",	/* stmt: LABELV */
/* 100 */	"\tj\t%0\n",	/* stmt: JUMPV(acon) */
/* 101 */	"\tjr\t$%0\n",	/* stmt: JUMPV(reg) */
/* 102 */	"\tbeq\t$%0,$%1,%a\n",	/* stmt: EQI4(reg,reg) */
/* 103 */	"\tbeq\t$%0,$%1,%a\n",	/* stmt: EQU4(reg,reg) */
/* 104 */	"\tbne\t$%0,$%1,%a\n",	/* stmt: NEI4(reg,reg) */
/* 105 */	"\tbne\t$%0,$%1,%a\n",	/* stmt: NEU4(reg,reg) */
/* 106 */	"\tble\t$%0,$%1,%a\n",	/* stmt: LEI4(reg,reg) */
/* 107 */	"\tbleu\t$%0,$%1,%a\n",	/* stmt: LEU4(reg,reg) */
/* 108 */	"\tblt\t$%0,$%1,%a\n",	/* stmt: LTI4(reg,reg) */
/* 109 */	"\tbltu\t$%0,$%1,%a\n",	/* stmt: LTU4(reg,reg) */
/* 110 */	"\tbge\t$%0,$%1,%a\n",	/* stmt: GEI4(reg,reg) */
/* 111 */	"\tbgeu\t$%0,$%1,%a\n",	/* stmt: GEU4(reg,reg) */
/* 112 */	"\tbgt\t$%0,$%1,%a\n",	/* stmt: GTI4(reg,reg) */
/* 113 */	"\tbgtu\t$%0,$%1,%a\n",	/* stmt: GTU4(reg,reg) */
/* 114 */	"\tjal\t%0\n",	/* reg: CALLI4(ar) */
/* 115 */	"\tjal\t%0\n",	/* reg: CALLP4(ar) */
/* 116 */	"\tjal\t%0\n",	/* reg: CALLU4(ar) */
/* 117 */	"\tjal\t%0\n",	/* stmt: CALLV(ar) */
/* 118 */	"%a",	/* ar: ADDRGP4 */
/* 119 */	"$%0",	/* ar: reg */
/* 120 */	"%a",	/* ar: CNSTP4 */
/* 121 */	"# ret\n",	/* stmt: RETI4(reg) */
/* 122 */	"# ret\n",	/* stmt: RETP4(reg) */
/* 123 */	"# ret\n",	/* stmt: RETU4(reg) */
/* 124 */	"# ret\n",	/* stmt: RETV(reg) */
/* 125 */	"# arg\n",	/* stmt: ARGI4(reg) */
/* 126 */	"# arg\n",	/* stmt: ARGP4(reg) */
/* 127 */	"# arg\n",	/* stmt: ARGU4(reg) */
/* 128 */	"# argb %0\n",	/* stmt: ARGB(INDIRB(reg)) */
/* 129 */	"# asgnb %0 %1\n",	/* stmt: ASGNB(reg,INDIRB(reg)) */
/* 130 */	"# read register\n",	/* reg: INDIRF4(VREGP) */
/* 131 */	"# write register\n",	/* stmt: ASGNF4(VREGP,reg) */
/* 132 */	";FP: l.s $f%c,%0\n",	/* reg: INDIRF4(addr) */
/* 133 */	";FP: s.s $f%1,%0\n",	/* stmt: ASGNF4(addr,reg) */
/* 134 */	";FP: add.s $f%c,$f%0,$f%1\n",	/* reg: ADDF4(reg,reg) */
/* 135 */	";FP: sub.s $f%c,$f%0,$f%1\n",	/* reg: SUBF4(reg,reg) */
/* 136 */	";FP: mul.s $f%c,$f%0,$f%1\n",	/* reg: MULF4(reg,reg) */
/* 137 */	";FP: div.s $f%c,$f%0,$f%1\n",	/* reg: DIVF4(reg,reg) */
/* 138 */	";FP: mov.s $f%c,$f%0\n",	/* reg: LOADF4(reg) */
/* 139 */	";FP: neg.s $f%c,$f%0\n",	/* reg: NEGF4(reg) */
/* 140 */	";FP: cvt.s.d $f%c,$f%0\n",	/* reg: CVFF4(reg) */
/* 141 */	";FP: mtc1 $%0,$f%c; cvt.s.w $f%c,$f%c\n",	/* reg: CVIF4(reg) */
/* 142 */	";FP: trunc.w.s $f2,$f%0,$%c; mfc1 $%c,$f2\n",	/* reg: CVFI4(reg) */
/* 143 */	";FP: c.eq.s $f%0,$f%1; bc1t %a\n",	/* stmt: EQF4(reg,reg) */
/* 144 */	";FP: c.ule.s $f%0,$f%1; bc1t %a\n",	/* stmt: LEF4(reg,reg) */
/* 145 */	";FP: c.ult.s $f%0,$f%1; bc1t %a\n",	/* stmt: LTF4(reg,reg) */
/* 146 */	";FP: c.lt.s $f%0,$f%1; bc1f %a\n",	/* stmt: GEF4(reg,reg) */
/* 147 */	";FP: c.le.s $f%0,$f%1; bc1f %a\n",	/* stmt: GTF4(reg,reg) */
/* 148 */	";FP: c.eq.s $f%0,$f%1; bc1f %a\n",	/* stmt: NEF4(reg,reg) */
/* 149 */	"\tjal\t%0\n",	/* reg: CALLF4(ar) */
/* 150 */	"# ret\n",	/* stmt: RETF4(reg) */
/* 151 */	"# arg\n",	/* stmt: ARGF4(reg) */
/* 152 */	"# read register\n",	/* reg: INDIRF8(VREGP) */
/* 153 */	"# write register\n",	/* stmt: ASGNF8(VREGP,reg) */
/* 154 */	";FP: l.d $f%c,%0\n",	/* reg: INDIRF8(addr) */
/* 155 */	";FP: s.d $f%1,%0\n",	/* stmt: ASGNF8(addr,reg) */
/* 156 */	";FP: add.d $f%c,$f%0,$f%1\n",	/* reg: ADDF8(reg,reg) */
/* 157 */	";FP: sub.d $f%c,$f%0,$f%1\n",	/* reg: SUBF8(reg,reg) */
/* 158 */	";FP: mul.d $f%c,$f%0,$f%1\n",	/* reg: MULF8(reg,reg) */
/* 159 */	";FP: div.d $f%c,$f%0,$f%1\n",	/* reg: DIVF8(reg,reg) */
/* 160 */	";FP: mov.d $f%c,$f%0\n",	/* reg: LOADF8(reg) */
/* 161 */	";FP: neg.d $f%c,$f%0\n",	/* reg: NEGF8(reg) */
/* 162 */	";FP: cvt.d.s $f%c,$f%0\n",	/* reg: CVFF8(reg) */
/* 163 */	";FP: mtc1 $%0,$f%c; cvt.d.w $f%c,$f%c\n",	/* reg: CVIF8(reg) */
/* 164 */	";FP: trunc.w.d $f2,$f%0,$%c; mfc1 $%c,$f2\n",	/* reg: CVFI4(reg) */
/* 165 */	";FP: c.eq.d $f%0,$f%1; bc1t %a\n",	/* stmt: EQF8(reg,reg) */
/* 166 */	";FP: c.ule.d $f%0,$f%1; bc1t %a\n",	/* stmt: LEF8(reg,reg) */
/* 167 */	";FP: c.ult.d $f%0,$f%1; bc1t %a\n",	/* stmt: LTF8(reg,reg) */
/* 168 */	";FP: c.lt.d $f%0,$f%1; bc1f %a\n",	/* stmt: GEF8(reg,reg) */
/* 169 */	";FP: c.le.d $f%0,$f%1; bc1f %a\n",	/* stmt: GTF8(reg,reg) */
/* 170 */	";FP: c.eq.d $f%0,$f%1; bc1f %a\n",	/* stmt: NEF8(reg,reg) */
/* 171 */	"\tjal\t%0\n",	/* reg: CALLF8(ar) */
/* 172 */	"# ret\n",	/* stmt: RETF8(reg) */
/* 173 */	"# arg\n",	/* stmt: ARGF8(reg) */
};

static char _isinstruction[] = {
/* 0 */	0,
/* 1 */	1,	/* # read register\n */
/* 2 */	1,	/* # read register\n */
/* 3 */	1,	/* # read register\n */
/* 4 */	1,	/* # read register\n */
/* 5 */	1,	/* # read register\n */
/* 6 */	1,	/* # read register\n */
/* 7 */	1,	/* # read register\n */
/* 8 */	1,	/* # write register\n */
/* 9 */	1,	/* # write register\n */
/* 10 */	1,	/* # write register\n */
/* 11 */	1,	/* # write register\n */
/* 12 */	1,	/* # write register\n */
/* 13 */	1,	/* # write register\n */
/* 14 */	1,	/* # write register\n */
/* 15 */	0,	/* %a */
/* 16 */	0,	/* %a */
/* 17 */	0,	/* %a */
/* 18 */	0,	/* %a */
/* 19 */	0,	/* %a */
/* 20 */	0,	/* %a */
/* 21 */	0,	/* %a */
/* 22 */	0,	/*  */
/* 23 */	0,	/* %0 */
/* 24 */	0,	/* %a */
/* 25 */	0,	/* $%0,%1 */
/* 26 */	0,	/* $%0,%1 */
/* 27 */	0,	/* $%0,%1 */
/* 28 */	0,	/* $0,%0 */
/* 29 */	0,	/* $%0,0 */
/* 30 */	0,	/* $29,%a+%F */
/* 31 */	0,	/* $29,%a+%F */
/* 32 */	1,	/* \tadd\t$%c,%0\n */
/* 33 */	1,	/* # reg\n */
/* 34 */	1,	/* # reg\n */
/* 35 */	1,	/* # reg\n */
/* 36 */	1,	/* # reg\n */
/* 37 */	1,	/* # reg\n */
/* 38 */	1,	/* # reg\n */
/* 39 */	1,	/* # reg\n */
/* 40 */	1,	/* \tstb\t$%1,%0\n */
/* 41 */	1,	/* \tsth\t$%1,%0\n */
/* 42 */	1,	/* \tstw\t$%1,%0\n */
/* 43 */	1,	/* \tstw\t$%1,%0\n */
/* 44 */	1,	/* \tstb\t$%1,%0\n */
/* 45 */	1,	/* \tsth\t$%1,%0\n */
/* 46 */	1,	/* \tstw\t$%1,%0\n */
/* 47 */	1,	/* \tldb\t$%c,%0\n */
/* 48 */	1,	/* \tldh\t$%c,%0\n */
/* 49 */	1,	/* \tldw\t$%c,%0\n */
/* 50 */	1,	/* \tldw\t$%c,%0\n */
/* 51 */	1,	/* \tldbu\t$%c,%0\n */
/* 52 */	1,	/* \tldhu\t$%c,%0\n */
/* 53 */	1,	/* \tldw\t$%c,%0\n */
/* 54 */	1,	/* \tldb\t$%c,%0\n */
/* 55 */	1,	/* \tldh\t$%c,%0\n */
/* 56 */	1,	/* \tldbu\t$%c,%0\n */
/* 57 */	1,	/* \tldhu\t$%c,%0\n */
/* 58 */	1,	/* \tldbu\t$%c,%0\n */
/* 59 */	1,	/* \tldhu\t$%c,%0\n */
/* 60 */	0,	/* %0 */
/* 61 */	0,	/* $%0 */
/* 62 */	1,	/* \tadd\t$%c,$%0,%1\n */
/* 63 */	1,	/* \tadd\t$%c,$%0,%1\n */
/* 64 */	1,	/* \tadd\t$%c,$%0,%1\n */
/* 65 */	1,	/* \tsub\t$%c,$%0,%1\n */
/* 66 */	1,	/* \tsub\t$%c,$%0,%1\n */
/* 67 */	1,	/* \tsub\t$%c,$%0,%1\n */
/* 68 */	1,	/* \tsub\t$%c,$0,$%0\n */
/* 69 */	1,	/* \tmul\t$%c,$%0,%1\n */
/* 70 */	1,	/* \tmulu\t$%c,$%0,%1\n */
/* 71 */	1,	/* \tdiv\t$%c,$%0,%1\n */
/* 72 */	1,	/* \tdivu\t$%c,$%0,%1\n */
/* 73 */	1,	/* \trem\t$%c,$%0,%1\n */
/* 74 */	1,	/* \tremu\t$%c,$%0,%1\n */
/* 75 */	1,	/* \tand\t$%c,$%0,%1\n */
/* 76 */	1,	/* \tand\t$%c,$%0,%1\n */
/* 77 */	1,	/* \tor\t$%c,$%0,%1\n */
/* 78 */	1,	/* \tor\t$%c,$%0,%1\n */
/* 79 */	1,	/* \txor\t$%c,$%0,%1\n */
/* 80 */	1,	/* \txor\t$%c,$%0,%1\n */
/* 81 */	1,	/* \txnor\t$%c,$0,$%0\n */
/* 82 */	1,	/* \txnor\t$%c,$0,$%0\n */
/* 83 */	0,	/* %a */
/* 84 */	0,	/* $%0 */
/* 85 */	1,	/* \tsll\t$%c,$%0,%1\n */
/* 86 */	1,	/* \tsll\t$%c,$%0,%1\n */
/* 87 */	1,	/* \tsar\t$%c,$%0,%1\n */
/* 88 */	1,	/* \tslr\t$%c,$%0,%1\n */
/* 89 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 90 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 91 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 92 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 93 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 94 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 95 */	1,	/* \tadd\t$%c,$0,$%0\n */
/* 96 */	1,	/* \tsll\t$%c,$%0,8*(4-%a)\n\tsar\t$%c,$%c,8*(4-%a)\n */
/* 97 */	1,	/* \tand\t$%c,$%0,(1<<(8*%a))-1\n */
/* 98 */	1,	/* \tand\t$%c,$%0,(1<<(8*%a))-1\n */
/* 99 */	1,	/* %a:\n */
/* 100 */	1,	/* \tj\t%0\n */
/* 101 */	1,	/* \tjr\t$%0\n */
/* 102 */	1,	/* \tbeq\t$%0,$%1,%a\n */
/* 103 */	1,	/* \tbeq\t$%0,$%1,%a\n */
/* 104 */	1,	/* \tbne\t$%0,$%1,%a\n */
/* 105 */	1,	/* \tbne\t$%0,$%1,%a\n */
/* 106 */	1,	/* \tble\t$%0,$%1,%a\n */
/* 107 */	1,	/* \tbleu\t$%0,$%1,%a\n */
/* 108 */	1,	/* \tblt\t$%0,$%1,%a\n */
/* 109 */	1,	/* \tbltu\t$%0,$%1,%a\n */
/* 110 */	1,	/* \tbge\t$%0,$%1,%a\n */
/* 111 */	1,	/* \tbgeu\t$%0,$%1,%a\n */
/* 112 */	1,	/* \tbgt\t$%0,$%1,%a\n */
/* 113 */	1,	/* \tbgtu\t$%0,$%1,%a\n */
/* 114 */	1,	/* \tjal\t%0\n */
/* 115 */	1,	/* \tjal\t%0\n */
/* 116 */	1,	/* \tjal\t%0\n */
/* 117 */	1,	/* \tjal\t%0\n */
/* 118 */	0,	/* %a */
/* 119 */	0,	/* $%0 */
/* 120 */	0,	/* %a */
/* 121 */	1,	/* # ret\n */
/* 122 */	1,	/* # ret\n */
/* 123 */	1,	/* # ret\n */
/* 124 */	1,	/* # ret\n */
/* 125 */	1,	/* # arg\n */
/* 126 */	1,	/* # arg\n */
/* 127 */	1,	/* # arg\n */
/* 128 */	1,	/* # argb %0\n */
/* 129 */	1,	/* # asgnb %0 %1\n */
/* 130 */	1,	/* # read register\n */
/* 131 */	1,	/* # write register\n */
/* 132 */	1,	/* ;FP: l.s $f%c,%0\n */
/* 133 */	1,	/* ;FP: s.s $f%1,%0\n */
/* 134 */	1,	/* ;FP: add.s $f%c,$f%0,$f%1\n */
/* 135 */	1,	/* ;FP: sub.s $f%c,$f%0,$f%1\n */
/* 136 */	1,	/* ;FP: mul.s $f%c,$f%0,$f%1\n */
/* 137 */	1,	/* ;FP: div.s $f%c,$f%0,$f%1\n */
/* 138 */	1,	/* ;FP: mov.s $f%c,$f%0\n */
/* 139 */	1,	/* ;FP: neg.s $f%c,$f%0\n */
/* 140 */	1,	/* ;FP: cvt.s.d $f%c,$f%0\n */
/* 141 */	1,	/* ;FP: mtc1 $%0,$f%c; cvt.s.w $f%c,$f%c\n */
/* 142 */	1,	/* ;FP: trunc.w.s $f2,$f%0,$%c; mfc1 $%c,$f2\n */
/* 143 */	1,	/* ;FP: c.eq.s $f%0,$f%1; bc1t %a\n */
/* 144 */	1,	/* ;FP: c.ule.s $f%0,$f%1; bc1t %a\n */
/* 145 */	1,	/* ;FP: c.ult.s $f%0,$f%1; bc1t %a\n */
/* 146 */	1,	/* ;FP: c.lt.s $f%0,$f%1; bc1f %a\n */
/* 147 */	1,	/* ;FP: c.le.s $f%0,$f%1; bc1f %a\n */
/* 148 */	1,	/* ;FP: c.eq.s $f%0,$f%1; bc1f %a\n */
/* 149 */	1,	/* \tjal\t%0\n */
/* 150 */	1,	/* # ret\n */
/* 151 */	1,	/* # arg\n */
/* 152 */	1,	/* # read register\n */
/* 153 */	1,	/* # write register\n */
/* 154 */	1,	/* ;FP: l.d $f%c,%0\n */
/* 155 */	1,	/* ;FP: s.d $f%1,%0\n */
/* 156 */	1,	/* ;FP: add.d $f%c,$f%0,$f%1\n */
/* 157 */	1,	/* ;FP: sub.d $f%c,$f%0,$f%1\n */
/* 158 */	1,	/* ;FP: mul.d $f%c,$f%0,$f%1\n */
/* 159 */	1,	/* ;FP: div.d $f%c,$f%0,$f%1\n */
/* 160 */	1,	/* ;FP: mov.d $f%c,$f%0\n */
/* 161 */	1,	/* ;FP: neg.d $f%c,$f%0\n */
/* 162 */	1,	/* ;FP: cvt.d.s $f%c,$f%0\n */
/* 163 */	1,	/* ;FP: mtc1 $%0,$f%c; cvt.d.w $f%c,$f%c\n */
/* 164 */	1,	/* ;FP: trunc.w.d $f2,$f%0,$%c; mfc1 $%c,$f2\n */
/* 165 */	1,	/* ;FP: c.eq.d $f%0,$f%1; bc1t %a\n */
/* 166 */	1,	/* ;FP: c.ule.d $f%0,$f%1; bc1t %a\n */
/* 167 */	1,	/* ;FP: c.ult.d $f%0,$f%1; bc1t %a\n */
/* 168 */	1,	/* ;FP: c.lt.d $f%0,$f%1; bc1f %a\n */
/* 169 */	1,	/* ;FP: c.le.d $f%0,$f%1; bc1f %a\n */
/* 170 */	1,	/* ;FP: c.eq.d $f%0,$f%1; bc1f %a\n */
/* 171 */	1,	/* \tjal\t%0\n */
/* 172 */	1,	/* # ret\n */
/* 173 */	1,	/* # arg\n */
};

static char *_string[] = {
/* 0 */	0,
/* 1 */	"reg: INDIRI1(VREGP)",
/* 2 */	"reg: INDIRI2(VREGP)",
/* 3 */	"reg: INDIRI4(VREGP)",
/* 4 */	"reg: INDIRP4(VREGP)",
/* 5 */	"reg: INDIRU1(VREGP)",
/* 6 */	"reg: INDIRU2(VREGP)",
/* 7 */	"reg: INDIRU4(VREGP)",
/* 8 */	"stmt: ASGNI1(VREGP,reg)",
/* 9 */	"stmt: ASGNI2(VREGP,reg)",
/* 10 */	"stmt: ASGNI4(VREGP,reg)",
/* 11 */	"stmt: ASGNP4(VREGP,reg)",
/* 12 */	"stmt: ASGNU1(VREGP,reg)",
/* 13 */	"stmt: ASGNU2(VREGP,reg)",
/* 14 */	"stmt: ASGNU4(VREGP,reg)",
/* 15 */	"con: CNSTI1",
/* 16 */	"con: CNSTI2",
/* 17 */	"con: CNSTI4",
/* 18 */	"con: CNSTP4",
/* 19 */	"con: CNSTU1",
/* 20 */	"con: CNSTU2",
/* 21 */	"con: CNSTU4",
/* 22 */	"stmt: reg",
/* 23 */	"acon: con",
/* 24 */	"acon: ADDRGP4",
/* 25 */	"addr: ADDI4(reg,acon)",
/* 26 */	"addr: ADDP4(reg,acon)",
/* 27 */	"addr: ADDU4(reg,acon)",
/* 28 */	"addr: acon",
/* 29 */	"addr: reg",
/* 30 */	"addr: ADDRFP4",
/* 31 */	"addr: ADDRLP4",
/* 32 */	"reg: addr",
/* 33 */	"reg: CNSTI1",
/* 34 */	"reg: CNSTI2",
/* 35 */	"reg: CNSTI4",
/* 36 */	"reg: CNSTP4",
/* 37 */	"reg: CNSTU1",
/* 38 */	"reg: CNSTU2",
/* 39 */	"reg: CNSTU4",
/* 40 */	"stmt: ASGNI1(addr,reg)",
/* 41 */	"stmt: ASGNI2(addr,reg)",
/* 42 */	"stmt: ASGNI4(addr,reg)",
/* 43 */	"stmt: ASGNP4(addr,reg)",
/* 44 */	"stmt: ASGNU1(addr,reg)",
/* 45 */	"stmt: ASGNU2(addr,reg)",
/* 46 */	"stmt: ASGNU4(addr,reg)",
/* 47 */	"reg: INDIRI1(addr)",
/* 48 */	"reg: INDIRI2(addr)",
/* 49 */	"reg: INDIRI4(addr)",
/* 50 */	"reg: INDIRP4(addr)",
/* 51 */	"reg: INDIRU1(addr)",
/* 52 */	"reg: INDIRU2(addr)",
/* 53 */	"reg: INDIRU4(addr)",
/* 54 */	"reg: CVII4(INDIRI1(addr))",
/* 55 */	"reg: CVII4(INDIRI2(addr))",
/* 56 */	"reg: CVUU4(INDIRU1(addr))",
/* 57 */	"reg: CVUU4(INDIRU2(addr))",
/* 58 */	"reg: CVUI4(INDIRU1(addr))",
/* 59 */	"reg: CVUI4(INDIRU2(addr))",
/* 60 */	"rc: con",
/* 61 */	"rc: reg",
/* 62 */	"reg: ADDI4(reg,rc)",
/* 63 */	"reg: ADDP4(reg,rc)",
/* 64 */	"reg: ADDU4(reg,rc)",
/* 65 */	"reg: SUBI4(reg,rc)",
/* 66 */	"reg: SUBP4(reg,rc)",
/* 67 */	"reg: SUBU4(reg,rc)",
/* 68 */	"reg: NEGI4(reg)",
/* 69 */	"reg: MULI4(reg,rc)",
/* 70 */	"reg: MULU4(reg,rc)",
/* 71 */	"reg: DIVI4(reg,rc)",
/* 72 */	"reg: DIVU4(reg,rc)",
/* 73 */	"reg: MODI4(reg,rc)",
/* 74 */	"reg: MODU4(reg,rc)",
/* 75 */	"reg: BANDI4(reg,rc)",
/* 76 */	"reg: BANDU4(reg,rc)",
/* 77 */	"reg: BORI4(reg,rc)",
/* 78 */	"reg: BORU4(reg,rc)",
/* 79 */	"reg: BXORI4(reg,rc)",
/* 80 */	"reg: BXORU4(reg,rc)",
/* 81 */	"reg: BCOMI4(reg)",
/* 82 */	"reg: BCOMU4(reg)",
/* 83 */	"rc5: CNSTI4",
/* 84 */	"rc5: reg",
/* 85 */	"reg: LSHI4(reg,rc5)",
/* 86 */	"reg: LSHU4(reg,rc5)",
/* 87 */	"reg: RSHI4(reg,rc5)",
/* 88 */	"reg: RSHU4(reg,rc5)",
/* 89 */	"reg: LOADI1(reg)",
/* 90 */	"reg: LOADI2(reg)",
/* 91 */	"reg: LOADI4(reg)",
/* 92 */	"reg: LOADP4(reg)",
/* 93 */	"reg: LOADU1(reg)",
/* 94 */	"reg: LOADU2(reg)",
/* 95 */	"reg: LOADU4(reg)",
/* 96 */	"reg: CVII4(reg)",
/* 97 */	"reg: CVUI4(reg)",
/* 98 */	"reg: CVUU4(reg)",
/* 99 */	"stmt: LABELV",
/* 100 */	"stmt: JUMPV(acon)",
/* 101 */	"stmt: JUMPV(reg)",
/* 102 */	"stmt: EQI4(reg,reg)",
/* 103 */	"stmt: EQU4(reg,reg)",
/* 104 */	"stmt: NEI4(reg,reg)",
/* 105 */	"stmt: NEU4(reg,reg)",
/* 106 */	"stmt: LEI4(reg,reg)",
/* 107 */	"stmt: LEU4(reg,reg)",
/* 108 */	"stmt: LTI4(reg,reg)",
/* 109 */	"stmt: LTU4(reg,reg)",
/* 110 */	"stmt: GEI4(reg,reg)",
/* 111 */	"stmt: GEU4(reg,reg)",
/* 112 */	"stmt: GTI4(reg,reg)",
/* 113 */	"stmt: GTU4(reg,reg)",
/* 114 */	"reg: CALLI4(ar)",
/* 115 */	"reg: CALLP4(ar)",
/* 116 */	"reg: CALLU4(ar)",
/* 117 */	"stmt: CALLV(ar)",
/* 118 */	"ar: ADDRGP4",
/* 119 */	"ar: reg",
/* 120 */	"ar: CNSTP4",
/* 121 */	"stmt: RETI4(reg)",
/* 122 */	"stmt: RETP4(reg)",
/* 123 */	"stmt: RETU4(reg)",
/* 124 */	"stmt: RETV(reg)",
/* 125 */	"stmt: ARGI4(reg)",
/* 126 */	"stmt: ARGP4(reg)",
/* 127 */	"stmt: ARGU4(reg)",
/* 128 */	"stmt: ARGB(INDIRB(reg))",
/* 129 */	"stmt: ASGNB(reg,INDIRB(reg))",
/* 130 */	"reg: INDIRF4(VREGP)",
/* 131 */	"stmt: ASGNF4(VREGP,reg)",
/* 132 */	"reg: INDIRF4(addr)",
/* 133 */	"stmt: ASGNF4(addr,reg)",
/* 134 */	"reg: ADDF4(reg,reg)",
/* 135 */	"reg: SUBF4(reg,reg)",
/* 136 */	"reg: MULF4(reg,reg)",
/* 137 */	"reg: DIVF4(reg,reg)",
/* 138 */	"reg: LOADF4(reg)",
/* 139 */	"reg: NEGF4(reg)",
/* 140 */	"reg: CVFF4(reg)",
/* 141 */	"reg: CVIF4(reg)",
/* 142 */	"reg: CVFI4(reg)",
/* 143 */	"stmt: EQF4(reg,reg)",
/* 144 */	"stmt: LEF4(reg,reg)",
/* 145 */	"stmt: LTF4(reg,reg)",
/* 146 */	"stmt: GEF4(reg,reg)",
/* 147 */	"stmt: GTF4(reg,reg)",
/* 148 */	"stmt: NEF4(reg,reg)",
/* 149 */	"reg: CALLF4(ar)",
/* 150 */	"stmt: RETF4(reg)",
/* 151 */	"stmt: ARGF4(reg)",
/* 152 */	"reg: INDIRF8(VREGP)",
/* 153 */	"stmt: ASGNF8(VREGP,reg)",
/* 154 */	"reg: INDIRF8(addr)",
/* 155 */	"stmt: ASGNF8(addr,reg)",
/* 156 */	"reg: ADDF8(reg,reg)",
/* 157 */	"reg: SUBF8(reg,reg)",
/* 158 */	"reg: MULF8(reg,reg)",
/* 159 */	"reg: DIVF8(reg,reg)",
/* 160 */	"reg: LOADF8(reg)",
/* 161 */	"reg: NEGF8(reg)",
/* 162 */	"reg: CVFF8(reg)",
/* 163 */	"reg: CVIF8(reg)",
/* 164 */	"reg: CVFI4(reg)",
/* 165 */	"stmt: EQF8(reg,reg)",
/* 166 */	"stmt: LEF8(reg,reg)",
/* 167 */	"stmt: LTF8(reg,reg)",
/* 168 */	"stmt: GEF8(reg,reg)",
/* 169 */	"stmt: GTF8(reg,reg)",
/* 170 */	"stmt: NEF8(reg,reg)",
/* 171 */	"reg: CALLF8(ar)",
/* 172 */	"stmt: RETF8(reg)",
/* 173 */	"stmt: ARGF8(reg)",
};

static short _decode_stmt[] = {
	0,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	22,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	117,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	131,
	133,
	143,
	144,
	145,
	146,
	147,
	148,
	150,
	151,
	153,
	155,
	165,
	166,
	167,
	168,
	169,
	170,
	172,
	173,
};

static short _decode_reg[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	114,
	115,
	116,
	130,
	132,
	134,
	135,
	136,
	137,
	138,
	139,
	140,
	141,
	142,
	149,
	152,
	154,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	171,
};

static short _decode_con[] = {
	0,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
};

static short _decode_acon[] = {
	0,
	23,
	24,
};

static short _decode_addr[] = {
	0,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
};

static short _decode_rc[] = {
	0,
	60,
	61,
};

static short _decode_rc5[] = {
	0,
	83,
	84,
};

static short _decode_ar[] = {
	0,
	118,
	119,
	120,
};

static int _rule(void *state, int goalnt) {
	if (goalnt < 1 || goalnt > 8)
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
	if (!state)
		return 0;
	switch (goalnt) {
	case _stmt_NT:	return _decode_stmt[((struct _state *)state)->rule._stmt];
	case _reg_NT:	return _decode_reg[((struct _state *)state)->rule._reg];
	case _con_NT:	return _decode_con[((struct _state *)state)->rule._con];
	case _acon_NT:	return _decode_acon[((struct _state *)state)->rule._acon];
	case _addr_NT:	return _decode_addr[((struct _state *)state)->rule._addr];
	case _rc_NT:	return _decode_rc[((struct _state *)state)->rule._rc];
	case _rc5_NT:	return _decode_rc5[((struct _state *)state)->rule._rc5];
	case _ar_NT:	return _decode_ar[((struct _state *)state)->rule._ar];
	default:
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
		return 0;
	}
}

static void _closure_reg(NODEPTR_TYPE, int);
static void _closure_con(NODEPTR_TYPE, int);
static void _closure_acon(NODEPTR_TYPE, int);
static void _closure_addr(NODEPTR_TYPE, int);

static void _closure_reg(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_ar_NT]) {
		p->cost[_ar_NT] = c + 0;
		p->rule._ar = 2;
	}
	if (c + 0 < p->cost[_rc5_NT]) {
		p->cost[_rc5_NT] = c + 0;
		p->rule._rc5 = 2;
	}
	if (c + 0 < p->cost[_rc_NT]) {
		p->cost[_rc_NT] = c + 0;
		p->rule._rc = 2;
	}
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 5;
		_closure_addr(a, c + 0);
	}
	if (c + 0 < p->cost[_stmt_NT]) {
		p->cost[_stmt_NT] = c + 0;
		p->rule._stmt = 8;
	}
}

static void _closure_con(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_rc_NT]) {
		p->cost[_rc_NT] = c + 0;
		p->rule._rc = 1;
	}
	if (c + 0 < p->cost[_acon_NT]) {
		p->cost[_acon_NT] = c + 0;
		p->rule._acon = 1;
		_closure_acon(a, c + 0);
	}
}

static void _closure_acon(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 4;
		_closure_addr(a, c + 0);
	}
}

static void _closure_addr(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 8;
		_closure_reg(a, c + 1);
	}
}

static void _label(NODEPTR_TYPE a) {
	int c;
	struct _state *p;

	if (!a)
		fatal("_label", "Null tree\n", 0);
	STATE_LABEL(a) = p = allocate(sizeof *p, FUNC);
	p->rule._stmt = 0;
	p->cost[1] =
	p->cost[2] =
	p->cost[3] =
	p->cost[4] =
	p->cost[5] =
	p->cost[6] =
	p->cost[7] =
	p->cost[8] =
		0x7fff;
	switch (OP_LABEL(a)) {
	case 41: /* ARGB */
		_label(LEFT_CHILD(a));
		if (	/* stmt: ARGB(INDIRB(reg)) */
			LEFT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 39;
			}
		}
		break;
	case 57: /* ASGNB */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNB(reg,INDIRB(reg)) */
			RIGHT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 40;
			}
		}
		break;
	case 73: /* INDIRB */
		_label(LEFT_CHILD(a));
		break;
	case 216: /* CALLV */
		_label(LEFT_CHILD(a));
		/* stmt: CALLV(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 31;
		}
		break;
	case 217: /* CALLB */
		break;
	case 233: /* LOADB */
		break;
	case 248: /* RETV */
		_label(LEFT_CHILD(a));
		/* stmt: RETV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 35;
		}
		break;
	case 584: /* JUMPV */
		_label(LEFT_CHILD(a));
		/* stmt: JUMPV(acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_acon_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 17;
		}
		/* stmt: JUMPV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 18;
		}
		break;
	case 600: /* LABELV */
		/* stmt: LABELV */
		if (0 + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = 0 + 0;
			p->rule._stmt = 16;
		}
		break;
	case 711: /* VREGP */
		break;
	case 1045: /* CNSTI1 */
		/* con: CNSTI1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 1;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI1 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 9;
			_closure_reg(a, c + 0);
		}
		break;
	case 1046: /* CNSTU1 */
		/* con: CNSTU1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 5;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU1 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 13;
			_closure_reg(a, c + 0);
		}
		break;
	case 1077: /* ASGNI1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 1;
			}
		}
		/* stmt: ASGNI1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 9;
		}
		break;
	case 1078: /* ASGNU1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 5;
			}
		}
		/* stmt: ASGNU1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 13;
		}
		break;
	case 1093: /* INDIRI1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 1;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 16;
			_closure_reg(a, c + 0);
		}
		break;
	case 1094: /* INDIRU1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 5;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 20;
			_closure_reg(a, c + 0);
		}
		break;
	case 1157: /* CVII1 */
		break;
	case 1158: /* CVIU1 */
		break;
	case 1205: /* CVUI1 */
		break;
	case 1206: /* CVUU1 */
		break;
	case 1253: /* LOADI1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 54;
			_closure_reg(a, c + 0);
		}
		break;
	case 1254: /* LOADU1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 58;
			_closure_reg(a, c + 0);
		}
		break;
	case 2069: /* CNSTI2 */
		/* con: CNSTI2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 2;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI2 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 10;
			_closure_reg(a, c + 0);
		}
		break;
	case 2070: /* CNSTU2 */
		/* con: CNSTU2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 6;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU2 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 14;
			_closure_reg(a, c + 0);
		}
		break;
	case 2101: /* ASGNI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 2;
			}
		}
		/* stmt: ASGNI2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 10;
		}
		break;
	case 2102: /* ASGNU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 6;
			}
		}
		/* stmt: ASGNU2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 14;
		}
		break;
	case 2117: /* INDIRI2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 2;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 17;
			_closure_reg(a, c + 0);
		}
		break;
	case 2118: /* INDIRU2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 6;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 21;
			_closure_reg(a, c + 0);
		}
		break;
	case 2181: /* CVII2 */
		break;
	case 2182: /* CVIU2 */
		break;
	case 2229: /* CVUI2 */
		break;
	case 2230: /* CVUU2 */
		break;
	case 2277: /* LOADI2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 55;
			_closure_reg(a, c + 0);
		}
		break;
	case 2278: /* LOADU2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 59;
			_closure_reg(a, c + 0);
		}
		break;
	case 4113: /* CNSTF4 */
		break;
	case 4117: /* CNSTI4 */
		/* con: CNSTI4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 3;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 11;
			_closure_reg(a, c + 0);
		}
		/* rc5: CNSTI4 */
		c = (range(a, 0, 31));
		if (c + 0 < p->cost[_rc5_NT]) {
			p->cost[_rc5_NT] = c + 0;
			p->rule._rc5 = 1;
		}
		break;
	case 4118: /* CNSTU4 */
		/* con: CNSTU4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 7;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 15;
			_closure_reg(a, c + 0);
		}
		break;
	case 4119: /* CNSTP4 */
		/* con: CNSTP4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 4;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTP4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 12;
			_closure_reg(a, c + 0);
		}
		/* ar: CNSTP4 */
		c = (range(a, 0, 0x03FFFFFF));
		if (c + 0 < p->cost[_ar_NT]) {
			p->cost[_ar_NT] = c + 0;
			p->rule._ar = 3;
		}
		break;
	case 4129: /* ARGF4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 50;
		}
		break;
	case 4133: /* ARGI4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 36;
		}
		break;
	case 4134: /* ARGU4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 38;
		}
		break;
	case 4135: /* ARGP4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 37;
		}
		break;
	case 4145: /* ASGNF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNF4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 41;
			}
		}
		/* stmt: ASGNF4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 42;
		}
		break;
	case 4149: /* ASGNI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 3;
			}
		}
		/* stmt: ASGNI4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 11;
		}
		break;
	case 4150: /* ASGNU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 7;
			}
		}
		/* stmt: ASGNU4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 15;
		}
		break;
	case 4151: /* ASGNP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNP4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 4;
			}
		}
		/* stmt: ASGNP4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 12;
		}
		break;
	case 4161: /* INDIRF4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRF4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 67;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRF4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 68;
			_closure_reg(a, c + 0);
		}
		break;
	case 4165: /* INDIRI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 3;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 18;
			_closure_reg(a, c + 0);
		}
		break;
	case 4166: /* INDIRU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 7;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 22;
			_closure_reg(a, c + 0);
		}
		break;
	case 4167: /* INDIRP4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRP4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 4;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRP4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 19;
			_closure_reg(a, c + 0);
		}
		break;
	case 4209: /* CVFF4 */
		_label(LEFT_CHILD(a));
		/* reg: CVFF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 75;
			_closure_reg(a, c + 0);
		}
		break;
	case 4213: /* CVFI4 */
		_label(LEFT_CHILD(a));
		/* reg: CVFI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((a->syms[0]->u.c.v.i == 4 ? 1 : LBURG_MAX));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 77;
			_closure_reg(a, c + 0);
		}
		/* reg: CVFI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((a->syms[0]->u.c.v.i == 8 ? 1 : LBURG_MAX));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 89;
			_closure_reg(a, c + 0);
		}
		break;
	case 4225: /* CVIF4 */
		_label(LEFT_CHILD(a));
		/* reg: CVIF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 76;
			_closure_reg(a, c + 0);
		}
		break;
	case 4229: /* CVII4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVII4(INDIRI1(addr)) */
			LEFT_CHILD(a)->op == 1093 /* INDIRI1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 23;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVII4(INDIRI2(addr)) */
			LEFT_CHILD(a)->op == 2117 /* INDIRI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 24;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVII4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 61;
			_closure_reg(a, c + 0);
		}
		break;
	case 4230: /* CVIU4 */
		break;
	case 4246: /* CVPU4 */
		break;
	case 4277: /* CVUI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUI4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 27;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUI4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 28;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 62;
			_closure_reg(a, c + 0);
		}
		break;
	case 4278: /* CVUU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUU4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 25;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUU4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 26;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 63;
			_closure_reg(a, c + 0);
		}
		break;
	case 4279: /* CVUP4 */
		break;
	case 4289: /* NEGF4 */
		_label(LEFT_CHILD(a));
		/* reg: NEGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 74;
			_closure_reg(a, c + 0);
		}
		break;
	case 4293: /* NEGI4 */
		_label(LEFT_CHILD(a));
		/* reg: NEGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 35;
			_closure_reg(a, c + 0);
		}
		break;
	case 4305: /* CALLF4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLF4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 78;
			_closure_reg(a, c + 0);
		}
		break;
	case 4309: /* CALLI4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLI4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 64;
			_closure_reg(a, c + 0);
		}
		break;
	case 4310: /* CALLU4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLU4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 66;
			_closure_reg(a, c + 0);
		}
		break;
	case 4311: /* CALLP4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLP4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 65;
			_closure_reg(a, c + 0);
		}
		break;
	case 4321: /* LOADF4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 73;
			_closure_reg(a, c + 0);
		}
		break;
	case 4325: /* LOADI4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 56;
			_closure_reg(a, c + 0);
		}
		break;
	case 4326: /* LOADU4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 60;
			_closure_reg(a, c + 0);
		}
		break;
	case 4327: /* LOADP4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 57;
			_closure_reg(a, c + 0);
		}
		break;
	case 4337: /* RETF4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 49;
		}
		break;
	case 4341: /* RETI4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 32;
		}
		break;
	case 4342: /* RETU4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 34;
		}
		break;
	case 4343: /* RETP4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 33;
		}
		break;
	case 4359: /* ADDRGP4 */
		/* acon: ADDRGP4 */
		if (0 + 0 < p->cost[_acon_NT]) {
			p->cost[_acon_NT] = 0 + 0;
			p->rule._acon = 2;
			_closure_acon(a, 0 + 0);
		}
		/* ar: ADDRGP4 */
		if (0 + 0 < p->cost[_ar_NT]) {
			p->cost[_ar_NT] = 0 + 0;
			p->rule._ar = 1;
		}
		break;
	case 4375: /* ADDRFP4 */
		/* addr: ADDRFP4 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 6;
			_closure_addr(a, 0 + 0);
		}
		break;
	case 4391: /* ADDRLP4 */
		/* addr: ADDRLP4 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 7;
			_closure_addr(a, 0 + 0);
		}
		break;
	case 4401: /* ADDF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 69;
			_closure_reg(a, c + 0);
		}
		break;
	case 4405: /* ADDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDI4(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 1;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 29;
			_closure_reg(a, c + 0);
		}
		break;
	case 4406: /* ADDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDU4(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 3;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 31;
			_closure_reg(a, c + 0);
		}
		break;
	case 4407: /* ADDP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDP4(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 2;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDP4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 30;
			_closure_reg(a, c + 0);
		}
		break;
	case 4417: /* SUBF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 70;
			_closure_reg(a, c + 0);
		}
		break;
	case 4421: /* SUBI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 32;
			_closure_reg(a, c + 0);
		}
		break;
	case 4422: /* SUBU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 34;
			_closure_reg(a, c + 0);
		}
		break;
	case 4423: /* SUBP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBP4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 33;
			_closure_reg(a, c + 0);
		}
		break;
	case 4437: /* LSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHI4(reg,rc5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 50;
			_closure_reg(a, c + 0);
		}
		break;
	case 4438: /* LSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHU4(reg,rc5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 51;
			_closure_reg(a, c + 0);
		}
		break;
	case 4453: /* MODI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 40;
			_closure_reg(a, c + 0);
		}
		break;
	case 4454: /* MODU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 41;
			_closure_reg(a, c + 0);
		}
		break;
	case 4469: /* RSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHI4(reg,rc5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 52;
			_closure_reg(a, c + 0);
		}
		break;
	case 4470: /* RSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHU4(reg,rc5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 53;
			_closure_reg(a, c + 0);
		}
		break;
	case 4485: /* BANDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 42;
			_closure_reg(a, c + 0);
		}
		break;
	case 4486: /* BANDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 43;
			_closure_reg(a, c + 0);
		}
		break;
	case 4501: /* BCOMI4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 48;
			_closure_reg(a, c + 0);
		}
		break;
	case 4502: /* BCOMU4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 49;
			_closure_reg(a, c + 0);
		}
		break;
	case 4517: /* BORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 44;
			_closure_reg(a, c + 0);
		}
		break;
	case 4518: /* BORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 45;
			_closure_reg(a, c + 0);
		}
		break;
	case 4533: /* BXORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 46;
			_closure_reg(a, c + 0);
		}
		break;
	case 4534: /* BXORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 47;
			_closure_reg(a, c + 0);
		}
		break;
	case 4545: /* DIVF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 72;
			_closure_reg(a, c + 0);
		}
		break;
	case 4549: /* DIVI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 38;
			_closure_reg(a, c + 0);
		}
		break;
	case 4550: /* DIVU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 39;
			_closure_reg(a, c + 0);
		}
		break;
	case 4561: /* MULF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 71;
			_closure_reg(a, c + 0);
		}
		break;
	case 4565: /* MULI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULI4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 36;
			_closure_reg(a, c + 0);
		}
		break;
	case 4566: /* MULU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULU4(reg,rc) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_rc_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 37;
			_closure_reg(a, c + 0);
		}
		break;
	case 4577: /* EQF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 43;
		}
		break;
	case 4581: /* EQI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 19;
		}
		break;
	case 4582: /* EQU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 20;
		}
		break;
	case 4593: /* GEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 46;
		}
		break;
	case 4597: /* GEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 27;
		}
		break;
	case 4598: /* GEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 28;
		}
		break;
	case 4609: /* GTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 47;
		}
		break;
	case 4613: /* GTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 29;
		}
		break;
	case 4614: /* GTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 30;
		}
		break;
	case 4625: /* LEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 44;
		}
		break;
	case 4629: /* LEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 23;
		}
		break;
	case 4630: /* LEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 24;
		}
		break;
	case 4641: /* LTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 45;
		}
		break;
	case 4645: /* LTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 25;
		}
		break;
	case 4646: /* LTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 26;
		}
		break;
	case 4657: /* NEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 48;
		}
		break;
	case 4661: /* NEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 21;
		}
		break;
	case 4662: /* NEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 22;
		}
		break;
	case 8209: /* CNSTF8 */
		break;
	case 8225: /* ARGF8 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 60;
		}
		break;
	case 8241: /* ASGNF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNF8(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 51;
			}
		}
		/* stmt: ASGNF8(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 52;
		}
		break;
	case 8257: /* INDIRF8 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRF8(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_rc_NT] == 0) {
					p->cost[_rc_NT] = 0;
					p->rule._rc = q->rule._rc;
				}
				if (q->cost[_rc5_NT] == 0) {
					p->cost[_rc5_NT] = 0;
					p->rule._rc5 = q->rule._rc5;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 79;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRF8(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 80;
			_closure_reg(a, c + 0);
		}
		break;
	case 8305: /* CVFF8 */
		_label(LEFT_CHILD(a));
		/* reg: CVFF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 87;
			_closure_reg(a, c + 0);
		}
		break;
	case 8321: /* CVIF8 */
		_label(LEFT_CHILD(a));
		/* reg: CVIF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 88;
			_closure_reg(a, c + 0);
		}
		break;
	case 8385: /* NEGF8 */
		_label(LEFT_CHILD(a));
		/* reg: NEGF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 86;
			_closure_reg(a, c + 0);
		}
		break;
	case 8401: /* CALLF8 */
		_label(LEFT_CHILD(a));
		/* reg: CALLF8(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 90;
			_closure_reg(a, c + 0);
		}
		break;
	case 8417: /* LOADF8 */
		_label(LEFT_CHILD(a));
		/* reg: LOADF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 85;
			_closure_reg(a, c + 0);
		}
		break;
	case 8433: /* RETF8 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 59;
		}
		break;
	case 8497: /* ADDF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 81;
			_closure_reg(a, c + 0);
		}
		break;
	case 8513: /* SUBF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 82;
			_closure_reg(a, c + 0);
		}
		break;
	case 8641: /* DIVF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 84;
			_closure_reg(a, c + 0);
		}
		break;
	case 8657: /* MULF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 83;
			_closure_reg(a, c + 0);
		}
		break;
	case 8673: /* EQF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 53;
		}
		break;
	case 8689: /* GEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 56;
		}
		break;
	case 8705: /* GTF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 57;
		}
		break;
	case 8721: /* LEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 54;
		}
		break;
	case 8737: /* LTF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 55;
		}
		break;
	case 8753: /* NEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF8(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 58;
		}
		break;
	default:
		fatal("_label", "Bad terminal %d\n", OP_LABEL(a));
	}
}

static void _kids(NODEPTR_TYPE p, int eruleno, NODEPTR_TYPE kids[]) {
	if (!p)
		fatal("_kids", "Null tree\n", 0);
	if (!kids)
		fatal("_kids", "Null kids\n", 0);
	switch (eruleno) {
	case 152: /* reg: INDIRF8(VREGP) */
	case 130: /* reg: INDIRF4(VREGP) */
	case 120: /* ar: CNSTP4 */
	case 118: /* ar: ADDRGP4 */
	case 99: /* stmt: LABELV */
	case 83: /* rc5: CNSTI4 */
	case 39: /* reg: CNSTU4 */
	case 38: /* reg: CNSTU2 */
	case 37: /* reg: CNSTU1 */
	case 36: /* reg: CNSTP4 */
	case 35: /* reg: CNSTI4 */
	case 34: /* reg: CNSTI2 */
	case 33: /* reg: CNSTI1 */
	case 31: /* addr: ADDRLP4 */
	case 30: /* addr: ADDRFP4 */
	case 24: /* acon: ADDRGP4 */
	case 21: /* con: CNSTU4 */
	case 20: /* con: CNSTU2 */
	case 19: /* con: CNSTU1 */
	case 18: /* con: CNSTP4 */
	case 17: /* con: CNSTI4 */
	case 16: /* con: CNSTI2 */
	case 15: /* con: CNSTI1 */
	case 7: /* reg: INDIRU4(VREGP) */
	case 6: /* reg: INDIRU2(VREGP) */
	case 5: /* reg: INDIRU1(VREGP) */
	case 4: /* reg: INDIRP4(VREGP) */
	case 3: /* reg: INDIRI4(VREGP) */
	case 2: /* reg: INDIRI2(VREGP) */
	case 1: /* reg: INDIRI1(VREGP) */
		break;
	case 153: /* stmt: ASGNF8(VREGP,reg) */
	case 131: /* stmt: ASGNF4(VREGP,reg) */
	case 14: /* stmt: ASGNU4(VREGP,reg) */
	case 13: /* stmt: ASGNU2(VREGP,reg) */
	case 12: /* stmt: ASGNU1(VREGP,reg) */
	case 11: /* stmt: ASGNP4(VREGP,reg) */
	case 10: /* stmt: ASGNI4(VREGP,reg) */
	case 9: /* stmt: ASGNI2(VREGP,reg) */
	case 8: /* stmt: ASGNI1(VREGP,reg) */
		kids[0] = RIGHT_CHILD(p);
		break;
	case 119: /* ar: reg */
	case 84: /* rc5: reg */
	case 61: /* rc: reg */
	case 60: /* rc: con */
	case 32: /* reg: addr */
	case 29: /* addr: reg */
	case 28: /* addr: acon */
	case 23: /* acon: con */
	case 22: /* stmt: reg */
		kids[0] = p;
		break;
	case 170: /* stmt: NEF8(reg,reg) */
	case 169: /* stmt: GTF8(reg,reg) */
	case 168: /* stmt: GEF8(reg,reg) */
	case 167: /* stmt: LTF8(reg,reg) */
	case 166: /* stmt: LEF8(reg,reg) */
	case 165: /* stmt: EQF8(reg,reg) */
	case 159: /* reg: DIVF8(reg,reg) */
	case 158: /* reg: MULF8(reg,reg) */
	case 157: /* reg: SUBF8(reg,reg) */
	case 156: /* reg: ADDF8(reg,reg) */
	case 155: /* stmt: ASGNF8(addr,reg) */
	case 148: /* stmt: NEF4(reg,reg) */
	case 147: /* stmt: GTF4(reg,reg) */
	case 146: /* stmt: GEF4(reg,reg) */
	case 145: /* stmt: LTF4(reg,reg) */
	case 144: /* stmt: LEF4(reg,reg) */
	case 143: /* stmt: EQF4(reg,reg) */
	case 137: /* reg: DIVF4(reg,reg) */
	case 136: /* reg: MULF4(reg,reg) */
	case 135: /* reg: SUBF4(reg,reg) */
	case 134: /* reg: ADDF4(reg,reg) */
	case 133: /* stmt: ASGNF4(addr,reg) */
	case 113: /* stmt: GTU4(reg,reg) */
	case 112: /* stmt: GTI4(reg,reg) */
	case 111: /* stmt: GEU4(reg,reg) */
	case 110: /* stmt: GEI4(reg,reg) */
	case 109: /* stmt: LTU4(reg,reg) */
	case 108: /* stmt: LTI4(reg,reg) */
	case 107: /* stmt: LEU4(reg,reg) */
	case 106: /* stmt: LEI4(reg,reg) */
	case 105: /* stmt: NEU4(reg,reg) */
	case 104: /* stmt: NEI4(reg,reg) */
	case 103: /* stmt: EQU4(reg,reg) */
	case 102: /* stmt: EQI4(reg,reg) */
	case 88: /* reg: RSHU4(reg,rc5) */
	case 87: /* reg: RSHI4(reg,rc5) */
	case 86: /* reg: LSHU4(reg,rc5) */
	case 85: /* reg: LSHI4(reg,rc5) */
	case 80: /* reg: BXORU4(reg,rc) */
	case 79: /* reg: BXORI4(reg,rc) */
	case 78: /* reg: BORU4(reg,rc) */
	case 77: /* reg: BORI4(reg,rc) */
	case 76: /* reg: BANDU4(reg,rc) */
	case 75: /* reg: BANDI4(reg,rc) */
	case 74: /* reg: MODU4(reg,rc) */
	case 73: /* reg: MODI4(reg,rc) */
	case 72: /* reg: DIVU4(reg,rc) */
	case 71: /* reg: DIVI4(reg,rc) */
	case 70: /* reg: MULU4(reg,rc) */
	case 69: /* reg: MULI4(reg,rc) */
	case 67: /* reg: SUBU4(reg,rc) */
	case 66: /* reg: SUBP4(reg,rc) */
	case 65: /* reg: SUBI4(reg,rc) */
	case 64: /* reg: ADDU4(reg,rc) */
	case 63: /* reg: ADDP4(reg,rc) */
	case 62: /* reg: ADDI4(reg,rc) */
	case 46: /* stmt: ASGNU4(addr,reg) */
	case 45: /* stmt: ASGNU2(addr,reg) */
	case 44: /* stmt: ASGNU1(addr,reg) */
	case 43: /* stmt: ASGNP4(addr,reg) */
	case 42: /* stmt: ASGNI4(addr,reg) */
	case 41: /* stmt: ASGNI2(addr,reg) */
	case 40: /* stmt: ASGNI1(addr,reg) */
	case 27: /* addr: ADDU4(reg,acon) */
	case 26: /* addr: ADDP4(reg,acon) */
	case 25: /* addr: ADDI4(reg,acon) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = RIGHT_CHILD(p);
		break;
	case 173: /* stmt: ARGF8(reg) */
	case 172: /* stmt: RETF8(reg) */
	case 171: /* reg: CALLF8(ar) */
	case 164: /* reg: CVFI4(reg) */
	case 163: /* reg: CVIF8(reg) */
	case 162: /* reg: CVFF8(reg) */
	case 161: /* reg: NEGF8(reg) */
	case 160: /* reg: LOADF8(reg) */
	case 154: /* reg: INDIRF8(addr) */
	case 151: /* stmt: ARGF4(reg) */
	case 150: /* stmt: RETF4(reg) */
	case 149: /* reg: CALLF4(ar) */
	case 142: /* reg: CVFI4(reg) */
	case 141: /* reg: CVIF4(reg) */
	case 140: /* reg: CVFF4(reg) */
	case 139: /* reg: NEGF4(reg) */
	case 138: /* reg: LOADF4(reg) */
	case 132: /* reg: INDIRF4(addr) */
	case 127: /* stmt: ARGU4(reg) */
	case 126: /* stmt: ARGP4(reg) */
	case 125: /* stmt: ARGI4(reg) */
	case 124: /* stmt: RETV(reg) */
	case 123: /* stmt: RETU4(reg) */
	case 122: /* stmt: RETP4(reg) */
	case 121: /* stmt: RETI4(reg) */
	case 117: /* stmt: CALLV(ar) */
	case 116: /* reg: CALLU4(ar) */
	case 115: /* reg: CALLP4(ar) */
	case 114: /* reg: CALLI4(ar) */
	case 101: /* stmt: JUMPV(reg) */
	case 100: /* stmt: JUMPV(acon) */
	case 98: /* reg: CVUU4(reg) */
	case 97: /* reg: CVUI4(reg) */
	case 96: /* reg: CVII4(reg) */
	case 95: /* reg: LOADU4(reg) */
	case 94: /* reg: LOADU2(reg) */
	case 93: /* reg: LOADU1(reg) */
	case 92: /* reg: LOADP4(reg) */
	case 91: /* reg: LOADI4(reg) */
	case 90: /* reg: LOADI2(reg) */
	case 89: /* reg: LOADI1(reg) */
	case 82: /* reg: BCOMU4(reg) */
	case 81: /* reg: BCOMI4(reg) */
	case 68: /* reg: NEGI4(reg) */
	case 53: /* reg: INDIRU4(addr) */
	case 52: /* reg: INDIRU2(addr) */
	case 51: /* reg: INDIRU1(addr) */
	case 50: /* reg: INDIRP4(addr) */
	case 49: /* reg: INDIRI4(addr) */
	case 48: /* reg: INDIRI2(addr) */
	case 47: /* reg: INDIRI1(addr) */
		kids[0] = LEFT_CHILD(p);
		break;
	case 128: /* stmt: ARGB(INDIRB(reg)) */
	case 59: /* reg: CVUI4(INDIRU2(addr)) */
	case 58: /* reg: CVUI4(INDIRU1(addr)) */
	case 57: /* reg: CVUU4(INDIRU2(addr)) */
	case 56: /* reg: CVUU4(INDIRU1(addr)) */
	case 55: /* reg: CVII4(INDIRI2(addr)) */
	case 54: /* reg: CVII4(INDIRI1(addr)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		break;
	case 129: /* stmt: ASGNB(reg,INDIRB(reg)) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	default:
		fatal("_kids", "Bad rule number %d\n", eruleno);
	}
}



static void address(Symbol s1, Symbol s2, long n) {
  if (s2->scope == GLOBAL ||
      s2->sclass == STATIC ||
      s2->sclass == EXTERN) {
    s1->x.name = stringf("%s%s%D", s2->x.name, n >= 0 ? "+" : "", n);
  } else {
    assert(n >= INT_MIN && n <= INT_MAX);
    s1->x.offset = s2->x.offset + n;
    s1->x.name = stringd(s1->x.offset);
  }
}


static void defaddress(Symbol s) {
  print("\t.word\t%s\n", s->x.name);
}


static void defconst(int suffix, int size, Value v) {
  float f;
  double d;
  unsigned *p;

  if (suffix == F && size == 4) {
    f = v.d;
    print("\t.word\t0x%x\n", * (unsigned *) &f);
  } else
  if (suffix == F && size == 8) {
    d = v.d;
    p = (unsigned *) &d;
    print("\t.word\t0x%x\n", p[swap]);
    print("\t.word\t0x%x\n", p[1 - swap]);
  } else
  if (suffix == P) {
    print("\t.word\t0x%x\n", (unsigned) v.p);
  } else
  if (size == 1) {
    print("\t.byte\t0x%x\n",
          (unsigned) ((unsigned char) (suffix == I ? v.i : v.u)));
  } else
  if (size == 2) {
    print("\t.half\t0x%x\n",
          (unsigned) ((unsigned short) (suffix == I ? v.i : v.u)));
  } else
  if (size == 4) {
    print("\t.word\t0x%x\n", (unsigned) (suffix == I ? v.i : v.u));
  }
}


static void defstring(int n, char *str) {
  char *s;

  for (s = str; s < str + n; s++) {
    print("\t.byte\t0x%x\n", (*s) & 0xFF);
  }
}


static void defsymbol(Symbol s) {
  if (s->scope >= LOCAL && s->sclass == STATIC) {
    s->x.name = stringf("L.%d", genlabel(1));
  } else
  if (s->generated) {
    s->x.name = stringf("L.%s", s->name);
  } else {
    assert(s->scope != CONSTANTS || isint(s->type) || isptr(s->type));
    s->x.name = s->name;
  }
}


static void export(Symbol s) {
  print("\t.export\t%s\n", s->name);
}


static int bitcount(unsigned mask) {
  unsigned i, n;

  n = 0;
  for (i = 1; i != 0; i <<= 1) {
    if (mask & i) {
      n++;
    }
  }
  return n;
}


static Symbol argreg(int argno, int offset, int ty, int sz, int ty0) {
  assert((offset & 3) == 0);
  if (offset > 12) {
    return NULL;
  }
  return ireg[(offset / 4) + 4];
}


static void function(Symbol f, Symbol caller[], Symbol callee[], int ncalls) {
  int i;
  Symbol p, q;
  Symbol r;
  int sizeisave;
  int saved;
  Symbol argregs[4];

  usedmask[0] = usedmask[1] = 0;
  freemask[0] = freemask[1] = ~((unsigned) 0);
  offset = 0;
  maxoffset = 0;
  maxargoffset = 0;
  for (i = 0; callee[i] != NULL; i++) {
    p = callee[i];
    q = caller[i];
    assert(q != NULL);
    offset = roundup(offset, q->type->align);
    p->x.offset = q->x.offset = offset;
    p->x.name = q->x.name = stringd(offset);
    r = argreg(i, offset, optype(ttob(q->type)),
               q->type->size, optype(ttob(caller[0]->type)));
    if (i < 4) {
      argregs[i] = r;
    }
    offset = roundup(offset + q->type->size, 4);
    if (variadic(f->type)) {
      p->sclass = AUTO;
    } else
    if (r != NULL && ncalls == 0 && !isstruct(q->type) &&
        !p->addressed && !(isfloat(q->type) && r->x.regnode->set == IREG)) {
      p->sclass = q->sclass = REGISTER;
      askregvar(p, r);
      assert(p->x.regnode && p->x.regnode->vbl == p);
      q->x = p->x;
      q->type = p->type;
    } else
    if (askregvar(p, rmap(ttob(p->type))) &&
        r != NULL && (isint(p->type) || p->type == q->type)) {
      assert(q->sclass != REGISTER);
      p->sclass = q->sclass = REGISTER;
      q->type = p->type;
    }
  }
  assert(caller[i] == NULL);
  offset = 0;
  gencode(caller, callee);
  if (ncalls != 0) {
    usedmask[IREG] |= ((unsigned) 1) << 31;
  }
  usedmask[IREG] &= 0x80FF0000;
  usedmask[FREG] &= 0xFFF00000;
  maxargoffset = roundup(maxargoffset, 4);
  if (ncalls != 0 && maxargoffset < 16) {
    maxargoffset = 16;
  }
  sizeisave = 4 * bitcount(usedmask[IREG]);
  framesize = roundup(maxargoffset + sizeisave + maxoffset, 16);
  segment(CODE);
  print("\t.align\t4\n");
  print("%s:\n", f->x.name);
  if (framesize > 0) {
    print("\tsub\t$29,$29,%d\n", framesize);
  }
  saved = maxargoffset;
  for (i = 16; i < 32; i++) {
    if (usedmask[IREG] & (1 << i)) {
      print("\tstw\t$%d,$29,%d\n", i, saved);
      saved += 4;
    }
  }
  for (i = 0; i < 4 && callee[i] != NULL; i++) {
    r = argregs[i];
    if (r && r->x.regnode != callee[i]->x.regnode) {
      Symbol out = callee[i];
      Symbol in = caller[i];
      int rn = r->x.regnode->number;
      int rs = r->x.regnode->set;
      int tyin = ttob(in->type);
      assert(out && in && r && r->x.regnode);
      assert(out->sclass != REGISTER || out->x.regnode);
      if (out->sclass == REGISTER &&
          (isint(out->type) || out->type == in->type)) {
        int outn = out->x.regnode->number;
        print("\tadd\t$%d,$0,$%d\n", outn, rn);
      } else {
        int off = in->x.offset + framesize;
        int n = (in->type->size + 3) / 4;
        int i;
        for (i = rn; i < rn + n && i <= 7; i++) {
          print("\tstw\t$%d,$29,%d\n", i, off + (i - rn) * 4);
        }
      }
    }
  }
  if (variadic(f->type) && callee[i - 1] != NULL) {
    i = callee[i - 1]->x.offset + callee[i - 1]->type->size;
    for (i = roundup(i, 4)/4; i <= 3; i++) {
      print("\tstw\t$%d,$29,%d\n", i + 4, framesize + 4 * i);
    }
  }
  emitcode();
  saved = maxargoffset;
  for (i = 16; i < 32; i++) {
    if (usedmask[IREG] & (1 << i)) {
      print("\tldw\t$%d,$29,%d\n", i, saved);
      saved += 4;
    }
  }
  if (framesize > 0) {
    print("\tadd\t$29,$29,%d\n", framesize);
  }
  print("\tjr\t$31\n");
  print("\n");
}


static void global(Symbol s) {
  if (s->type->align != 0) {
    print("\t.align\t%d\n", s->type->align);
  } else {
    print("\t.align\t%d\n", 4);
  }
  print("%s:\n", s->x.name);
}


static void import(Symbol s) {
  print("\t.import\t%s\n", s->name);
}


static void local(Symbol s) {
  if (askregvar(s, rmap(ttob(s->type))) == 0) {
    mkauto(s);
  }
}


static void setSwap(void) {
  union {
    char c;
    int i;
  } u;

  u.i = 0;
  u.c = 1;
  swap = ((u.i == 1) != IR->little_endian);
}


static void progbeg(int argc, char *argv[]) {
  int i;

  setSwap();
  segment(CODE);
  parseflags(argc, argv);
  for (i = 0; i < 32; i++) {
    ireg[i] = mkreg("%d", i, 1, IREG);
  }
  iregw = mkwildcard(ireg);
  for (i = 0; i < 32; i += 2) {
    freg2[i] = mkreg("%d", i, 3, FREG);
  }
  freg2w = mkwildcard(freg2);
  tmask[IREG] = INTTMP;
  vmask[IREG] = INTVAR;
  tmask[FREG] = FLTTMP;
  vmask[FREG] = FLTVAR;
  blkreg = mkreg("8", 8, 7, IREG);
}


static void progend(void) {
}


static void segment(int n) {
  static int currSeg = -1;
  int newSeg;

  switch (n) {
    case CODE:
      newSeg = CODE;
      break;
    case BSS:
      newSeg = BSS;
      break;
    case DATA:
      newSeg = DATA;
      break;
    case LIT:
      newSeg = DATA;
      break;
  }
  if (currSeg == newSeg) {
    return;
  }
  switch (newSeg) {
    case CODE:
      print("\t.code\n");
      break;
    case BSS:
      print("\t.bss\n");
      break;
    case DATA:
      print("\t.data\n");
      break;
  }
  currSeg = newSeg;
}


static void space(int n) {
  print("\t.space\t%d\n", n);
}


static Symbol rmap(int opk) {
  switch (optype(opk)) {
    case I:
    case U:
    case P:
    case B:
      return iregw;
    case F:
      return freg2w;
    default:
      return 0;
  }
}


static void blkfetch(int size, int off, int reg, int tmp) {
  assert(size == 1 || size == 2 || size == 4);
  if (size == 1) {
    print("\tldbu\t$%d,$%d,%d\n", tmp, reg, off);
  } else
  if (size == 2) {
    print("\tldhu\t$%d,$%d,%d\n", tmp, reg, off);
  } else
  if (size == 4) {
    print("\tldw\t$%d,$%d,%d\n", tmp, reg, off);
  }
}


static void blkstore(int size, int off, int reg, int tmp) {
  assert(size == 1 || size == 2 || size == 4);
  if (size == 1) {
    print("\tstb\t$%d,$%d,%d\n", tmp, reg, off);
  } else
  if (size == 2) {
    print("\tsth\t$%d,$%d,%d\n", tmp, reg, off);
  } else
  if (size == 4) {
    print("\tstw\t$%d,$%d,%d\n", tmp, reg, off);
  }
}


static void blkloop(int dreg, int doff,
                    int sreg, int soff,
                    int size, int tmps[]) {
  int label;

  label = genlabel(1);
  print("\tadd\t$%d,$%d,%d\n", sreg, sreg, size & ~7);
  print("\tadd\t$%d,$%d,%d\n", tmps[2], dreg, size & ~7);
  blkcopy(tmps[2], doff, sreg, soff, size & 7, tmps);
  print("L.%d:\n", label);
  print("\tsub\t$%d,$%d,%d\n", sreg, sreg, 8);
  print("\tsub\t$%d,$%d,%d\n", tmps[2], tmps[2], 8);
  blkcopy(tmps[2], doff, sreg, soff, 8, tmps);
  print("\tbltu\t$%d,$%d,L.%d\n", dreg, tmps[2], label);
}


static void emit2(Node p) {
  static int ty0;
  int ty, sz;
  Symbol q;
  int src;
  int dst, n;

  switch (specific(p->op)) {
    case ARG+I:
    case ARG+P:
    case ARG+U:
      ty = optype(p->op);
      sz = opsize(p->op);
      if (p->x.argno == 0) {
        ty0 = ty;
      }
      q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, sz, ty0);
      src = getregnum(p->x.kids[0]);
      if (q == NULL) {
        print("\tstw\t$%d,$29,%d\n", src, p->syms[2]->u.c.v.i);
      }
      break;
    case ASGN+B:
      dalign = p->syms[1]->u.c.v.i;
      salign = p->syms[1]->u.c.v.i;
      blkcopy(getregnum(p->x.kids[0]), 0,
              getregnum(p->x.kids[1]), 0,
              p->syms[0]->u.c.v.i, tmpregs);
      break;
    case ARG+B:
      dalign = 4;
      salign = p->syms[1]->u.c.v.i;
      blkcopy(29, p->syms[2]->u.c.v.i,
              getregnum(p->x.kids[0]), 0,
              p->syms[0]->u.c.v.i, tmpregs);
      n = p->syms[2]->u.c.v.i + p->syms[0]->u.c.v.i;
      dst = p->syms[2]->u.c.v.i;
      for (; dst <= 12 && dst < n; dst += 4) {
        print("\tldw\t$%d,$29,%d\n", (dst / 4) + 4, dst);
      }
      break;
  }
}


static void doarg(Node p) {
  static int argno;
  int align;
  int size;
  int offset;

  if (argoffset == 0) {
    argno = 0;
  }
  p->x.argno = argno++;
  align = p->syms[1]->u.c.v.i;
  if (align < 4) {
    align = 4;
  }
  size = p->syms[0]->u.c.v.i;
  offset = mkactual(align, size);
  p->syms[2] = intconst(offset);
}


static void target(Node p) {
  static int ty0;
  int ty;
  Symbol q;

  assert(p);
  switch (specific(p->op)) {
    case CNST+I:
    case CNST+P:
    case CNST+U:
      if (range(p, 0, 0) == 0) {
        setreg(p, ireg[0]);
        p->x.registered = 1;
      }
      break;
    case CALL+I:
    case CALL+P:
    case CALL+U:
      rtarget(p, 0, ireg[25]);
      setreg(p, ireg[2]);
      break;
    case CALL+F:
      rtarget(p, 0, ireg[25]);
      setreg(p, freg2[0]);
      break;
    case CALL+V:
      rtarget(p, 0, ireg[25]);
      break;
    case RET+I:
    case RET+P:
    case RET+U:
      rtarget(p, 0, ireg[2]);
      break;
    case RET+F:
      rtarget(p, 0, freg2[0]);
      break;
    case ARG+I:
    case ARG+P:
    case ARG+U:
    case ARG+F:
      ty = optype(p->op);
      q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, opsize(p->op), ty0);
      if (p->x.argno == 0) {
        ty0 = ty;
      }
      if (q && !(ty == F && q->x.regnode->set == IREG)) {
        rtarget(p, 0, q);
      }
      break;
    case ASGN+B:
      rtarget(p->kids[1], 0, blkreg);
      break;
    case ARG+B:
      rtarget(p->kids[0], 0, blkreg);
      break;
  }
}


static void clobber(Node p) {
  assert(p);
  switch (specific(p->op)) {
    case CALL+F:
      spill(INTTMP | INTRET, IREG, p);
      spill(FLTTMP, FREG, p);
      break;
    case CALL+I:
    case CALL+P:
    case CALL+U:
      spill(INTTMP, IREG, p);
      spill(FLTTMP | FLTRET, FREG, p);
      break;
    case CALL+V:
      spill(INTTMP | INTRET, IREG, p);
      spill(FLTTMP | FLTRET, FREG, p);
      break;
  }
}


Interface eco32IR = {
  1, 1, 0,  /* char */
  2, 2, 0,  /* short */
  4, 4, 0,  /* int */
  4, 4, 0,  /* long */
  4, 4, 0,  /* long long */
  4, 4, 1,  /* float */
  8, 4, 1,  /* double */
  8, 4, 1,  /* long double */
  4, 4, 0,  /* T * */
  0, 1, 0,  /* struct */
  0,        /* little_endian */
  0,        /* mulops_calls */
  0,        /* wants_callb */
  1,        /* wants_argb */
  1,        /* left_to_right */
  0,        /* wants_dag */
  0,        /* unsigned_char */
  address,
  blockbeg,
  blockend,
  defaddress,
  defconst,
  defstring,
  defsymbol,
  emit,
  export,
  function,
  gen,
  global,
  import,
  local,
  progbeg,
  progend,
  segment,
  space,
  0, 0, 0, 0, 0, 0, 0,
  {
    1,      /* max_unaligned_load */
    rmap,
    blkfetch, blkstore, blkloop,
    _label,
    _rule,
    _nts,
    _kids,
    _string,
    _templates,
    _isinstruction,
    _ntname,
    emit2,
    doarg,
    target,
    clobber
  }
};
