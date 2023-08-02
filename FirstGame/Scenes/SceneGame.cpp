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

	defaultResolution = sf::Vector2f(1920.0f, 1080.0f);
	windowSize = sf::Vector2f(FRAMEWORK.GetWindowSize());
	centerPos = sf::Vector2f(windowSize * 0.5f);
	resolutionScale = windowSize.x / 1920.0f;

	player = (Player*)AddGo(new Player("Player"));
	player->sortLayer = 3;

	BaseGun* baseGun = (BaseGun*)AddGo(new BaseGun(player, "graphics/game/gun.png", "BaseGun"));
	baseGun->sortLayer = 4;
	baseGun->SetPosition(player->GetPosition());

	// Pause Button

	float pausedThumbSize = 64.0f;

	BaseUI* PauseButton = (BaseUI*)AddGo(new BaseUI("BackButton", uiType::Thumbnail));
	PauseButton->sortLayer = 100;
	PauseButton->SetPosition(defaultResolution.x * resolutionScale - pausedThumbSize - 50.0f, 50.0f);
	PauseButton->SetColor(8, 8, 8, 192);
	PauseButton->SetThumbnailColor(255, 255, 255, 192);

	PauseButton->OnEnter = [PauseButton]()
	{
		PauseButton->SetColor(255, 255, 255, 192);
		PauseButton->SetThumbnailColor(16, 16, 16, 192);
	};
	PauseButton->OnExit = [PauseButton]()
	{
		PauseButton->SetColor(8, 8, 8, 192);
		PauseButton->SetThumbnailColor(255, 255, 255, 192);
	};
	PauseButton->OnClick = [PauseButton, this]()
	{
		PauseButton->SetColor(8, 8, 8, 192);
		PauseButton->SetThumbnailColor(255, 255, 255, 192);
		Reset();
		SCENE_MGR.ChangeScene(SceneId::Title);
	};

	// Hp, Exp HUD

	float uiPos = 50.0f;
	float magicNumber = 5.0f;

	CreateBar("graphics/bar_back.png", "HpBarBack", uiPos, uiPos, 1);
	CreateBar("graphics/bar_main.png", "HpBarMain", uiPos, uiPos, 2, sf::Color(255, 32, 0, 255));
	CreateBar("graphics/bar_bord.png", "HpBarBord", uiPos, uiPos, 3);

	CreateBar("graphics/bar_back.png", "ExpBarBack", uiPos, uiPos * 2, 1);
	CreateBar("graphics/bar_main.png", "ExpBarMain", uiPos, uiPos * 2, 2, sf::Color::Transparent);
	CreateBar("graphics/bar_bord.png", "ExpBarBord", uiPos, uiPos * 2, 3);

	CreateBar("graphics/material_icon.png", "MaterialIcon", uiPos - magicNumber * 2, uiPos * 3);

	CreateText("WaveCount", "Wave 1", windowSize.x * 0.5f, uiPos, 40, true);
	CreateText("WaveTimer", "30", windowSize.x * 0.5f, uiPos * 2, 64, true);

	CreateText("MaterialCount", "0", uiPos * 2 + magicNumber, uiPos * 3);

	TextGo* hpText = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", "HpText"));
	hpText->sortLayer = 110;
	hpText->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	hpText->text.setOutlineThickness(3.0f);
	hpText->SetOrigin(Origins::TL);
	hpText->SetPosition(uiPos + magicNumber * 3, uiPos + magicNumber);
	hpText->SetCharacterSize(24);

	TextGo* lvText = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", "LvText"));
	lvText->sortLayer = 110;
	lvText->text.setOutlineColor(sf::Color(0, 0, 0, 255));
	lvText->text.setOutlineThickness(3.0f);
	lvText->SetOrigin(Origins::TL);
	lvText->SetPosition(uiPos + magicNumber * 3, uiPos * 2 + magicNumber);
	lvText->SetCharacterSize(24);

	// Shop, Upgrade, PlayerInfo UI

	float uiBlank = 24.0f * resolutionScale;
	float uiSizeX = 360.0f * resolutionScale;

	CreateShopUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 0.5f, windowSize.y * 0.25f, "Taser Gun", resolutionScale);
	CreateShopUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 1.5f, windowSize.y * 0.25f, "Shotgun", resolutionScale);
	CreateShopUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 2.5f, windowSize.y * 0.25f, "Pistol", resolutionScale);
	
	for (int i = 0; i < 9; i++)
	{
		CreateShopUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 0.5f, windowSize.y * 0.25f, shopNames[i], resolutionScale);
	}

	for (int i = 0; i < 8; i++)
	{
		CreateUpgradeUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 0, windowSize.y * 0.375f, upgradeNamesColumn1[i], resolutionScale);
		CreateUpgradeUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 1, windowSize.y * 0.375f, upgradeNamesColumn2[i], resolutionScale);
		CreateUpgradeUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 2, windowSize.y * 0.375f, upgradeNamesColumn3[i], resolutionScale);
		CreateUpgradeUI(windowSize.x / 10 + (uiSizeX + uiBlank) * 3, windowSize.y * 0.375f, upgradeNamesColumn4[i], resolutionScale);
	}

	CreatePlayerInfoUI(windowSize.x - uiSizeX * 1 - uiPos, windowSize.y / 2, resolutionScale);

	// World Tile

	SpriteGo* groundOutline = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground_outline.png", "GroundOutline"));
	groundOutline->sortLayer = 2;
	groundOutline->SetOrigin(Origins::MC);

	SpriteGo* ground = (SpriteGo*)AddGo(new SpriteGo("graphics/game/ground.png", "Ground"));
	ground->sortLayer = 0;
	ground->SetOrigin(Origins::MC);

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

	// InitObjectPool

	CreateMonsters(256);
	CreateDieEffect(256);
	CreateBulletHitEffect(256);
	CreateEntityEffect(256);
	CreateDamageTextEffect(256);

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
	ClearObjectPool(damageTextEffectPool);

	SetActiveAllUpgradeUI(false);
	SetPaused(false);

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
	ClearObjectPool(damageTextEffectPool);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	player->Reset();
	player->SetPosition(0.0f, 0.0f);

	money = 0;
	currentPlayerLevel = 1;
	waveCount = 1;
	waveTimer = 30.0f;
	lastSpawnTime = 5.0f;

	SetCountUI("MaterialCount", money);

	isPlaying = false;

	TextGo* waveCountText = (TextGo*)FindGo("WaveCount");
	waveCountText->SetString("Wave " + std::to_string(waveCount));
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

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::P))
	{
		SetPaused(!IsPaused());
	}

	if (!isPlaying && !isShop)
	{
		isShop = true;

		SetActiveShopUI("Taser Gun", true);
		SetActiveShopUI("Shotgun", true);
		SetActiveShopUI("Pistol", true);

		SetPaused(!IsPaused());
	}

	if (IsPaused())
	{
		return;
	}

	waveTimer -= dt;
	lastSpawnTime += dt;
	flowTime += dt;

	currentPlayerPosition = player->GetPosition();

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
	
	if (lastSpawnTime >= 3.0f)
	{
		SpawnMonsters(waveCount + 10, currentPlayerPosition, { 0.0f, 0.0f }, 900.0f);
		lastSpawnTime = 0.0f;
	}
	
	if (levelUpPoint >= 1 && !isUpgrade)
	{
		isUpgrade = true;

		SetActiveUpgradeUI(upgradeNamesColumn1[Utils::RandomRangeWithWeights({ 60, 25, 10, 5, 60, 25, 10, 5 })], true);
		SetActiveUpgradeUI(upgradeNamesColumn2[Utils::RandomRangeWithWeights({ 60, 25, 10, 5, 60, 25, 10, 5 })], true);
		SetActiveUpgradeUI(upgradeNamesColumn3[Utils::RandomRangeWithWeights({ 60, 25, 10, 5, 60, 25, 10, 5 })], true);
		SetActiveUpgradeUI(upgradeNamesColumn4[Utils::RandomRangeWithWeights({ 60, 25, 10, 5, 60, 25, 10, 5 })], true);

		SetPaused(!IsPaused());
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
	std::vector<int> weights = monsterSpawnWeights;

	monsterPool.OnCreate = [this, &weights](Monster* monster)
	{
		Monster::Types monsterType = (Monster::Types)Utils::RandomRangeWithWeights(weights);
		monster->SetType(monsterType);
		monster->SetPlayer(player);
		monster->sortLayer = 5;
		monster->SetActive(false);
		monster->SetBulletHitEffectPool(&bulletHitEffectPool);
		monster->SetDieEffectPool(&dieEffectPool);
		monster->SetEntityEffectPool(&entityEffectPool);
		monster->SetDamageTextEffectPool(&damageTextEffectPool);
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
		monster->SetState(Character::StateType::Spawn);

		do {
			spawnPosition = mapCenter + Utils::RandomInCircle(radius);
		} while (Utils::Distance(playerCenter, spawnPosition) < 200.0f && Utils::Distance(mapCenter, spawnPosition) < 950.0f);

		monster->SetPosition(spawnPosition);
		monster->SetEntityEffect(spawnPosition, [this, monster]() 
		{
			monster->SetActive(true);
			monster->SetState(Character::StateType::Idle);
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

	monsterCount = monsterPool.GetUseList().size();

	//SpawnItem(pos);
}

Monster* SceneGame::GetNearMonsterSearch()
{
	if (player == nullptr)
	{
		return 0;
	}

	float nearMonsterSearchDistance = 1200.0f;

	const std::list<Monster*>* monsterList = GetMonsterList();
	Monster* nearMonster = nullptr;

	for (Monster* monster : *monsterList)
	{
		if (monster->GetState() == Monster::StateType::Spawn)
		{
			continue;
		}
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

void SceneGame::CreateDamageTextEffect(int count)
{
	damageTextEffectPool.OnCreate = [this](DamageTextEffect* damageTextEffect)
	{
		damageTextEffect->SetName("DamageTextEffect");
		damageTextEffect->SetDuration(0.3f);
		damageTextEffect->fontId = "fonts/RoboNoto-Medium.ttf"; // fonts/Chewy-Regular.ttf
		damageTextEffect->sortLayer = 9;
		damageTextEffect->sortOrder = 1;
		damageTextEffect->SetActive(false);
		damageTextEffect->SetPool(&damageTextEffectPool);
	};
	damageTextEffectPool.Init(count);
}

void SceneGame::CreateUpgradeUI(float posiX, float posiY, std::string name, float scale)
{
	const UpgradeInfo& upgradeInfo = DATATABLE_MGR.Get<UpgradeTable>(DataTable::Ids::Upgrade)->Get(name);

	std::string textureId = upgradeInfo.textureId;
	name                  = upgradeInfo.name;
	std::string category  = upgradeInfo.category;
	std::string highlight = upgradeInfo.highlight;
	std::string title     = upgradeInfo.title;
	float       value     = upgradeInfo.value;
	int         amount    = upgradeInfo.amount;
	int         tier      = upgradeInfo.tier;

	float blank = 20.0f * scale;
	sf::Vector2f thumbSize(100.0f * scale, 100.0f * scale);

	sf::Vector2f fullBoxSize(360.0f * scale, 270.0f * scale);
	sf::Vector2f buttonBoxSize(180.0f * scale, 60.0f * scale);

	BaseUI* box = (BaseUI*)AddGo(new BaseUI("Box" + name, uiType::Box));
	box->sortLayer = 111;
	box->SetPosition(posiX, posiY);
	box->SetSizeAdd(fullBoxSize.x, fullBoxSize.y);
	box->SetActive(false);

	SpriteGo* thumbnail = (SpriteGo*)AddGo(new SpriteGo(textureId, "thumbnail_" + name));
	thumbnail->sortLayer = 113;
	thumbnail->SetPosition(posiX + blank, posiY + blank);
	thumbnail->sprite.setScale(scale, scale); // 100 x 100 px * scale
	thumbnail->SetActive(false);

	BaseUI* thumbnailBox = (BaseUI*)AddGo(new BaseUI("thumbnailBox" + name, uiType::Box));
	thumbnailBox->sortLayer = 112;
	thumbnailBox->SetColor(255, 255, 255, 32);
	thumbnailBox->SetPosition(posiX + blank, posiY + blank);
	thumbnailBox->SetSizeAdd(thumbSize.x, thumbSize.y);
	thumbnailBox->SetActive(false);

	BaseUI* buttonBox = (BaseUI*)AddGo(new BaseUI("ButtonBox" + name, uiType::Box));
	buttonBox->sortLayer = 112;
	buttonBox->SetColor(255, 255, 255, 32);
	buttonBox->SetPosition(posiX + fullBoxSize.x * 0.25f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	buttonBox->SetSizeAdd(buttonBoxSize.x, buttonBoxSize.y);
	buttonBox->SetActive(false);

	buttonBox->OnEnter = [buttonBox]()
	{
		buttonBox->SetColor(255, 255, 255, 64);
	};
	buttonBox->OnExit = [buttonBox]()
	{
		buttonBox->SetColor(255, 255, 255, 32);
	};
	buttonBox->OnClick = [buttonBox, this, name, category, value]()
	{
		SetPaused(!IsPaused());

		levelUpPoint = 0;
		
		isUpgrade = false;

		SetActiveAllUpgradeUI(false);
		player->UpgradeStat(name, category, value);

		buttonBox->SetColor(255, 255, 255, 32);
	};

	std::string amountString = std::to_string(amount);

	TextGo* moneyAmount = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", amountString + name));
	moneyAmount->sortLayer = 114;
	moneyAmount->SetOrigin(Origins::TC);
	moneyAmount->SetCharacterSize(40);
	moneyAmount->SetPosition(posiX + fullBoxSize.x * 0.5f + 3.0f, posiY + fullBoxSize.y - buttonBoxSize.y - blank + 4.0f);
	moneyAmount->SetFillColor(sf::Color(255, 255, 255, 255));
	moneyAmount->SetScale(scale, scale);
	moneyAmount->SetString("SELECT");	//moneyAmount->SetString(amountString);
	moneyAmount->text.setLetterSpacing(1.15f);
	moneyAmount->SetActive(false);

	TextGo* upgradeName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "upgrade_" + name));
	upgradeName->sortLayer = 115;
	upgradeName->SetOrigin(Origins::TL);
	upgradeName->SetCharacterSize(30);
	upgradeName->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 30.0f);
	upgradeName->SetFillColor(sf::Color(255, 255, 255, 255));
	upgradeName->text.setScale(0.9f * scale, 1.0f * scale);
	upgradeName->SetString(name);
	upgradeName->SetActive(false);

	TextGo* upgradeCategory = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", category + name));
	upgradeCategory->sortLayer = 115;
	upgradeCategory->SetOrigin(Origins::TL);
	upgradeCategory->SetCharacterSize(25);
	upgradeCategory->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 60.0f);
	upgradeCategory->SetFillColor(sf::Color(255, 255, 128, 255));
	upgradeCategory->SetScale(0.9f * scale, 1.0f * scale);
	upgradeCategory->SetString("Upgrade");
	upgradeCategory->SetActive(false);

	TextGo* upgradeHighlight = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", highlight + name));
	upgradeHighlight->sortLayer = 116;
	upgradeHighlight->SetOrigin(Origins::TL);
	upgradeHighlight->SetCharacterSize(21);
	upgradeHighlight->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	upgradeHighlight->SetFillColor(sf::Color(0, 192, 64, 255));
	upgradeHighlight->text.setOutlineThickness(2.0f);
	upgradeHighlight->SetScale(0.9f * scale, 1.0f * scale);
	upgradeHighlight->SetString(highlight);
	upgradeHighlight->SetActive(false);

	TextGo* upgradeTitle = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", title + name));
	upgradeTitle->sortLayer = 115;
	upgradeTitle->SetOrigin(Origins::TL);
	upgradeTitle->SetCharacterSize(21);
	upgradeTitle->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2);
	upgradeTitle->SetFillColor(sf::Color(255, 255, 255, 255));
	upgradeTitle->SetScale(0.9f * scale, 1.0f * scale);
	upgradeTitle->SetString(title);
	upgradeTitle->SetActive(false);

	switch (tier) // Tier Color, Highlight Color Font Sharpness
	{
	case 1:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		upgradeHighlight->text.setOutlineColor(sf::Color(0, 0, 0, 255));
		break;
	case 2:
		box->SetStrokeColor(75, 175, 225, 255);
		box->SetColor(15, 30, 45, 255);
		upgradeHighlight->text.setOutlineColor(sf::Color(15, 30, 45, 255));
		break;
	case 3:
		box->SetStrokeColor(150, 75, 225, 255);
		box->SetColor(15, 15, 30, 255);
		upgradeHighlight->text.setOutlineColor(sf::Color(15, 15, 30, 255));
		break;
	case 4:
		box->SetStrokeColor(225, 50, 50, 255);
		box->SetColor(30, 15, 15, 255);
		upgradeHighlight->text.setOutlineColor(sf::Color(30, 15, 15, 255));
		break;
	default:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		upgradeHighlight->text.setOutlineColor(sf::Color(0, 0, 0, 255));
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
	float       value     = upgradeInfo.value;
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

void SceneGame::SetActiveAllUpgradeUI(bool active)
{
	std::vector<std::string> allUpgradeNames;
	allUpgradeNames.insert(allUpgradeNames.end(), upgradeNamesColumn1.begin(), upgradeNamesColumn1.end());
	allUpgradeNames.insert(allUpgradeNames.end(), upgradeNamesColumn2.begin(), upgradeNamesColumn2.end());
	allUpgradeNames.insert(allUpgradeNames.end(), upgradeNamesColumn3.begin(), upgradeNamesColumn3.end());
	allUpgradeNames.insert(allUpgradeNames.end(), upgradeNamesColumn4.begin(), upgradeNamesColumn4.end());

	for (const std::string& name : allUpgradeNames)
	{
		SetActiveUpgradeUI(name, active);
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
	int         amount   = itemInfo.amount;
	int         tier     = itemInfo.tier;
	int         damage   = itemInfo.damage;
	float       critical = itemInfo.critical;
	float       cooldown = itemInfo.cooldown;
	float       range    = itemInfo.range;

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
	thumbnail->sortLayer = 123;
	thumbnail->SetPosition(posiX + blank, posiY + blank);
	thumbnail->sprite.setScale(scale, scale); // 100 x 100 px * scale
	thumbnail->SetActive(false);

	BaseUI* thumbnailBox = (BaseUI*)AddGo(new BaseUI("thumbnailBox" + name, uiType::Box));
	thumbnailBox->sortLayer = 122;
	thumbnailBox->SetColor(255, 255, 255, 32);
	thumbnailBox->SetPosition(posiX + blank, posiY + blank);
	thumbnailBox->SetSizeAdd(thumbSize.x, thumbSize.y);
	thumbnailBox->SetActive(false);

	BaseUI* buttonBox = (BaseUI*)AddGo(new BaseUI("ButtonBox" + name, uiType::Box));
	buttonBox->sortLayer = 122;
	buttonBox->SetColor(255, 255, 255, 32);
	buttonBox->SetPosition(posiX + fullBoxSize.x * 0.25f, posiY + fullBoxSize.y - buttonBoxSize.y - blank);
	buttonBox->SetSizeAdd(buttonBoxSize.x, buttonBoxSize.y);
	buttonBox->SetActive(false);

	buttonBox->OnEnter = [buttonBox]()
	{
		buttonBox->SetColor(255, 255, 255, 64);
	};
	buttonBox->OnExit = [buttonBox]()
	{
		buttonBox->SetColor(255, 255, 255, 32);
	};
	buttonBox->OnClick = [buttonBox, this, name, damage, critical, cooldown, range]()
	{
		SetPaused(!IsPaused());
		isShop = false;
		isPlaying = true;

		SetGun(name, damage, critical, cooldown, range);

		SetActiveShopUI("Taser Gun", false);
		SetActiveShopUI("Shotgun", false);
		SetActiveShopUI("Pistol", false);
		SetActiveAllShopUI(false);

		buttonBox->SetColor(255, 255, 255, 32);
	};

	/* 
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
	*/

	std::string amountString = std::to_string(amount);
	TextGo* moneyAmount = (TextGo*)AddGo(new TextGo("fonts/Chewy-Regular.ttf", amountString + name));
	moneyAmount->sortLayer = 124;
	moneyAmount->SetOrigin(Origins::TC);
	moneyAmount->SetCharacterSize(40);
	moneyAmount->SetPosition(posiX + fullBoxSize.x * 0.5f + 3.0f, posiY + fullBoxSize.y - buttonBoxSize.y - blank + 4.0f);
	moneyAmount->SetFillColor(sf::Color(255, 255, 255, 255));
	moneyAmount->SetScale(scale, scale);
	moneyAmount->SetString("SELECT");
	moneyAmount->text.setLetterSpacing(1.15f);
	moneyAmount->SetActive(false);

	TextGo* itemName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "item_" + name));
	itemName->sortLayer = 125;
	itemName->SetOrigin(Origins::TL);
	itemName->SetCharacterSize(30);
	itemName->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 30.0f);
	itemName->SetFillColor(sf::Color(255, 255, 255, 255));
	itemName->text.setScale(0.9f * scale, 1.0f * scale);
	itemName->SetString(name);
	itemName->SetActive(false);

	TextGo* itemCategory = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", category + name));
	itemCategory->sortLayer = 125;
	itemCategory->SetOrigin(Origins::TL);
	itemCategory->SetCharacterSize(25);
	itemCategory->SetPosition(posiX + thumbSize.x + blank * 2, posiY + 60.0f);
	itemCategory->SetFillColor(sf::Color(255, 255, 128, 255));
	itemCategory->SetScale(0.9f * scale, 1.0f * scale);
	itemCategory->SetString(category);
	itemCategory->SetActive(false);

	TextGo* itemTitle = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", title + name));
	itemTitle->sortLayer = 129;
	itemTitle->SetOrigin(Origins::TL);
	itemTitle->SetCharacterSize(20);
	itemTitle->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2.0f - 3.0f);
	itemTitle->SetFillColor(sf::Color(0, 255, 32, 255));
	itemTitle->text.setOutlineThickness(2.0f);
	itemTitle->SetScale(0.9f * scale, 1.0f * scale);
	itemTitle->SetString(title);
	itemTitle->SetActive(false);

	std::string damageString = std::to_string(damage);
	TextGo* itemDamage = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", damageString + name));
	itemDamage->sortLayer = 128;
	itemDamage->SetOrigin(Origins::TL);
	itemDamage->SetCharacterSize(20);
	itemDamage->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 2.0f - 3.0f);
	itemDamage->SetFillColor(sf::Color(255, 255, 255, 255));
	itemDamage->SetScale(0.9f * scale, 1.0f * scale);
	itemDamage->SetString("Damage : " + damageString);
	itemDamage->SetActive(false);

	std::string criticalString = FloatToString(critical, 1);
	TextGo* itemCritical = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", criticalString + name));
	itemCritical->sortLayer = 128;
	itemCritical->SetOrigin(Origins::TL);
	itemCritical->SetCharacterSize(20);
	itemCritical->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 3.5f - 3.0f);
	itemCritical->SetFillColor(sf::Color(255, 255, 255, 255));
	itemCritical->SetScale(0.9f * scale, 1.0f * scale);
	itemCritical->SetString("Critical : " + criticalString + "%");
	itemCritical->SetActive(false);

	std::string cooldownString = FloatToString(cooldown, 2);
	TextGo* itemCooldown = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", cooldownString + name));
	itemCooldown->sortLayer = 128;
	itemCooldown->SetOrigin(Origins::TL);
	itemCooldown->SetCharacterSize(20);
	itemCooldown->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 5.0f - 3.0f);
	itemCooldown->SetFillColor(sf::Color(255, 255, 255, 255));
	itemCooldown->SetScale(0.9f * scale, 1.0f * scale);
	itemCooldown->SetString("Cooldown : " + cooldownString);
	itemCooldown->SetActive(false);

	std::string rangeString = FloatToString(range, 0);
	TextGo* itemRange = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", rangeString + name));
	itemRange->sortLayer = 128;
	itemRange->SetOrigin(Origins::TL);
	itemRange->SetCharacterSize(20);
	itemRange->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 6.5f - 3.0f);
	itemRange->SetFillColor(sf::Color(255, 255, 255, 255));
	itemRange->SetScale(0.9f * scale, 1.0f * scale);
	itemRange->SetString("Range : " + rangeString);
	itemRange->SetActive(false);

	TextGo* itemInfoText = (TextGo*)AddGo(new TextGo("fonts/Kanit-Medium.ttf", infoText + name));
	itemInfoText->sortLayer = 125;
	itemInfoText->SetOrigin(Origins::TL);
	itemInfoText->SetCharacterSize(20);
	itemInfoText->SetPosition(posiX + blank, posiY + thumbSize.y + blank * 8.5f);
	itemInfoText->SetFillColor(sf::Color(255, 255, 255, 255));
	itemInfoText->SetScale(0.9f * scale, 1.0f * scale);
	itemInfoText->SetString(infoText);
	itemInfoText->SetActive(false);

	switch (tier) // Tier Color, Highlight Color Font Sharpness
	{
	case 1:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		itemTitle->text.setOutlineColor(sf::Color(0, 0, 0, 255));
		break;
	case 2:
		box->SetStrokeColor(75, 175, 225, 255);
		box->SetColor(15, 30, 45, 255);
		itemTitle->text.setOutlineColor(sf::Color(15, 30, 45, 255));
		break;
	case 3:
		box->SetStrokeColor(150, 75, 225, 255);
		box->SetColor(15, 15, 30, 255);
		itemTitle->text.setOutlineColor(sf::Color(15, 15, 30, 255));
		break;
	case 4:
		box->SetStrokeColor(225, 50, 50, 255);
		box->SetColor(30, 15, 15, 255);
		itemTitle->text.setOutlineColor(sf::Color(30, 15, 15, 255));
		break;
	default:
		box->SetStrokeColor(0, 0, 0, 0);
		box->SetColor(0, 0, 0, 255);
		itemTitle->text.setOutlineColor(sf::Color(0, 0, 0, 255));
		break;
	}
}

