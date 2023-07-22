#pragma once
#include "SpriteGo.h"

class Button : public SpriteGo
{
protected:
	bool isHover = false;

public:
	sf::Text text;

	std::vector<sf::Sprite> corners;
	sf::Sprite corner;

	std::vector<sf::RectangleShape> sides;
	sf::RectangleShape side;

	sf::RectangleShape body;
	sf::Color bodyColor;

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

	void SetColor(int red = 0, int green = 0, int blue = 0, int alpha = 255);
	void SetString(const std::string& string);
};

