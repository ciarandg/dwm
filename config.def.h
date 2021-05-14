/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int gappih    = 25;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 25;       /* vert inner gap between windows */
static const unsigned int gappoh    = 25;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 25;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const char *fonts[]          = {
  "NotoSansMono Nerd Font:style=Condensed SemiBold,Bold:size=11",
  "mplus Nerd Font Mono,M+ 2p:style=Bold:size=11",
  "Twemoji:style=Regular:size=9",
  "monospace:size=11"
};
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
  [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = { "st", "-n", "spcalc", "-g", "60x17", "-e", "R", NULL };
const char *spcmd2[] = { "st", "-n", "spmusic", "-g", "118x20", "-e", "ncmpcpp-ueberzug", NULL };
static Sp scratchpads[] = {
  /* name        cmd  */
  { "spcalc",    spcmd1},
  { "spmusic",   spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance        title   tags mask       isfloating      monitor */
  {  "Gimp",    NULL,	        NULL,   0,              1,              -1 },
  {  "Firefox", NULL,	        NULL,   1 << 8,	        0,              -1 },
  {  NULL,      "spcalc",       NULL,   SPTAG(0),       1,              -1 },
  {  NULL,      "spmusic",      NULL,   SPTAG(1),       1,              -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "H[]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "><>",	NULL },			/* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* personal variables */
static char term_shell[] = "zsh";

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run" };
static const char *termcmd[]  = { "st", "-e", term_shell, NULL };

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key			function       	argument */
	STACKKEYS(MODKEY,					focus)
	STACKKEYS(MODKEY|ShiftMask,				push)
	TAGKEYS(                        XK_1,                      		0)
	TAGKEYS(                        XK_2,                      		1)
	TAGKEYS(                        XK_3,                      		2)
	TAGKEYS(                        XK_4,                      		3)
	TAGKEYS(                        XK_5,                      		4)
	TAGKEYS(                        XK_6,                      		5)
	TAGKEYS(                        XK_7,                      		6)
	TAGKEYS(                        XK_8,                      		7)
	TAGKEYS(                        XK_9,                      		8)

	{ MODKEY,			XK_grave,		spawn,		SHCMD("gopass_dmenu") },
	{ MODKEY,			XK_minus,		spawn,		SHCMD("brightinc -10") },
	{ MODKEY,			XK_equal,		spawn,		SHCMD("brightinc 10") },

	{ MODKEY,                       XK_q,			killclient,	{0} },
	{ MODKEY,			XK_w,			spawn,		SHCMD("$BROWSER") },
	{ MODKEY,			XK_e,			spawn,		SHCMD("emacs") },
	{ MODKEY,			XK_r,			spawn,		SHCMD("st -e lf") },
	{ MODKEY|ShiftMask,		XK_r,			spawn,		SHCMD("st -e htop") },

	{ MODKEY,			XK_t,			setlayout,	{.v = &layouts[0]} }, // tile
	{ MODKEY|ShiftMask,		XK_t,			setlayout,	{.v = &layouts[1]} }, // bstack
	{ MODKEY,			XK_y,			setlayout,	{.v = &layouts[2]} }, // spiral
	{ MODKEY|ShiftMask,		XK_y,			setlayout,	{.v = &layouts[3]} }, // dwindle
	{ MODKEY,			XK_u,			setlayout,	{.v = &layouts[4]} }, // deck
	{ MODKEY|ShiftMask,		XK_u,			setlayout,	{.v = &layouts[5]} }, // monocle
	{ MODKEY,			XK_i,			spawn,		SHCMD("st -e weechat") },
	{ MODKEY,			XK_o,			incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_o,			incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_p,			spawn,		SHCMD("mpc toggle; mpd_update") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("audiomenu") },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("mpc seek -10; mpd_update") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("mpc seek -60; mpd_update") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("mpc seek +10; mpd_update") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("mpc seek +60; mpd_update") },

	{ MODKEY, 			XK_backslash,		spawn,		SHCMD("mpv_clipboard") },

	{ MODKEY,			XK_a,			togglegaps,	{0} },
	{ MODKEY|ShiftMask,		XK_a,			defaultgaps,	{0} },
	{ MODKEY,			XK_s,			togglesticky,	{0} },
	{ MODKEY|ShiftMask,             XK_s,			spawn,		SHCMD("steam") },
	{ MODKEY,                       XK_d,			spawn,		{.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,			spawn,		SHCMD("Discord") },
	{ MODKEY,                       XK_f,			togglefullscr,	{0} },
	{ MODKEY|ShiftMask,		XK_f,			setlayout,	{.v = &layouts[6]} }, // floating
	{ MODKEY,			XK_g,			shiftview,	{.i = -1} },
	{ MODKEY|ShiftMask,		XK_g,			shifttag,	{.i = -1} },
	{ MODKEY,                       XK_h,			setmfact,	{.f = -0.05} },
	/* J and K are automatically bound above in STACKKEYS */
	{ MODKEY,                       XK_l,			setmfact,	{.f = +0.05} },
	{ MODKEY,			XK_semicolon,		shiftview,	{.i = 1} },
	{ MODKEY|ShiftMask,		XK_semicolon,		shifttag,	{.i = 1} },
	{ MODKEY,			XK_z,			incrgaps,	{.i = +3 } },
	{ MODKEY|ShiftMask,		XK_z,			incrgaps,	{.i = -3 } },
	{ MODKEY,			XK_x,			spawn,		SHCMD("i3lock -c 282828") },
	{ MODKEY,			XK_c,	 		togglescratch,	{.ui = 0 } },
	{ MODKEY,                       XK_b,			togglebar,	{0} },
	{ MODKEY,			XK_m,	 		togglescratch,	{.ui = 1 } },
	{ MODKEY,			XK_comma,		spawn,		SHCMD("mpc seek 0%; mpd_update") },
	{ MODKEY|ShiftMask,		XK_comma,		spawn,		SHCMD("mpc prev; mpd_update") },
	{ MODKEY,			XK_period,		spawn,		SHCMD("mpc repeat; mpd_update") },
	{ MODKEY|ShiftMask,		XK_period,		spawn,		SHCMD("mpc next; mpd_update") },

	{ MODKEY,		XK_BackSpace,		spawn,		SHCMD("sysact") },
	{ MODKEY|ShiftMask,		XK_BackSpace,		spawn,		SHCMD("sysact") },
	{ MODKEY,                       XK_Tab,    		view,		{0} },
	{ MODKEY,			XK_Return, 		spawn,		{.v = termcmd } },
	{ MODKEY,                       XK_space,  		zoom,		{0} },
	{ MODKEY,			XK_F2,			quit,		{1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
