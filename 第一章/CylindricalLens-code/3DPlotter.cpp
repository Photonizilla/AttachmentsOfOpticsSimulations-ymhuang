#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#define N 80

double xCenter = 0, yCenter = 0;
double xRange = 100, yRange = 100, zRange = 10;
double xSize = 10, ySize = 10, zSize = 1;
double z[N + 1][N + 1];
double t;

struct Vector {
    double x;
    double y;
    double z;
};


Vector* p = new Vector [N+1];
Vector zero;
double R = 1e4;

inline double dotProduct(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double amplitude(Vector q) {
    return sqrt(dotProduct(q, q));
}

inline Vector addition(Vector a, Vector b) {
    return Vector {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

inline Vector multiplication(double a, Vector v) {
    return Vector {
        a * v.x,
        a * v.y,
        a * v.z
    };
}

inline Vector direction(Vector r) {
    double amp = amplitude(r);
    return Vector {
        r.x / amp,
        r.y / amp,
        r.z / amp
    };
}

inline double distance(Vector q_1, Vector q_2) {
    return amplitude(addition(q_1, multiplication(-1.0, q_2)));
}

Vector camera = {
    0.0,
    -4.2e6,
    2.0e6
};

inline void drawBox() {
	glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);                  // Container
    glLineWidth(1e4f);
    
    glVertex3d(-1e6, -1e6, 0.0);
    glVertex3d(-1e6, 1e6, 0.0);
    glVertex3d(-1e6, 1e6, 0.0);
    glVertex3d(-1e6, 1e6, 1e6);
    glVertex3d(-1e6, 1e6, 1e6);
    glVertex3d(-1e6, -1e6, 1e6);
    glVertex3d(-1e6, -1e6, 1e6);
    glVertex3d(-1e6, -1e6, 0.0);
    
    glVertex3d(1e6, -1e6, 0.0);
    glVertex3d(1e6, 1e6, 0.0);
    glVertex3d(1e6, 1e6, 0.0);
    glVertex3d(1e6, 1e6, 1e6);
    glVertex3d(1e6, 1e6, 1e6);
    glVertex3d(1e6, -1e6, 1e6);
    glVertex3d(1e6, -1e6, 1e6);
    glVertex3d(1e6, -1e6, 0.0);
    
    glVertex3d(-1e6, 1e6, 0.0);
    glVertex3d(1e6, 1e6, 0.0);
    glVertex3d(-1e6, 1e6, 1e6);
    glVertex3d(1e6, 1e6, 1e6);
    glVertex3d(-1e6, -1e6, 0.0);
    glVertex3d(1e6, -1e6, 0.0);
    glVertex3d(-1e6, -1e6, 1e6);
    glVertex3d(1e6, -1e6, 1e6);
    
	double xi = floor(xCenter / xSize) * xSize * 2.0 / xRange;
	while (xi > -1.0) {
		glVertex3d(xi * 1e6, -1e6, 0.0);
		glVertex3d(xi * 1e6, 1e6, 0.0);
		glVertex3d(xi * 1e6, -1e6, 0.0);
		glVertex3d(xi * 1e6, -1e6, 1e6);
		xi -= 2.0 * xSize / xRange;
	}
	xi = ceil(xCenter / xSize) * xSize * 2.0 / xRange;
	while (xi < 1.0) {
		glVertex3d(xi * 1e6, -1e6, 0.0);
		glVertex3d(xi * 1e6, 1e6, 0.0);
		glVertex3d(xi * 1e6, -1e6, 0.0);
		glVertex3d(xi * 1e6, -1e6, 1e6);
		xi += 2.0 * xSize / xRange;
	}
	
	double yi = floor(yCenter / ySize) * ySize * 2.0 / yRange;
	while (yi > -1.0) {
		glVertex3d(-1e6, yi * 1e6, 0.0);
		glVertex3d(1e6, yi * 1e6, 0.0);
		glVertex3d(-1e6, yi * 1e6, 0.0);
		glVertex3d(-1e6, yi * 1e6, 1e6);
		yi -= 2.0 * ySize / yRange;
	}
	yi = ceil(yCenter / ySize) * ySize * 2.0 / yRange;
	while (yi < 1.0) {
		glVertex3d(-1e6, yi * 1e6, 0.0);
		glVertex3d(1e6, yi * 1e6, 0.0);
		glVertex3d(-1e6, yi * 1e6, 0.0);
		glVertex3d(-1e6, yi * 1e6, 1e6);
		yi += 2.0 * ySize / yRange;
	}
	
	double zi = zSize / zRange;
	while (zi < 1.0) {
//		glVertex3d(-1e6 - 1e4, -1e6 + 1e4, zi * 1e6);
//		glVertex3d(-1e6 + 1e4, -1e6 - 1e4, zi * 1e6);
		glVertex3d(-1e6, -1e6, zi * 1e6);
		glVertex3d(-1e6, 1e6, zi * 1e6);
		glVertex3d(-1e6, -1e6, zi * 1e6);
		glVertex3d(1e6, -1e6, zi * 1e6);
		zi += zSize / zRange;
	}
    
    glEnd();
    
    glTranslated(-2.0 * xCenter / xRange * 1e6, -1e6, 0.0);
    glutSolidSphere(R, 10, 10);
    glTranslated(2.0 * xCenter / xRange * 1e6 - 1e6, 1e6 - 2.0 * yCenter / yRange * 1e6, 0.0);
    glutSolidSphere(R, 10, 10);
    glTranslated(1e6, 2.0 * yCenter / yRange * 1e6, 0.0);
}

void PlotDisplay() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 2.0, 1e5, 1e7);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.2e6, 0.0, 0.0, 1.0);
    
	glBegin(GL_LINES);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			glColor3f(1.0f * sin(z[i][j] / zRange * M_PI / 2.0), 1.0f * sin(z[i][j] / zRange * M_PI), 1.0f * cos(z[i][j] / zRange * M_PI));
			glVertex3d(-1e6 + 2e6 * i / N, -1e6 + 2e6 * j / N, 1e6 * z[i][j] / zRange);
			glVertex3d(-1e6 + 2e6 * (i + 1) / N, -1e6 + 2e6 * j / N, 1e6 * z[i + 1][j] / zRange);
			glVertex3d(-1e6 + 2e6 * i / N, -1e6 + 2e6 * j / N, 1e6 * z[i][j] / zRange);
			glVertex3d(-1e6 + 2e6 * i / N, -1e6 + 2e6 * (j + 1) / N, 1e6 * z[i][j + 1] / zRange);
		}
	}
	for (int i = 0; i < N; i++) {
		glColor3f(1.0f * sin(z[i][N] / zRange * M_PI / 2.0), 1.0f * sin(z[i][N] / zRange * M_PI), 1.0f * cos(z[i][N] / zRange * M_PI / 2.0));
		glVertex3d(-1e6 + 2e6 * i / N, 1e6, 1e6 * z[i][N] / zRange);
		glVertex3d(-1e6 + 2e6 * (i + 1) / N, 1e6, 1e6 * z[i + 1][N] / zRange);
		
		glColor3f(1.0f * sin(z[N][i] / zRange * M_PI / 2.0), 1.0f * sin(z[N][i] / zRange * M_PI), 1.0f * cos(z[N][i] / zRange * M_PI / 2.0));
		glVertex3d(1e6, -1e6 + 2e6 * i / N, 1e6 * z[N][i] / zRange);
		glVertex3d(1e6, -1e6 + 2e6 * (i + 1) / N, 1e6 * z[N][i + 1] / zRange);
	}
	glEnd();
    
    drawBox();
    
    glFlush();
    glutSwapBuffers();
}

