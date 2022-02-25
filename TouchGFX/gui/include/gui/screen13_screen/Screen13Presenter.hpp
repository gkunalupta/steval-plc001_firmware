#ifndef SCREEN13PRESENTER_HPP
#define SCREEN13PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen13View;

class Screen13Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen13Presenter(Screen13View& v);

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

    virtual ~Screen13Presenter() {};

private:
    Screen13Presenter();

    Screen13View& view;
};

#endif // SCREEN13PRESENTER_HPP
