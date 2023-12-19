#include "utils.h"
#include "wx/unichar.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <locale>
#include "cistem.h"

wxString merkmaleToString(const std::unordered_set<std::wstring>& stems,
	const std::unordered_set<std::wstring>& stem_origins)
{
	wxString listing;
	for (const auto& origin : stem_origins)
	{
		listing.append(origin + ";");
	}
	return listing;
}

void emplaceAllMerkmale(
	const std::unordered_map<std::wstring, std::unordered_set<std::wstring>>
	& stem_list,
	std::unordered_set<std::wstring>& all_merkmale)
{
	for (const auto& [stem, origins] : stem_list)
	{
		all_merkmale.insert(origins.begin(), origins.end());
	}
}

void appendVectorForRegex(const std::unordered_set<std::wstring>& strings,
	std::wstring& regexString)
{
	auto set_iter = strings.begin();
	const size_t limit{ strings.size() - 1 };

	for (size_t i = 0; i < limit; ++i)
	{
		regexString.append(*set_iter + "|");
		++set_iter;
	}
	regexString.append(*set_iter);
}

std::wstring_view next_line(const wchar_t*& p, const wchar_t* last)
{
	const wchar_t* end = std::find(p, last, '\n');
	std::wstring_view result(p, end - p);

	if (end == last)
		p = end;
	else
		p = end + 1;

	return result;
}

void fillTreeFromBzList(wxTreeListCtrl* treeList, const std::wstring& text) {
	treeList->DeleteAllItems();
	wxTreeListItem item;

	const wchar_t* cursor = text.c_str();
	const wchar_t* const in_end = cursor + text.size();

	while (cursor != in_end)
	{
		const std::wstring_view line = next_line(cursor, in_end);
		auto tab_pos = line.find('\t');
		if (tab_pos == line.npos) {
			tab_pos = line.find(' ');
		}
		if (tab_pos != line.npos) {
			item = treeList->AppendItem(treeList->GetRootItem(), std::wstring(line.substr(0, tab_pos)));
			treeList->SetItemText(item, 1, std::wstring(line.substr(tab_pos)));
		}
	}
}

void fill_vector(std::vector<std::wstring>& words, std::wstring_view text) {
	size_t start{ 0 }, end{ 0 };
	while (end != text.npos) {
		end = text.find_first_of(L".,; ", end + 1);
		words.emplace_back(text.substr(start, end - start));
		if (end != text.npos) {
			if (text[end] != ' ') {
				words.emplace_back(Cistem::stem(std::wstring(text.substr(end, 1))));
			}
			start = end + 1;
		}
	}
	for (const auto word : words) {
		std::cout << word << '\n';
	}
}
