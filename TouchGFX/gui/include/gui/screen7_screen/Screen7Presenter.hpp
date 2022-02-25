#ifndef SCREEN7PRESENTER_HPP
#define SCREEN7PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen7View;

class Screen7Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen7Presenter(Screen7View& v);

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

    virtual ~Screen7Presenter() {};

private:
    Screen7Presenter();

    Screen7View& view;
};

#endif // SCREEN7PRESENTER_HPP
