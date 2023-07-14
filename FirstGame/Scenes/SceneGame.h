#pragma once
#include "Scene.h"

class Player;
class SpriteGo;
class SceneGame : public Scene
{
protected:
	Player* playerBody;
	Player* playerLeftLegs;
	Player* playerRightLegs;

	SpriteGo* background;

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
};

