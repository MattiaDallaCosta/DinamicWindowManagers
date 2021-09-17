/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx          = 3;  /* border pixel of windows */
static const unsigned int igappx            = 8;  /* size of inner gaps */
static const unsigned int ogappx            = 8;  /* size of outer gaps */
static const int gapsforone                 = 1;  /* 1 enable gaps when only one window is open */
static const unsigned int snap              = 0; /* snap pixel */
static const unsigned int cornerrad         = 0;
static const int showbar                    = 1;  /* 0 means no bar */
static const int topbar                     = 1;   /* 0 means bottom bar */
static const int vertpad                    = 4;  /* vertical padding of bar */
static const int sidepad                    = 4;  /* horizontal padding of bar */
static const int boollogo                   = 0; /* 0 means no logo at the begining of the bar */
static const char *logo                     = ""; /* the icon that you want to put as the logo */
static const char *fonts[]                  = {"Jetbrains Mono Nerd Font:size=13"};
static const char dmenufont[]               = "Mononoki Nerd Font:size=12";
static const char dmenuxpos[]               = "4";
static const char dmenuypos[]               = "32";
static const char dmenuwidth[]              = "432";
static const char prompt[]                  = "Software:";
static const char NormBg[]                  = "#1c1e26";
static const char NormFg[]                  = "#7799bb";
static const char NormBorder[]              = "#1c1e26";
static const char SelBg[]                   = "#2d2f37";
static const char SelFg[]                   = "#e0e0e0";
static const char SelBorder[]               = "#557799";
static const char TagsNormBg[]              = "#1c1e26";
static const char TagsNormFg[]              = "#7799bb";
static const char TagsSelBg[]               = "#2d2f37";
static const char TagsSelFg[]               = "#e0e0e0";
static const char InfoNormBg[]              = "#1c1e26";
static const char InfoNormFg[]              = "#7799bb";
static const char InfoSelBg[]               = "#999999";
static const char InfoSelFg[]               = "#333333";
static const char StatusBg[]                = "#2d2f37";
static const char StatusFg[]                = "#7799bb";
static const char LogoBg[]                  = "#2d2f37";
static const char LogoFg[]                  = "#557799";
static const char LayoutBg[]                = "#2d2f37";
static const char LayoutFg[]                = "#7799bb";
static const unsigned int baralpha          = 194;
static const unsigned int borderalpha       = OPAQUE;
static const char *colors[][3] = {
    /*                       fg         bg         border   */
    [SchemeNorm]        = {NormFg, NormBg, NormBorder},
    [SchemeSel]         = {SelFg, SelBg, SelBorder},
    [SchemeLogo]        = {LogoFg, LogoBg, "#000000"}, // Logo left {text,background,not used but cannot be empty}
    [SchemeLayout]      = {LayoutFg, LayoutBg, "#000000"}, // Logo left {text,background,not used but cannot be empty}
    [SchemeStatus]      = {StatusFg, StatusBg, "#000000"}, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsNorm]    = {TagsNormFg, TagsNormBg, "#000000"}, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeTagsSel]     = {TagsSelFg, TagsSelBg, "#000000"}, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]    = {InfoNormFg, InfoNormBg, "#000000"}, // infobar middle unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]     = {InfoSelFg, InfoSelBg, "#000000"}, // infobar middle selected {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3] = {
    /*                       fg      bg        border     */
    [SchemeNorm]        = {OPAQUE, baralpha, borderalpha},
    [SchemeSel]         = {OPAQUE, baralpha, borderalpha},
    [SchemeLogo]        = {OPAQUE, OPAQUE, borderalpha},
    [SchemeLayout]      = {OPAQUE, OPAQUE, borderalpha},
    [SchemeStatus]      = {OPAQUE, OPAQUE, borderalpha},
    [SchemeTagsNorm]    = {OPAQUE, baralpha, borderalpha},
    [SchemeTagsSel]     = {OPAQUE, OPAQUE, borderalpha},
    [SchemeInfoNorm]    = {OPAQUE, baralpha, borderalpha},
    [SchemeInfoSel]     = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1", "", "3", "4", "5", "6", "切", "阮", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/
    { "Gimp",       NULL,       NULL,       0,              1,          -1,     50,50,500,500,           3 },
    { "firefox", "Navigator",   NULL,       1 << 1,         0,          -1,     50,50,500,500,           3 },
    { "firefox", "Toolkit",     NULL,       0,              1,          -1,     1216,801,693,390,        3 },
    { "TelegramDesktop",NULL,   NULL,       1 << 6,         0,          -1,     50,50,500,500,           3 },
    { "Carla2",     NULL,       NULL,       1 << 8,         0,          -1,     50,50,500,500,           3 },
    { "Onboard",    NULL,       NULL,       0,              1,          -1,     50,50,500,500,           3 },
    { "Calculator", NULL,       NULL,       0,              1,          -1,     1397,32,510,110,         3 },
};

