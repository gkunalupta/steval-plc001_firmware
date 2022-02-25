#ifndef SCREEN10PRESENTER_HPP
#define SCREEN10PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen10View;

class Screen10Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen10Presenter(Screen10View& v);

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

    virtual ~Screen10Presenter() {};

private:
    Screen10Presenter();

    Screen10View& view;
};

#endif // SCREEN10PRESENTER_HPP
