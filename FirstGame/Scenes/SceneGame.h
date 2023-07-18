#pragma once
#include "Scene.h"

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

	bool TestFunc(float distance);
};

