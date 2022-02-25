#ifndef SCREEN11PRESENTER_HPP
#define SCREEN11PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen11View;

class Screen11Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen11Presenter(Screen11View& v);

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

    virtual ~Screen11Presenter() {};

private:
    Screen11Presenter();

    Screen11View& view;
};

#endif // SCREEN11PRESENTER_HPP
