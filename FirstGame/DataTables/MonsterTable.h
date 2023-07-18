#pragma once
#include "DataTable.h"
#include "Monster.h"

struct MonsterInfo
{
    int monsterType;
    std::string textureId;
    float speed;
    int maxHp;
    int damage;
    float attackRate;
};

class MonsterTable : public DataTable
{
protected:
    std::unordered_map<Monster::Types, MonsterInfo> table;
    //std::vector<Monster::Types, MonsterInfo> table;

public:
    MonsterTable() : DataTable(DataTable::Ids::Monster) {};
    virtual ~MonsterTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const MonsterInfo& Get(Monster::Types id);
};
