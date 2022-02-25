#ifndef SCREEN15PRESENTER_HPP
#define SCREEN15PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen15View;

class Screen15Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen15Presenter(Screen15View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen15Presenter() {};

private:
    Screen15Presenter();

    Screen15View& view;
};

#endif // SCREEN15PRESENTER_HPP
