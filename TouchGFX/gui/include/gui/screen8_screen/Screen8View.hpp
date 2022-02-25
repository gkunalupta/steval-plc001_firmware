#ifndef SCREEN8VIEW_HPP
#define SCREEN8VIEW_HPP

#include <gui_generated/screen8_screen/Screen8ViewBase.hpp>
#include <gui/screen8_screen/Screen8Presenter.hpp>

class Screen8View : public Screen8ViewBase
{
public:
    Screen8View();
    virtual ~Screen8View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN8VIEW_HPP
