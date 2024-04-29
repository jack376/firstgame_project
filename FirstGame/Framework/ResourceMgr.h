#pragma once
#include "Singleton.h"

class ResourceMgr : public Singleton<ResourceMgr>
{
	friend Singleton<ResourceMgr>;

protected:
	ResourceMgr() = default;
	virtual ~ResourceMgr() override;
	
	std::map<std::string, std::tuple<sf::Texture*, bool>> mapTexture;
	std::map<std::string, std::tuple<sf::Font*, bool>> mapFont;
	std::map<std::string, std::tuple<sf::SoundBuffer*, bool>> mapSoundBuffer;

public:
	void Init();

	void UnloadAll();

	void LoadFromCSV(const std::string path, bool isDefault);
	void Load(ResourceTypes t, const std::string path, bool isDefault);

	void Unload(ResourceTypes t, const std::string path);

	sf::Texture* GetTexture(const std::string& id);
	sf::Font* GetFont(const std::string& id);
	sf::SoundBuffer* GetSoundBuffer(const std::string& id);
};

#define RESOURCE_MGR (ResourceMgr::Instance())
