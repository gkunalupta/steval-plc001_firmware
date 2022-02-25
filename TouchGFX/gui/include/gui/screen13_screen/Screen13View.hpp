#ifndef SCREEN13VIEW_HPP
#define SCREEN13VIEW_HPP

#include <gui_generated/screen13_screen/Screen13ViewBase.hpp>
#include <gui/screen13_screen/Screen13Presenter.hpp>

class Screen13View : public Screen13ViewBase
{
public:
    Screen13View();
    virtual ~Screen13View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN13VIEW_HPP
