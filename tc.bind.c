/* $Header$ */
/*
 * tc.bind.c: Key binding functions
 */
/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "sh.h"

RCSID("$Id$")

#include "ed.h"
#include "ed.defns.h"

#ifdef OBSOLETE
static	int    tocontrol	(int);
static	char  *unparsekey	(int);
static	KEYCMD getkeycmd	(Char **);
static	int    parsekey		(Char **);
static	void   pkeys		(int, int);
#endif /* OBSOLETE */

static	void   printkey		(const KEYCMD *, CStr *);
static	KEYCMD parsecmd		(Char *);
static  void   bad_spec		(const Char *);
static	CStr  *parsestring	(const Char *, CStr *);
static	CStr  *parsebind	(const Char *, CStr *);
static	void   print_all_keys	(void);
static	void   printkeys	(KEYCMD *, int, int);
static	void   bindkey_usage	(void);
static	void   list_functions	(void);

extern int MapsAreInited;




/*ARGSUSED*/
void
dobindkey(Char **v, struct command *c)
{
    KEYCMD *map;
    int     ntype, no, removeb, key, bindk;
    Char   *par;
    Char    p;
    KEYCMD  cmd;
    CStr    in;
    CStr    out;
    uChar   ch;

    USE(c);
    if (!MapsAreInited)
	ed_InitMaps();

    map = CcKeyMap;
    ntype = XK_CMD;
    key = removeb = bindk = 0;
    for (no = 1, par = v[no]; 
	 par != NULL && (*par++ & CHAR) == '-'; no++, par = v[no]) {
	if ((p = (*par & CHAR)) == '-') {
	    no++;
	    break;
	}
	else 
	    switch (p) {
	    case 'b':
		bindk = 1;
		break;
	    case 'k':
		key = 1;
		break;
	    case 'a':
		map = CcAltMap;
		break;
	    case 's':
		ntype = XK_STR;
		break;
	    case 'c':
		ntype = XK_EXE;
		break;
	    case 'r':
		removeb = 1;
		break;
	    case 'v':
		ed_InitVIMaps();
		return;
	    case 'e':
		ed_InitEmacsMaps();
		return;
	    case 'd':
#ifdef VIDEFAULT
		ed_InitVIMaps();
#else /* EMACSDEFAULT */
		ed_InitEmacsMaps();
#endif /* VIDEFAULT */
		return;
	    case 'l':
		list_functions();
		return;
	    default:
		bindkey_usage();
		return;
	    }
    }

    if (!v[no]) {
	print_all_keys();
	return;
    }

    if (key) {
	if (!IsArrowKey(v[no]))
	    xprintf(CGETS(20, 1, "Invalid key name `%S'\n"), v[no]);
	in.buf = Strsave(v[no++]);
	in.len = Strlen(in.buf);
    }
    else {
	if (bindk) {
	    if (parsebind(v[no++], &in) == NULL)
		return;
	}
	else {
	    if (parsestring(v[no++], &in) == NULL)
		return;
	}
    }

#ifndef WINNT_NATIVE
    if (in.buf[0] > 0xFF) {
	bad_spec(in.buf);
	xfree(in.buf);
	return;
    }
#endif
    ch = (uChar) in.buf[0];

    if (removeb) {
	if (key)
	    (void) ClearArrowKeys(&in);
	else if (in.len > 1) {
	    (void) DeleteXkey(&in);
	}
	else if (map[ch] == F_XKEY) {
	    (void) DeleteXkey(&in);
	    map[ch] = F_UNASSIGNED;
	}
	else {
	    map[ch] = F_UNASSIGNED;
	}
	xfree(in.buf);
	return;
    }
    if (!v[no]) {
	if (key)
	    PrintArrowKeys(&in);
	else
	    printkey(map, &in);
	xfree(in.buf);
	return;
    }
    if (v[no + 1]) {
	bindkey_usage();
	xfree(in.buf);
	return;
    }
    switch (ntype) {
    case XK_STR:
    case XK_EXE:
	if (parsestring(v[no], &out) == NULL) {
	    xfree(in.buf);
	    return;
	}
	if (key) {
	    if (SetArrowKeys(&in, XmapStr(&out), ntype) == -1)
		xprintf(CGETS(20, 2, "Bad key name: %S\n"), in.buf);
	}
	else
	    AddXkey(&in, XmapStr(&out), ntype);
	xfree(out.buf);
	map[ch] = F_XKEY;
	break;
    case XK_CMD:
	if ((cmd = parsecmd(v[no])) == 0) {
	    xfree(in.buf);
	    return;
	}
	if (key)
	    (void) SetArrowKeys(&in, XmapCmd((int) cmd), ntype);
	else {
	    if (in.len > 1) {
		AddXkey(&in, XmapCmd((int) cmd), ntype);
		map[ch] = F_XKEY;
	    }
	    else {
		ClearXkey(map, &in);
		map[ch] = cmd;
	    }
	}
	break;
    default:
	abort();
	break;
    }
    xfree(in.buf);
    if (key)
	BindArrowKeys();
}

