#pragma once
#include "DataTable.h"

struct PlayerInfo
{
    std::string textureId;
    std::string name;
    int maxHp;
    int maxExp;
    float hpRegen;
    float damage;
    float attackSpeed;
    float critical;
    float armor;
    float dodge;
    float moveSpeed;
};

class PlayerInfoTable : public DataTable
{
protected:
    std::unordered_map<std::string, PlayerInfo> table;

public:
    PlayerInfoTable() : DataTable(DataTable::Ids::PlayerInfo) {};
    virtual ~PlayerInfoTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const PlayerInfo& Get(const std::string& name);
};