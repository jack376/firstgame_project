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
#include "ShopTable.h"
#include "UpgradeTable.h"
#include <sstream>
#include <iomanip>


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
	float pausedThumbSize = 64.0f;

	player = (Player*)AddGo(new Player("Player"));
	player->sortLayer = 3;

	BaseGun* baseGun = (BaseGun*)AddGo(new BaseGun(player, "graphics/game/gun.png", "BaseGun"));
	baseGun->sortLayer = 4;
	baseGun->SetPosition(player->GetPosition());

	BaseUI* PauseButton = (BaseUI*)AddGo(new BaseUI("BackButton", uiType::Thumbnail));
	PauseButton->sortLayer = 100;
	PauseButton->SetPosition(defaultResolution.x * resolutionScale - pausedThumbSize - 50.0f, 50.0f);
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
		Reset();
		SCENE_MGR.ChangeScene(SceneId::Title);
	};

	float uiPos = 50.0f;
	float magicNumber = 5.0f;

	CreateBar("graphics/bar_back.png", "HpBarBack", uiPos, uiPos, 1);
	CreateBar("graphics/bar_main.png", "HpBarMain", uiPos, uiPos, 2, sf::Color(255, 32, 0, 255));
	CreateBar("graphics/bar_bord.png", "HpBarBord", uiPos, uiPos, 3);

	CreateBar("graphics/bar_back.png", "ExpBarBack", uiPos, uiPos * 2, 1);
	CreateBar("graphics/bar_main.png", "ExpBarMain", uiPos, uiPos * 2, 2, sf::Color::Transparent);
	CreateBar("graphics/bar_bord.png", "ExpBarBord", uiPos, uiPos * 2, 3);

	CreateBar("graphics/material_icon.png", "MaterialIcon", uiPos - magicNumber * 2, uiPos * 3);
	//CreateBar("graphics/material_bag.png", "MaterialBag", uiPos - magicNumber * 2, uiPos * 4);

	CreateText("WaveCount", "Wave 1", windowSize.x * 0.5f, uiPos, 40, true);
	CreateText("WaveTimer", "30", windowSize.x * 0.5f, uiPos * 2, 64, true);

	CreateText("MaterialCount", "0", uiPos * 2 + magicNumber, uiPos * 3);
	//CreateText("MaterialKeeps", "33", uiPos * 2 + magicNumber, uiPos * 4);

	TextGo* hpText = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", "HpText"));
	hpText->sortLayer = 110;
	hpText->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	hpText->text.setOutlineThickness(3.0f);
	hpText->SetOrigin(Origins::TL);
	hpText->SetPosition(uiPos + magicNumber * 3, uiPos + magicNumber);
	hpText->SetCharacterSize(24);
	hpText->SetString("20 / 20");

	TextGo* lvText = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", "LvText"));
	lvText->sortLayer = 110;
	lvText->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	lvText->text.setOutlineThickness(3.0f);
	lvText->SetOrigin(Origins::TL);
	lvText->SetPosition(uiPos + magicNumber * 3, uiPos * 2 + magicNumber);
	lvText->SetCharacterSize(24);
	lvText->SetString("Lv 1");

	SpriteGo* groundOutline = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground_outline.png", "GroundOutline"));
	groundOutline->sortLayer = 2;
	groundOutline->SetOrigin(Origins::MC);

	SpriteGo* ground = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground.png", "Ground"));
	ground->sortLayer = 0;
	ground->SetOrigin(Origins::MC);

	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 1, windowSize.y / 4, "Plasma Rifle", resolutionScale);
	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 2, windowSize.y / 4, "Laser Gun", resolutionScale);
	CreateShopUI(windowSize.x / 64 + shopUiPositionX * 3, windowSize.y / 4, "Gatling Laser", resolutionScale);
	
	CreateUpgradeUI(windowSize.x / 64 + shopUiPositionX * 1, windowSize.y / 3, "Back I", resolutionScale);
	CreateUpgradeUI(windowSize.x / 64 + shopUiPositionX * 2, windowSize.y / 3, "Chest II", resolutionScale);
	CreateUpgradeUI(windowSize.x / 64 + shopUiPositionX * 3, windowSize.y / 3, "Gun IV", resolutionScale);

	CreatePlayerInfoUI(windowSize.x - shopUiPositionX * 1 - uiPos, windowSize.y / 2, resolutionScale);

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

	player->Reset();
	player->SetPosition(0.0f, 0.0f);

	money = 0;
	playerLevel = 1;
	waveCount = 1;
	waveTimer = 30.0f;
	lastSpawnTime = 5.0f;

	SetCountUI("MaterialCount", money);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (player == nullptr)
	{
		return;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1)) // Test Code
	{
		SetPaused(!IsPaused());
	}

	if (IsPaused())
	{
		return;
	}

	currentPlayerPosition = player->GetPosition();

	waveTimer -= dt;
	lastSpawnTime += dt;

	if (waveTimer <= 0.0f)
	{
		waveCount++;
		TextGo* waveCountText = (TextGo*)FindGo("WaveCount");
		waveCountText->SetString("Wave " + std::to_string(waveCount));
		waveTimer = 30.0f;
	}
	TextGo* waveTimerText = (TextGo*)FindGo("WaveTimer");
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << (int)waveTimer;
	waveTimerText->SetString(ss.str());
	
	if (lastSpawnTime >= 5.0f)
	{
		SpawnMonsters(waveCount + 5, currentPlayerPosition, { 0.0f, 0.0f }, 900.0f);
		lastSpawnTime = 0.0f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2)) // Test Code
	{
		isUpgrade = !isUpgrade;
		if (!upgradeCreated && isUpgrade)
		{
			SetActiveUpgradeUI("Back I", true);
			SetActiveUpgradeUI("Chest II", true);
			SetActiveUpgradeUI("Gun IV", true);

			upgradeCreated = true;
		}
		if (upgradeCreated && !isUpgrade)
		{
			SetActiveUpgradeUI("Back I", false);
			SetActiveUpgradeUI("Chest II", false);
			SetActiveUpgradeUI("Gun IV", false);

			upgradeCreated = false;
		}
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3)) // Test Code
	{
		isInfo = !isInfo;
		if (!infoCreated && isInfo)
		{
			SetActivePlayerInfoUI(true);

			infoCreated = true;
		}
		if (infoCreated && !isInfo)
		{
			SetActivePlayerInfoUI(false);

			infoCreated = false;
		}
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4)) // Test Code
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
		monster->sortLayer = 5;
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
	player->OnKilled();
	monsterKillCount++;
	money++;

	SetCountUI("MaterialCount", money);
	//std::cout << monsterPool.GetUseList().size() << std::endl; // 몬스터가 일정 수 이하면 스폰
	monsterCount = monsterPool.GetUseList().size();

	//sf::Vector2f pos = monster->GetPosition();

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

