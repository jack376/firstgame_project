#include "stdafx.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "MonsterTable.h"
#include "ShopTable.h"
#include "UpgradeTable.h"
#include "PlayerInfoTable.h"

void DataTableMgr::LoadAll()
{
	tables.insert({ DataTable::Ids::String, new StringTable() });
	tables.insert({ DataTable::Ids::Monster, new MonsterTable() });
	tables.insert({ DataTable::Ids::Shop, new ShopTable() });
	tables.insert({ DataTable::Ids::Upgrade, new UpgradeTable() });
	tables.insert({ DataTable::Ids::PlayerInfo, new PlayerInfoTable() });

	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERR: DATA TABLE LOAD FAIL" << std::endl;
			exit(-1);
		}
	}
}

void DataTableMgr::ReleaseAll()
{
	for (auto pair : tables)
	{
		//pair.second->Release();
		delete pair.second;
	}
	tables.clear();
}
