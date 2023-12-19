#pragma once
#include <unordered_map>
#include <unordered_set>
#include <wx/string.h>
#include <wx/treelist.h>
#include <memory>


struct CustomComparator
{
	bool operator()(std::wstring a, std::wstring b) const
	{
		if (a == b)
		{
			return false;
		};
		int int1{ std::stoi(a) }, int2{ std::stoi(b) };

		if (int1 < int2)
		{
			return true;
		}
		else if (int2 < int1)
		{
			return false;
		}
		else
		{
			if (a.length() < b.length())
			{
				return true;
			}
			else if (b.length() < a.length())
			{
				return false;
			}
		}
		return false;
	}
};


wxString merkmaleToString(const std::unordered_set<std::wstring>& stems,
	const std::unordered_set<std::wstring>& stem_origins);

void emplaceAllMerkmale(
	const std::unordered_map<std::wstring, std::unordered_set<std::wstring>>
	& merkmale,
	std::unordered_set<std::wstring>& merkVector);

void appendVectorForRegex(const std::unordered_set<std::wstring>& strings,
	std::wstring& regexString);

std::wstring_view next_line(const wchar_t*& p, const wchar_t* last);

void fillTreeFromBzList(wxTreeListCtrl* treeList, const std::wstring& text);

void fill_vector(std::vector<std::wstring>& words, std::wstring_view text);