void SceneGame::CreateUpgradeUI(float posiX, float posiY, std::string name, float scale)
{
	const UpgradeInfo& upgradeInfo = DATATABLE_MGR.Get<UpgradeTable>(DataTable::Ids::Upgrade)->Get(name);

	std::string textureId = upgradeInfo.textureId;
	name                  = upgradeInfo.name;
	std::string category  = upgradeInfo.category;
	std::string highlight = upgradeInfo.highlight;
	std::string title     = upgradeInfo.title;
	int         value     = upgradeInfo.value;
	int         amount    = upgradeInfo.amount;
	int         tier      = upgradeInfo.tier;

	float blank = 20.0f * scale;
	sf::Vector2f thumbSize(100.0f * scale, 100.0f * scale);

	sf::Vector2f fullBoxSize(360.0f * scale, 270.0f * scale);
	sf::Vector2f buttonBoxSize(180.0f * scale, 60.0f * scale);

	BaseUI* box = (BaseUI*)AddGo(new BaseUI("Box" + name, uiType::Box));
	box->sortLayer = 101;
	box->SetPosition(posiX, posiY);
	box->SetSizeAdd(fullBoxSize.x, fullBoxSize.y);
	box->SetActive(false);

	SpriteGo* thumbnail = (SpriteGo*)AddGo(new SpriteGo(textureId, "thumbnail_" + name));
	thumbnail->sortLayer = 103;
	thumbnail->SetPosition(posiX + blank, posiY + blank);
	thumbnail->sprite.setScale(scale, scale); // 100 x 100 px * scale
	thumbnail->SetActive(false);

	BaseUI* thumbnailBox = (BaseUI*)AddGo(new BaseUI("thumbnailBox" + name, uiType::Box));
	thumbnailBox->sortLayer = 102;
	thumbnailBox->SetColor(255, 255, 255, 32);
	thumbnailBox->SetPosition(posiX + blank, posiY + blank);
	thumbnailBox->SetSizeAdd(thumbSize.x, thumbSize.y);
	thumbnailBox->SetActive(false);

	BaseUI* buttonBox = (BaseUI*)AddGo(new BaseUI("ButtonBox" + name, uiType::Box));
	buttonBox->sortLayer = 102;
	buttonBox->SetColor(255, 255, 255, 32);
	buttonBox->SetPosition(posiX + fullBoxSize.x * 0.25f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	buttonBox->SetSizeAdd(buttonBoxSize.x, buttonBoxSize.y);
	buttonBox->SetActive(false);

	std::string amountString = std::to_string(amount);

	TextGo* moneyAmount = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", amountString + name));
	moneyAmount->sortLayer = 104;
	moneyAmount->SetOrigin(Origins::TR);
	moneyAmount->SetCharacterSize(48);
	moneyAmount->SetPosition(posiX + fullBoxSize.x * 0.5f + blank - 7.0f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	moneyAmount->SetFillColor(sf::Color(255, 255, 255, 255));
	moneyAmount->SetScale(scale, scale);
	moneyAmount->SetString(amountString);
	moneyAmount->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	moneyAmount->text.setOutlineThickness(7.0f * scale);
	moneyAmount->SetActive(false);

	SpriteGo* materialIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/game/material_ui.png", "MaterialIcon" + name));
	materialIcon->sortLayer = 104;
	materialIcon->SetOrigin(Origins::TL);
	materialIcon->SetPosition(posiX + fullBoxSize.x * 0.5f + blank, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	materialIcon->SetActive(false);

	TextGo* upgradeName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "upgrade_" + name));
	upgradeName->sortLayer = 105;
	upgradeName->SetOrigin(Origins::TL);
	upgradeName->SetCharacterSize(30);
	upgradeName->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 30.0f);
	upgradeName->SetFillColor(sf::Color(255, 255, 255, 255));
	upgradeName->text.setScale(0.9f * scale, 1.0f * scale);
	upgradeName->SetString(name);
	upgradeName->SetActive(false);

	TextGo* upgradeCategory = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", category + name));
	upgradeCategory->sortLayer = 105;
	upgradeCategory->SetOrigin(Origins::TL);
	upgradeCategory->SetCharacterSize(25);
	upgradeCategory->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 60.0f);
	upgradeCategory->SetFillColor(sf::Color(255, 255, 128, 255));
	upgradeCategory->SetScale(0.9f * scale, 1.0f * scale);
	upgradeCategory->SetString(category);
	upgradeCategory->SetActive(false);

	TextGo* upgradeHighlight = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", highlight + name));
	upgradeHighlight->sortLayer = 106;
	upgradeHighlight->SetOrigin(Origins::TL);
	upgradeHighlight->SetCharacterSize(20);
	upgradeHighlight->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	upgradeHighlight->SetFillColor(sf::Color(0, 192, 64, 255));
	upgradeHighlight->SetScale(0.9f * scale, 1.0f * scale);
	upgradeHighlight->SetString(highlight);
	upgradeHighlight->SetActive(false);

	TextGo* upgradeTitle = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", title + name));
	upgradeTitle->sortLayer = 105;
	upgradeTitle->SetOrigin(Origins::TL);
	upgradeTitle->SetCharacterSize(20);
	upgradeTitle->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	upgradeTitle->SetFillColor(sf::Color(255, 255, 255, 255));
	upgradeTitle->SetScale(0.9f * scale, 1.0f * scale);
	upgradeTitle->SetString(title);
	upgradeTitle->SetActive(false);

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

