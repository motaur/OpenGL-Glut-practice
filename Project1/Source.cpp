#include <GL/glut.h>
#include <math.h>

// angle of rotation for the camera direction
float angle = 0;
float x = 0, y = 1, z = 5;

// actual vector representing the camera's direction
float lx = 0, lz = -1, ly = 0;

float deltaAngle = 0;
int xOrigin = -1;
int yOrigin = 0;

void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	//escape
	if (key == 27)
		exit(0);
}


void UpdateProjection(GLboolean toggle)
{
	static GLboolean s_usePerspective = GL_TRUE;

	// toggle the control variable if appropriate
	if (toggle)
		s_usePerspective = !s_usePerspective;

	// select the projection matrix and clear it out
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// choose the appropriate projection based on the currently toggled mode
	if (s_usePerspective)
	{
		// set the perspective with the appropriate aspect ratio
		glFrustum(-1.0, 1.0, -1.0, 1.0, 5, 100);
	}
	else
	{
		// set up an orthographic projection with the same near clip plane
		glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	}

	// select modelview matrix and clear it out
	glMatrixMode(GL_MODELVIEW);
} // end UpdateProjection

void drawFigure()
{
	// BIG SPHERE
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// SMALL SPHERES
	//glPushMatrix();

	glTranslatef(2, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);

	glTranslatef(-4, 0, 0.0f);
	glutSolidSphere(0.05f, 10, 10);

	glPopMatrix();
}

void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	// GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	// GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	// GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// GLfloat position[] = { 1.5f, 1.0f, 1.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	// glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	// glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0, 1.0, 0.0);

	// Draw figure
	glPushMatrix();	

		drawFigure();

	glPopMatrix();

	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1;

	switch (key) 
	{
		case GLUT_KEY_LEFT:
			angle -= 0.01;
			lx = sin(angle);
			lz = -cos(angle);
			break;

		case GLUT_KEY_RIGHT:
			angle += 0.01;
			lx = sin(angle);
			lz = -cos(angle);
			break;

		case GLUT_KEY_UP:
			x += lx * fraction;
			z += lz * fraction;
			break;

		case GLUT_KEY_DOWN:
			x -= lx * fraction;
			z -= lz * fraction;
			break;

		case GLUT_KEY_F1:
			UpdateProjection(GL_TRUE);
			break;
		
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  // state = GLUT_DOWN
			xOrigin = x;

	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lev Berlinkov HW3 - 3d figures and lights");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	//keyboard func
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// mouse functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}