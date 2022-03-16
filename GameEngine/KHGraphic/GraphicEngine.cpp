#include "GraphicsEngine.h"
#include "KHGraphic.h"

GraphicEngine* GraphicEngine::Graphic = nullptr;

GraphicEngine* GraphicEngine::Get()
{
	if (Graphic == nullptr)
	{
		Graphic = new KHGraphic();
	}

	return Graphic;
}