void SceneGame::SetActiveUpgradeUI(std::string name, bool active)
{
	const UpgradeInfo& upgradeInfo = DATATABLE_MGR.Get<UpgradeTable>(DataTable::Ids::Upgrade)->Get(name);

	std::string textureId = upgradeInfo.textureId;
	name                  = upgradeInfo.name;
	std::string category  = upgradeInfo.category;
	std::string highlight = upgradeInfo.highlight;
	std::string title     = upgradeInfo.title;
	int         value     = upgradeInfo.value;
	int         amount    = upgradeInfo.amount;

	std::string amountString = std::to_string(amount);

	std::vector<std::string> gameObjectNames =
	{
		"Box"          + name,
		"thumbnail_"   + name,
		"thumbnailBox" + name,
		"ButtonBox"    + name,
		"MaterialIcon" + name,
		"upgrade_"     + name,
		category       + name,
		highlight      + name,
		title          + name,
		amountString   + name
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

void SceneGame::CreateShopUI(float posiX, float posiY, std::string name, float scale)
{
	const ShopItemInfo& itemInfo = DATATABLE_MGR.Get<ShopTable>(DataTable::Ids::Shop)->Get(name);

	std::string path     = itemInfo.textureId;
	name                 = itemInfo.name;
	std::string category = itemInfo.category;
	std::string title    = itemInfo.title;
	std::string infoText = itemInfo.info;
	int			amount   = itemInfo.amount;
	int         tier     = itemInfo.tier;

	float blank = 20.0f * scale;

	sf::Vector2f fullBoxSize(360.0f * scale, 480.0f * scale);
	sf::Vector2f buttonBoxSize(180.0f * scale, 60.0f * scale);

	sf::Vector2f thumbSize(100.0f * scale, 100.0f * scale);

	BaseUI* box = (BaseUI*)AddGo(new BaseUI("Box" + name, uiType::Box));
	box->sortLayer = 101;
	box->SetPosition(posiX, posiY);
	box->SetSizeAdd(fullBoxSize.x, fullBoxSize.y);
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
	thumbnailBox->SetSizeAdd(thumbSize.x, thumbSize.y);
	thumbnailBox->SetActive(false);

	BaseUI* buttonBox = (BaseUI*)AddGo(new BaseUI("ButtonBox" + name, uiType::Box));
	buttonBox->sortLayer = 102;
	buttonBox->SetColor(255, 255, 255, 32);
	buttonBox->SetPosition(posiX + fullBoxSize.x * 0.25f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	buttonBox->SetSizeAdd(buttonBoxSize.x, buttonBoxSize.y);
	buttonBox->SetActive(false);

	SpriteGo* materialIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/game/material_ui.png", "MaterialIcon" + name));
	materialIcon->sortLayer = 104;
	materialIcon->SetOrigin(Origins::TL);
	materialIcon->SetPosition(posiX + fullBoxSize.x * 0.5f + blank, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	materialIcon->SetActive(false);

	std::string amountString = std::to_string(amount);

	TextGo* moneyAmount = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", amountString + name));
	moneyAmount->sortLayer = 104;
	moneyAmount->SetOrigin(Origins::TR);
	moneyAmount->SetCharacterSize(48);
	moneyAmount->SetPosition(posiX + fullBoxSize.x * 0.5f + blank - 3.0f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	moneyAmount->SetFillColor(sf::Color(255, 255, 255, 255));
	moneyAmount->SetScale(0.9f * scale, 1.0f * scale);
	moneyAmount->SetString(amountString);
	moneyAmount->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	moneyAmount->text.setOutlineThickness(7.0f * scale);
	moneyAmount->SetActive(false);

	TextGo* itemName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "item_" + name));
	itemName->sortLayer = 105;
	itemName->SetOrigin(Origins::TL);
	itemName->SetCharacterSize(30);
	itemName->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 30.0f);
	itemName->SetFillColor(sf::Color(255, 255, 255, 255));
	itemName->text.setScale(0.9f * scale, 1.0f * scale);
	itemName->SetString(name);
	itemName->SetActive(false);

	TextGo* itemCategory = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", category + name));
	itemCategory->sortLayer = 105;
	itemCategory->SetOrigin(Origins::TL);
	itemCategory->SetCharacterSize(25);
	itemCategory->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 60.0f);
	itemCategory->SetFillColor(sf::Color(255, 255, 128, 255));
	itemCategory->SetScale(0.9f * scale, 1.0f * scale);
	itemCategory->SetString(category);
	itemCategory->SetActive(false);

	TextGo* itemTitle = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", title + name));
	itemTitle->sortLayer = 106;
	itemTitle->SetOrigin(Origins::TL);
	itemTitle->SetCharacterSize(20);
	itemTitle->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	itemTitle->SetFillColor(sf::Color(0, 255, 32, 255));
	itemTitle->SetScale(0.9f * scale, 1.0f * scale);
	itemTitle->SetString(title);
	itemTitle->SetActive(false);

	TextGo* itemInfoText = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", infoText + name));
	itemInfoText->sortLayer = 105;
	itemInfoText->SetOrigin(Origins::TL);
	itemInfoText->SetCharacterSize(20);
	itemInfoText->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	itemInfoText->SetFillColor(sf::Color(255, 255, 255, 255));
	itemInfoText->SetScale(0.9f * scale, 1.0f * scale);
	itemInfoText->SetString(infoText);
	itemInfoText->SetActive(false);

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
	const ShopItemInfo& itemInfo = DATATABLE_MGR.Get<ShopTable>(DataTable::Ids::Shop)->Get(name);

	std::string path     = itemInfo.textureId;
	name                 = itemInfo.name;
	std::string category = itemInfo.category;
	std::string title    = itemInfo.title;
	std::string infoText = itemInfo.info;
	int         amount   = itemInfo.amount;

	std::string amountString = std::to_string(amount);

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
		amountString   + name
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

void SceneGame::CreatePlayerInfoUI(float posiX, float posiY, float scale)
{
	float blank = 20.0f * scale;
	sf::Vector2f thumbSize(100.0f * scale, 100.0f * scale);

	sf::Vector2f fullBoxSize(360.0f * scale, 450.0f * scale);
	sf::Vector2f buttonBoxSize(180.0f * scale, 60.0f * scale);

	BaseUI* playerInfoBox = (BaseUI*)AddGo(new BaseUI("PlayerInfoBox", uiType::Box));
	playerInfoBox->sortLayer = 101;
	playerInfoBox->SetPosition(posiX, posiY);
	playerInfoBox->SetColor(0, 0, 0, 160);
	playerInfoBox->SetSizeAdd(fullBoxSize.x, fullBoxSize.y);
	playerInfoBox->SetActive(false);

	SpriteGo* statsMerge = (SpriteGo*)AddGo(new SpriteGo("graphics/game/stats_merge.png", "StatsMerge"));
	statsMerge->sortLayer = 103;
	statsMerge->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	statsMerge->sprite.setScale(0.25f * scale, 0.25f * scale);
	statsMerge->SetActive(false);

	TextGo* boxName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "BoxName"));
	boxName->sortLayer = 108;
	boxName->SetOrigin(Origins::TC);
	boxName->SetCharacterSize(40);
	boxName->SetPosition(posiX + fullBoxSize.x * 0.5f, posiY + thumbSize.y * 0.5f);
	boxName->SetFillColor(sf::Color(255, 255, 128, 255));
	boxName->text.setScale(0.9f * scale, 1.0f * scale);
	boxName->SetString("Player Info");
	boxName->SetActive(false);

	TextGo* statName = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", "StatName"));
	statName->sortLayer = 107;
	statName->SetOrigin(Origins::TL);
	statName->SetCharacterSize(20);
	statName->SetPosition(posiX + thumbSize.x * 0.5f + blank, posiY + thumbSize.y + blank * 2);
	statName->SetFillColor(sf::Color(255, 255, 255, 255));
	statName->text.setScale(0.9f * scale, 1.0f * scale);
	statName->SetString("Maximum HP\nHP Regeneration\nAttack Damage\nAttack Speed\nCritical Chance\nRange\nArmor Point\nDodge Point\nMove Speed");
	statName->SetActive(false);

	TextGo* statPoint = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", "StatPoint"));
	statPoint->sortLayer = 106;
	statPoint->SetOrigin(Origins::TL);
	statPoint->SetCharacterSize(20);
	statPoint->SetPosition(posiX + fullBoxSize.x * 0.75f + blank, posiY + thumbSize.y + 1.5f + blank * 2);
	statPoint->SetFillColor(sf::Color(0, 255, 32, 255));
	statPoint->text.setScale(0.9f * scale, 1.0f * scale);
	statPoint->SetString("20\n0\n30\n100%\n3%\n700\n0\n0\n500");
	statPoint->SetActive(false);
}

