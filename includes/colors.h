#ifndef COLORS_H
# define COLORS_H

// Foreground colors
typedef enum VGA_ForegroundColor {
	FG_BLACK			= 0x0,
	FG_BLUE				= 0x1,
	FG_GREEN			= 0x2,
	FG_CYAN				= 0x3,
	FG_RED				= 0x4,
	FG_MAGENTA			= 0x5,
	FG_BROWN			= 0x6,
	FG_LIGHT_GRAY		= 0x7,
	FG_DARK_GRAY		= 0x8,
	FG_LIGHT_BLUE		= 0x9,
	FG_LIGHT_GREEN		= 0xA,
	FG_LIGHT_CYAN		= 0xB,
	FG_LIGHT_RED		= 0xC,
	FG_LIGHT_MAGENTA	= 0xD,
	FG_YELLOW			= 0xE,
	FG_WHITE			= 0xF
}	t_color_fg;

// Background colors
typedef enum VGA_BackgroundColor {
	BG_BLACK			= 0x0 << 4,
	BG_BLUE				= 0x1 << 4,
	BG_GREEN			= 0x2 << 4,
	BG_CYAN				= 0x3 << 4,
	BG_RED				= 0x4 << 4,
	BG_MAGENTA			= 0x5 << 4,
	BG_BROWN			= 0x6 << 4,
	BG_LIGHT_GRAY		= 0x7 << 4,
	BG_DARK_GRAY		= 0x8 << 4,
	BG_LIGHT_BLUE		= 0x9 << 4,
	BG_LIGHT_GREEN		= 0xA << 4,
	BG_LIGHT_CYAN		= 0xB << 4,
	BG_LIGHT_RED		= 0xC << 4,
	BG_LIGHT_MAGENTA	= 0xD << 4,
	BG_YELLOW			= 0xE << 4,
	BG_WHITE			= 0xF << 4
}	t_color_bg;


char	create_color(t_color_fg color_fg, t_color_bg color_bg)
{
	return (((char)color_fg | (char)color_bg));
}

#endif
