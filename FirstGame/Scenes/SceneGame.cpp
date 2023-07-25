#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneGame.h"
#include "GameObject.h"
#include "TextGo.h"
#include "VertexArrayGo.h"
#include "Player.h"
#include "Monster.h"
#include "BaseUI.h"
#include "BaseGun.h"
#include "BaseBullet.h"
#include "DataTableMgr.h"
#include "UIShopTable.h"
#include <sstream>

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
}

void SceneGame::Init()
{
	Release();

	sf::Vector2f defaultResolution(1920.0f, 1080.0f);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos  = windowSize * 0.5f;

	float resolutionScale = windowSize.x / 1920.0f;
	float shopUiPositionX = 375.0f * resolutionScale;
	float ThumbnailSize   = 64.0f;

	player = (Player*)AddGo(new Player("Player"));
	player->sortLayer = 3;

	BaseGun* baseGun = (BaseGun*)AddGo(new BaseGun(player, "graphics/game/gun.png", "BaseGun"));
	baseGun->sortLayer = 4;
	baseGun->SetPosition(player->GetPosition());

	BaseUI* PauseButton = (BaseUI*)AddGo(new BaseUI("BackButton", uiType::Thumbnail));
	PauseButton->sortLayer = 100;
	PauseButton->SetPosition(defaultResolution.x * resolutionScale - ThumbnailSize - 50.0f, 50.0f);
	PauseButton->SetColor(0, 0, 0, 192);
	PauseButton->SetThumbnailColor(255, 255, 255, 192);
	PauseButton->OnEnter = [PauseButton]()
	{
		PauseButton->SetColor(255, 255, 255, 192);
		PauseButton->SetThumbnailColor(0, 0, 0, 192);
		std::cout << "Enter" << std::endl;
	};
	PauseButton->OnExit = [PauseButton]()
	{
		PauseButton->SetColor();
		PauseButton->SetThumbnailColor(255, 255, 255, 192);
		std::cout << "Exit" << std::endl;
	};
	PauseButton->OnClick = [this]()
	{
		std::cout << "Click" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Title);
	};
	
	float barPosition = 50.0f;

	CreateBar("graphics/bar_back.png", "HpBarBack", barPosition, 1);
	CreateBar("graphics/bar_main.png", "HpBarMain", barPosition, 2, sf::Color(255, 64, 0, 255));
	CreateBar("graphics/bar_bord.png", "HpBarBord", barPosition, 3);

	CreateBar("graphics/bar_back.png", "ExpBarBack", barPosition * 2, 1);
	CreateBar("graphics/bar_main.png", "ExpBarMain", barPosition * 2, 2, sf::Color(0, 224, 64, 255));
	CreateBar("graphics/bar_bord.png", "ExpBarBord", barPosition * 2, 3);

	CreateBar("graphics/material_icon.png", "MaterialIcon", barPosition * 3);
	CreateBar("graphics/material_bag.png",  "MaterialBag" , barPosition * 4);

	SpriteGo* groundOutline = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground_outline.png", "GroundOutline"));
	groundOutline->sortLayer = 2;
	groundOutline->SetOrigin(Origins::MC);

	SpriteGo* ground = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground.png", "Ground"));
	ground->sortLayer = 0;
	ground->SetOrigin(Origins::MC);

	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 1, windowSize.y / 4, "Plasma Rifle", resolutionScale);
	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 2, windowSize.y / 4, "Laser Gun", resolutionScale);
	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 3, windowSize.y / 4, "Gatling Laser", resolutionScale);

	sf::Vector2f tileWorldSize   = { 64.0f, 64.0f };
	sf::Vector2f tileTextureSize = { 64.0f, 64.0f };

	VertexArrayGo* tile = CreateTile("graphics/game/tile.png", { 32, 32 }, tileWorldSize, tileTextureSize);
	tile->sortLayer = 1;
	tile->SetOrigin(Origins::MC);
	tile->SetPosition(0.0f, 0.0f);

	wallBounds = tile->vertexArray.getBounds();

	wallBounds.left   += tileWorldSize.x;
	wallBounds.top    += tileWorldSize.y;
	wallBounds.width  -= tileWorldSize.x * 2;
	wallBounds.height -= tileWorldSize.y * 2;

	player->SetWallBounds(wallBounds);

	AddGo(tile);

	CreateMonsters(256);
	CreateDieEffect(256);
	CreateBulletHitEffect(256);
	CreateEntityEffect(256);

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
	ClearObjectPool(dieEffectPool);
	ClearObjectPool(bulletHitEffectPool);
	ClearObjectPool(entityEffectPool);

	isPlaying = true;

	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void SceneGame::Enter()
{
	Scene::Enter();
	ClearObjectPool(monsterPool);
	ClearObjectPool(dieEffectPool);
	ClearObjectPool(bulletHitEffectPool);
	ClearObjectPool(entityEffectPool);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	player->SetPosition(0.0f, 0.0f);
}

