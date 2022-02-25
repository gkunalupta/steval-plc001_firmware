#ifndef SCREEN12PRESENTER_HPP
#define SCREEN12PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen12View;

class Screen12Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen12Presenter(Screen12View& v);

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

    virtual ~Screen12Presenter() {};

private:
    Screen12Presenter();

    Screen12View& view;
};

#endif // SCREEN12PRESENTER_HPP
