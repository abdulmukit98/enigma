/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<bits/stdc++.h>
#include <stdlib.h>

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

void circle(double x, double y, double radius);
void circle_poly(double x, double y, double radius);
void circle_half(double x, double y, double radius, char s);

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT );


    glColor3d(0,0.78,.45);
    circle(-.5, .5, .15);

    circle(-.57, .57, .03);
    circle(-.43, .57, .03);

    circle_half(-.5, .47, .05, 'd');


    glBegin(GL_LINES);
        glVertex2d(-.5, .5-.15);
        glVertex2d(-.5, -.5);

        glVertex2d(-.5, .2);
        glVertex2d(-.75, 0);

        glVertex2d(-.5, .2);
        glVertex2d(-.25, 0);

        glVertex2d(-.5, -.5);
        glVertex2d(-.75, -.75);

        glVertex2d(-.5, -.5);
        glVertex2d(-.25, -.75);

        glVertex2d(-.25, -.5);
        glVertex2d(-.25, .85);

        glVertex2d(-.25, .85);
        glVertex2d(-.20, .85);

        glVertex2d(-.25, .15);
        glVertex2d(-.20, .15);


    glEnd();

    glBegin(GL_POLYGON);

        glVertex2d(-.20, .85);
        glVertex2d(-.20, .15);
        glVertex2d(.5, .15);
        glVertex2d(.5, .85);

    glEnd();

    glColor3d(1, 0, 0);
    circle_poly(.15, .5, .2);

    glFlush();
    glutSwapBuffers();
}


void circle(double x, double y, double radius)
{
    double pi = 3.141592653589793238;
    glBegin(GL_LINES);
        for(double i = 0; i<2*pi ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    glEnd();
}


void circle_poly(double x, double y, double radius)
{
    double pi = 3.141592653589793238;
    glBegin(GL_POLYGON);
        for(double i = 0; i<2*pi ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    glEnd();
}


void circle_half(double x, double y, double radius, char s)
{
    double pi = 3.141592653589793238;
    glBegin(GL_LINES);

    if(s == 'd'){
        for(double i = pi; i<2*pi ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    }

    if(s == 'u'){
        for(double i = 0; i<pi ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    }


    if(s == 'l'){
        for(double i = pi/2; i<pi+pi/2 ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    }


    if(s == 'r'){
        for(double i = 0; i<pi/2 ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
        for(double i = pi+pi/2; i<2*pi ; i += pi/50)
        {
            glVertex2d(x + radius * cos(i), y + radius * sin(i));
            double k = i + pi/50;
            glVertex2d(x + radius * cos(k), y + radius * sin(k));
        }
    }

    glEnd();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
   // glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    //glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glutMainLoop();

    return EXIT_SUCCESS;
}
