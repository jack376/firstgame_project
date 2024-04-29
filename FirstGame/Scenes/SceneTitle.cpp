#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneTitle.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "BaseUI.h"

SceneTitle::SceneTitle() : Scene(SceneId::Title)
{
	resourceListPath = "scripts/SceneTitleResourceList.csv";
}

void SceneTitle::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	float bgScaleX = windowSize.x / 1920.0f;
	float bgScaleY = windowSize.y / 1080.0f;
	float buttonPos = 50.0f;
	float buttonGap = 125.0f;

	BaseUI* exitButton = CreateButton
	(
		"ExitButton", "EXIT", buttonPos, windowSize.y - buttonPos - buttonGap * 1.0f, [this]()
		{
			FRAMEWORK.GetWindow().close();
		}
	);

	BaseUI* optionButton = CreateButton
	(
		"OptionButton", "OPTION", buttonPos, windowSize.y - buttonPos - buttonGap * 2.0f, [this]()
		{
			// Unused
		}
	);
	
	BaseUI* startButton = CreateButton
	(
		"StartButton", "START", buttonPos, windowSize.y - buttonPos - buttonGap * 3.0f, [this]()
		{
			SCENE_MGR.ChangeScene(SceneId::Game);
		}
	);

	BgSpriteGo("graphics/title/title_logo.png", "TitleLogo", 56, bgScaleX, 1.0f);
	BgSpriteGo("graphics/title/title_fade.png", "TitleFade", 55, bgScaleX, bgScaleY);
	BgSpriteGo("graphics/title/title_splash_front.png", "TitleSplashFront", 54, bgScaleX, bgScaleY);
	BgSpriteGo("graphics/title/title_character.png", "TitleCharacter", 53, bgScaleX, bgScaleY);
	BgSpriteGo("graphics/title/title_splash_middle.png", "TitleSplashMiddle", 52, bgScaleX, bgScaleY);
	BgSpriteGo("graphics/title/title_splash_rear.png", "TitleSplashRear", 51, bgScaleX, bgScaleY);
	BgSpriteGo("graphics/title/title_bg.png", "TitleBg", 50, bgScaleX, bgScaleY);

	SpriteGo* titleCharacter = (SpriteGo*)FindGo("TitleCharacter");
	titleCharacter->SetPosition(0.0f, 200.0f);

	TextGo* version = (TextGo*)AddGo(new TextGo("fonts/RoboNoto-Medium.ttf", "Version"));
	version->sortLayer = 100;
	version->SetString("version 1.0a");
	version->SetOrigin(Origins::BR);
	version->SetPosition(windowSize.x - 22.0f, windowSize.y - 25.0f);
	version->SetCharacterSize(24);
	version->SetFillColor(sf::Color(255, 255, 255, 192));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	flowTime += dt;

	SpriteGo* titleCharacter = (SpriteGo*)FindGo("TitleCharacter");
	titleCharacter->sprite.setScale(TitleCharacterAnimation(1.0f, 0.1f, flowTime));

	SpriteGo* titleSplashFront = (SpriteGo*)FindGo("TitleSplashFront");
	titleSplashFront->SetPosition(TitleSplashAnimation(-50.0f, 100.0f, flowTime, false));

	SpriteGo* titleSplashRear = (SpriteGo*)FindGo("TitleSplashRear");
	titleSplashRear->SetPosition(TitleSplashAnimation(-50.0f, 100.0f, flowTime, true));
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

BaseUI* SceneTitle::CreateButton(const std::string& name, const std::string& text, float posX, float posY, std::function<void()> onClickAction)
{
	BaseUI* newButton = (BaseUI*)AddGo(new BaseUI(name));
	newButton->sortLayer = 100;
	newButton->SetString(text);
	newButton->SetPosition(posX, posY);
	newButton->SetSizeAdd(0.0f, -15.0f);
	newButton->OnEnter = [newButton]()
	{
		newButton->SetColor(255, 255, 255, 192);
		newButton->SetTextColor(0, 0, 0, 255);
	};
	newButton->OnExit = [newButton]()
	{
		newButton->SetColor();
		newButton->SetTextColor();
	};
	newButton->OnClick = onClickAction;

	return newButton;
}

SpriteGo* SceneTitle::BgSpriteGo(const std::string& imagePath, const std::string& name, int sortLayer, float scaleX, float scaleY)
{
	SpriteGo* newSpriteGo = (SpriteGo*)AddGo(new SpriteGo(imagePath, name));
	newSpriteGo->sortLayer = sortLayer;
	newSpriteGo->SetOrigin(Origins::MC);
	newSpriteGo->sprite.setScale(scaleX, scaleY);

	return newSpriteGo;
}

sf::Vector2f SceneTitle::TitleCharacterAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	float animationX = sin(flowTimeBySpeed * 1.15f * _PI);
	float scaleX = defaultScale + ((animationX + 1.0f) / 2.0f) * scaleRange;

	float animationY = -sin(flowTimeBySpeed * 1.15f * _PI);
	float scaleY = defaultScale + ((animationY + 1.0f) / 2.0f) * scaleRange;

	return sf::Vector2f(scaleX, scaleY);
}

sf::Vector2f SceneTitle::TitleSplashAnimation(float defaultPosition, float positionRange, float flowTimeBySpeed, bool invert)
{
	if (!invert)
	{
		float animationX = cos(flowTimeBySpeed * 0.5f * _PI);
		float positionX = defaultPosition + ((animationX + 1.0f) / 2.0f) * positionRange;

		return sf::Vector2f(positionX, 0.0f);
	}
	else
	{
		float animationX = -cos(flowTimeBySpeed * 0.5f * _PI);
		float positionX = defaultPosition + ((animationX + 1.0f) / 2.0f) * positionRange;

		return sf::Vector2f(positionX, 0.0f);
	}
}