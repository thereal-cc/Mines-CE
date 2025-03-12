# ----------------------------
# Makefile Options
# ----------------------------

NAME = MinesCE
ICON = icon.png
DESCRIPTION = "Minesweeper for TI 84+ CE"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
