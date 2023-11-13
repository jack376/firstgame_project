#include "stdafx.h"
#include "MonsterTable.h"
#include "rapidcsv.h"

bool MonsterTable::Load()
{
    rapidcsv::Document doc("tables/MonsterTable.csv");

    auto types       = doc.GetColumn<int>(0);
    auto textureIds  = doc.GetColumn<std::string>(1);
    auto speeds      = doc.GetColumn<float>(2);
    auto maxHps      = doc.GetColumn<int>(3);
    auto damages     = doc.GetColumn<int>(4);
    auto attackRates = doc.GetColumn<float>(5);

    for (int i = 0; i < types.size(); ++i)
    {
        MonsterInfo info;

        info.monsterType = types[i];
        info.textureId   = textureIds[i];
        info.speed       = speeds[i];
        info.maxHp       = maxHps[i];
        info.damage      = damages[i];
        info.attackRate  = attackRates[i];

        table[(Monster::Types)types[i]] = info;
    }

    return true;
}

void MonsterTable::Release()
{
    table.clear();
}

const MonsterInfo& MonsterTable::Get(Monster::Types id)
{
    auto find = table.find(id);
    if (find == table.end())
    {
        throw std::runtime_error("ERROR : Undefined Monster Type");
    }

    return find->second;
}