void SceneGame::SetActivePlayerInfoUI(bool active)
{
	BaseUI* playerInfoBox = (BaseUI*)FindGo("PlayerInfoBox");
	playerInfoBox->SetActive(active);

	SpriteGo* statsMerge = (SpriteGo*)FindGo("StatsMerge");
	statsMerge->SetActive(active);

	TextGo* boxName = (TextGo*)FindGo("BoxName");
	boxName->SetActive(active);

	TextGo* statName = (TextGo*)FindGo("StatName");
	statName->SetActive(active);

	TextGo* statPoint = (TextGo*)FindGo("StatPoint");
	statPoint->SetActive(active);
}

void SceneGame::CreateBar(const std::string& id, const std::string& name, float posX, float posY, int sort, sf::Color color)
{
	SpriteGo* bar = (SpriteGo*)AddGo(new SpriteGo(id, name));
	bar->sortLayer = 100 + sort;
	bar->sprite.setColor(color);
	bar->SetOrigin(Origins::TL);
	bar->SetPosition(posX, posY);
}

void SceneGame::CreateText(const std::string& name, const std::string& str, float posX, float posY, int fontSize, bool originMC)
{
	TextGo* text = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", name));
	text->sortLayer = 110;
	text->SetOrigin(Origins::TL);
	text->SetPosition(posX, posY);
	text->SetCharacterSize(fontSize);
	text->SetFillColor(sf::Color(255, 255, 255, 255));
	text->SetString(str);
	text->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	text->text.setOutlineThickness(5.0f);

	if (originMC)
	{
		text->SetOrigin(Origins::MC);
		text->SetPosition(posX, posY + 30.0f);
	}
}