void SceneGame::SetActiveAllShopUI(bool active)
{
	std::vector<std::string> allShopNames;
	allShopNames.insert(allShopNames.end(), shopNames.begin(), shopNames.end());

	for (const std::string& name : allShopNames)
	{
		SetActiveShopUI(name, active);
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
	int         tier     = itemInfo.tier;
	int         damage   = itemInfo.damage;
	float       critical = itemInfo.critical;
	float       cooldown = itemInfo.cooldown;
	float       range    = itemInfo.range;

	std::string amountString   = std::to_string(amount);
	std::string tierString     = std::to_string(tier);
	std::string damageString   = std::to_string(damage);
	std::string criticalString = FloatToString(critical, 1);
	std::string cooldownString = FloatToString(cooldown, 2);
	std::string rangeString    = FloatToString(range, 0);

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
		amountString   + name,
		tierString     + name,
		damageString   + name,
		criticalString + name,
		cooldownString + name,
		rangeString    + name,
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

	TextGo* boxName = (TextGo*)AddGo(new TextGo("fonts/Kanit-Bold.ttf", "BoxName"));
	boxName->sortLayer = 108;
	boxName->SetOrigin(Origins::TC);
	boxName->SetCharacterSize(40);
	boxName->SetPosition(posiX + fullBoxSize.x * 0.5f, posiY + thumbSize.y * 0.5f);
	boxName->SetFillColor(sf::Color(255, 255, 128, 255));
	boxName->text.setScale(0.9f * scale, 1.0f * scale);
	boxName->SetString("Player Info");
	boxName->SetActive(false);

	SpriteGo* statsMerge = (SpriteGo*)AddGo(new SpriteGo("graphics/game/stats_merge.png", "StatsMerge"));
	statsMerge->sortLayer = 103;
	statsMerge->SetPosition(posiX + fullBoxSize.x * 0.1f + blank * 0.5f, posiY + thumbSize.y + blank * 2);
	statsMerge->sprite.setScale(scale, scale);
	statsMerge->SetActive(false);

	TextGo* statName = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", "StatName"));
	statName->sortLayer = 107;
	statName->SetOrigin(Origins::TL);
	statName->SetCharacterSize(20);
	statName->SetPosition(posiX + fullBoxSize.x * 0.2f + blank, posiY + thumbSize.y + blank * 2);
	statName->SetFillColor(sf::Color(255, 255, 255, 255));
	statName->text.setScale(0.9f * scale, 1.0f * scale);
	statName->SetString("Maximum HP\nHP Regeneration\nAttack Damage\nAttack Speed\nCritical Chance\nArmor Point\nDodge Point\nMove Speed");
	statName->SetActive(false);

	TextGo* statPoint = (TextGo*)AddGo(new TextGo("fonts/Kanit-SemiBold.ttf", "StatPoint"));
	statPoint->sortLayer = 106;
	statPoint->SetOrigin(Origins::TL);
	statPoint->SetCharacterSize(20);
	statPoint->SetPosition(posiX + fullBoxSize.x * 0.7f + blank, posiY + thumbSize.y + 1.5f + blank * 2);
	statPoint->SetFillColor(sf::Color(0, 255, 32, 255));
	statPoint->text.setScale(0.9f * scale, 1.0f * scale);
	statPoint->SetString
	(
		FloatToString(player->GetStat("maxHp")) + "\n" +
		FloatToString(player->GetStat("hpRegen"), 1) + "\n" +
		FloatToPercentString(player->GetStat("damage")) + "\n" +
		FloatToPercentString(player->GetStat("attackSpeed")) + "\n" +
		FloatToPercentString(player->GetStat("critical")) + "\n" +
		FloatToString(player->GetStat("armor")) + "\n" +
		FloatToPercentString(player->GetStat("dodge")) + "\n" +
		FloatToString(player->GetStat("moveSpeed")) + "\n"
	);
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
	statPoint->SetString
	(
		FloatToString(player->GetStat("maxHp")) + "\n" +
		FloatToString(player->GetStat("hpRegen"), 1) + "\n" +
		FloatToPercentString(player->GetStat("damage")) + "\n" +
		FloatToPercentString(player->GetStat("attackSpeed")) + "\n" +
		FloatToPercentString(player->GetStat("critical")) + "\n" +
		FloatToString(player->GetStat("armor")) + "\n" +
		FloatToPercentString(player->GetStat("dodge")) + "\n" +
		FloatToString(player->GetStat("moveSpeed")) + "\n"
	);
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

void SceneGame::SetCountUI(const std::string& name, int count)
{
	std::string countString = std::to_string(count);

	TextGo* text = (TextGo*)FindGo(name);
	text->SetString(countString);
	text->SetActive(true);
}

void SceneGame::SetHpUI(float currentHp, float maxHp)
{
	if (currentHp > 0.0f)
	{
		SpriteGo* hpBar = (SpriteGo*)FindGo("HpBarMain");
		hpBar->sprite.setOrigin(0.0f, 0.0f);
		hpBar->sprite.setScale(currentHp / maxHp, 1.0f);

		std::string currentHpString = std::to_string((int)currentHp);
		std::string maxHpString = std::to_string((int)maxHp);

		TextGo* hpText = (TextGo*)FindGo("HpText");
		hpText->SetString(currentHpString + " / " + maxHpString);
	}
}

void SceneGame::SetExpUI(float currentExp)
{
	SpriteGo* expBar = (SpriteGo*)FindGo("ExpBarMain");
	expBar->sprite.setOrigin(0.0f, 0.0f);
	expBar->sprite.setScale(1.0f * currentExp, 1.0f);
	expBar->sprite.setColor(sf::Color(0, 192, 64, 255));

	std::string levelString = std::to_string(currentPlayerLevel);

	TextGo* expCurrentText = (TextGo*)FindGo("LvText");
	expCurrentText->SetString("Lv " + levelString);
}

void SceneGame::SetLevelUpUI(int level)
{
	currentPlayerLevel = level;

	if (currentPlayerLevel == nextPlayerLevel)
	{
		levelUpPoint++;
		nextPlayerLevel++;
	}
}

void SceneGame::SetGun(const std::string& name, float damage, float critical, float cooldown, float range)
{
	BaseGun* gun = (BaseGun*)FindGo("BaseGun");

	// 미리 로딩하고 불러올지 새로 텍스처를 씌울지..
}

void SceneGame::OnDiePlayer()
{
	isPlaying = false;
	player->Reset();

	SCENE_MGR.ChangeScene(SceneId::Title);
}

std::string SceneGame::FloatToString(float value, int precision)
{
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(precision) << value;
	return stream.str();
}

std::string SceneGame::FloatToPercentString(float value)
{
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(0) << (value * 100) << "%";
	return stream.str();
}