#include "MainWindow.h"
//#include "img/check_16.xpm"
//#include "img/warning_16.xpm"
#include "utils.h"
#include "wx/event.h"
#include "wx/gdicmn.h"
#include "wx/timer.h"
#include <algorithm>
#include <chrono>
#include <locale>
#include <regex>
#include <string>
#include <wx/bitmap.h>
#include <wx/regex.h>
#include "cistem.h"
// #include <wx/richtext/richtextctrl.h>

MainWindow::MainWindow()
	: wxFrame(nullptr, wxID_ANY, "Bezugszeichenprüfvorrichtung",
		wxDefaultPosition, wxSize(1200, 800)) {
	setupUi();
	loadIcons();
	setupBindings();
}

void MainWindow::getNumbers(wxCommandEvent& event) {
	setupAndClear();

	std::wsregex_iterator regex_begin(m_fullText.begin(), m_fullText.end(),
		m_regex);
	std::wsregex_iterator regex_end;
	std::wstring merkmal;
	while (regex_begin != regex_end) {
		m_graph.emplace((*regex_begin)[2], (*regex_begin)[1]);
		//m_graph[(*regex_begin)[2]].emplace((*regex_begin)[1]);
		++regex_begin;
	}

	fillListTree();
	//findUnnumberedWords();
	m_noNumberLabel->SetLabel("0/" + std::to_wstring(m_noNumberPos.size() / 2) +
		"\t");
	m_wrongNumberLabel->SetLabel(
		"0/" + std::to_wstring(m_wrongNumberPos.size() / 2) + "\t");
	m_splitNumberLabel->SetLabel(
		"0/" + std::to_wstring(m_splitNumberPos.size() / 2) + "\t");

	fillBzList();
}

void MainWindow::debounceFunc(wxCommandEvent& event) {
	m_debounce_timer.Start(500, true);
}

void MainWindow::debounceBzListe(wxCommandEvent& event) {
	m_debounce_timer_bzlist.Start(500, true);
}

void MainWindow::scanText(wxTimerEvent& event) {
	std::wstring_view text_view{ m_fullText };
	fill_vector(m_textStems, m_fullText);
}

void MainWindow::fillListTree() {
	// Used to refer to the line we add afterwards
	wxTreeListItem item;
	// Iterate over every saved item and insert it
	for (const auto& [bezugszeichen, merkmal] : m_graph) {
		item = m_merkmaleList->AppendItem(m_merkmaleList->GetRootItem(), bezugszeichen, 0);
		m_merkmaleList->SetItemText(item, 1, merkmal);
	}
}

void MainWindow::fillListBzList(wxTimerEvent& event) {
	fillTreeFromBzList(m_merkmaleList.get(), m_bzList->GetValue().ToStdWstring());
}

void MainWindow::loadIcons() {
	wxImageList* m_imageList = new wxImageList(16, 16, false, 0);
	const wxBitmap check = wxBITMAP_PNG(check_16);
	const wxBitmap warning = wxBITMAP_PNG(warning_16);
	m_imageList->Add(check);
	m_imageList->Add(warning);
	m_merkmaleList->AssignImageList(m_imageList);
}

void MainWindow::setupAndClear() {
	m_fullText = m_textBox->GetValue();
	m_full_words.clear();
	m_graph.clear();
	m_all_merkmale.clear();
	m_merkmale_to_bz.clear();
	m_BzToPosition.clear();
	m_StemToPosition.clear();
	m_merkmaleList->DeleteAllItems();
	m_noNumberPos.clear();
	m_wrongNumberPos.clear();
	m_splitNumberPos.clear();
	m_merkmale_to_bz.clear();
	m_textBox->SetStyle(0, m_textBox->GetValue().length(), m_neutral_style);
}

void MainWindow::stemWord(std::wstring& word) {
	word[0] = std::tolower(word[0]);
	Cistem::stem(word);
	//m_germanStemmer(word);
}

