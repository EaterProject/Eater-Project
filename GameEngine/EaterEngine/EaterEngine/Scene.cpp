#include "Scene.h"
Scene::Scene()
{
	Name = "Scene";
	ThreadRun = false;
}
Scene::~Scene()
{

}

void Scene::ThreadFunction()
{

}

void Scene::Run()
{
	ThreadRun = true;
	ThreadFunction();
	ThreadRun = false;
}



