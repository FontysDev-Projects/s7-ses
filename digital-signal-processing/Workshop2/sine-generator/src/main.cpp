#include <cmath>
#include <csignal>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Complex.h"
#include "plot_lib.hpp"
#include "iterator.hpp"

using namespace plot;

static volatile std::sig_atomic_t run = true;

#define PI 3.14159265358979323846f

int main()
{
    std::signal(SIGINT, [](int)
                { run = false; });

    TerminalInfo term;
    term.detect();

    RealCanvas<BrailleCanvas> circle({{-1.2f, 1.2f}, {1.2f, -1.2f}}, Size(37, 17), term);
    auto layout = margin(frame(&circle, term));

    auto bounds = circle.bounds();
    // auto size = circle.size();
    // auto pixel = circle.unmap_size({1, 1});

    // float A = size.y / 2.0f;
    // float f = 2.0f;

    // int track_length = (size.x / pixel.x) / (2 * f) / 2;

    // auto sin = [A, f](float t)
    // {
    //     return A * std::sin(2 * PI * f * t);
    // };

    // auto cos = [A, f](float t)
    // {
    //     return A * std::cos(2 * PI * f * t);
    // };

    // auto sin2 = [sin](float t)
    // {
    //     auto val = sin(t);
    //     return val * val;
    // };

    // auto sincos = [sin, cos](float t)
    // {
    //     return sin(t) * cos(t);
    // };

    // auto plot_fn = [](auto const &fn, float t_)
    // {
    //     return [&fn, t_](float x) -> Pointf
    //     {
    //         return {x, fn(t_ + x)};
    //     };
    // };

    constexpr Color z_color(0.2f, 0.2f, 1.0f);
    constexpr Color radius_color(1.0f, 0.4f, 0.4f);
    constexpr Color x_component_color(1.0f, 0.8f, 0.2f);
    constexpr Color y_component_color(0.4f, 1.0f, 0.4f);

    float t = 0.0f;

    Complex gen(1.0, 0.0);

    while (true)
    {
        Complex z(std::cos(t), std::sin(t));
        gen = gen * z;
        Pointf pos = Pointf(gen.getReal(), gen.getImag());

        circle.clear()
            // X axis
            .line(term.foreground_color, {bounds.p1.x, 0}, {bounds.p2.x, 0})
            // Y axis
            .line(term.foreground_color, {0, bounds.p1.y}, {0, bounds.p2.y})
            // pos.x component
            .line(x_component_color, {0, pos.y}, pos)
            // pos.y component
            .line(y_component_color, {pos.x, 0}, pos)
            // radius
            .line(radius_color, {0, 0}, pos)
            // Draw small cross at pos
            .push()
            .dot(z_color, pos)
            .dot(z_color, pos - circle.unmap_size({1, 0}))
            .dot(z_color, pos + circle.unmap_size({1, 0}))
            .dot(z_color, pos - circle.unmap_size({0, 1}))
            .dot(z_color, pos + circle.unmap_size({0, 1}))
            .pop()
            .push();

        // for (int i = 0; i < track_length; ++i)
        // {
        //     auto x = i * pixel.x;
        //     Pointf start(sincos(t + (bounds.p2.x - x)), sin2(t + (bounds.p2.x - x)));
        //     Pointf end(sincos(t + (bounds.p2.x - x - pixel.x)), sin2(t + (bounds.p2.x - x - pixel.x)));
        //     circle.line(term.foreground_color.alpha(float(track_length - i) / track_length), start, end);
        // }

        circle.pop();

        std::cout << layout << std::flush;

        if (!run)
            break;

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(40ms);

        if (!run)
            break;

        t += 0.007f;
        if (t >= 1.0f)
            t -= std::trunc(t);

        std::cout << term.move_up(layout.size().y) << std::flush;
    }

    return 0;
}