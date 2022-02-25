#ifndef SCREEN6PRESENTER_HPP
#define SCREEN6PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen6View;

class Screen6Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen6Presenter(Screen6View& v);

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

    virtual ~Screen6Presenter() {};

private:
    Screen6Presenter();

    Screen6View& view;
};

#endif // SCREEN6PRESENTER_HPP
