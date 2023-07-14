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

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	playerBody = (Player*)AddGo(new Player("graphics/potato_body_default.png", "PlayerBody"));
	playerBody->sortLayer = 2;
	playerBody->SetOrigin(Origins::BC);

	playerLeftLegs = (Player*)AddGo(new Player("graphics/potato_left_legs_default.png", "PlayerLeftLegs"));
	playerLeftLegs->sortLayer = 1;
	playerLeftLegs->SetOrigin(Origins::BC);

	playerRightLegs = (Player*)AddGo(new Player("graphics/potato_right_legs_default.png", "PlayerRightLegs"));
	playerRightLegs->sortLayer = 1;
	playerRightLegs->SetOrigin(Origins::BC);

	background = (SpriteGo*)AddGo(new SpriteGo("graphics/bg.png", "Bg"));
	background->sortLayer = 0;
	background->SetOrigin(Origins::MC);


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

void SceneGame::Reset()
{
}

void SceneGame::Enter()
{
	Scene::Enter();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneGame::Exit()
{
	playerBody->Reset();
	background->Reset();

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
