#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneGame.h"
#include "Player.h"
#include "Button.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
}

void SceneGame::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	Player* player = (Player*)AddGo(new Player());

	/*
	Button* button = (Button*)AddGo(new Button("graphics/button.png", "Button"));
	button->SetOrigin(Origins::TR);
	button->sortLayer = 100;
	button->SetPosition(size.x, 0.0f);
	button->OnEnter = [button]()
	{
		sf::Texture* tex = RESOURCE_MGR.GetTexture("graphics/button2.png");
		button->sprite.setTexture(*tex);

		std::cout << "Enter" << std::endl;
	};
	button->OnExit = [button]()
	{
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->textureId);
		button->sprite.setTexture(*tex);

		std::cout << "Exit" << std::endl;
	};
	button->OnClick = [this]() 
	{
		std::cout << "Click" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Title);
	};
	*/

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
