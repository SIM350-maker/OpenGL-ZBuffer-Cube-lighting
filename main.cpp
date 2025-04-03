
#include <GL/glut.h>

GLfloat light_position[] = { 0.0f, 0.5f, -1.5f, 1.0f }; // Light position
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // White light

float redCubeAngleX = 0.0f, redCubeAngleY = 0.0f;
float yellowCubeAngleX = 0.0f, yellowCubeAngleY = 0.0f;
float tiltAngle = 15.0f;

GLfloat shadowMatrix[16];

// Function to calculate shadow projection matrix
void calculateShadowMatrix(GLfloat* shadowMatrix, GLfloat light[4]) {
    GLfloat groundPlane[4] = { 0.0f, 1.0f, 0.0f, 0.3f };
    GLfloat dot = groundPlane[0] * light[0] +
                  groundPlane[1] * light[1] +
                  groundPlane[2] * light[2] +
                  groundPlane[3] * light[3];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shadowMatrix[i * 4 + j] = (i == j) ? dot - groundPlane[i] * light[j] : -groundPlane[i] * light[j];
        }
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    calculateShadowMatrix(shadowMatrix, light_position);
}

void drawCube(float red, float green, float blue, float angleX, float angleY, float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glColor3f(red, green, blue);
    glutSolidCube(0.6);
    glPopMatrix();
}

void drawShadow(float angleX, float angleY, float x, float z) {
    glPushMatrix();
    glMultMatrixf(shadowMatrix);
    glTranslatef(x, 0.0f, z);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidCube(0.6);
    glPopMatrix();
}

void drawLightSource() {
    glPushMatrix();
    glTranslatef(light_position[0], light_position[1], light_position[2]);

    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1, 16, 16);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
    glutSolidSphere(0.2, 16, 16);
    glDisable(GL_BLEND);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 3.5, 0.0, 0.0, -1.5, 0.0, 1.0, 0.0);
    glRotatef(tiltAngle, 1.0f, 0.0f, 0.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPushMatrix();
    glColor3f(0.05f, 0.05f, 0.05f);
    glTranslatef(0.0f, -0.3f, -1.5f);
    glScalef(3.0f, 0.05f, 3.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    drawShadow(redCubeAngleX, redCubeAngleY, -0.75f, -1.5f);
    drawShadow(yellowCubeAngleX, yellowCubeAngleY, 0.75f, -1.5f);
    glEnable(GL_LIGHTING);

    drawCube(1.0f, 0.3f, 0.3f, redCubeAngleX, redCubeAngleY, -0.75f, -1.5f);
    drawCube(1.0f, 1.0f, 0.3f, yellowCubeAngleX, yellowCubeAngleY, 0.75f, -1.5f);

    drawLightSource();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': light_position[0] -= 0.1f; break;
        case 'd': light_position[0] += 0.1f; break;
        case 'w': light_position[1] += 0.1f; break;
        case 's': light_position[1] -= 0.1f; break;
        case 'i': yellowCubeAngleX -= 5.0f; break;
        case 'k': yellowCubeAngleX += 5.0f; break;
        case 'j': yellowCubeAngleY -= 5.0f; break;
        case 'l': yellowCubeAngleY += 5.0f; break;
    }
    calculateShadowMatrix(shadowMatrix, light_position);
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: redCubeAngleX -= 5.0f; break;
        case GLUT_KEY_DOWN: redCubeAngleX += 5.0f; break;
        case GLUT_KEY_LEFT: redCubeAngleY -= 5.0f; break;
        case GLUT_KEY_RIGHT: redCubeAngleY += 5.0f; break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cubes with Movable Light and Rotation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();

    return 0;
}
