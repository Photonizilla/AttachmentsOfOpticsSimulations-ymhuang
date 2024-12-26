#include <GL/glut.h>
#include <cstdio>
#include <cmath>

#define N 2000

float T = 0;
float f_1 = 5;
float f_2 = 6;
float A_1 = 3;
float A_2 = 2;
float v = M_PI;

inline float E_1(float x, float t) {
    return A_1 * cos(2 * M_PI * f_1 * (x / v - t));
}

inline float E_2(float x, float t) {
    return A_2 * cos(2 * M_PI * f_2 * (x / v - t));
}

inline float E(float x, float t) {
    return E_1(x, t) + E_2(x, t);
}

inline float E_e(float x, float t) {
    return (A_1 + A_2) * cos(M_PI * (f_1 - f_2) * (x / v - t)) - (A_1 - A_2) * sin(M_PI * (f_1 - f_2) * (x / v - t));
}

inline float I(float x, float t) {
    return E(x, t) * E(x, t);
}

inline float I_e(float x, float t) {
    return A_1 * A_1 + A_2 * A_2 + 2 * A_1 * A_2 * cos(2 * M_PI * (f_1 - f_2) * (x / v - t));
}

inline GLfloat Red(float p) {
    float A = (A_1 + A_2) * (A_1 + A_2);
    if (p > A / 2.0)
        return 1.0f;
    return GLfloat(2.0 * p / A);
}

inline GLfloat Green(float p) {
    float p_ = p / ((A_1 + A_2) * (A_1 + A_2));
    if (p_ <= 0.5)
        return GLfloat(2.0 * p_);
    return GLfloat(1.0 - (p_ - 0.5) * 0.7);
}

void PlotDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw boxes
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.9f, 0.92f);
    glVertex2f(0.9f, 0.92f);
    glVertex2f(0.9f, 0.52f);
    glVertex2f(-0.9f, 0.52f);
    glVertex2f(-0.9f, 0.92f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.9f, 0.44f);
    glVertex2f(0.9f, 0.44f);
    glVertex2f(0.9f, 0.04f);
    glVertex2f(-0.9f, 0.04f);
    glVertex2f(-0.9f, 0.44f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.9f, -0.04f);
    glVertex2f(0.9f, -0.04f);
    glVertex2f(0.9f, -0.44f);
    glVertex2f(-0.9f,-0.44f);
    glVertex2f(-0.9f, -0.04f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.9f, -0.52f);
    glVertex2f(0.9f, -0.52f);
    glVertex2f(0.9f, -0.92f);
    glVertex2f(-0.9f,-0.92f);
    glVertex2f(-0.9f, -0.52f);
    glEnd();
    
    glBegin(GL_LINES);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        glColor3f(Red(I(X, T)), Green(I(X, T)), 0.0f);
        glVertex2f(x, 0.91f);
        glVertex2f(x, 0.53f);
    }
    glEnd();
    
    glColor3f(0.9255f, 0.4157f, 0.3961f);
    glBegin(GL_LINE_STRIP);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        GLfloat y = 0.24f + 0.19f * E_1(X, T) / fmax(A_1, A_2);
        glVertex2f(x, y);
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(0.2314f, 0.5216f, 0.9686f);
    glBegin(GL_LINE_STRIP);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        GLfloat y = 0.24f + 0.19f * E_2(X, T) / fmax(A_1, A_2);
        glVertex2f(x, y);
        glVertex2f(x, y);
    }
    glEnd();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        GLfloat y = -0.24f + 0.19f * E(X, T) / (A_1 + A_2);
        glVertex2f(x, y);
        glVertex2f(x, y);
    }
    glEnd();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        GLfloat y = -0.91f + 0.38f * I(X, T) / (A_1 + A_2) / (A_1 + A_2);
        glVertex2f(x, y);
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(0.4f, 0.8f, 0.4f);
    glBegin(GL_LINE_STRIP);
    for (GLfloat x = -0.89f; x <= 0.89f; x += 1.78f / N) {
        float X = float(x / 0.89f) * 5;
        GLfloat y = -0.91f + 0.38f * I_e(X, T) / (A_1 + A_2) / (A_1 + A_2);
        glVertex2f(x, y);
        glVertex2f(x, y);
    }
    glEnd();
    
    glFlush();
    glutSwapBuffers();
}

void timeEvolution() {
    T += 0.002;
//    T = -5.0 / M_PI;
    PlotDisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(350, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("2D Plotter");
    glutDisplayFunc(&PlotDisplay);
    glutIdleFunc(&timeEvolution);
    glutMainLoop();

    return 0;
}

