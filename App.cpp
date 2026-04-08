#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* frame = new MainFrame(nullptr);

    frame->Show();

    return true;
}
