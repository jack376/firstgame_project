#include "stdafx.h"
#include "MonsterTable.h"
#include "rapidcsv.h"

bool MonsterTable::Load()
{
    rapidcsv::Document doc("tables/MonsterTable.csv");

    std::vector<int>         types       = doc.GetColumn<int>(0);
    std::vector<std::string> textureIds  = doc.GetColumn<std::string>(1);
    std::vector<float>       speeds      = doc.GetColumn<float>(2);
    std::vector<int>         maxHps      = doc.GetColumn<int>(3);
    std::vector<int>         damages     = doc.GetColumn<int>(4);
    std::vector<float>       attackRates = doc.GetColumn<float>(5);

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
        throw std::runtime_error("ERROR : Undefined Zombie Type");
    }
    return find->second;
}