/* layout(s) */
static const int dirs[3]        = {DirHor, DirVer, DirVer}; /* tiling dirs */
static const float facts[3]     = {1.1, 1.1, 1.1};       /* tiling facts */
static const int nmaster        = 1; /* number of clients in master area */
static const int resizehints    = 1; /* 1 means respect size hints in tiled resizals */

/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",        tile    }, /* first entry is default */
    { "><>",        NULL    }, /* no layout function means floating behavior */
    { "[M]",        monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN, KEY, TAG)                                                           \
  { MODKEY,                       CHAIN,     KEY,    view,       { .ui = 1 << TAG } },         \
  { MODKEY|ControlMask,           CHAIN,     KEY,    toggleview, { .ui = 1 << TAG } },         \
  { MODKEY|ShiftMask,             CHAIN,     KEY,    tag,        { .ui = 1 << TAG } },         \
  { MODKEY|ControlMask|ShiftMask, CHAIN,     KEY,    toggletag,  { .ui = 1 << TAG } },
#define DIRS(CHAIN, KEY, G, M, S)                                                          \
  { MODKEY, CHAIN, KEY, setdirs, {.v = (int[]) { INC(G * +1), INC(M * +1), INC(S * +1)}} },
#define DIMS(CHAIN, MOD, G, M, S)                                              \
  { MOD, CHAIN, XK_h, setfacts, {.v = (float[]) {INC(G * -0.1), INC(M * -0.1), INC(S * -0.1)}} },             \
  { MOD, CHAIN, XK_l, setfacts, {.v = (float[]){INC(G * +0.1), INC(M * +0.1), INC(S * +0.1)}} },