static void
printkey(const KEYCMD *map, CStr *in)
{
    struct KeyFuncs *fp;

    if (in->len < 2) {
	unsigned char *unparsed;

	unparsed = unparsestring(in, STRQQ);
	for (fp = FuncNames; fp->name; fp++) {
	    if (fp->func == map[(uChar) *(in->buf)]) {
		xprintf("%s\t->\t%s\n", unparsed, fp->name);
	    }
	}
	xfree(unparsed);
    }
    else
	PrintXkey(in);
}

static  KEYCMD
parsecmd(Char *str)
{
    struct KeyFuncs *fp;

    for (fp = FuncNames; fp->name; fp++) {
	if (strcmp(short2str(str), fp->name) == 0) {
	    return (KEYCMD) fp->func;
	}
    }
    xprintf(CGETS(20, 3, "Bad command name: %S\n"), str);
    return 0;
}


static void
bad_spec(const Char *str)
{
    xprintf(CGETS(20, 4, "Bad key spec %S\n"), str);
}

static CStr *
parsebind(const Char *s, CStr *str)
{
    struct Strbuf b = Strbuf_INIT;

    if (Iscntrl(*s)) {
	Strbuf_append1(&b, *s);
	goto end;
    }

    switch (*s) {
    case '^':
	s++;
#ifdef IS_ASCII
	Strbuf_append1(&b, (*s == '?') ? '\177' : ((*s & CHAR) & 0237));
#else
	Strbuf_append1(&b, (*s == '?') ? CTL_ESC('\177')
		       : _toebcdic[_toascii[*s & CHAR] & 0237]);
#endif
	break;

    case 'F':
    case 'M':
    case 'X':
    case 'C':
#ifdef WINNT_NATIVE
    case 'N':
#endif /* WINNT_NATIVE */
	if (s[1] != '-' || s[2] == '\0')
	    goto bad_spec;
	s += 2;
	switch (s[-2]) {
	case 'F': case 'f':	/* Turn into ^[str */
	    Strbuf_append1(&b, CTL_ESC('\033'));
	    Strbuf_append(&b, s);
	    break;

	case 'C': case 'c':	/* Turn into ^c */
#ifdef IS_ASCII
	    Strbuf_append1(&b, (*s == '?') ? '\177' : ((*s & CHAR) & 0237));
#else
	    Strbuf_append1(&b, (*s == '?') ? CTL_ESC('\177')
			   : _toebcdic[_toascii[*s & CHAR] & 0237]);
#endif
	    break;

	case 'X' : case 'x':	/* Turn into ^Xc */
#ifdef IS_ASCII
	    Strbuf_append1(&b, 'X' & 0237);
#else
	    Strbuf_append1(&b, _toebcdic[_toascii['X'] & 0237]);
#endif
	    Strbuf_append1(&b, *s);
	    break;

	case 'M' : case 'm':	/* Turn into 0x80|c */
	    if (!NoNLSRebind) {
		Strbuf_append1(&b, CTL_ESC('\033'));
	    	Strbuf_append1(&b, *s);
	    } else {
#ifdef IS_ASCII
		Strbuf_append1(&b, *s | 0x80);
#else
		Strbuf_append1(&b, _toebcdic[_toascii[*s] | 0x80]);
#endif
	    }
	    break;
#ifdef WINNT_NATIVE
	case 'N' : case 'n':	/* NT */
		{
			Char bnt;

			bnt = nt_translate_bindkey(s);
			if (bnt != 0)
			        Strbuf_append1(&b, bnt);
			else
				bad_spec(s);
		}
	    break;
#endif /* WINNT_NATIVE */

	default:
	    abort();
	    /*NOTREACHED*/
	    return NULL;
	}
	break;

    default:
	goto bad_spec;
    }

 end:
    Strbuf_terminate(&b);
    str->buf = xrealloc(b.s, (b.len + 1) * sizeof (*str->buf));
    str->len = b.len;
    return str;

 bad_spec:
    bad_spec(s);
    xfree(b.s);
    return NULL;
}


