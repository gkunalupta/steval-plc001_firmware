#ifndef SCREEN14VIEW_HPP
#define SCREEN14VIEW_HPP

#include <gui_generated/screen14_screen/Screen14ViewBase.hpp>
#include <gui/screen14_screen/Screen14Presenter.hpp>

class Screen14View : public Screen14ViewBase
{
public:
    Screen14View();
    virtual ~Screen14View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN14VIEW_HPP