#define NEXTPREV(MOD, CHAIN, ACTION)                                           \
  { MOD, CHAIN, XK_j,       ACTION##stack, {.i = INC(+1)} },                           \
  { MOD, CHAIN, XK_k,       ACTION##stack, {.i = INC(-1)} },                       \
  { MOD, CHAIN, XK_grave,   ACTION##stack, {.i = PREVSEL} },
#define NUMS(MOD, CHAIN, ACTION)                                               \
  { MOD, CHAIN, XK_1,       ACTION##stack, {.i = 0}},                                 \
  { MOD, CHAIN, XK_2,       ACTION##stack, {.i = 1}},                             \
  { MOD, CHAIN, XK_3,       ACTION##stack, {.i = 2}},                             \
  { MOD, CHAIN, XK_l,       ACTION##stack, {.i = -1}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]         = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", NormBg, "-nf", NormFg, "-sb", SelBg, "-sf", SelFg, "-l", "54", "-g", "2", "-x", dmenuxpos, "-y", dmenuypos, "-z", dmenuwidth, "-p", prompt, NULL};
static const char *termcmd[]    = { "kitty", NULL };

static Key keys[] = {
    /* modifier                   chain key     key       function argument */
    {MODKEY, XK_a, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY, XK_a, XK_t, spawn, {.v = termcmd}},
    {MODKEY, XK_a, XK_b, spawn, SHCMD("swapfocus firefox || firefox")},
    {MODKEY, XK_a, XK_a, spawn,
     SHCMD("swapfocus telegram || telegram-desktop")},
    {MODKEY, XK_a, XK_s, spawn, SHCMD("swapfocus spotify || spotify")},
    {MODKEY, XK_a, XK_m, spawn,
     SHCMD("swapfocus pulsemixer || kitty -e pulsemixer")},
    {MODKEY, XK_a, XK_v, spawn, SHCMD("swapfocus carla || pw-jack carla")},
    {MODKEY, XK_m, XK_m, spawn,
     SHCMD("xrandr --output eDP1 --auto --primary --output DP3 --left-of eDP1 "
           "--auto")},
    {0, -1, XK_Print, spawn,
     SHCMD("flameshot gui -r -p ~/Pictures/Screenshots")},
    {MODKEY, -1, XK_Print, spawn,
     SHCMD("flameshot screen -r -c -p ~/Pictures/Screenshots/FullScreen")},
    {0, -1, XF86XK_MonBrightnessUp, spawn,
     SHCMD("xbacklight +5 && pkill -USR1 slstatus")},
    {0, -1, XF86XK_MonBrightnessDown, spawn,
     SHCMD("xbacklight -5 && pkill -USR1 slstatus")},
    {0, -1, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("pulsemixer --change-volume +5 && pkill -USR1 slstatus")},
    {0, -1, XF86XK_AudioLowerVolume, spawn,
     SHCMD("pulsemixer --change-volume -5 && pkill -USR1 slstatus")},
    {0, -1, XF86XK_AudioMute, spawn,
     SHCMD("pactl -- set-sink-mute 0 toggle && pkill -USR1 slstatus")},
    {0, -1, XF86XK_AudioPlay, spawn,
     SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify "
           "/org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.PlayPause")},
    NEXTPREV(MODKEY, -1, focus) NEXTPREV(MODKEY | ShiftMask, -1, push)
        NUMS(MODKEY, XK_s, push){MODKEY, -1, XK_i, incnmaster, {.i = +1}},
    {MODKEY, -1, XK_o, incnmaster, {.i = -1}},
    {MODKEY, -1, XK_Return, zoom, {0}},
    {MODKEY, -1, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, -1, XK_w, killclient, {0}},
    {MODKEY, XK_w, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_w, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_w, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, -1, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, -1, XK_f, fullscreenmode, {.i = 1}},
    DIRS(-1, XK_z, 1, 0, 0) DIRS(-1, XK_x, 0, 1, 0) DIRS(-1, XK_c, 0, 0, 1)
        DIMS(-1, MODKEY, 1, 0, 0) DIMS(-1, MODKEY | ControlMask, 0, 1, 0)
            DIMS(-1, MODKEY | ShiftMask, 0, 0,
                 1){MODKEY | ShiftMask,
                    -1,
                    XK_t,
                    setdirs,
                    {.v = (int[]){DirHor, DirVer, DirVer}}},
    {MODKEY | ControlMask,
     -1,
     XK_t,
     setdirs,
     {.v = (int[]){DirVer, DirHor, DirHor}}},
    {MODKEY | ShiftMask, -1, XK_space, togglefloating, {0}},
    {MODKEY, -1, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, -1, XK_0, tag, {.ui = ~0}},
    {MODKEY, -1, XK_comma, focusmon, {.i = -1}},
    {MODKEY, -1, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, -1, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, -1, XK_period, tagmon, {.i = +1}},
    TAGKEYS(-1, XK_1, 0) TAGKEYS(-1, XK_2, 1) TAGKEYS(-1, XK_3, 2)
        TAGKEYS(-1, XK_4, 3) TAGKEYS(-1, XK_5, 4) TAGKEYS(-1, XK_6, 5)
            TAGKEYS(-1, XK_7, 6) TAGKEYS(-1, XK_8, 7)
                TAGKEYS(-1, XK_9, 8){MODKEY | ShiftMask, -1, XK_q, quit, {0}},
};

/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
    /* width change         height change */
    { +scrollsensetivity,   0                  },
    { -scrollsensetivity,   0                  },
    { 0,                    +scrollsensetivity },
    { 0,                    -scrollsensetivity },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument */
    { ClkLtSymbol,              0,          Button1,        setlayout,      {0} },
    { ClkLtSymbol,              0,          Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,              0,          Button2,        zoom,           {0} },
    { ClkStatusText,            0,          Button2,        spawn,          {.v = termcmd}},
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,        MODKEY|ShiftMask,Button1,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,         Button4,    resizemousescroll,  {.v = &scrollargs[0]} },
    { ClkClientWin,         MODKEY,         Button5,    resizemousescroll,  {.v = &scrollargs[1]} },
    { ClkClientWin,         MODKEY,         Button6,    resizemousescroll,  {.v = &scrollargs[2]} },
    { ClkClientWin,         MODKEY,         Button7,    resizemousescroll,  {.v = &scrollargs[3]} },
    { ClkTagBar,                0,          Button1,        view,           {0} },
    { ClkTagBar,                0,          Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