void MainWindow::selectNextNoNumber(wxCommandEvent& event) {
	m_noNumberSelected += 2;
	if (m_noNumberSelected >= m_noNumberPos.size() || m_noNumberSelected < 0) {
		m_noNumberSelected = 0;
	}
	if (m_noNumberPos.size()) {
		m_textBox->SetSelection(m_noNumberPos[m_noNumberSelected],
			m_noNumberPos[m_noNumberSelected + 1]);
		m_textBox->ShowPosition(m_noNumberPos[m_noNumberSelected]);
	}

	if (m_noNumberPos.size()) {
		m_noNumberLabel->SetLabel(std::to_wstring(m_noNumberSelected / 2 + 1) +
			"/" + std::to_wstring(m_noNumberPos.size() / 2) +
			"\t");
	}
}

void MainWindow::selectPreviousNoNumber(wxCommandEvent& event) {
	m_noNumberSelected -= 2;
	if (m_noNumberSelected >= m_noNumberPos.size() || m_noNumberSelected < 0) {
		m_noNumberSelected = m_noNumberPos.size() - 2;
	}
	if (m_noNumberPos.size()) {
		m_textBox->SetSelection(m_noNumberPos[m_noNumberSelected],
			m_noNumberPos[m_noNumberSelected + 1]);
		m_textBox->ShowPosition(m_noNumberPos[m_noNumberSelected]);
	}

	if (m_noNumberPos.size()) {
		m_noNumberLabel->SetLabel(std::to_wstring(m_noNumberSelected / 2 + 1) +
			"/" + std::to_wstring(m_noNumberPos.size() / 2) +
			"\t");
	}
}

void MainWindow::selectNextWrongNumber(wxCommandEvent& event) {
	m_wrongNumberSelected += 2;
	if (m_wrongNumberSelected >= m_wrongNumberPos.size() ||
		m_wrongNumberSelected < 0) {
		m_wrongNumberSelected = 0;
	}
	if (m_wrongNumberPos.size()) {
		m_textBox->SetSelection(m_wrongNumberPos[m_wrongNumberSelected],
			m_wrongNumberPos[m_wrongNumberSelected + 1]);
		m_textBox->ShowPosition(m_wrongNumberPos[m_wrongNumberSelected]);
	}

	if (m_wrongNumberPos.size()) {
		m_wrongNumberLabel->SetLabel(
			std::to_wstring(m_wrongNumberSelected / 2 + 1) + "/" +
			std::to_wstring(m_wrongNumberPos.size() / 2) + "\t");
	}
}

void MainWindow::selectPreviousWrongNumber(wxCommandEvent& event) {
	m_wrongNumberSelected -= 2;
	if (m_wrongNumberSelected >= m_wrongNumberPos.size() ||
		m_wrongNumberSelected < 0) {
		m_wrongNumberSelected = m_wrongNumberPos.size() - 2;
	}
	if (m_wrongNumberPos.size()) {
		m_textBox->SetSelection(m_wrongNumberPos[m_wrongNumberSelected],
			m_wrongNumberPos[m_wrongNumberSelected + 1]);
		m_textBox->ShowPosition(m_wrongNumberPos[m_wrongNumberSelected]);
	}

	if (m_wrongNumberPos.size()) {
		m_wrongNumberLabel->SetLabel(
			std::to_wstring(m_wrongNumberSelected / 2 + 1) + "/" +
			std::to_wstring(m_wrongNumberPos.size() / 2) + "\t");
	}
}

void MainWindow::selectNextSplitNumber(wxCommandEvent& event) {
	m_splitNumberSelected += 2;
	if (m_splitNumberSelected >= m_splitNumberPos.size() ||
		m_splitNumberSelected < 0) {
		m_splitNumberSelected = 0;
	}
	if (m_splitNumberPos.size()) {
		m_textBox->SetSelection(m_splitNumberPos[m_splitNumberSelected],
			m_splitNumberPos[m_splitNumberSelected + 1]);
		m_textBox->ShowPosition(m_splitNumberPos[m_splitNumberSelected]);
	}

	if (m_splitNumberPos.size()) {
		m_splitNumberLabel->SetLabel(
			std::to_wstring(m_splitNumberSelected / 2 + 1) + "/" +
			std::to_wstring(m_splitNumberPos.size() / 2) + "\t");
	}
}

