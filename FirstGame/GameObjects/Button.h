#pragma once
#include "GameObject.h"

class Button : public GameObject
{
protected:
	bool isHover = false;
	unsigned int fontSize;
	sf::Text text;
	sf::RectangleShape body;
	sf::Color bodyColor;
	sf::RectangleShape buttonCollider;
	
	std::vector<sf::Sprite> corners;
	sf::Sprite corner;

	std::vector<sf::RectangleShape> sides;
	sf::RectangleShape side;

	sf::Vector2f buttonModifySize;
	sf::Vector2f buttonModifyPosition;

public:
	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	Button(const std::string& n);
	virtual ~Button() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetColor(int red = 0, int green = 0, int blue = 0, int alpha = 192);
	void SetString(const std::string& string);
	void SetSize(float x, float y);
	void SetPosition(float x, float y);
};

