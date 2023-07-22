#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "Button.h"
#include "BaseGun.h"
#include "BaseBullet.h"
#include "VertexArrayGo.h"
#include "TextGo.h"
#include <sstream>

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
}

void SceneGame::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	player = (Player*)AddGo(new Player("Player"));
	player->sortLayer = 1;

	BaseGun* baseGun = (BaseGun*)AddGo(new BaseGun(player, "graphics/gun.png", "BaseGun"));
	baseGun->sortLayer = 4;
	baseGun->SetPosition(player->GetPosition());

	Button* button = (Button*)AddGo(new Button("graphics/button.png", "Button"));
	button->SetOrigin(Origins::TL);
	button->sortLayer = 100;
	button->SetPosition(50.0f, 50.0f);
	button->SetColor(0, 0, 0, 192);
	button->SetString("test");

	button->OnEnter = [button]()
	{
		button->SetColor(255, 0, 0, 192);

		std::cout << "Enter" << std::endl;
	};
	button->OnExit = [button]()
	{
		button->SetColor(0, 0, 0, 192);

		std::cout << "Exit" << std::endl;
	};
	button->OnClick = [this]()
	{
		std::cout << "Click" << std::endl;
		//SCENE_MGR.ChangeScene(SceneId::Title);
	};

	SpriteGo* background = (SpriteGo*)AddGo(new SpriteGo("graphics/bg.png", "Bg"));
	background->sortLayer = -2;
	background->SetOrigin(Origins::MC);

	SpriteGo* backgroundOutline = (SpriteGo*)AddGo(new SpriteGo("graphics/tiles_outline.png", "BgOutline"));
	backgroundOutline->sortLayer = 0;
	backgroundOutline->SetOrigin(Origins::MC);

	sf::Vector2f tileWorldSize   = { 64.0f, 64.0f };
	sf::Vector2f tileTextureSize = { 64.0f, 64.0f };

	VertexArrayGo* tile = CreateTile("graphics/tile.png", { 32, 32 }, tileWorldSize, tileTextureSize);
	tile->sortLayer = -1;
	tile->SetOrigin(Origins::MC);
	tile->SetPosition(0.0f, 0.0f);

	wallBounds = tile->vertexArray.getBounds();

	wallBounds.left   += tileWorldSize.x;
	wallBounds.top    += tileWorldSize.y;
	wallBounds.width  -= tileWorldSize.x * 2.0f;
	wallBounds.height -= tileWorldSize.y * 2.0f;
	player->SetWallBounds(wallBounds);
	
	AddGo(tile);

	CreateBulletHitEffect(500);
	CreateMonsters(500);

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
	ClearObjectPool(monsterPool);
	ClearObjectPool(bulletHitEffectPool);

	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void SceneGame::Enter()
{
	Scene::Enter();

	ClearObjectPool(monsterPool);
	ClearObjectPool(bulletHitEffectPool);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneGame::Exit()
{
	ClearObjectPool(monsterPool);
	ClearObjectPool(bulletHitEffectPool);

	player->Reset();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (player == nullptr)
	{
		return;
	}
	
	currentPlayerPosition = player->GetPosition();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnMonsters(10, currentPlayerPosition, { 0.0f, 0.0f }, 950.0f);
	}

	sf::Vector2f halfViewSize = worldView.getSize() * 0.5f;

	float wallEdgeView = 100.0f;
	halfViewSize -= sf::Vector2f(wallEdgeView, wallEdgeView);

	sf::Vector2f minValue = { wallBounds.left + halfViewSize.x, wallBounds.top + halfViewSize.y };
	sf::Vector2f maxValue = { wallBounds.left + wallBounds.width - halfViewSize.x, wallBounds.top + wallBounds.height - halfViewSize.y };

	sf::Vector2f clampPlayerPosistion = Utils::Clamp(currentPlayerPosition, minValue, maxValue);

	worldView.setCenter(clampPlayerPosistion);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

VertexArrayGo* SceneGame::CreateTile(std::string textureId, sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize)
{
	VertexArrayGo* tile = new VertexArrayGo(textureId, "Tile");

	tile->vertexArray.setPrimitiveType(sf::Quads);
	tile->vertexArray.resize(size.x * size.y * 4);

	sf::Vector2f startPos = { 0.0f, 0.0f };
	sf::Vector2f offsets[4] =
	{
		{ 0.0f, 0.0f },
		{ tileSize.x, 0.0f },
		{ tileSize.x, tileSize.y },
		{ 0.0f, tileSize.y }
	}; 
	sf::Vector2f texOffsets[4] =
	{
		{ 0.0f, 0.0f },
		{ texSize.x, 0.0f },
		{ texSize.x, texSize.y },
		{ 0.0f, texSize.y }
	};

	sf::Vector2f currPos = startPos;
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			if (Utils::RandomRange(0, 20) != 0) // 20% 확률로 타일을 그림
			{
				currPos.x += tileSize.x;
				continue;
			}

			int textureIndex = Utils::RandomRange(0, 12);

			int tileIndex = size.x * i + j;

			int textureRow = textureIndex / 3;
			int textureCol = textureIndex % 4;

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = tileIndex * 4 + k;
				tile->vertexArray[vertexIndex].position = currPos + offsets[k];
				tile->vertexArray[vertexIndex].texCoords = texOffsets[k];
				tile->vertexArray[vertexIndex].texCoords.x += texSize.x * textureCol;
				tile->vertexArray[vertexIndex].texCoords.y += texSize.y * textureRow;
			}
			currPos.x += tileSize.x;
		}
		currPos.x = startPos.x;
		currPos.y += tileSize.y;
	}

	return tile;
}