static CStr *
parsestring(const Char *str, CStr *buf)
{
    struct Strbuf b = Strbuf_INIT;
    const Char   *p;
    eChar  es;

    if (*str == 0) {
	xprintf(CGETS(20, 5, "Null string specification\n"));
	return NULL;
    }

    for (p = str; *p != 0; p++) {
	if ((*p & CHAR) == '\\' || (*p & CHAR) == '^') {
	    if ((es = parseescape(&p)) == CHAR_ERR) {
		xfree(b.s);
		return 0;
	    } else
		Strbuf_append1(&b, es);
	}
	else
	    Strbuf_append1(&b, *p & CHAR);
    }
    Strbuf_terminate(&b);
    buf->buf = xrealloc(b.s, (b.len + 1) * sizeof (*buf->buf));
    buf->len = b.len;
    return buf;
}

static void
print_all_keys(void)
{
    int     prev, i;
    CStr nilstr;
    nilstr.buf = NULL;
    nilstr.len = 0;


    xprintf(CGETS(20, 6, "Standard key bindings\n"));
    prev = 0;
    for (i = 0; i < 256; i++) {
	if (CcKeyMap[prev] == CcKeyMap[i])
	    continue;
	printkeys(CcKeyMap, prev, i - 1);
	prev = i;
    }
    printkeys(CcKeyMap, prev, i - 1);

    xprintf(CGETS(20, 7, "Alternative key bindings\n"));
    prev = 0;
    for (i = 0; i < 256; i++) {
	if (CcAltMap[prev] == CcAltMap[i])
	    continue;
	printkeys(CcAltMap, prev, i - 1);
	prev = i;
    }
    printkeys(CcAltMap, prev, i - 1);
    xprintf(CGETS(20, 8, "Multi-character bindings\n"));
    PrintXkey(NULL);	/* print all Xkey bindings */
    xprintf(CGETS(20, 9, "Arrow key bindings\n"));
    PrintArrowKeys(&nilstr);
}

static void
printkeys(KEYCMD *map, int first, int last)
{
    struct KeyFuncs *fp;
    Char    firstbuf[2], lastbuf[2];
    CStr fb, lb;
    unsigned char *unparsed;
    fb.buf = firstbuf;
    lb.buf = lastbuf;

    firstbuf[0] = (Char) first;
    firstbuf[1] = 0;
    lastbuf[0] = (Char) last;
    lastbuf[1] = 0;
    fb.len = 1;
    lb.len = 1;

    unparsed = unparsestring(&fb, STRQQ);
    if (map[first] == F_UNASSIGNED) {
	if (first == last)
	    xprintf(CGETS(20, 10, "%-15s->  is undefined\n"), unparsed);
	xfree(unparsed);
	return;
    }

    for (fp = FuncNames; fp->name; fp++) {
	if (fp->func == map[first]) {
	    if (first == last)
		xprintf("%-15s->  %s\n", unparsed, fp->name);
	    else {
		unsigned char *p;

		p = unparsestring(&lb, STRQQ);
		xprintf("%-4s to %-7s->  %s\n", unparsed, p, fp->name);
		xfree (p);
	    }
	    xfree(unparsed);
	    return;
	}
    }
    xprintf(CGETS(20, 11, "BUG!!! %s isn't bound to anything.\n"), unparsed);
    if (map == CcKeyMap)
	xprintf("CcKeyMap[%d] == %d\n", first, CcKeyMap[first]);
    else
	xprintf("CcAltMap[%d] == %d\n", first, CcAltMap[first]);
    xfree(unparsed);
}

