#define _USE_MATH_DEFINES
#include<stdlib.h>
#include<GL/glut.h>
#include<glm/glm.hpp>
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<cmath>

using namespace std;
using namespace glm;

vec3 s = vec3(0.0, 0.0, 1.0), e = vec3(0.0, 0.0, 0.0), os = vec3(0.0, 0.0, 0.0), objCenter, center;
vector<vec3> allVertices, allPolygons, splineVertices, p, tangents;
GLint numberOfVertices = 0, numberOfPolygons = 0, numberOfSplineVertices = 0, numberOfSegments,
	perspective, width = 500, height = 500, window, lastTime, timeNow, segment;

void myDisplay();
void myIdle();
void myReshape(int width, int height);

vec3 getVertices(int i) {
	for (int j = 1; j <= allVertices.size(); j++) {		// Dobivanje svih toèaka jednog poligona
		if (j == i) {
			return allVertices[j - 1];
		}
	}
}

vector<string> splitString(string s, string delimiter) {		// funkcija koja razdvaja string na dijelove ovisno o stringu koji mu predamo
	size_t pos = 0;												// npr predamo mu neki string i možemo rastaviti reèenicu na rijeèi tako da mu predamo string jednog razmaka
	vector<string> tokens;
	while ((pos = s.find(delimiter)) != string::npos) {
		tokens.push_back(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s);
	return tokens;
}

void readSplineFile(string fileName) {							//Èitanje spline datoteke i zapis toèaka u varijablu splineVertices
	ifstream file;
	file.open(fileName);
	string line;
	center = vec3(0, 0, 0);
	vec3 minimum(0, 0, 0);
	bool firstVec = true;
	while (getline(file, line)) {
		vector<string> split = splitString(line, " ");
		splineVertices.push_back({ stod(split[0]), stod(split[1]), stod(split[2]) });
		numberOfSplineVertices++;
		if (firstVec) {
			minimum.x = stod(split[0]);
			minimum.y = stod(split[1]);
			minimum.z = stod(split[2]);
			firstVec = false;
		}
		if (stod(split[0]) < minimum.x) {
			minimum.x = stod(split[0]);
		}
		if (stod(split[1]) < minimum.y) {
			minimum.y = stod(split[1]);
		}
		if (stod(split[2]) < minimum.z) {
			minimum.z = stod(split[2]);
		}
		if (stod(split[0]) > center.x) {
			center.x = stod(split[0]);
		}
		if (stod(split[1]) > center.y) {
			center.y = stod(split[1]);
		}
		if (stod(split[2]) > center.z) {
			center.z = stod(split[2]);
		}
	}
	center.x = (center.x + minimum.x) / 2;
	center.y = (center.y + minimum.y) / 2;
	center.z = (center.z + minimum.z) / 2;
	/*cout << "Spline vertices: " << endl;
	for (int i = 0; i < splineVertices.size(); i++) {
		cout << splineVertices[i].x << " " << splineVertices[i].y << " " << splineVertices[i].z << endl;
	}*/
	numberOfSegments = splineVertices.size() - 3;
}

void readFile(string fileName) {
	ifstream file;
	file.open(fileName);							// Èitaju se linija po linija, broji se kolièina poligona i toèaka te se upisuju u svoje vektore
	string line;
	while (getline(file, line)) {
		if (line.at(0) == 'v') {
			vector<string> split = splitString(line, " ");
			allVertices.push_back({ stod(split[1]), stod(split[2]), stod(split[3]) });
			numberOfVertices++;
		}
		if (line.at(0) == 'f') {
			vector<string> split = splitString(line, " ");
			allPolygons.push_back({ stod(split[1]), stod(split[2]), stod(split[3]) });
			numberOfPolygons++;
		}
	}
	objCenter.x = 0;
	objCenter.y = 0;
	objCenter.z = 0;
	for (int i = 0; i < allVertices.size(); i++) {
		objCenter.x += allVertices[i].x;
		objCenter.y += allVertices[i].y;
		objCenter.z += allVertices[i].z;
		//cout << allVertices[i].x << " " << allVertices[i].y << " " << allVertices[i].z << endl;
	}
	objCenter.x /= numberOfVertices;
	objCenter.y /= numberOfVertices;
	objCenter.z /= numberOfVertices;
	//cout << centar.x << " " << centar.y << " " << centar.z << endl;
}

void calculateBSpline() {							//Raèunanje aproksimacije krivulje i tangenti
	vec3 vec;
	for (int i = 0; i < numberOfSegments; i++) {
		vec3 r1 = splineVertices[i], r2 = splineVertices[i + 1], r3 = splineVertices[i + 2], r4 = splineVertices[i + 3];
		for (int t = 0; t < 100; t++) {
			GLdouble b1, b2, b3, b4, t1, t2, t3, t4, j = t / 100.0;
			b1 = (-pow(j, 3) + 3 * pow(j, 2) - 3 * j + 1) / 6;
			b2 = (3 * pow(j, 3) - 6 * pow(j, 2) + 4) / 6;
			b3 = (-3 * pow(j, 3) + 3 * pow(j, 2) + 3 * j + 1) / 6;
			b4 = pow(j, 3) / 6;
			vec.x = b1 * r1.x + b2 * r2.x + b3 * r3.x + b4 * r4.x;
			vec.y = b1 * r1.y + b2 * r2.y + b3 * r3.y + b4 * r4.y;
			vec.z = b1 * r1.z + b2 * r2.z + b3 * r3.z + b4 * r4.z;
			p.push_back(vec);
			tangents.push_back(vec);

			t1 = 0.5 * (-pow(j, 2.0) + 2 * j - 1);
			t2 = 0.5 * (3 * pow(j, 2.0) - 4 * j);
			t3 = 0.5 * (-3 * pow(j, 2.0) + 2 * j + 1);
			t4 = 0.5 * (pow(j, 2.0));
			vec.x = (t1 * r1.x + t2 * r2.x + t3 * r3.x + t4 * r4.x) / 2;
			vec.x = tangents[tangents.size() - 1].x + vec.x;
			vec.y = (t1 * r1.y + t2 * r2.y + t3 * r3.y + t4 * r4.y) / 2;
			vec.y = tangents[tangents.size() - 1].y + vec.y;
			vec.z = (t1 * r1.z + t2 * r2.z + t3 * r3.z + t4 * r4.z) / 2;
			vec.z = tangents[tangents.size() - 1].z + vec.z;
			tangents.push_back(vec);
		}
	}
}

GLdouble calculateAngle() {							//Raèunanje rotacije objekta
	e.x = tangents[2 * segment + 1].x - tangents[2 * segment].x;
	e.y = tangents[2 * segment + 1].y - tangents[2 * segment].y;
	e.z = tangents[2 * segment + 1].z - tangents[2 * segment].z;
	os.x = s.y * e.z - e.y * s.z;
	os.y = e.x * s.z - s.x * e.z;
	os.z = s.x * e.y - s.y * e.x;
	GLdouble angle = (s.x * e.x + s.y * e.y + s.z * e.z) /
		(sqrt(s.x * s.x + s.y * s.y + s.z * s.z) * sqrt(e.x * e.x + e.y * e.y + e.z * e.z));
	return acos(angle) * 180 / M_PI;
}

int main(int argc, char** argv)
{
	cout << "Perspektiva: ";
	cin >> perspective;
	readFile("ptica.obj");
	readSplineFile("bSpline.txt");
	calculateBSpline();
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("BSpline");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();
	return 0;
}

void myIdle() {
	timeNow = glutGet(GLUT_ELAPSED_TIME);
	if (timeNow - lastTime > 5) {
		lastTime = timeNow;
		myDisplay();
	}
}

void myReshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / height, 0.5, 100.0);
	if (perspective == 1) {
		gluLookAt(center.x + 60, center.y, center.z, center.x, center.y, center.z, 0.0, 0.0, 1.0);
	}
	else if (perspective == 2) {
		gluLookAt(center.x, center.y, center.z + 35, center.x, center.y, center.z, 1.0, 0.0, 0.0);
	}
	else{
		gluLookAt(center.x + 60, center.y, center.z, center.x, center.y, center.z, 0.0, -1.0, 0.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

void myDisplay()
{
	glColor3f(0.0, 0.0, 0.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_LINE_STRIP);							//Crtanje krivulje
	for (int i = 0; i < 100 * numberOfSegments; i++) {
		glVertex3f(p[i].x, p[i].y, p[i].z);
	}
	glEnd();

	glBegin(GL_LINES);								//Crtanje tangenti
	for (int i = 0; i < tangents.size()-1; i += 1) {
		if (i % 40 == 0) {
			glVertex3f(tangents[i].x, tangents[i].y, tangents[i].z);
			glVertex3f(tangents[i+1].x, tangents[i+1].y, tangents[i + 1].z);
		}
	}
	glEnd();

	glTranslatef(p[segment].x, p[segment].y, p[segment].z);			//Pomicanje i rotacija objekta
	glRotatef(calculateAngle(), os.x, os.y, os.z);
	glTranslatef(-objCenter.x, -objCenter.y, -objCenter.z);

	glBegin(GL_LINES);								//Crtanje objekta
	for (int i = 0; i < numberOfPolygons; i++) {
		vec3 v1, v2, v3;
		v1.x = getVertices(allPolygons[i].x).x;
		v1.y = getVertices(allPolygons[i].x).y;
		v1.z = getVertices(allPolygons[i].x).z;
		v2.x = getVertices(allPolygons[i].y).x;
		v2.y = getVertices(allPolygons[i].y).y;
		v2.z = getVertices(allPolygons[i].y).z;
		v3.x = getVertices(allPolygons[i].z).x;
		v3.y = getVertices(allPolygons[i].z).y;
		v3.z = getVertices(allPolygons[i].z).z;
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	glEnd();
	
	segment++;
	if (segment >= 100 * numberOfSegments){ 
		segment = 0;
	}
	glFlush();
}