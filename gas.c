
/* GL02Primitive.cpp: Vertex, Primitive and Color
 * Draw Simple 2D colored Shapes: quad, triangle and polygon.
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "margolus.h"

MARG m;

/* Initialize OpenGL Graphics */
void initGL() {
   // Set "clearing" or background color
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

#define timeout 20  // milliseconds
#define N 200
#define sq(x) ((x)*(x))

void initGasPoint() {
  set(m, m->rows / 2, m->cols / 2, 1);
}

/* circular gas thing */
void initGasCircle() {
  int rad2 = (N/3)*(N/3);
  int XC = N/2, YC = N/2;
  unsigned count = 0, on = 0 ;
  int r, c;

  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      if (sq(r-YC) + sq(c-XC) <= rad2) { on++; set(m, r, c, 1); }
      count++;
    }
  }
}
 
void initGasRandom() {
  int r, c;

  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      if (random() % 2) { set(m, r, c, 1); }
    }
  }
}

void initGasSquare(unsigned s) {
  int r, c;
  unsigned count = 0;

  for (r=s; r < N-s; r++) {
    for (c=s; c < N-s; c++) {
      set(m, r, c, 1);
    }
  }
}

void initGasRect(unsigned top, unsigned left, unsigned ht, unsigned wd, unsigned col) {
  int r, c;

  for (r=top; r < top+ht; r++) {
    for (c=left; c < left+wd; c++) {
      set(m, r, c, col);
    }
  }
}

void  initGas2Rectangles() {
  initGasRect(N/6, N/6, 2*N/3, N/3, 1);
  initGasRect(N/6, N/2, 2*N/3, N/3, 2);
}


void initGas() {
  m = new(N, N);

  initGas2Rectangles();
}


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
  void drawGas(void);
  
  glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

  draw(m);
  glutSwapBuffers();  // Render now
  iterate_neighborhoods(m, rot_random);
  toggle_mode(m);
}

void Timer(int v) {
  glutPostRedisplay();
  glutTimerFunc(timeout, Timer, 0); // next Timer call milliseconds later 
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);          // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE);
   glutCreateWindow("Gas Diffusion");  // Create window with the given title
   glutInitWindowSize(N, N); // Set the window's initial width & height
   glutInitWindowPosition(1500, 50); // Position the window's initial top-left corner
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutTimerFunc(timeout, Timer, 0);
   initGL();                       // Our own OpenGL initialization
   initGas();
   glutMainLoop();                 // Enter the event-processing loop
   return 0;
}
