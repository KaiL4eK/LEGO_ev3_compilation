#include <led.h>



extern "C" {

#include <ev3_led.h>

void Led_set_color ( Led_side_t side, float red, float green )
{
    using namespace std;

    clip_arg( red, 0.0f, 1.0f );
    clip_arg( green, 0.0f, 1.0f );

    switch ( side ) {
        case RIGHT:
            led::set_color( led::right, { red, green } );
            break;

        case LEFT:
            led::set_color( led::left, { red, green } );
            break;
    }
}

void Led_set_flash ( Led_side_t side, Led_main_color_t mcolor, unsigned int on_ms, unsigned int off_ms )
{
    led *red = nullptr,
        *green = nullptr;

    switch ( side ) {
        case RIGHT:
            red = &led::red_right;
            green = &led::green_right;
            break;

        case LEFT:
            red = &led::red_left;
            green = &led::green_left;
            break;

        default:
            return;
    }

    switch ( mcolor ) {
        case RED:
            red->flash( on_ms, off_ms );
            break;

        case GREEN:
            green->flash( on_ms, off_ms );
            break;

        default:
            return;
    }
}

}

led::led(std::string name)
{
    static const std::string _strClassDir { SYS_ROOT "/leds/" };
    connect(_strClassDir, name, std::map<std::string, std::set<std::string>>());
}

//-----------------------------------------------------------------------------

void led::flash(unsigned on_ms, unsigned off_ms)
{
    static const mode_type timer("timer");
    set_trigger(timer);
    if (on_ms)
    {
        // A workaround for ev3dev/ev3dev#225.
        // It takes some time for delay_{on,off} sysfs attributes to appear after
        // led trigger has been set to "timer".
        for (int i = 0; ; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            try {
                set_delay_on (on_ms );
                set_delay_off(off_ms);
                break;
            } catch(...) {
                if (i >= 5) 
                    throw;
            }
        }
    }
}

//-----------------------------------------------------------------------------

led led::red_left{"ev3:left:red:ev3dev"};
led led::red_right{"ev3:right:red:ev3dev"};
led led::green_left{"ev3:left:green:ev3dev"};
led led::green_right{"ev3:right:green:ev3dev"};

std::vector<led*> led::left{ &led::red_left, &led::green_left };
std::vector<led*> led::right{ &led::red_right, &led::green_right };

std::vector<float> led::black{ static_cast<float>(0), static_cast<float>(0) };
std::vector<float> led::red{ static_cast<float>(1), static_cast<float>(0) };
std::vector<float> led::green{ static_cast<float>(0), static_cast<float>(1) };
std::vector<float> led::amber{ static_cast<float>(1), static_cast<float>(1) };
std::vector<float> led::orange{ static_cast<float>(1), static_cast<float>(0.5) };
std::vector<float> led::yellow{ static_cast<float>(0.1), static_cast<float>(1) };

//-----------------------------------------------------------------------------
void led::all_off() {

    red_left.off();
    red_right.off();
    green_left.off();
    green_right.off();

}

//-----------------------------------------------------------------------------

void led::set_color(const std::vector<led*> &group, const std::vector<float> &color) {
    const size_t n = std::min(group.size(), color.size());
    for(size_t i = 0; i < n; ++i)
        group[i]->set_brightness_pct(color[i]);
}