static void
bindkey_usage(void)
{
    xprintf(CGETS(20, 12,
	    "Usage: bindkey [options] [--] [KEY [COMMAND]]\n"));
    xprintf(CGETS(20, 13,
    	    "    -a   list or bind KEY in alternative key map\n"));
    xprintf(CGETS(20, 14,
	    "    -b   interpret KEY as a C-, M-, F- or X- key name\n"));
    xprintf(CGETS(20, 15,
            "    -s   interpret COMMAND as a literal string to be output\n"));
    xprintf(CGETS(20, 16,
            "    -c   interpret COMMAND as a builtin or external command\n"));
    xprintf(CGETS(20, 17,
	    "    -v   bind all keys to vi bindings\n"));
    xprintf(CGETS(20, 18,
	    "    -e   bind all keys to emacs bindings\n"));
    xprintf(CGETS(20, 19,
	    "    -d   bind all keys to default editor's bindings\n"));
    xprintf(CGETS(20, 20,
	    "    -l   list editor commands with descriptions\n"));
    xprintf(CGETS(20, 21,
	    "    -r   remove KEY's binding\n"));
    xprintf(CGETS(20, 22,
	    "    -k   interpret KEY as a symbolic arrow-key name\n"));
    xprintf(CGETS(20, 23,
	    "    --   force a break from option processing\n"));
    xprintf(CGETS(20, 24,
	    "    -u   (or any invalid option) this message\n"));
    xprintf("\n");
    xprintf(CGETS(20, 25,
	    "Without KEY or COMMAND, prints all bindings\n"));
    xprintf(CGETS(20, 26,
	    "Without COMMAND, prints the binding for KEY.\n"));
}

static void
list_functions(void)
{
    struct KeyFuncs *fp;

    for (fp = FuncNames; fp->name; fp++) {
	xprintf("%s\n          %s\n", fp->name, fp->desc);
    }
}

#ifdef OBSOLETE

/*
 * Unfortunately the apollo optimizer does not like & operations
 * with 0377, and produces illegal instructions. So we make it
 * an unsigned char, and hope for the best.
 * Of-course the compiler is smart enough to produce bad assembly
 * language instructions, but dumb when it comes to fold the constant :-)
 */
#ifdef apollo
static unsigned char APOLLO_0377 = 0377;
#else /* sane */
# define APOLLO_0377    0377
#endif /* apollo */

static int
tocontrol(int c)
{
    c &= CHAR;
    if (Islower(c))
	c = Toupper(c);
    else if (c == ' ')
	c = '@';
    if (c == '?')
	c = CTL_ESC('\177');
    else
#ifdef IS_ASCII
	c &= 037;
#else
	/* EBCDIC: simulate ASCII-behavior by transforming to ASCII and back */
	c  = _toebcdic[_toascii[c] & 037];
#endif
    return (c);
}

static char *
unparsekey(int c)		/* 'c' -> "c", '^C' -> "^" + "C" */
{
    char *cp;
    static char tmp[10];

    cp = tmp;

    if (c & 0400) {
	*cp++ = 'A';
	*cp++ = '-';
	c &= APOLLO_0377;
    }
    if ((c & META) && !(Isprint(c) || (Iscntrl(c) && Isprint(c | 0100)))) {
	*cp++ = 'M';
	*cp++ = '-';
	c &= ASCII;
    }
    if (Isprint(c)) {
	*cp++ = (char) c;
	*cp = '\0';
	return (tmp);
    }
    switch (c) {
    case ' ':
	(void) strcpy(cp, "Spc");
	return (tmp);
    case '\n':
	(void) strcpy(cp, "Lfd");
	return (tmp);
    case '\r':
	(void) strcpy(cp, "Ret");
	return (tmp);
    case '\t':
	(void) strcpy(cp, "Tab");
	return (tmp);
#ifdef IS_ASCII
    case '\033':
	(void) strcpy(cp, "Esc");
	return (tmp);
    case '\177':
	(void) strcpy(cp, "Del");
	return (tmp);
    default:
	*cp++ = '^';
	if (c == '\177') {
	    *cp++ = '?';
	}
	else {
	    *cp++ = c | 0100;
	}
	*cp = '\0';
	return (tmp);
#else /* IS_ASCII */
    default:
        if (*cp == CTL_ESC('\033')) {
	    (void) strcpy(cp, "Esc");
	    return (tmp);
	}
	else if (*cp == CTL_ESC('\177')) {
	    (void) strcpy(cp, "Del");
	    return (tmp);
	}
	else if (Isupper(_toebcdic[_toascii[c]|0100])
		|| strchr("@[\\]^_", _toebcdic[_toascii[c]|0100]) != NULL) {
	    *cp++ = '^';
	    *cp++ = _toebcdic[_toascii[c]|0100]
	}
	else {
	    xsnprintf(cp, 3, "\\%3.3o", c);
	    cp += 4;
	}
#endif /* IS_ASCII */
    }
}

