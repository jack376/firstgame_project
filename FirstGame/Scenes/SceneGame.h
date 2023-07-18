#pragma once
#include "Scene.h"
#include "ObjectPool.h"

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

	ObjectPool<Monster> monsterPool;

	Player* player = nullptr;

	int monsterCount = 0;

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

	//bool GunRangeSearch(float distance);

	ObjectPool<Monster>& GetMonsterPool();
	const std::list<Monster*>* GetMonsterList();

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void CreateMonsters(int count);
	void SpawnMonsters(int count, sf::Vector2f center, float radius);

	Monster* GetClosestMonsterToPlayer();
};