//void SceneGame::CreateCount(const std::string& name, int count, float posX, float posY, int fontSize)
//{
//	std::string countString = std::to_string(count);
//
//	TextGo* text = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", name));
//	text->sortLayer = 110;
//	text->SetOrigin(Origins::TL);
//	text->SetPosition(posX, posY);
//	text->SetCharacterSize(fontSize);
//	text->SetFillColor(sf::Color(255, 255, 255, 255));
//	text->SetString(countString);
//	text->text.setOutlineColor(sf::Color(0, 0, 0, 255));
//	text->text.setOutlineThickness(5.0f);
//	text->SetActive(false);
//}

void SceneGame::SetCountUI(const std::string& name, int count)
{
	std::string countString = std::to_string(count);

	TextGo* text = (TextGo*)FindGo(name);
	text->SetString(countString);
	text->SetActive(true);
}


void SceneGame::SetHpUI(float currentHp, int maxHp)
{
	SpriteGo* hpBar = (SpriteGo*)FindGo("HpBarMain");
	hpBar->sprite.setOrigin(0.0f, 0.0f);
	hpBar->sprite.setScale(currentHp / (float)maxHp, 1.0f);

	std::string currentHpString = std::to_string((int)currentHp);
	std::string maxHpString = std::to_string(maxHp);

	TextGo* hpText = (TextGo*)FindGo("HpText");
	hpText->SetString(currentHpString + " / " + maxHpString);
}

