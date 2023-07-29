#pragma once
#include "GameObject.h"

class BaseUI : public GameObject
{
protected:
	uiType uiStatus = uiType::Text;

	bool isHover = false;
	unsigned int fontSize = 54;

	float buttonSizeX = 0.0f;
	float buttonSizeY = 0.0f;
	float textHeight  = 0.0f;

	sf::Sprite thumbnail;

	sf::Text text;
	sf::Color textColor;

	sf::RectangleShape body;
	sf::Color bodyColor;

	std::vector<sf::Sprite> cornerStrokes;
	sf::Sprite cornerStroke;

	std::vector<sf::RectangleShape> sides;
	sf::RectangleShape side;

	sf::Color strokeColor;

	std::vector<sf::Sprite> corners;
	sf::Sprite corner;

	std::vector<sf::Sprite> sideStrokes;
	sf::Sprite sideStroke;

	sf::Vector2f buttonModifySize;
	sf::Vector2f buttonModifyPosition;

	sf::RectangleShape buttonCollider;

public:
	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	BaseUI(const std::string& n = "", uiType uiStatus = uiType::Text);
	virtual ~BaseUI() override;

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	bool CanBePaused() const override { return false; }

	void SetColor(int red = 0, int green = 0, int blue = 0, int alpha = 255);
	void SetTextColor(int red = 255, int green = 255, int blue = 255, int alpha = 255);
	void SetStrokeColor(int red = 0, int green = 0, int blue = 0, int alpha = 0);
	void SetThumbnailColor(int red = 0, int green = 0, int blue = 0, int alpha = 255);

	void SetString(const std::string& string);
	void SetCharacterSize(unsigned int fontSize = 54);
	void SetSizeAdd(float x, float y);
	void SetPosition(float x, float y);
};