static  KEYCMD
getkeycmd(Char **sp)
{
    Char *s = *sp;
    char c;
    KEYCMD keycmd = F_UNASSIGNED;
    KEYCMD *map;
    int     meta = 0;
    Char   *ret_sp = s;

    map = CcKeyMap;

    while (*s) {
	if (*s == '^' && s[1]) {
	    s++;
	    c = tocontrol(*s++);
	}
	else
	    c = *s++;

	if (*s == '\0')
	    break;

	switch (map[c | meta]) {
	case F_METANEXT:
	    meta = META;
	    keycmd = F_METANEXT;
	    ret_sp = s;
	    break;

	case F_XKEY:
	    keycmd = F_XKEY;
	    ret_sp = s;
	    /* FALLTHROUGH */

	default:
	    *sp = ret_sp;
	    return (keycmd);

	}
    }
    *sp = ret_sp;
    return (keycmd);
}

static int
parsekey(Char **sp)
{
    int c, meta = 0, control = 0, ctrlx = 0;
    Char   *s = *sp;
    KEYCMD  keycmd;

    if (s == NULL) {
	xprintf(CGETS(20, 27, "bad key specification -- null string\n"));
	return -1;
    }
    if (*s == 0) {
	xprintf(CGETS(20, 28, "bad key specification -- empty string\n"));
	return -1;
    }

    (void) strip(s);		/* trim to 7 bits. */

    if (s[1] == 0)		/* single char */
	return (s[0] & APOLLO_0377);

    if ((s[0] == 'F' || s[0] == 'f') && s[1] == '-') {
	if (s[2] == 0) {
	    xprintf(CGETS(20, 29,
		   "Bad function-key specification.  Null key not allowed\n"));
	    return (-1);
	}
	*sp = s + 2;
	return (-2);
    }

    if (s[0] == '0' && s[1] == 'x') {	/* if 0xn, then assume number */
	c = 0;
	for (s += 2; *s; s++) {	/* convert to hex; skip the first 0 */
	    c *= 16;
	    if (!Isxdigit(*s)) {
		xprintf(CGETS(20, 30,
			"bad key specification -- malformed hex number\n"));
		return -1;	/* error */
	    }
	    if (Isdigit(*s))
		c += *s - '0';
	    else if (*s >= 'a' && *s <= 'f')
		c += *s - 'a' + 0xA;
	    else if (*s >= 'F' && *s <= 'F')
		c += *s - 'A' + 0xA;
	}
    }
    else if (s[0] == '0' && Isdigit(s[1])) {	/* if 0n, then assume number */
	c = 0;
	for (s++; *s; s++) {	/* convert to octal; skip the first 0 */
	    if (!Isdigit(*s) || *s == '8' || *s == '9') {
		xprintf(CGETS(20, 31,
			"bad key specification -- malformed octal number\n"));
		return -1;	/* error */
	    }
	    c = (c * 8) + *s - '0';
	}
    }
    else if (Isdigit(s[0]) && Isdigit(s[1])) {	/* decimal number */
	c = 0;
	for (; *s; s++) {	/* convert to octal; skip the first 0 */
	    if (!Isdigit(*s)) {
		xprintf(CGETS(20, 32,
		       "bad key specification -- malformed decimal number\n"));
		return -1;	/* error */
	    }
	    c = (c * 10) + *s - '0';
	}
    }
    else {
	keycmd = getkeycmd(&s);

	if ((s[0] == 'X' || s[0] == 'x') && s[1] == '-') {	/* X- */
	    ctrlx++;
	    s += 2;
	    keycmd = getkeycmd(&s);
	}
	if ((*s == 'm' || *s == 'M') && s[1] == '-') {	/* meta */
	    meta++;
	    s += 2;
	    keycmd = getkeycmd(&s);
	}
	else if (keycmd == F_METANEXT && *s) {	/* meta */
	    meta++;
	    keycmd = getkeycmd(&s);
	}
	if (*s == '^' && s[1]) {
	    control++;
	    s++;
	    keycmd = getkeycmd(&s);
	}
	else if ((*s == 'c' || *s == 'C') && s[1] == '-') {	/* control */
	    control++;
	    s += 2;
	    keycmd = getkeycmd(&s);
	}

	if (keycmd == F_XKEY) {
	    if (*s == 0) {
		xprintf(CGETS(20, 33,
			      "Bad function-key specification.\n"));
		xprintf(CGETS(20, 34, "Null key not allowed\n"));
		return (-1);
	    }
	    *sp = s;
	    return (-2);
	}

	if (s[1] != 0) {	/* if symbolic name */
	    char   *ts;

	    ts = short2str(s);
	    if (!strcmp(ts, "space") || !strcmp(ts, "Spc"))
		c = ' ';
	    else if (!strcmp(ts, "return") || !strcmp(ts, "Ret"))
		c = '\r';
	    else if (!strcmp(ts, "newline") || !strcmp(ts, "Lfd"))
		c = '\n';
	    else if (!strcmp(ts, "linefeed"))
		c = '\n';
	    else if (!strcmp(ts, "tab"))
		c = '\t';
	    else if (!strcmp(ts, "escape") || !strcmp(ts, "Esc"))
		c = CTL_ESC('\033');
	    else if (!strcmp(ts, "backspace"))
		c = '\b';
	    else if (!strcmp(ts, "delete"))
		c = CTL_ESC('\177');
	    else {
		xprintf(CGETS(20, 35,
			"bad key specification -- unknown name \"%S\"\n"), s);
		return -1;	/* error */
	    }
	}
	else
	    c = *s;		/* just a single char */

	if (control)
	    c = tocontrol(c);
	if (meta)
	    c |= META;
	if (ctrlx)
	    c |= 0400;
    }
    return (c & 0777);
}


