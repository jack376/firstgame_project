#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneGame.h"
#include "Character.h"
#include "Button.h"
#include "BaseGun.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
}

void SceneGame::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	player = (Character*)AddGo(new Character("player"));
	player->sortLayer = 2;
	player->SetOrigin(Origins::BC);

	gun = (BaseGun*)AddGo(new BaseGun(player, "graphics/gun.png", "gun"));
	gun->sortLayer = 3;
	gun->SetPosition(player->GetPosition());
	gun->SetOrigin(Origins::MR);

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
	player->Reset();
	gun->Reset();
	background->Reset();
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
	player->Reset();
	gun->Reset();
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
