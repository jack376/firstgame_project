#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneTitle.h"
#include "Player.h"
#include "Button.h"

SceneTitle::SceneTitle() : Scene(SceneId::Game)
{
}

void SceneTitle::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
