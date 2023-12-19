#include "MainWindow.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxImage::AddHandler(new wxPNGHandler);
        MainWindow* frame = new MainWindow();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);