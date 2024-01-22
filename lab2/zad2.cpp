#define _USE_MATH_DEFINES
#include <windows.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>

using namespace glm;
using namespace std;

GLuint window, width = 512, height = 512, slika;
GLdouble velIzvora = 0.5, pocAlpha = 1.0;
int lastTime, nowTime, mode = 1;
vec3 pocetnaBoja(0.0, 0.0, 0.0), s(0.0, 0.0, 1.0), os, e, ociste(0.0, 6.0, 75.0);

class Cestica {
public:
	vec3 lokacija, boja, ciljnaBoja, ciljnaLokacija, ciljnaRotacija;
	GLint vrijemeZivota;
	GLdouble alpha = 1.0, kutRotacije, velicina, ciljnaVelicina;
};
vector<Cestica> cestice;

void myDisplay();
void myIdle();
void myReshape(int width, int height);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

GLuint LoadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	unsigned char* data;

	FILE* file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 256;
	height = 256;
	data = (unsigned char*)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

void crtanje() {
	for (int i = 0; i < cestice.size(); i++) {
		glColor4f(cestice[i].boja.r, cestice[i].boja.g, cestice[i].boja.b, cestice[i].alpha);
		glTranslatef(cestice[i].lokacija.x, cestice[i].lokacija.y, cestice[i].lokacija.z);
		glRotatef(cestice[i].kutRotacije, cestice[i].ciljnaRotacija.x, cestice[i].ciljnaRotacija.y, cestice[i].ciljnaRotacija.z);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(-cestice[i].velicina, -cestice[i].velicina); 
		glTexCoord2i(0, 1); glVertex2f(-cestice[i].velicina, cestice[i].velicina);
		glTexCoord2i(1, 1); glVertex2f(cestice[i].velicina, cestice[i].velicina);
		glTexCoord2i(1, 0); glVertex2f(cestice[i].velicina, -cestice[i].velicina);
		glEnd();
		glRotatef(-cestice[i].kutRotacije, cestice[i].ciljnaRotacija.x, cestice[i].ciljnaRotacija.y, cestice[i].ciljnaRotacija.z);
		glTranslatef(-cestice[i].lokacija.x, -cestice[i].lokacija.y, -cestice[i].lokacija.z);
	}
}

Cestica novaCestica() {
	Cestica cestica;
	cestica.vrijemeZivota = rand() % 10 + 100;
	cestica.boja = pocetnaBoja;
	cestica.ciljnaBoja.r = (rand() % 2000 - 1000) / (double)1000;
	cestica.ciljnaBoja.g = (rand() % 2000 - 1000) / (double)1000;
	cestica.ciljnaBoja.b = (rand() % 2000 - 1000) / (double)1000;
	cestica.ciljnaBoja.r = cestica.ciljnaBoja.r / (double)cestica.vrijemeZivota;
	cestica.ciljnaBoja.g = cestica.ciljnaBoja.g / (double)cestica.vrijemeZivota;
	cestica.ciljnaBoja.b = cestica.ciljnaBoja.b / (double)cestica.vrijemeZivota;
	cestica.alpha = pocAlpha;
	cestica.velicina = velIzvora;
	
	if (mode == 1) {
		GLdouble br;
		vec3 konacnaLokacija;
		konacnaLokacija.x = (rand() % 2000 - 1000) / (double)1000;
		konacnaLokacija.y = (rand() % 2000 - 1000) / (double)1000;
		konacnaLokacija.z = (rand() % 2000 - 1000) / (double)1000;
		br = sqrt(pow(konacnaLokacija.x, 2) + pow(konacnaLokacija.y, 2) + pow(konacnaLokacija.z, 2));
		cestica.lokacija = vec3(0.0, 0.0, 0.0);
		cestica.ciljnaLokacija.x = konacnaLokacija.x / br;
		cestica.ciljnaLokacija.y = konacnaLokacija.y / br;
		cestica.ciljnaLokacija.z = konacnaLokacija.z / br;
		cestica.ciljnaVelicina = (rand() % 62 - 31) / (double)10;
		cestica.ciljnaVelicina = cestica.ciljnaVelicina / (double)cestica.vrijemeZivota;
	}
	else {
		cestica.lokacija = vec3(0.0, 30.0, 0.0);
		cestica.lokacija.x = (rand() % 1024 - 512) / (double)10;
		cestica.ciljnaLokacija.x = 0;
		cestica.ciljnaLokacija.y = -1.2;
		cestica.ciljnaLokacija.z = 0;
		cestica.ciljnaVelicina = -cestica.velicina / (double)cestica.vrijemeZivota;
	}
	return cestica;
}

