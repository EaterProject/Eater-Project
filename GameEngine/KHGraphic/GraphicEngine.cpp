#include "GraphicsEngine.h"
#include "KHGraphic.h"

GraphicEngine* GraphicEngine::Create()
{
	GraphicEngine* graphicEngine = new KHGraphic();

	return graphicEngine;
}