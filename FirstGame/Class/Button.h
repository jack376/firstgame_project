#pragma once
#include "SpriteGo.h"

class Button : public SpriteGo
{
protected:
	bool isHover = false;

public:
	sf::Text text;

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	Button(const std::string& textureId, const std::string& n);
	virtual ~Button() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