void SceneGame::Exit()
{
	ClearObjectPool(monsterPool);
	ClearObjectPool(dieEffectPool);
	ClearObjectPool(bulletHitEffectPool);
	ClearObjectPool(entityEffectPool);

	isPlaying = false;
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

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1)) // Test Code
	{
		SpawnMonsters(3, currentPlayerPosition, { 0.0f, 0.0f }, 900.0f);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2)) // Test Code
	{
		isShop = !isShop;
		if (!shopCreated && isShop)
		{
			SetActiveShopUI("Plasma Rifle", true);
			SetActiveShopUI("Laser Gun", true);
			SetActiveShopUI("Gatling Laser", true);

			shopCreated = true;
		}
		if (shopCreated && !isShop)
		{
			SetActiveShopUI("Plasma Rifle", false);
			SetActiveShopUI("Laser Gun", false);
			SetActiveShopUI("Gatling Laser", false);

			shopCreated = false;
		}
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
			if (Utils::RandomRange(0, 5) != 0)
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
		monster->sortLayer = 2;
		monster->SetActive(false);
		monster->SetBulletHitEffectPool(&bulletHitEffectPool);
		monster->SetDieEffectPool(&dieEffectPool);
		monster->SetEntityEffectPool(&entityEffectPool);
	};
	monsterPool.Init(count);
}

