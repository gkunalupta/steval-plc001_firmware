#ifndef SCREEN7VIEW_HPP
#define SCREEN7VIEW_HPP

#include <gui_generated/screen7_screen/Screen7ViewBase.hpp>
#include <gui/screen7_screen/Screen7Presenter.hpp>

class Screen7View : public Screen7ViewBase
{
public:
    Screen7View();
    virtual ~Screen7View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN7VIEW_HPP
