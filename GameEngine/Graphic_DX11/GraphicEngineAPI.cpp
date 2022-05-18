#include "GraphicEngineAPI.h"
#include "Graphic_DX11.h"

GraphicEngine* GraphicEngine::Graphic = nullptr;

GraphicEngine* GraphicEngine::Get()
{
	if (Graphic == nullptr)
	{
		Graphic = new Graphic_DX11();
	}

	return Graphic;
}