void SceneGame::SpawnMonsters(int count, sf::Vector2f playerCenter, sf::Vector2f mapCenter, float radius)
{
	for (int i = 0; i < count; i++)
	{
		Monster* monster = monsterPool.Get();
		sf::Vector2f spawnPosition;
		monster->SetActive(false);

		do {
			spawnPosition = mapCenter + Utils::RandomInCircle(radius);
		} while (Utils::Distance(playerCenter, spawnPosition) < 50.0f && Utils::Distance(mapCenter, spawnPosition) < 950.0f);

		monster->SetPosition(spawnPosition);
		monster->SetEntityEffect(spawnPosition, [this, monster]() 
		{
			monster->SetActive(true);
			monster->Reset();
			AddGo(monster);
		});
	}
	monsterCount = monsterPool.GetUseList().size();
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

Monster* SceneGame::GetNearMonsterSearch()
{
	float nearMonsterSearchDistance = 700.0f;

	const std::list<Monster*>* monsterList = GetMonsterList();
	Monster* nearMonster = nullptr;

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

void SceneGame::CreateEntityEffect(int count)
{
	entityEffectPool.OnCreate = [this](EntityEffect* entityEffect)
	{
		entityEffect->SetName("EntityEffect");
		entityEffect->SetDuration(1.0f);
		entityEffect->textureId = "graphics/game/entity_effect.png";
		entityEffect->sortLayer = 2;
		entityEffect->sortOrder = 1;
		entityEffect->SetActive(false);
		entityEffect->SetPool(&entityEffectPool);
	};
	entityEffectPool.Init(count);
}

void SceneGame::CreateBulletHitEffect(int count)
{
	bulletHitEffectPool.OnCreate = [this](BulletHitEffect* bulletHitEffect)
	{
		bulletHitEffect->SetName("BulletHitEffect");
		bulletHitEffect->SetDuration(0.1f);
		bulletHitEffect->textureId = "graphics/game/bullet_hit.png";
		bulletHitEffect->sortLayer = 6;
		bulletHitEffect->sortOrder = 1;
		bulletHitEffect->SetActive(false);
		bulletHitEffect->SetPool(&bulletHitEffectPool);
	};
	bulletHitEffectPool.Init(count);
}

void SceneGame::CreateDieEffect(int count)
{
	dieEffectPool.OnCreate = [this](DieEffect* dieEffect)
	{
		dieEffect->SetName("DieEffect");
		dieEffect->SetDuration(2.0f);
		dieEffect->textureId = "graphics/game/die_effect.png";
		dieEffect->sortLayer = 1;
		dieEffect->sortOrder = 0;
		dieEffect->SetActive(false);
		dieEffect->SetPool(&dieEffectPool);
	};
	dieEffectPool.Init(count);
}

void SceneGame::CreateShopUI(float posiX, float posiY, std::string name, float scale = 1.0f)
{
	const ShopItemInfo& itemInfo = DATATABLE_MGR.Get<UIShopTable>(DataTable::Ids::Shop)->Get(name);

	std::string path     = itemInfo.textureId;
	name                 = itemInfo.name;
	std::string category = itemInfo.category;
	std::string title    = itemInfo.title;
	std::string infoText = itemInfo.info;
	std::string amount   = itemInfo.amount;
	int         tier     = itemInfo.tier;

	float sizeX = 360.0f * scale;
	float sizeY = 480.0f * scale;
	float thumbnailSizeX = 100.0f * scale;
	float thumbnailSizeY = 100.0f * scale;
	float blank = 20.0f * scale;

	BaseUI* box = (BaseUI*)AddGo(new BaseUI("Box" + name, uiType::Box));
	box->sortLayer = 101;
	box->SetPosition(posiX, posiY);
	box->SetSizeAdd(sizeX, sizeY);
	box->SetActive(false);

	SpriteGo* thumbnail = (SpriteGo*)AddGo(new SpriteGo(path, "thumbnail_" + name));
	thumbnail->sortLayer = 103;
	thumbnail->SetPosition(posiX + blank, posiY + blank);
	thumbnail->sprite.setScale(scale, scale); // 100 x 100 px * scale
	thumbnail->SetActive(false);

	BaseUI* thumbnailBox = (BaseUI*)AddGo(new BaseUI("thumbnailBox" + name, uiType::Box));
	thumbnailBox->sortLayer = 102;
	thumbnailBox->SetColor(255, 255, 255, 32);
	thumbnailBox->SetPosition(posiX + blank, posiY + blank);
	thumbnailBox->SetSizeAdd(thumbnailSizeX, thumbnailSizeY);
	thumbnailBox->SetActive(false);

	BaseUI* buttonBox = (BaseUI*)AddGo(new BaseUI("ButtonBox" + name, uiType::Box));
	buttonBox->sortLayer = 102;
	buttonBox->SetColor(255, 255, 255, 32);
	buttonBox->SetPosition(posiX + sizeX * 0.25f, posiY + sizeY * 0.875f - blank);
	buttonBox->SetSizeAdd(sizeX * 0.5f, sizeY * 0.125f);
	buttonBox->SetActive(false);

	float buttonBoxCenterX = sizeX * 0.250f + posiX + sizeX * 0.5f / 2;
	float buttonBoxCenterY = sizeY * 0.875f + posiY - blank + sizeY * 0.125f / 2;
	float heightTuning = 8.0f * scale;

	SpriteGo* materialIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/game/material_ui.png", "MaterialIcon" + name));
	materialIcon->sortLayer = 104;
	materialIcon->SetOrigin(Origins::MC);
	materialIcon->SetPosition(buttonBoxCenterX + blank * 2.5f, buttonBoxCenterY);
	materialIcon->SetActive(false);

	TextGo* itemName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "item_" + name));
	itemName->sortLayer = 105;
	itemName->SetOrigin(Origins::TL);
	itemName->SetCharacterSize(30);
	itemName->SetPosition(posiX + sizeX * 0.33f + blank - 3.0f, posiY + sizeY * 0.0625f);
	itemName->SetFillColor(sf::Color(255, 255, 255, 255));
	itemName->text.setScale(0.9f * scale, 1.0f * scale);
	itemName->SetString(name);
	itemName->SetActive(false);

	TextGo* itemCategory = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", category + name));
	itemCategory->sortLayer = 105;
	itemCategory->SetOrigin(Origins::TL);
	itemCategory->SetCharacterSize(25);
	itemCategory->SetPosition(posiX + sizeX * 0.33f + blank - 2.0f, posiY + sizeY * 0.135f);
	itemCategory->SetFillColor(sf::Color(255, 255, 128, 255));
	itemCategory->SetScale(0.9f * scale, 1.0f * scale);
	itemCategory->SetString(category);
	itemCategory->SetActive(false);

	TextGo* itemTitle = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", title + name));
	itemTitle->sortLayer = 106;
	itemTitle->SetOrigin(Origins::TL);
	itemTitle->SetCharacterSize(20);
	itemTitle->SetPosition(posiX + blank, posiY + sizeY * 0.25f + heightTuning);
	itemTitle->SetFillColor(sf::Color(0, 255, 32, 255));
	itemTitle->SetScale(0.9f * scale, 1.0f * scale);
	itemTitle->SetString(title);
	itemTitle->SetActive(false);

	TextGo* itemInfoText = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", infoText + name));
	itemInfoText->sortLayer = 105;
	itemInfoText->SetOrigin(Origins::TL);
	itemInfoText->SetCharacterSize(20);
	itemInfoText->SetPosition(posiX + blank, posiY + sizeY * 0.25f + heightTuning);
	itemInfoText->SetFillColor(sf::Color(255, 255, 255, 255));
	itemInfoText->SetScale(0.9f * scale, 1.0f * scale);
	itemInfoText->SetString(infoText);
	itemInfoText->SetActive(false);

	TextGo* moneyAmount = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", amount + name));
	moneyAmount->sortLayer = 104;
	moneyAmount->SetOrigin(Origins::MC);
	moneyAmount->SetCharacterSize(48);
	moneyAmount->SetPosition(buttonBoxCenterX - blank, buttonBoxCenterY - heightTuning);
	moneyAmount->SetFillColor(sf::Color(255, 255, 255, 255));
	moneyAmount->SetScale(0.9f * scale, 1.0f * scale);
	moneyAmount->SetString(amount);
	moneyAmount->text.setOutlineColor(sf::Color(16, 16, 16, 255));
	moneyAmount->text.setOutlineThickness(7.0f * scale);
	moneyAmount->SetActive(false);

	switch (tier) // Tier Color
	{
	case 1:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		break;
	case 2:
		box->SetStrokeColor(75, 175, 225, 255);
		box->SetColor(15, 30, 45, 255);
		break;
	case 3:
		box->SetStrokeColor(150, 75, 225, 255);
		box->SetColor(15, 15, 30, 255);
		break;
	case 4:
		box->SetStrokeColor(225, 50, 50, 255);
		box->SetColor(30, 15, 15, 255);
		break;
	default:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		break;
	}
}

