#include <fstream>
#include <codecvt>
#include "pch.h"

#include "Data.h"
std::vector<Item> __Items__;

void LoadData()
{
	std::wifstream in("Items.db");
	in.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

	std::wstring temp;
	Item temp_item;
	while (in)
	{
		std::getline(in, temp);
		temp_item.Id = temp;
		if (!in)
			break;

		std::getline(in, temp);
		temp_item.Name = temp;
		if (!in)
			break;

		std::getline(in, temp);
		temp_item.Price = temp;
		if (!in)
			break;

		std::getline(in, temp);
		temp_item.Date = temp;
		if (!in)
			break;

		std::getline(in, temp);
		temp_item.IsForSale = temp;
		if (!in)
			break;

		__Items__.push_back(temp_item);
	}
}

std::vector<Item>& GetItems()
{
	if (__Items__.empty())
		LoadData();
	return __Items__;
}

bool SafeItems()
{
	std::wofstream out("Items.db", std::ios::ate);
	out.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
	
	if (!out)
		return false;

	for (const auto& item : __Items__)
	{
		out << item.Id			<< std::endl;
		out << item.Name		<< std::endl;
		out << item.Price		<< std::endl;
		out << item.Date		<< std::endl;
		out << item.IsForSale	<< std::endl;
	}

	return true;
}

