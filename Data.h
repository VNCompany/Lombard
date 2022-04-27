#pragma once

#include <vector>
#include <string>

class Item
{
public:
	std::wstring	Id;
	std::wstring	Name;
	std::wstring	Price;
	std::wstring	Date;
	std::wstring	IsForSale;
};

extern std::vector<Item> __Items__;

std::vector<Item>& GetItems();
bool SafeItems();