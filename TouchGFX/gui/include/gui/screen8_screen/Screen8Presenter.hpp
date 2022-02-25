#ifndef SCREEN8PRESENTER_HPP
#define SCREEN8PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen8View;

class Screen8Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen8Presenter(Screen8View& v);

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

    virtual ~Screen8Presenter() {};

private:
    Screen8Presenter();

    Screen8View& view;
};

#endif // SCREEN8PRESENTER_HPP