void timeEvolution() {
	camera.x = 4.2e6 * sin(t);
	camera.y = -4.2e6 * cos(t);
//	t += 0.005;
	t = M_PI / 6.0 * 5.0;
    PlotDisplay();
}


int main(int argc, char** argv) {
   
	double d = 100, f = 400, n = 1.5;
	double C = sqrt(pow(f,2.0) + pow(d / 2.0, 2.0));
    for (int i = 0; i <= N; i++) {
    	for (int j = 0; j <= N; j++) {
    		double x = xCenter - xRange / 2.0 + xRange * i / N;
    		double y = yCenter - yRange / 2.0 + yRange * j / N;
			if(-d / 2.0 <= x && x <= d / 2.0 && -d / 2.0 <= y && y <= d / 2.0)
				z[i][j] = (2 * (n * C - f) - sqrt(4 * pow(n * C - f, 2.0) - 4 * (n * n - 1) * (C * C - f * f - x * x))) / (2 * (n * n - 1));
    	}
    }
	printf("h_0 = %f\n", (n * C - f) / (n * n - 1) - sqrt(pow(n * C - f, 2.0) / pow(n * n - 1, 2.0) - (C * C - f * f) / (n * n - 1)));      // Maximum Thickness
	printf("R = %f\n", sqrt(pow(n * C - f, 2.0) - (n * n - 1) * (C * C - f * f)));      // Radius of Curvature
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(60, 120);
    glutInitWindowSize(1400, 700);
    glutCreateWindow("3D Plotter");
    glutDisplayFunc(&PlotDisplay);
    glutIdleFunc(&timeEvolution);
    glutMainLoop();
    
    return 0;
}


