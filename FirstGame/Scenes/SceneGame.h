#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include <SpriteEffect.h>
#include <BaseBullet.h>

class BassBullet;
class VertexArrayGo;
class Monster;
class Player;
class BaseGun;
class SpriteGo;

class SceneGame : public Scene
{
protected:	
	sf::FloatRect wallBounds;

	sf::Vector2f currentPlayerPosition;
	Player* player = nullptr;

	int monsterCount = 0;
	ObjectPool<Monster> monsterPool;
	ObjectPool<SpriteEffect> bulletHitEffectPool;

	std::vector<std::vector<std::list<Monster*>>> gridList;

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

	//void CreateGridList(int gridCount, int cellSize);
	//std::vector<std::vector<std::list<Monster*>>>& GetGridList();

	VertexArrayGo* CreateTile(std::string textureId, sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize);

	ObjectPool<Monster>& GetMonsterPool();
	ObjectPool<SpriteEffect>& GetBulletHitEffectPool();
	const std::list<Monster*>* GetMonsterList();
	Monster* GetNearMonsterSearch(float posX, float posY);

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void CreateMonsters(int count);
	void SpawnMonsters(int count, sf::Vector2f center, float radius);

	void CreateBulletHitEffect(int count);
	void OnDieMonster(Monster* monster);
};

