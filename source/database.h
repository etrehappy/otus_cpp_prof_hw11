#pragma once

#include <map>
#include <vector>
#include <string>
#include "parser.h"
#include "settings.h"
//struct SimpleTable
//{
//	int id;
//	std::string name;
//};

/*!
* @brief	int - column id,
*			std::string - column name
*/
using SimpleTable = std::map<int, std::string>;

class Database
{
public:
	Database();
	~Database();

	void CreateTable(std::string table_name);
	ECommandStatus Insert(const InsertArgs& );
	ECommandStatus Truncate(const std::string& table_name);
	std::string Intersection() const;
	std::string SymmetricDifference() const;
	

private:

	std::vector<std::pair<std::string, SimpleTable>> database_;
};