template<typename T>
inline void SceneGame::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}

void SceneGame::CreateMonsters(int count)
{
	monsterPool.OnCreate = [this](Monster* monster)
	{
		Monster::Types monsterType = (Monster::Types)Utils::RandomRange(0, 3);
		monster->SetType(monsterType);
		monster->SetPlayer(player);
		monster->sortLayer = 1;
		monster->SetActive(false);
		monster->SetBulletHitEffectPool(&bulletHitEffectPool);
	};
	monsterPool.Init(count);
}

void SceneGame::SpawnMonsters(int count, sf::Vector2f playerCenter, sf::Vector2f mapCenter, float radius)
{
	for (int i = 0; i < count; i++)
	{
		Monster* monster = monsterPool.Get();
		sf::Vector2f spawnPosition;
		monster->SetActive(true);

		do {
			spawnPosition = mapCenter + Utils::RandomInCircle(radius);
		} while (Utils::Distance(playerCenter, spawnPosition) < 50.0f && Utils::Distance(mapCenter, spawnPosition) < 950.0f);

		monster->SetPosition(spawnPosition);
		monster->Reset();

		AddGo(monster);
	}

	monsterCount = monsterPool.GetUseList().size();
	//TextGo* uiZombieCount = (TextGameObj*)FindGo("uiZombieCount");
	//stringstream ss;
	//ss << "Zombie : " << zombiecount;  //남은 수 표시 
	//uiZombieCount->SetString(ss.str());
}

ObjectPool<Monster>& SceneGame::GetMonsterPool()
{
	return monsterPool;
}

const std::list<Monster*>* SceneGame::GetMonsterList()
{
	return &monsterPool.GetUseList();
}

ObjectPool<SpriteEffect>& SceneGame::GetBulletHitEffectPool()
{
	return bulletHitEffectPool;
}

void SceneGame::CreateBulletHitEffect(int count)
{
	bulletHitEffectPool.OnCreate = [this](SpriteEffect* bulletHitEffect)
	{
		bulletHitEffect->SetName("BulletHitEffect");
		bulletHitEffect->SetDuration(0.1f);
		bulletHitEffect->textureId = "graphics/bullet_hit.png";
		bulletHitEffect->sortLayer = 1;
		bulletHitEffect->sortOrder = 1;
		bulletHitEffect->SetActive(false);
		bulletHitEffect->SetPool(&bulletHitEffectPool);
	};
	bulletHitEffectPool.Init(count);
}

Monster* SceneGame::GetNearMonsterSearch()
{
	const std::list<Monster*>* monsterList = GetMonsterList();
	Monster* nearMonster = nullptr;

	float nearMonsterSearchDistance = 700.0f;

	for (Monster* monster : *monsterList)
	{
		float distance = Utils::Distance(player->GetPosition(), monster->GetPosition());
		if (distance <= nearMonsterSearchDistance)
		{
			nearMonsterSearchDistance = distance;
			nearMonster = monster;
		}
	}
	return nearMonster;
}

void SceneGame::OnDieMonster(Monster* monster)
{
	RemoveGo(monster);
	monsterPool.Return(monster);

	//sf::Vector2f pos = monster->GetPosition();
	//monsterCount = monsterPool.GetUseList().size();
	//TextGo* uiMonsterCount = (TextGo*)FindGo("uiMonsterCount");
	//stringstream ss;
	//ss << "Zombie : " << zombiecount;
	//uiZombieCount->SetString(ss.str());

	//AddScore(1);
	//SpawnItem(pos);
}