/*ARGSUSED*/
void
dobind(Char **v, struct command *dummy)
{
    int c;
    struct KeyFuncs *fp;
    int i, prev;
    Char   *p, *l;
    CStr    cstr;
    Char    buf[3];

    USE(dummy);
    /*
     * Assume at this point that i'm given 2 or 3 args - 'bind', the f-name,
     * and the key; or 'bind' key to print the func for that key.
     */

    if (!MapsAreInited)
	ed_InitMaps();

    if (v[1] && v[2] && v[3]) {
	xprintf(CGETS(20, 36,
	 "usage: bind [KEY | COMMAND KEY | \"emacs\" | \"vi\" | \"-a\"]\n"));
	return;
    }

    if (v[1] && v[2]) {		/* if bind FUNCTION KEY */
	for (fp = FuncNames; fp->name; fp++) {
	    if (strcmp(short2str(v[1]), fp->name) == 0) {
		Char   *s = v[2];

		if ((c = parsekey(&s)) == -1)
		    return;
		if (c == -2) {	/* extended key */
		    for (i = 0; i < 256; i++) {
			if (i != CTL_ESC('\033') && (CcKeyMap[i] == F_XKEY ||
					 CcAltMap[i] == F_XKEY)) {
			    struct Strbuf sb = Strbuf_INIT;

#ifdef IS_ASCII
			    if (i > 0177) {
				Strbuf_append1(&sb, 033);
				Strbuf_append1(&sb, i & ASCII);
			    }
			    else
				Strbuf_append1(&sb, (Char) i);
#else
			    Strbuf_append1(&sb, (Char) i);
#endif
			    for (l = s; *l != 0; l++)
				Strbuf_append1(&sb, *l);
			    Strbuf_terminate(&sb);
			    cstr.buf = sb.s;
			    cstr.len = Strlen(sb.s);
			    AddXkey(&cstr, XmapCmd(fp->func), XK_CMD);
			    xfree(sb.s);
			}
		    }
		    return;
		}
		if (c & 0400) {
		    if (VImode) {
			CcAltMap[c & APOLLO_0377] = fp->func;	
			/* bind the vi cmd mode key */
			if (c & META) {
			    buf[0] = CTL_ESC('\033');
			    buf[1] = c & ASCII;
			    buf[2] = 0;
			    cstr.buf = buf;
			    cstr.len = Strlen(buf);
			    AddXkey(&cstr, XmapCmd(fp->func), XK_CMD);
			}
		    }
		    else {
			buf[0] = CTL_ESC('\030');	/* ^X */
			buf[1] = c & APOLLO_0377;
			buf[2] = 0;
			cstr.buf = buf;
			cstr.len = Strlen(buf);
			AddXkey(&cstr, XmapCmd(fp->func), XK_CMD);
			CcKeyMap[CTL_ESC('\030')] = F_XKEY;
		    }
		}
		else {
		    CcKeyMap[c] = fp->func;	/* bind the key */
		    if (c & META) {
			buf[0] = CTL_ESC('\033');
			buf[1] = c & ASCII;
			buf[2] = 0;
			cstr.buf = buf;
			cstr.len = Strlen(buf);
			AddXkey(&cstr, XmapCmd(fp->func), XK_CMD);
		    }
		}
		return;
	    }
	}
	stderror(ERR_NAME | ERR_STRING, CGETS(20, 37, "Invalid function"));
    }
    else if (v[1]) {
	char   *cv = short2str(v[1]);

	if (strcmp(cv, "list") == 0) {
	    for (fp = FuncNames; fp->name; fp++) {
		xprintf("%s\n", fp->name);
	    }
	    return;
	}
	if ((strcmp(cv, "emacs") == 0) ||
#ifndef VIDEFAULT
	    (strcmp(cv, "defaults") == 0) ||
	    (strcmp(cv, "default") == 0) ||
#endif
	    (strcmp(cv, "mg") == 0) ||
	    (strcmp(cv, "gnumacs") == 0)) {
	    /* reset keys to default */
	    ed_InitEmacsMaps();
#ifdef VIDEFAULT
	}
	else if ((strcmp(cv, "vi") == 0)
		 || (strcmp(cv, "default") == 0)
		 || (strcmp(cv, "defaults") == 0)) {
#else
	}
	else if (strcmp(cv, "vi") == 0) {
#endif
	    ed_InitVIMaps();
	}
	else {			/* want to know what this key does */
	    Char   *s = v[1];

	    if ((c = parsekey(&s)) == -1)
		return;
	    if (c == -2) {	/* extended key */
		cstr.buf = s;
		cstr.len = Strlen(s);
		PrintXkey(&cstr);
		return;
	    }
	    pkeys(c, c);	/* must be regular key */
	}
    }
    else {			/* list all the bindings */
	prev = 0;
	for (i = 0; i < 256; i++) {
	    if (CcKeyMap[prev] == CcKeyMap[i])
		continue;
	    pkeys(prev, i - 1);
	    prev = i;
	}
	pkeys(prev, i - 1);
	prev = 0;
	for (i = 256; i < 512; i++) {
	    if (CcAltMap[prev & APOLLO_0377] == CcAltMap[i & APOLLO_0377])
		continue;
	    pkeys(prev, i - 1);
	    prev = i;
	}
	pkeys(prev, i - 1);
	cstr.buf = NULL;
	cstr.len = 0;
	PrintXkey(&cstr);	/* print all Xkey bindings */
    }
    return;
}

