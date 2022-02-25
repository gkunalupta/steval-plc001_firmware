#ifndef SCREEN15VIEW_HPP
#define SCREEN15VIEW_HPP

#include <gui_generated/screen15_screen/Screen15ViewBase.hpp>
#include <gui/screen15_screen/Screen15Presenter.hpp>

class Screen15View : public Screen15ViewBase
{
public:
    Screen15View();
    virtual ~Screen15View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN15VIEW_HPP
