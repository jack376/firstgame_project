#pragma once
#include "Scene.h"

class TextGo;
class SpriteGo;
class BaseUI;

class SceneTitle : public Scene
{
protected:
	float flowTime = 0.0f;

public:
	SceneTitle();
	virtual ~SceneTitle() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	BaseUI* CreateButton(const std::string& name, const std::string& text, float posX, float posY, std::function<void()> onClickAction);

	SpriteGo* BgSpriteGo(const std::string& imagePath, const std::string& name, int sortLayer, float scaleX, float scaleY);

	sf::Vector2f TitleCharacterAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
	sf::Vector2f TitleSplashAnimation(float defaultPosition, float positionRange, float flowTimeBySpeed, bool invert = false);
};

