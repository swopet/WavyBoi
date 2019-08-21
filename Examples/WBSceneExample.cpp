#include "pch.h"
#include "WBSceneExample.h"

struct GLVector3f {
	GLfloat x, y, z;
};

float length(GLVector3f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

GLVector3f operator-(GLVector3f vec) {
	return GLVector3f({ -vec.x,-vec.y,-vec.z });
}

GLVector3f operator/(GLVector3f vec, float val) {
	return GLVector3f({vec.x / val, vec.y / val,vec.z / val });
}

GLVector3f operator+(GLVector3f vec, GLVector3f other) {
	return GLVector3f({ vec.x + other.x, vec.y + other.y, vec.z + other.z });
}

GLVector3f operator-(GLVector3f vec, GLVector3f other) {
	return GLVector3f({vec.x - other.x,vec.y - other.y,vec.z - other.z });
}

GLVector3f operator*(GLVector3f vec, float val) {
	return GLVector3f({vec.x * val, vec.y * val, vec.z * val });
}

GLVector3f normalize(GLVector3f vec) {
	float len = length(vec);
	if (len != 0) {
		return vec / length(vec);
	}
	else {
		return vec;
	}
}

float dot(GLVector3f vec1, GLVector3f vec2) {
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

GLVector3f cross(GLVector3f vec1, GLVector3f vec2) {
	GLfloat x, y, z;
	x = vec1.y * vec2.z - vec1.z * vec2.y;
	y = vec1.x * vec2.z - vec1.z * vec2.x;
	z = vec1.x * vec2.y - vec1.y * vec2.x;
	return GLVector3f({ x,y,z });
}

std::ostream& operator<<(std::ostream &os, GLVector3f vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

void WBSceneExample::draw()
{
	render_target->pushGLStates();
	render_target->setActive();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, render_target->getSize().x, render_target->getSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = (double)render_target->getSize().x / (double)render_target->getSize().y;
	double size = 10.0;
	//glFrustum(-ratio * size, ratio*size, -size, size, 2.0, 100.0);
	glOrtho(-ratio*size, ratio*size, -size, size, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	int num_points = 144;
	int num_divs = 24;
	double radius = 4.0;
	float strand_thickness = radius / 12.0;
	double bass = 1.0;
	double mids = 1.0;
	try {
		Parameter param = params.at(std::string("bass"));
		bass = param.getValue().float_val;
	}
	catch (std::out_of_range ex){
		bass = 0.0;
	}
	try {
		Parameter param = params.at(std::string("mids"));
		mids = param.getValue().float_val;
	}
	catch (std::out_of_range ex) {
		mids = 0.0;
	}
	bass = 0.9 * last_bass + 0.1 * bass;
	last_bass = bass;
	//PUT YOUR OPENGL rendering code here my friend
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0, 1.0, 1.0);
	int seeds[] = {7,17,37,47,59,67};
	GLVector3f * points = (GLVector3f *)malloc(num_points*sizeof(GLVector3f));
	GLfloat * real_points = (GLfloat *)malloc(num_points*num_divs * 6 * sizeof(GLfloat));
	GLuint * indices = (GLuint *)malloc(num_points*num_divs * 3 * 2 * sizeof(GLuint));
	for (int i = 0; i < num_points; i++) {
		double radians = (double)i / (double)num_points * 6.283;
		double offset = -0.2 + 0.4*SimplexNoise::noise(7 + cos(radians + time / 12.0), sin(radians + time / 12.0), time / 6.0);
		offset = offset * bass;
		double rad_offset = -0.01 + 0.02 * SimplexNoise::noise(13 + cos(radians + time / 12.0), sin(radians + time / 12.0), time / 6.0);
		rad_offset = rad_offset * bass;
		GLfloat x = (radius) * (1.0 + offset) * cos(radians + rad_offset);
		GLfloat y = (radius) * (1.0 + offset) * sin(radians + rad_offset);
		GLfloat z = 0.0;
		points[i].x = x;
		points[i].y = y;
		points[i].z = z;
	}
	for (int i = 0; i < num_points; i++) {

		GLVector3f l_1 = normalize(points[i] - points[(i + num_points - 1) % num_points]);
		GLVector3f l_2 = normalize(points[i] - points[(i + 1) % num_points]);
		GLVector3f N_2 = normalize(l_1 + l_2);
		GLVector3f N_1 = normalize(cross(l_1, l_2));
		if (N_1.z > 0){
			N_1 = -N_1;
			N_2 = -N_2;
		}
		
		if (length(N_2) == 0) {
			N_2 = normalize(cross(l_1, N_1));
		}
		for (int j = 0; j < num_divs; j++) {
			float radians = ((float)(j) + (float)(i%(num_divs*2))*0.5) * 6.283 / (float)num_divs;
			GLVector3f norm = N_1 * cos(radians) + N_2 * sin(radians);
			GLVector3f temp_vec = points[i] + (norm * strand_thickness);
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex3f(temp_vec.x, temp_vec.y, temp_vec.z);
			int ind = i * num_divs + j;
			real_points[ind * 6] = norm.x;
			real_points[ind * 6 + 1] = norm.y;
			real_points[ind * 6 + 2] = norm.z;
			real_points[ind * 6 + 3] = temp_vec.x;
			real_points[ind * 6 + 4] = temp_vec.y;
			real_points[ind * 6 + 5] = temp_vec.z;
			indices[ind * 6] = ind;
			indices[ind * 6 + 1] = ((i + 1) % num_points)*num_divs + j;
			indices[ind * 6 + 2] = i * num_divs + (j + 1) % num_divs;
			indices[ind * 6 + 3] = ind;
			indices[ind * 6 + 4] = ((i + 1) % num_points)*num_divs + (j + num_divs - 1) % num_divs;
			indices[ind * 6 + 5] = ((i + 1) % num_points)*num_divs + j;
		}
	}
	glPushMatrix();
	glTranslatef(0.0, 0.0, -radius*5.0);
	glRotatef(time * 12.0 - 1.0 * mids, 1.0, 1.0, 1.0);
	glInterleavedArrays(GL_N3F_V3F, 0, real_points);
	for (int i = -3; i <= 3; i++) {
		glPushMatrix();
		glTranslatef(i * radius * 1.5, 0.0, 0.0);
		glRotatef(90.0 * i, 1.0, 0.0, 0.0);
		glDrawElements(GL_TRIANGLES, num_points*num_divs * 3 * 2, GL_UNSIGNED_INT, indices);
		glPopMatrix();
	}
	glPopMatrix();
	free(points);
	free(real_points);
	free(indices);
	render_target->display();
	render_target->popGLStates();
}

void WBSceneExample::update()
{
	time = clock.getElapsedTime().asSeconds();
	draw();
}

void WBSceneExample::setParamsToDefault()
{
}

WBSceneExample::WBSceneExample()
{
	context_settings.depthBits = 32;
	render_target = new sf::RenderTexture();
	render_target->create(1080, 720, context_settings);
	std::cout << "initializing example" << std::endl;
	clock = sf::Clock();
}


WBSceneExample::~WBSceneExample()
{
}

extern "C" {

	__declspec(dllexport) WBScene* GetWBScene() {
		return new WBSceneExample();
	}

}