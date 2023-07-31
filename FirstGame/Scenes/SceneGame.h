#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include "BulletHitEffect.h"
#include "DieEffect.h"
#include "EntityEffect.h"

class Monster;
class Player;
class BaseUI;
class BassBullet;
class BaseGun;
class SpriteGo;
class VertexArrayGo;

class SceneGame : public Scene
{
protected:	
	int monsterCount = 0;
	int monsterKillCount = 0;

	int money = 0;
	int playerLevel = 1;
	int levelUpPoint = 0;

	int waveCount = 1;
	float waveTimer = 30.0f;
	float lastSpawnTime = 5.0f;

	bool isPlaying   = false;

	bool isShop      = false;
	bool shopCreated = false;

	bool isInfo      = false;
	bool infoCreated = false;

	bool isUpgrade      = false;
	bool upgradeCreated = false;

	bool isSlide = false;
	bool slideCreated = false;

	sf::FloatRect wallBounds;

	sf::Vector2f currentPlayerPosition;
	Player* player = nullptr;

	ObjectPool<Monster> monsterPool;

	ObjectPool<BulletHitEffect> bulletHitEffectPool;
	ObjectPool<DieEffect> dieEffectPool;
	ObjectPool<EntityEffect> entityEffectPool;

public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;
	void Reset();
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	VertexArrayGo* CreateTile(std::string textureId, sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize);

	Monster* GetNearMonsterSearch();
	const std::list<Monster*>* GetMonsterList() { return &monsterPool.GetUseList(); }
	
	ObjectPool<Monster> & GetMonsterPool() { return monsterPool; }
	ObjectPool<BulletHitEffect> & GetBulletHitEffectPool() { return bulletHitEffectPool; }
	ObjectPool<DieEffect> & GetDieEffectPool() { return dieEffectPool; }
	ObjectPool<EntityEffect> & GetEntityEffectPool() { return entityEffectPool; }

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void CreateMonsters(int count);
	void SpawnMonsters(int count, sf::Vector2f playerCenter, sf::Vector2f mapCenter, float radius);
	void OnDieMonster(Monster* monster);

	void CreateBulletHitEffect(int count);
	void CreateDieEffect(int count);
	void CreateEntityEffect(int count);

	void CreateUpgradeUI(float posiX, float posiY, std::string name, float scale = 1.0f);
	void SetActiveUpgradeUI(std::string name, bool active);

	void CreateShopUI(float posiX, float posiY, std::string name, float scale = 1.0f);
	void SetActiveShopUI(std::string name, bool active);

	void CreatePlayerInfoUI(float posiX, float posiY, float scale = 1.0f);
	void SetActivePlayerInfoUI( bool active);

	void CreateBar(const std::string& id, const std::string& name, float posX, float posY, int sort = 0, sf::Color color = sf::Color::White);
	void CreateText(const std::string& name, const std::string& str, float posX = 0.0f, float posY = 0.0f, int fontSize = 48, bool originMC = false);

	void SetCountUI(const std::string& name, int count);
	void SetHpUI(float currentHp, float maxHp);
	void SetExpUI(float currentExp);
	void SetLevelUpUI(int level);

	void OnDiePlayer();
};

