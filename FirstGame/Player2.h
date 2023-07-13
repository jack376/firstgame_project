#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player2 : public SpriteGo
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
		bool flipX = false;
		sf::Vector2f point;
	};

protected:
	AnimationController animation;
	sf::Vector2f direction;
	float speed = 500.f;

	bool filpX = false;
	bool isGround = true;

	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

public:
	Player2(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Player2() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);
};

