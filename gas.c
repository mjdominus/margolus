
/* GL02Primitive.cpp: Vertex, Primitive and Color
 * Draw Simple 2D colored Shapes: quad, triangle and polygon.
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <assert.h>

#define N 200                   /* pixel array size */
#define MIN (-N/2)
#define MAX (N/2)
unsigned char g[N][N];          /* box of gas */

#define chk(r, c) (((r) < MIN || (r) >= MAX) ?   \
(fprintf(stderr, "r=%d out of range\n", (r)), abort())      \
: ((c) < MIN || (c) >= MAX) ?                                           \
 (fprintf(stderr, "c=%d out of range\n", (c)), abort())                 \
   : 1)
  
#define set(r, c) do { chk(r,c); g[(r)-MIN][(c)-MIN] = 1; } while (0)
#define clr(r, c) do { chk(r,c); g[(r)-MIN][(c)-MIN] = 0; } while (0)
#define pxl(r, c) ((chk(r,c)), g[(r)-MIN][(c)-MIN])

/* Initialize OpenGL Graphics */
void initGL() {
   // Set "clearing" or background color
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

#define sq(x) ((x) * (x))

/* circular gas thing */
void initGas() {
  int rad2 = (N/4)*(N/4);
  int XC = 0, YC = 0;
  unsigned count = 0, on = 0, on2 = 0 ;
  int r, c;
  for (r=MIN; r < MAX; r++) {
    for (c=MIN; c < MAX; c++) {
      if (sq(r-YC) + sq(c-XC) <= rad2) { on++; set(r, c); }
      count++;
    }
  }
  fprintf(stderr, "Counted %u pixels on out of %u\n", on, count);
}
 
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
  int r, c;
  
  glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
 
  // Define shapes enclosed within a pair of glBegin and glEnd
  glBegin(GL_POINTS); 

  for (r=MIN; r < MAX; r++) {
    for (c=MIN; c < MAX; c++) {
      glColor3f(0.0f, 0.0f, pxl(r, c) ? 1.0f : 0.0f);
      glVertex2f(c/(float)MAX, r/(float)MAX);
    }
  }
   glEnd();
 
   glFlush();  // Render now
}
 
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);          // Initialize GLUT
   glutCreateWindow("Circle");  // Create window with the given title
   glutInitWindowSize(N, N); // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   initGL();                       // Our own OpenGL initialization
   initGas();
   glutMainLoop();                 // Enter the event-processing loop
   return 0;
}
