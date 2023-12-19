#pragma once
#include "BZComparator.h"
//#include "german_stem.h"
#include "utils.h"
#include "wx/notebook.h"
#include "wx/richtext/richtextctrl.h"
#include "wx/textctrl.h"
#include "wx/timer.h"
#include "wx/treelist.h"
#include <map>
#include <regex>
#include <wx/dataview.h>
#include <wx/listctrl.h>
#include <wx/wx.h>

class MainWindow : public wxFrame {
public:
    MainWindow();

private:
    void setupUi();
    void setupBindings();
    void getNumbers(wxCommandEvent& event);
    void scanText(wxTimerEvent& event);
    void fillListTree();
    void fillBzList();
    void loadIcons();
    void setupAndClear();
    void stemWord(std::wstring& word);
    void debounceFunc(wxCommandEvent& event);
    void debounceBzListe(wxCommandEvent& event);
    void fillListBzList(wxTimerEvent& event);
    void selectNextNoNumber(wxCommandEvent& event);
    void selectPreviousNoNumber(wxCommandEvent& event);
    void selectNextWrongNumber(wxCommandEvent& event);
    void selectPreviousWrongNumber(wxCommandEvent& event);
    void selectNextSplitNumber(wxCommandEvent& event);
    void selectPreviousSplitNumber(wxCommandEvent& event);

    std::wregex m_regex{
        L"(\\b[[:alpha:]���]+\\b)[[:s:]](\\b[[:digit:]]+[a-zA-Z']*\\b)",
        std::regex_constants::ECMAScript | std::regex_constants::optimize |
            std::regex_constants::icase };
    //stemming::german_stem<> m_germanStemmer;
    std::wstring m_fullText;
    std::vector<std::wstring> m_textStems;
    wxTextAttr m_neutral_style;
    wxTextAttr m_yellow_style;
    wxTimer m_debounce_timer;
    wxTimer m_debounce_timer_bzlist;

    std::map<std::wstring, std::wstring, CustomComparator>
        m_graph;

    std::unordered_map<std::wstring, std::vector<std::wstring>> m_ok_neighbours;

    std::unordered_map<std::wstring, std::unordered_set<std::wstring>>
        m_merkmale_to_bz;
    std::unordered_map<std::wstring, std::unordered_set<std::wstring>>
        m_full_words;
    wxNotebook* m_notebookList;
    std::unordered_set<std::wstring> m_all_merkmale;
    std::unordered_map<std::wstring, std::vector<size_t>> m_BzToPosition;
    std::unordered_map<std::wstring, std::vector<size_t>> m_StemToPosition;
    wxRichTextCtrl* m_textBox;
    std::shared_ptr<wxTextCtrl> m_bzList;
    //std::shared_ptr<wxImageList> m_imageList;
    std::shared_ptr<wxTreeListCtrl> m_merkmaleList;
    std::shared_ptr<wxButton> m_ButtonForwardNoNumber;
    std::shared_ptr<wxButton> m_ButtonBackwardNoNumber;
    std::shared_ptr<wxButton> m_ButtonForwardWrongNumber;
    std::shared_ptr<wxButton> m_ButtonBackwardWrongNumber;
    std::shared_ptr<wxButton> m_ButtonForwardSplitNumber;
    std::shared_ptr<wxButton> m_ButtonBackwardSplitNumber;
    BZComparator m_BZComparator;

    std::vector<int> m_noNumberPos;
    int m_noNumberSelected{ -2 };
    std::shared_ptr<wxStaticText> m_noNumberLabel;
    std::vector<int> m_wrongNumberPos;
    int m_wrongNumberSelected{ -2 };
    std::shared_ptr<wxStaticText> m_wrongNumberLabel;
    std::vector<int> m_splitNumberPos;
    int m_splitNumberSelected{ -2 };
    std::shared_ptr<wxStaticText> m_splitNumberLabel;
};