void MainWindow::selectPreviousSplitNumber(wxCommandEvent& event) {
	m_splitNumberSelected -= 2;
	if (m_splitNumberSelected >= m_splitNumberPos.size() ||
		m_splitNumberSelected < 0) {
		m_splitNumberSelected = m_splitNumberPos.size() - 2;
	}
	if (m_splitNumberPos.size()) {
		m_textBox->SetSelection(m_splitNumberPos[m_splitNumberSelected],
			m_splitNumberPos[m_splitNumberSelected + 1]);
		m_textBox->ShowPosition(m_splitNumberPos[m_splitNumberSelected]);
	}

	if (m_splitNumberPos.size()) {
		m_splitNumberLabel->SetLabel(
			std::to_wstring(m_splitNumberSelected / 2 + 1) + "/" +
			std::to_wstring(m_splitNumberPos.size() / 2) + "\t");
	}
}

void MainWindow::setupUi() {
	// Create a panel to hold our controls
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// Create a horizontal box sizer to arrange the controls
	wxBoxSizer* viewSizer = new wxBoxSizer(wxHORIZONTAL);
	panel->SetSizer(viewSizer);

	// Create a vertical box sizer to keep the text, list, and buttons
	wxBoxSizer* outputSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* numberSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* scanSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* noNumberSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* wrongNumberSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* splitNumberSizer = new wxBoxSizer(wxHORIZONTAL);

	m_notebookList = new wxNotebook(panel, wxID_ANY);

	// Add a text box to the sizer
	m_textBox = new wxRichTextCtrl(panel);
	m_bzList =
		std::make_shared<wxTextCtrl>(m_notebookList, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxSize(250, -1), wxTE_PROCESS_TAB | wxTE_MULTILINE);
	// viewSizer->Add(m_bzList.get(), 0, wxEXPAND | wxALL, 10);
	viewSizer->Add(m_textBox, 1, wxEXPAND | wxALL, 10);
	viewSizer->Add(outputSizer, 0, wxEXPAND, 10);

	// Add a dataView to the sizer
	m_merkmaleList = std::make_shared<wxTreeListCtrl>(
		m_notebookList, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_merkmaleList->AppendColumn("#", wxCOL_WIDTH_AUTOSIZE, wxALIGN_RIGHT);
	m_merkmaleList->AppendColumn("Merkmal");
	// m_treeList->SetItemComparator(&m_BZComparator);
	outputSizer->Add(m_notebookList, 2, wxEXPAND | wxALL, 10);
	m_notebookList->AddPage(m_merkmaleList.get(), "Liste");
	m_notebookList->AddPage(m_bzList.get(), "Edit");
	// outputSizer->Add(m_treeList.get(), 2, wxEXPAND | wxALL, 10);

	// Add button(s?) to scan for Merkmale
	wxButton* m_getNumbersButton = new wxButton(panel, wxID_ANY, "Scan", wxDefaultPosition, wxSize(50, -1));
	m_getNumbersButton->Bind(wxEVT_BUTTON, &MainWindow::getNumbers, this);
	scanSizer->Add(m_getNumbersButton);
	outputSizer->Add(scanSizer, 0, wxALL, 10);

	// Add the Buttons to cycle through errors
	m_ButtonBackwardNoNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, "<", wxDefaultPosition, wxSize(50, -1));
	m_ButtonForwardNoNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, ">", wxDefaultPosition, wxSize(50, -1));
	m_ButtonBackwardWrongNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, "<", wxDefaultPosition, wxSize(50, -1));
	m_ButtonForwardWrongNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, ">", wxDefaultPosition, wxSize(50, -1));
	m_ButtonBackwardSplitNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, "<", wxDefaultPosition, wxSize(50, -1));
	m_ButtonForwardSplitNumber = std::make_shared<wxButton>(
		panel, wxID_ANY, ">", wxDefaultPosition, wxSize(50, -1));

	// Position the buttons in the sizers
	noNumberSizer->Add(m_ButtonBackwardNoNumber.get());
	noNumberSizer->Add(m_ButtonForwardNoNumber.get());
	auto noNumberDescription =
		new wxStaticText(panel, wxID_ANY, "Unnumbered", wxDefaultPosition,
			wxSize(150, -1), wxST_ELLIPSIZE_END | wxALIGN_LEFT);
	m_noNumberLabel = std::make_shared<wxStaticText>(panel, wxID_ANY, "0/0\t");
	noNumberSizer->Add(m_noNumberLabel.get(), 0, wxLEFT, 10);
	noNumberSizer->Add(noNumberDescription, 0, wxLEFT, 0);

	wrongNumberSizer->Add(m_ButtonBackwardWrongNumber.get());
	wrongNumberSizer->Add(m_ButtonForwardWrongNumber.get());
	auto wrongNumberDescription = new wxStaticText(
		panel, wxID_ANY, "Multiple Words per Number", wxDefaultPosition,
		wxSize(150, -1), wxST_ELLIPSIZE_END | wxALIGN_LEFT);
	m_wrongNumberLabel = std::make_shared<wxStaticText>(panel, wxID_ANY, "0/0\t");
	wrongNumberSizer->Add(m_wrongNumberLabel.get(), 0, wxLEFT, 10);
	wrongNumberSizer->Add(wrongNumberDescription, 0, wxLEFT, 0);

	splitNumberSizer->Add(m_ButtonBackwardSplitNumber.get());
	splitNumberSizer->Add(m_ButtonForwardSplitNumber.get());
	auto splitNumberDescription = new wxStaticText(
		panel, wxID_ANY, "Multiple Numbers per Word", wxDefaultPosition,
		wxSize(150, -1), wxST_ELLIPSIZE_END | wxALIGN_LEFT);
	m_splitNumberLabel = std::make_shared<wxStaticText>(panel, wxID_ANY, "0/0\t");
	splitNumberSizer->Add(m_splitNumberLabel.get(), 0, wxLEFT, 10);
	splitNumberSizer->Add(splitNumberDescription, 0, wxLEFT, 0);

	numberSizer->Add(noNumberSizer, wxLEFT);
	numberSizer->Add(wrongNumberSizer, wxLEFT);
	numberSizer->Add(splitNumberSizer, wxLEFT);

	outputSizer->Add(numberSizer, 0, wxALL, 10);

	m_neutral_style.SetBackgroundColour(
		wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	m_yellow_style.SetBackgroundColour(*wxYELLOW);

	// m_treeList->SetSortColumn(0);
}