void SceneGame::SetExpUI(float currentExp)
{
	SpriteGo* expBar = (SpriteGo*)FindGo("ExpBarMain");
	expBar->sprite.setOrigin(0.0f, 0.0f);
	expBar->sprite.setScale(1.0f * currentExp, 1.0f);
	expBar->sprite.setColor(sf::Color(0, 192, 64, 255));

	std::string levelString = std::to_string(playerLevel);

	TextGo* expCurrentText = (TextGo*)FindGo("LvText");
	expCurrentText->SetString("Lv " + levelString);
}

void SceneGame::SetLevelUpUI(int level)
{
	playerLevel = level;

	/*
	TextGo* levelText = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", "PlayerLevel"));
	levelText->sortLayer = 100;
	levelText->SetOrigin(Origins::TL);
	levelText->SetPosition(posX + 75.0f, posY);
	levelText->SetCharacterSize(fontSize);
	levelText->SetFillColor(sf::Color(255, 255, 255, 255));
	levelText->SetString(str);
	levelText->text.setOutlineColor(sf::Color(16, 16, 16, 255));
	levelText->text.setOutlineThickness(5.0f);
	*/

	// 웨이브 끝나고 업그레이드 대기
}

void SceneGame::OnDiePlayer()
{
	isPlaying = false;
	player->Reset();
}
