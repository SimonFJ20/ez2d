#include "SDL_timer.h"
#include "gui.hpp"
#include "sdl.hpp"
#include <SDL_main.h>
#include <argparse/argparse.hpp>
#include <cstdint>
#include <exception>
#include <fmt/core.h>
#include <iostream>

int main(int argc, char** argv)
{
    auto args = argparse::ArgumentParser("ez2d-server");

    args.add_argument("-h", "--hostname")
        .default_value(std::string { "127.0.0.1" })
        .required();
    args.add_argument("-p", "--port")
        .default_value<uint16_t>(8000)
        .required()
        .scan<'i', uint16_t>();

    try {
        args.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl << args;
        std::terminate();
    }

    auto port = args.get<uint16_t>("--port");
    std::cout << port << '\n';

    auto server = GUI::SDL::Server {};
    auto window1 = server.open(400, 400);
    window1->set_title("window 1");
    auto window2 = server.open(500, 500);
    window2->set_title("window 2");

    while (true) {
        if (window1)
            window1->update();
        while (auto event = window1->poll_event()) {
            fmt::print("window1: got event {}\n", event->type);
            if (event->type == GUI::Events::CloseEvent)
                window1.reset();
        }
        if (window2)
            window2->update();

        while (auto event = window1->poll_event()) {
            if (event->type == GUI::Events::CloseEvent)
                window1.reset();
        }
        server.update();
        SDL_Delay(6);
    }

    return 0;
}
