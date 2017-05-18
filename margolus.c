
/* A margolus grid is an array of pixels
 * Plus a gutter of pixels around the edge
 * And a neighborhood offset state
 * It has a method to retrieve neighborhoods or four pixels
 */

#include "margolus.h"
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define memfail() do { fprintf(stderr, "Couldn't allocate memory\n"); exit(11); } while (0)

MARG new(unsigned rows, unsigned cols) {
  MARG marg = (MARG) malloc(sizeof(struct st_marg));
  if (! marg) memfail();

  {
    marg->cols = cols;
    marg->rows = rows;
    marg->C = cols + 2;
    marg->R = rows + 2;
    marg->mode = 0;
  }

  {
    size_t memsize = marg->R * marg->C * sizeof(pixel);
    pixel *g = malloc(memsize);
    if (! g) memfail();
    memset(g, 0, memsize);
    marg->g = g;
  }

  return marg;
}

/* Say the marg has 100 cols.  They are numbered from 0 to 99
   Additionally there is a column -1 and a column 100 which are scratch space
   And are always empty

   Returns true if r, c is in range for this marg, false if not

   gutter_ok says whether it is okay for r,c to be in the gutter
*/

#define GUTTER_OK     1
#define GUTTER_NOT_OK 0
unsigned chk(MARG m, int r, int c, unsigned gutter_ok) {
  if (gutter_ok || 1) {
    return in_gutter(m, r, c);
  }
  
  if (r < 0 || r > m->rows+1 ) {
    fprintf(stderr, "Row %d out of range\n", r);
    return 0;
  } else if (c < 0 || r >= m->cols ) {
    fprintf(stderr, "Col %d out of range\n", c);
    return 0;
  }
  return 1;
}

/* Is this pixel in the invisible gutter? */
unsigned in_gutter(MARG m, int r, int c) {
  if ((r == 0 || r == m->rows) ||
      (c == 0 || c == m->cols)) { return 1; }
  return 0;
}

/* Say we ask for the pixel in the 0 row, 0 column of a marg with 100 columns
   That is actually g[1][1] which is g[1 * (m->cols+2) + 1] = g[103]
*/
pixel get(MARG m, int r, int c) {
  unsigned i = (r+1) * m->C + (c+1);
  chk(m, r, c, GUTTER_OK);  
  if (in_gutter(m, r, c)) {
    return BLANK_PIXEL;
  } else {
    return m->g[ (r+1) * m->C + (c+1) ];
  }
}

void set(MARG m, int r, int c, pixel p) {
  chk(m, r, c, GUTTER_NOT_OK);
  m->g[ (r+1) * m->C + (c+1) ] = p;
}

void clr(MARG m, int r, int c) {
  set(m, r, c, BLANK_PIXEL);
}

void toggle_mode(MARG m) {
  m->mode = 1 - m->mode;
}

/* How many neighborhoods are there?
 * It's half the number of (rows+1), rounded UP (because of the gutter)
 * or (rows+2) in ODD mode (because of the extra gutter)
 */
void iterate_neighborhoods(MARG m, void (*callback)(NBHD)) {
  unsigned mode = m->mode;
  unsigned nnr = (m->rows + 1 + mode)  / 2, nnc = (m->cols + 1 + mode) / 2;
  unsigned nr, nc;
  unsigned it = 0;
  struct st_nbhd nbhd;

  for (nr = 0; nr < nnr; nr++) {
    for (nc = 0; nc < nnc; nc++) {
      get_nbhd(m, nr, nc, &nbhd);
      (*callback)(&nbhd);
      put_nbhd(m, nr, nc, &nbhd);
      it++;
    }
  }
}

unsigned chk_nbhd(MARG m, unsigned nr, unsigned nc) {
  unsigned num_c = (m->cols + 1 + m->mode)/2;
  unsigned num_r = (m->rows + 1 + m->mode)/2;
  if (nr < 0 || nr >= num_r) {
    fprintf(stderr, "Neighborhood row %d out of range [0..%u)\n", nr, num_r);
    return 0;
  } else if (nc < 0 || nr >= num_c ) {
    fprintf(stderr, "Neighborhood col %d out of range [0..%u)\n", nc, num_c);
    return 0;
  }
  return 1;
}

void get_nbhd(MARG m, unsigned nr, unsigned nc, NBHD nb) {
  unsigned co, ro;
  chk_nbhd(m, nr, nc);
  for (ro = 0; ro < 2; ro++) {
    for (co = 0; co < 2; co++) {
      nb->g[ro*2 + co] = get(m, ro + nr * 2 - m->mode, co + nc * 2 - m->mode);
    }
  }
}

void put_nbhd(MARG m, unsigned nr, unsigned nc, NBHD nb) {
  unsigned co, ro;
  chk_nbhd(m, nr, nc);
  for (ro = 0; ro < 2; ro++) {
    for (co = 0; co < 2; co++) {
      set(m, ro + nr * 2 - m->mode, co + nc * 2 - m->mode, nb->g[ro*2 + co]);
    }
  }
}

void rot_cw (NBHD nb) {
  pixel tmp = nb->g[0];
  nb->g[0] = nb->g[2];
  nb->g[2] = nb->g[3];
  nb->g[3] = nb->g[1];
  nb->g[1] = tmp;
}

void rot_ccw (NBHD nb) {
  pixel tmp = nb->g[0];
  nb->g[0] = nb->g[1];
  nb->g[1] = nb->g[3];
  nb->g[3] = nb->g[2];
  nb->g[2] = tmp;
}

void rot_random (NBHD nb) {
  if (random() % 2 == 0) { rot_cw(nb); }
  else { rot_ccw(nb); }
}

void scramble (NBHD nb) {
  unsigned i;
  for (i=0; i<4; i++) {
    nb->g[i] = random() % 10 ? 0 : 1;
  }
}

/* need better mapping of points to GL viewport
 * Instead of points, use quads of the right size */
void draw (MARG m) {
  unsigned r, c;
  float x, y;

  glBegin(GL_POINTS);
  for (r=0; r < m->rows; r++) {
    y = 2.0*r / m->rows - 1.0;
    for (c=0; c < m->cols; c++) {
      float color = get(m, r, c) ? 1.0f : 0.0f;
      x = 2.0*c / m->cols - 1.0;
      glColor3f(color, color, color);
      glVertex2f(x, y);
    }
  }
  glEnd();
}

void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}