void prilagodiCestice() {
	for (int i = 0; i < cestice.size(); i++) {
		cestice[i].vrijemeZivota--;
		if (cestice[i].vrijemeZivota < 1) {
			cestice.erase(cestice.begin() + i);
			continue;
		}		
		cestice[i].boja.r += cestice[i].ciljnaBoja.r;
		cestice[i].boja.g += cestice[i].ciljnaBoja.g;
		cestice[i].boja.b += cestice[i].ciljnaBoja.b;
		cestice[i].velicina += cestice[i].ciljnaVelicina;
		e.x = cestice[i].lokacija.x - ociste.x;
		e.y = cestice[i].lokacija.y - ociste.y;
		e.z = cestice[i].lokacija.z - ociste.z;
		os.x = s.y * e.z - e.y * s.z;
		os.y = e.x * s.z - s.x * e.z;
		os.z = s.x * e.y - s.y * e.x;
		GLdouble angle = (s.x * e.x + s.y * e.y + s.z * e.z) /
			(sqrt(s.x * s.x + s.y * s.y + s.z * s.z) * sqrt(e.x * e.x + e.y * e.y + e.z * e.z));
		cestice[i].kutRotacije = acos(angle) * 180 / M_PI;
		cestice[i].ciljnaRotacija.x = os.x;
		cestice[i].ciljnaRotacija.y = os.y;
		cestice[i].ciljnaRotacija.z = os.z;
		cestice[i].lokacija.x += 0.5 * cestice[i].ciljnaLokacija.x;
		cestice[i].lokacija.y += 0.5 * cestice[i].ciljnaLokacija.y;
		cestice[i].lokacija.z += 0.5 * cestice[i].ciljnaLokacija.z;
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Cestice");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);

	cout << "Upisi mode: ";
	cin >> mode;
	if (mode != 1) {
		pocetnaBoja = vec3(1.0, 1.0, 1.0);
		velIzvora = 1.5;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	slika = LoadTexture("snow.bmp");
	glutMainLoop();
	return 0;
}

void myReshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / height, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}


void myDisplay()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(ociste.x, ociste.y, -ociste.z);
	crtanje();
	glutSwapBuffers();
}

void myIdle() {
	nowTime = glutGet(GLUT_ELAPSED_TIME);
	if ((nowTime - lastTime) >= 60) {
		cestice.push_back(novaCestica());
		prilagodiCestice();
		myDisplay();
		lastTime = nowTime;
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'q':
		if (pocetnaBoja.r > 0.0) {
			pocetnaBoja.r -= 0.1;
		}
		break;
	case 'w':
		if (pocetnaBoja.r < 1.0) {
			pocetnaBoja.r += 0.1;
		}
		break;
	case 'a':
		if (pocetnaBoja.g > 0.0) {
			pocetnaBoja.g -= 0.1;
		}
		break;
	case 's':
		if (pocetnaBoja.g < 1.0) {
			pocetnaBoja.g += 0.1;
		}
		break;
	case 'y':
		if (pocetnaBoja.b > 0.0) {
			pocetnaBoja.b -= 0.1;
		}
		break;
	case 'x':
		if (pocetnaBoja.b < 1.0) {
			pocetnaBoja.b += 0.1;
		}
		break;
	case '-':
		if (velIzvora > 0.01) {
			velIzvora -= 0.1;
		}
		break;
	case '+':
		if (velIzvora < 3.0) {
			velIzvora += 0.1;
		}
		break;
	case 'e':
		if (pocAlpha > 0.0) {
			pocAlpha -= 0.1;
		}
		break;
	case 'r':
		if (pocAlpha < 1.0) {
			pocAlpha += 0.1;
		}
		break;
	case 27:
		exit(0);
		break;
	}
}