void MainWindow::setupBindings() {
	m_textBox->Bind(wxEVT_TEXT, &MainWindow::debounceFunc, this);
	m_bzList->Bind(wxEVT_TEXT, &MainWindow::debounceBzListe, this);
	m_debounce_timer.Bind(wxEVT_TIMER, &MainWindow::scanText, this);
	m_debounce_timer_bzlist.Bind(wxEVT_TIMER, &MainWindow::fillListBzList, this);
	m_ButtonBackwardNoNumber->Bind(wxEVT_BUTTON,
		&MainWindow::selectPreviousNoNumber, this);
	m_ButtonForwardNoNumber->Bind(wxEVT_BUTTON, &MainWindow::selectNextNoNumber,
		this);
	m_ButtonBackwardWrongNumber->Bind(
		wxEVT_BUTTON, &MainWindow::selectPreviousWrongNumber, this);
	m_ButtonForwardWrongNumber->Bind(wxEVT_BUTTON,
		&MainWindow::selectNextWrongNumber, this);
	m_ButtonBackwardSplitNumber->Bind(
		wxEVT_BUTTON, &MainWindow::selectPreviousSplitNumber, this);
	m_ButtonForwardSplitNumber->Bind(wxEVT_BUTTON,
		&MainWindow::selectNextSplitNumber, this);
}

void MainWindow::fillBzList() {
	auto treeItem = m_merkmaleList->GetFirstItem();
	wxString bz;
	while (treeItem.IsOk()) {
		bz.Append(m_merkmaleList->GetItemText(treeItem, 0) + '\t' + m_merkmaleList->GetItemText(treeItem, 1) + '\n');
		treeItem = m_merkmaleList->GetNextItem(treeItem);
	}
	m_bzList->ChangeValue(bz);
}
