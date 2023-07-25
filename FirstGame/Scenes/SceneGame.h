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

	bool isPlaying   = false;
	bool isShop      = false;
	bool shopCreated = false;

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
	
	ObjectPool<Monster>& GetMonsterPool() { return monsterPool; }
	ObjectPool<BulletHitEffect>& GetBulletHitEffectPool() { return bulletHitEffectPool; }
	ObjectPool<DieEffect>& GetDieEffectPool() { return dieEffectPool; }
	ObjectPool<EntityEffect>& GetEntityEffectPool() { return entityEffectPool; }

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void CreateMonsters(int count);
	void SpawnMonsters(int count, sf::Vector2f playerCenter, sf::Vector2f mapCenter, float radius);
	void OnDieMonster(Monster* monster);

	void CreateBulletHitEffect(int count);
	void CreateDieEffect(int count);
	void CreateEntityEffect(int count);

	void CreateShopUI(float posiX, float posiY, std::string name, float scale);
	void SetActiveShopUI(std::string name, bool active);

	void CreateBar(const std::string& id, const std::string& name, float posY, int sort = 0, sf::Color color = sf::Color::White);

	void SetHpUI(float currentHp);
	void SetExpUI(float currentExp);
	void OnDiePlayer();
};

