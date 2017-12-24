#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

// Component theming
#define THEME_BG        (Color){24, 24, 24, 0.7}
#define THEME_BG_FOCUS  (Color){32, 32, 32, 0.9}

// Panel spacing (vert)
#define VSPACE_GAP 4
#define VSPACE_HALF 44
#define VSPACE_THIRD 28

#define HSPACE_GAP 2

// Panel positioning
#define VPOS_TOP (VSPACE_GAP)
#define VPOS_HALF_1 (VPOS_TOP)
#define VPOS_HALF_2 (VPOS_HALF_1 + VSPACE_HALF + VSPACE_GAP)
#define VPOS_THIRD_1 (VPOS_TOP)
#define VPOS_THIRD_2 (VPOS_THIRD_1 + VSPACE_THIRD + VSPACE_GAP)
#define VPOS_THIRD_3 (VPOS_THIRD_2 + VSPACE_THIRD + VSPACE_GAP)

// Panel sizing
#define VSIZE_WHOLE (100 - 2 * VSPACE_GAP)
#define VSIZE_1_HALF (VSPACE_HALF)
#define VSIZE_2_HALF (2 * VSPACE_HALF + VSPACE_GAP)
#define VSIZE_1_THIRD (VSPACE_THIRD)
#define VSIZE_2_THIRD (2 * VSPACE_THIRD + VSPACE_GAP)
#define VSIZE_3_THIRD (3 * VSPACE_THIRD + 2 * VSPACE_GAP)

void createConsole();
void createPlayerPanel();
void createCharPanel();

#endif

