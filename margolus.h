

typedef char pixel;

#define BLANK_PIXEL 0
struct st_color {
  float r,g,b;
};

typedef struct st_marg {
  unsigned cols, rows;          /* size of field */
  unsigned C, R;                /* size of PIXEL ARRAY (C=cols+2, R=rows+2) */
  unsigned mode;                /* 0-based neighborhoods or 1-based? */
  struct st_color cmap[255];    /* color map */
  pixel *g;                     /* stub for pixel array */
} *MARG;

/* One neighborhood */
typedef struct st_nbhd {
  pixel g[4];
} *NBHD;

MARG new(unsigned, unsigned);
unsigned chk(MARG, int, int, unsigned);
unsigned in_gutter(MARG, int, int);
pixel get(MARG, int, int);
void set(MARG, int, int, pixel);
void clr(MARG, int, int);
void die(const char *);

void iterate_neighborhoods(MARG, void (*)(NBHD));
void get_nbhd(MARG, unsigned, unsigned, NBHD);
void put_nbhd(MARG, unsigned, unsigned, NBHD);
unsigned chk_nbhd(MARG, unsigned, unsigned);

void rot_cw (NBHD);
void rot_ccw (NBHD);
void rot_random (NBHD);
void scramble (NBHD);

void toggle_mode(MARG);
void draw (MARG);