static void
pkeys(int first, int last)
{
    struct KeyFuncs *fp;
    KEYCMD *map;
    int mask;
    char    buf[8];

    if (last & 0400) {
	map = CcAltMap;
	first &= APOLLO_0377;
	last &= APOLLO_0377;
	mask = 0400;
    }
    else {
	map = CcKeyMap;
	mask = 0;
    }
    if (map[first] == F_UNASSIGNED) {
	if (first == last)
	    xprintf(CGETS(20, 38, " %s\t\tis undefined\n"),
		    unparsekey(first | mask));
	return;
    }

    for (fp = FuncNames; fp->name; fp++) {
	if (fp->func == map[first]) {
	    if (first == last) 
		xprintf(" %s\t\t%s\n", 
			unparsekey((first & APOLLO_0377) | mask), fp->name);
	    else {
		(void) strcpy(buf, unparsekey((first & APOLLO_0377) | mask));
		xprintf(" %s..%s\t\t%s\n", buf,
		        unparsekey((last & APOLLO_0377) | mask), fp->name);
	    }
	    return;
	}
    }
    if (map == CcKeyMap) {
	xprintf(CGETS(20, 11, "BUG!!! %s isn't bound to anything.\n"),
		unparsekey(first));
	xprintf("CcKeyMap[%d] == %d\n", first, CcKeyMap[first]);
    }
    else {
	xprintf(CGETS(20, 11, "BUG!!! %s isn't bound to anything.\n"),
		unparsekey(first & 0400));
	xprintf("CcAltMap[%d] == %d\n", first, CcAltMap[first]);
    }
}
#endif /* OBSOLETE */
