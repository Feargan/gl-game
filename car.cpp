#include "car.h"
#include "cuboid.h"
#include "cylinder.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CCar::CCar()
{
	constexpr auto& p = g_carPhys;
	// Kola i osie
	createComponent<CCylinder>(CVec3d(0, 1, -p.tire), p.tire, p.tireRadius * 2 - 0.1);
	createComponent<CCylinder>(CVec3d(0, 1, p.widthTires), p.tire, p.tireRadius * 2 - 0.1);
	createComponent<CCylinder>(CVec3d(p.lengthTires, 1, -p.tire), p.tire, p.tireRadius * 2 - 0.1);
	createComponent<CCylinder>(CVec3d(p.lengthTires, 1, p.widthTires), p.tire, p.tireRadius * 2 - 0.1);
	createComponent<CCylinder>(CVec3d(0, 1, 0.0), p.widthTires, p.tireRadius / 3.0);
	createComponent<CCylinder>(CVec3d(p.lengthTires, 1, 0.0), p.widthTires, p.tireRadius / 3.0);

	// Swiatla
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, 0.4), CVec3d(0.1, 0.6, 0.3));
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, 3.5), CVec3d(0.1, 0.6, 0.3));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, 3.5), CVec3d(0.1, 0.6, 0.3));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, 0.4), CVec3d(0.1, 0.6, 0.3));

	// Maska
	createComponent<CCuboid>(CVec3d(3.4, p.baseHeight, 0.11), CVec3d(p.lengthTires - p.carFrontExt - 0.4, p.carWidth - 0.11, 0.1))
		->setColor(0.2, 0.1, 0.5);
}

CCar::~CCar()
{
}

void CCar::renderComponent() const
{
	constexpr auto& p = g_carPhys;
	double alpha, x, y;

	//Nadkola
	//Lewe tylne ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, 0.1);
	}
	glEnd();

	//Prawe tylne ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carWidth);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carWidth);
	}
	glEnd();

	//Lewe przednie ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, 0.1);
	}
	glEnd();

	//Prawe przednie ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carWidth);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carWidth);
	}
	glEnd();

	//D馧 karoserii
	//Lewa strona
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, 0.1);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.lengthTires - p.tireRadius, 1, 0.1);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1, 0.1);
	glVertex3d(p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, 0.1);
	glVertex3d(-p.tireRadius, 1, 0.1);
	glVertex3d(-p.carFrontExt, 1, 0.1);
	glVertex3d(-p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glEnd();
	//Prawa strona
	//D馧 karoserii od lewej strony
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.lengthTires - p.tireRadius, 1, p.carWidth);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1, p.carWidth);
	glVertex3d(p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, p.carWidth);
	glVertex3d(-p.tireRadius, 1, p.carWidth);
	glVertex3d(-p.carFrontExt, 1, p.carWidth);
	glVertex3d(-p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glEnd();

	//Pokrycie spodu karoserii (g鏎a)
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, 1, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, 1, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);

	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, 0.1);

	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, 0.1);
	glEnd();

	//Sp鏚 nad ko豉mi
	//Tylnie lewe
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, 0.1);
		glVertex3d(x, y, p.tire + 0.1);
	}
	glEnd();
	//Przednie lewe
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, 0.1);
		glVertex3d(x, y, p.tire + 0.1);
	}
	glEnd();
	//Tylnie prawe
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carWidth - p.tire);
		glVertex3d(x, y, p.carWidth);
	}
	glEnd();
	//Przednie prawe
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carWidth - p.tire);
		glVertex3d(x, y, p.carWidth);
	}
	glEnd();

	//Karoseria
	glColor3d(1.0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(0, p.top, 0.2);
	glVertex3d(2, p.baseHeight, 0.1);
	glVertex3d(2.5, p.top, 0.1);
	glVertex3d(3.5, p.baseHeight, 0.1);

	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glVertex3d(3.5, p.baseHeight, p.carWidth);
	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glVertex3d(1, p.baseHeight, p.carWidth);
	glVertex3d(0, p.top, p.carWidth - 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(0, p.top, 0.2);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(0, p.top, p.carWidth - 0.1);
	glVertex3d(2.5, p.top, 0.1);
	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glEnd();
}

void CCar::updateComponent()
{
}