void SceneGame::SetActiveShopUI(std::string name, bool active)
{
	const ShopItemInfo& itemInfo = DATATABLE_MGR.Get<UIShopTable>(DataTable::Ids::Shop)->Get(name);

	std::string path     = itemInfo.textureId;
	name                 = itemInfo.name;
	std::string category = itemInfo.category;
	std::string title    = itemInfo.title;
	std::string infoText = itemInfo.info;
	std::string amount   = itemInfo.amount;

	std::vector<std::string> gameObjectNames = 
	{
		"Box"          + name,
		"thumbnail_"   + name,
		"thumbnailBox" + name,
		"ButtonBox"    + name,
		"MaterialIcon" + name,
		"item_"        + name,
		category       + name,
		title          + name,
		infoText       + name,
		amount         + name
	};

	for (const std::string& gameObjectName : gameObjectNames)
	{
		GameObject* gameObject = FindGo(gameObjectName);
		if (gameObject != nullptr)
		{
			gameObject->SetActive(active);
		}
	}
}

void SceneGame::CreateBar(const std::string& id, const std::string& name, float posY, int sort, sf::Color color)
{
	SpriteGo* bar = (SpriteGo*)AddGo(new SpriteGo(id, name));
	bar->sortLayer = 100 + sort;
	bar->sprite.setColor(color);
	bar->SetOrigin(Origins::TL);
	bar->SetPosition(50.0f, posY);

}

void SceneGame::SetHpUI(float currentHp)
{
	SpriteGo* hp = (SpriteGo*)FindGo("HpBarMain");
	hp->sprite.setScale(1.0f * currentHp, 1.0f);
}

void SceneGame::SetExpUI(float currentExp)
{
	SpriteGo* exp = (SpriteGo*)FindGo("ExpBarMain");
	exp->sprite.setScale(1.0f * currentExp, 1.0f);
}

void SceneGame::OnDiePlayer()
{
	isPlaying = false;
}
