#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <glut.h>
void emptyWindow(void) {
}
int main(int argc, char* argv[]) {
	// Initialize GLUT
	glutInit(&argc, argv);
	// Set the window size and position
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello, GL");
	glutDisplayFunc(emptyWindow);
	glutMainLoop();
}
