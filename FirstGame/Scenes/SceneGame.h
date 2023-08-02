#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include "BulletHitEffect.h"
#include "DieEffect.h"
#include "EntityEffect.h"
#include "DamageTextEffect.h"

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
	float flowTime = 0.0f;

	int monsterCount = 0;
	int monsterKillCount = 0;

	int money = 0;
	int currentPlayerLevel = 1;
	int nextPlayerLevel = 2;
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
	ObjectPool<DamageTextEffect> damageTextEffectPool;

	sf::Vector2f defaultResolution;
	sf::Vector2f windowSize;
	sf::Vector2f centerPos;
	float resolutionScale = 0.0f;

	std::vector<std::string> shopNames = { "Laser Gun", "Flame Gun", "SMG", "Plasma Rifle", "Rocket Launcher", "Minigun", "Gatling Laser", "Nuclear Launcher", "Chain Gun" };

	std::vector<std::string> upgradeNamesColumn1 = { "Chest I", "Chest II", "Chest III", "Chest IV", "Lungs I", "Lungs II", "Lungs III", "Lungs IV" };
	std::vector<std::string> upgradeNamesColumn2 = { "Reflexes I", "Reflexes II", "Reflexes III", "Reflexes IV", "Gun I", "Gun II", "Gun III", "Gun IV" };
	std::vector<std::string> upgradeNamesColumn3 = { "Fingers I", "Fingers II", "Fingers III", "Fingers IV", "Heart I", "Heart II", "Heart III", "Heart IV" };
	std::vector<std::string> upgradeNamesColumn4 = { "Back I", "Back II", "Back III", "Back IV", "Legs I", "Legs II", "Legs III", "Legs IV" };

	std::vector<int> monsterSpawnWeights = { 70, 50, 30, 15, 10, 10, 5, 5, 5 };

	float damageTextDuration = 1.0f;

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
	
	ObjectPool<Monster>& GetMonsterPool() { return monsterPool; }
	ObjectPool<BulletHitEffect>& GetBulletHitEffectPool() { return bulletHitEffectPool; }
	ObjectPool<DieEffect>& GetDieEffectPool() { return dieEffectPool; }
	ObjectPool<EntityEffect>& GetEntityEffectPool() { return entityEffectPool; }
	ObjectPool<DamageTextEffect>& GetDamageTextEffectPool() { return damageTextEffectPool; }

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void CreateMonsters(int count);
	void SpawnMonsters(int count, sf::Vector2f playerCenter, sf::Vector2f mapCenter, float radius);
	void OnDieMonster(Monster* monster);

	void CreateBulletHitEffect(int count);
	void CreateDieEffect(int count);
	void CreateEntityEffect(int count);
	void CreateDamageTextEffect(int count);

	void CreateUpgradeUI(float posiX, float posiY, std::string name, float scale = 1.0f);
	void SetActiveUpgradeUI(std::string name, bool active);
	void SetActiveAllUpgradeUI(bool active);

	void CreateShopUI(float posiX, float posiY, std::string name, float scale = 1.0f);
	void SetActiveShopUI(std::string name, bool active);
	void SetActiveAllShopUI(bool active);

	void CreatePlayerInfoUI(float posiX, float posiY, float scale = 1.0f);
	void SetActivePlayerInfoUI( bool active);

	void CreateBar(const std::string& id, const std::string& name, float posX, float posY, int sort = 0, sf::Color color = sf::Color::White);
	void CreateText(const std::string& name, const std::string& str, float posX = 0.0f, float posY = 0.0f, int fontSize = 48, bool originMC = false);

	void SetCountUI(const std::string& name, int count);
	void SetHpUI(float currentHp, float maxHp);
	void SetExpUI(float currentExp);
	void SetLevelUpUI(int level);

	void OnDiePlayer();

	std::string FloatToString(float value, int precision = 0);
	std::string FloatToPercentString(float value);
};

