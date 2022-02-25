#ifndef SCREEN9VIEW_HPP
#define SCREEN9VIEW_HPP

#include <gui_generated/screen9_screen/Screen9ViewBase.hpp>
#include <gui/screen9_screen/Screen9Presenter.hpp>

class Screen9View : public Screen9ViewBase
{
public:
    Screen9View();
    virtual ~Screen9View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN9VIEW_HPP
