#ifndef SCREEN12VIEW_HPP
#define SCREEN12VIEW_HPP

#include <gui_generated/screen12_screen/Screen12ViewBase.hpp>
#include <gui/screen12_screen/Screen12Presenter.hpp>

class Screen12View : public Screen12ViewBase
{
public:
    Screen12View();
    virtual ~Screen12View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN12VIEW_HPP
