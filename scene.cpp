#include "scene.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

void CScene::update()
{
	for (auto &p : m_objects)
		p->update();
}

void CScene::render() const
{
	/*gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
		objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
		objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);*/
	for (auto &p : m_objects)
		p->render();
}
