#ifndef SCREEN9PRESENTER_HPP
#define SCREEN9PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen9View;

class Screen9Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen9Presenter(Screen9View& v);

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

    virtual ~Screen9Presenter() {};

private:
    Screen9Presenter();

    Screen9View& view;
};

#endif // SCREEN9PRESENTER_HPP
