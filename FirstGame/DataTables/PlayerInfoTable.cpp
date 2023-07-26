#include "stdafx.h"
#include "PlayerInfoTable.h"
#include "rapidcsv.h"

bool PlayerInfoTable::Load()
{
    rapidcsv::Document doc("tables/PlayerInfoTable.csv");

    std::vector<std::string> textureId   = doc.GetColumn<std::string>(0);
    std::vector<std::string> name        = doc.GetColumn<std::string>(1);
    std::vector<int>         maxHp       = doc.GetColumn<int>(2);
    std::vector<int>         maxExp      = doc.GetColumn<int>(3);
    std::vector<float>       hpRegen     = doc.GetColumn<float>(4);
    std::vector<float>       damage      = doc.GetColumn<float>(5);
    std::vector<float>       attackSpeed = doc.GetColumn<float>(6);
    std::vector<float>       critical    = doc.GetColumn<float>(7);
    std::vector<float>       armor       = doc.GetColumn<float>(8);
    std::vector<float>       dodge       = doc.GetColumn<float>(9);
    std::vector<float>       moveSpeed   = doc.GetColumn<float>(10);

    for (int i = 0; i < textureId.size(); ++i)
    {
        PlayerInfo playerInfo;

        playerInfo.textureId   = textureId[i];
        playerInfo.name        = name[i];
        playerInfo.maxHp       = maxHp[i];
        playerInfo.maxExp      = maxExp[i];
        playerInfo.hpRegen     = hpRegen[i];
        playerInfo.damage      = damage[i];
        playerInfo.attackSpeed = attackSpeed[i];
        playerInfo.critical    = critical[i];
        playerInfo.armor       = armor[i];
        playerInfo.dodge       = dodge[i];
        playerInfo.moveSpeed   = moveSpeed[i];

        table[name[i]] = playerInfo;
    }
    return true;
}

void PlayerInfoTable::Release()
{
    table.clear();
}

const PlayerInfo& PlayerInfoTable::Get(const std::string& name)
{
    auto find = table.find(name);
    if (find == table.end())
    {
        throw std::runtime_error("ERROR : Undefined Item Name");
    }
    return find->second;
}