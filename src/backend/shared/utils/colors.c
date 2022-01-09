#include <shared/utils/colors.h>

static const char Reset[] = "\e[0m";
static const char Gray[] = "\e[38;5;8m";
static const char Silver[] = "\e[38;5;7m";
static const char Cyan[] = "\e[38;5;6m";
static const char Magenta[] = "\e[38;5;5m";
static const char Blue[] = "\e[38;5;4m";
static const char Yellow[] = "\e[38;5;3m";
static const char Green[] = "\e[38;5;2m";
static const char Red[] = "\e[38;5;1m";

const struct colors_lib colors = {
        .Reset = Reset,
        .Gray = Gray,
        .Silver = Silver,
        .Cyan = Cyan,
        .Magenta = Magenta,
        .Blue = Blue,
        .Yellow = Yellow,
        .Green = Green,
        .Red = Red
};
