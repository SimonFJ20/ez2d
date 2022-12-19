#include "sdl.hpp"
#include "SDL_events.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "gui.hpp"
#include <SDL.h>
#include <chrono>
#include <cstdint>
#include <exception>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>

namespace GUI::SDL {

EventHandler::EventHandler(uint32_t _window_id)
    : window_id { _window_id }
{
    SDL_AddEventWatch(
        reinterpret_cast<SDL_EventFilter>(event_handler_caller), this);
}

EventHandler::~EventHandler()
{

    SDL_DelEventWatch(
        reinterpret_cast<SDL_EventFilter>(event_handler_caller), this);
}

auto EventHandler::handle_event(const SDL_Event* event) noexcept -> void
{
    if (auto maybe_event = convert_sdl_event(event))
        push_event(*maybe_event);
}

[[nodiscard]] auto EventHandler::pop_event() noexcept -> std::optional<Event>
{
    auto events_lock = std::lock_guard { event_mutex };
    if (events.size() <= 0)
        return {};
    auto event = events.back();
    events.pop_back();
    return event;
}

[[nodiscard]] auto EventHandler::convert_sdl_event(
    const SDL_Event* event) noexcept -> std::optional<Event>
{
    switch (event->type) {
    case SDL_WINDOWEVENT:
        return convert_sdl_window_event(event);
    case SDL_KEYUP:
    case SDL_KEYDOWN:
        return convert_sdl_key_event(event);
    default:
        return {};
    }
}

[[nodiscard]] auto EventHandler::convert_sdl_window_event(
    const SDL_Event* event) noexcept -> std::optional<Event>
{
    fmt::print(
        "{}: event->window.type({}) == SDL_WINDOWEVENT_RESIZED({}) == {}\n",
        std::chrono::system_clock::now().time_since_epoch(), event->window.type,
        SDL_WINDOWEVENT_RESIZED, event->window.type == SDL_WINDOWEVENT_RESIZED);
    switch (event->window.type) {
    case SDL_WINDOWEVENT_RESIZED:

        return Event {
            .type = Events::ResizeEvent,
            .timestamp = event->window.timestamp,
            .data = ResizeEvent {
                .newWidth = event->window.data1,
                .newHeigh = event->window.data2,
            },
        };
    case SDL_WINDOWEVENT_CLOSE:
        return Event {
            .type = Events::ResizeEvent,
            .timestamp = event->window.timestamp,
            .data = ResizeEvent {
                .newWidth = event->window.data1,
                .newHeigh = event->window.data2,
            },
        };
    default:
        return {};
    }
}

[[nodiscard]] auto EventHandler::convert_sdl_key_event(
    const SDL_Event* event) noexcept -> std::optional<Event>
{
    if (event->key.windowID != window_id)
        return {};
    auto action = [&] {
        switch (event->type) {
        case SDL_KEYDOWN:
            return KeyActions::Press;
        case SDL_KEYUP:
            return KeyActions::Release;
        default:
            std::terminate();
        }
    }();
    return Event {
        .type = Events::KeyboardEvent, .timestamp = event->key.timestamp,
        .data = KeyboardEvent {
            .key = static_cast<uint16_t>(event->key.keysym.scancode),
            .action = action,
        },
    };
}

auto EventHandler::push_event(Event event) noexcept -> void
{
    auto events_lock = std::lock_guard { event_mutex };
    events.push_back(event);
}

auto event_handler_caller(EventHandler* data, const SDL_Event* event) noexcept
    -> int
{
    data->handle_event(event);
    return 0;
}

Window::Window(
    int width, int height, SDL_Window* _window, SDL_Renderer* _renderer)
    : _width { width }
    , _height { height }
    , window { _window }
    , renderer { _renderer }
    , event_handler { SDL_GetWindowID(_window) }
{
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

auto Window::draw_line(
    Point begin, Point end, int thickness, Color color) noexcept -> void
{
    SDL_SetRenderDrawColor(
        renderer, color.red, color.green, color.blue, color.alpha);
    auto halfThickness = int { thickness / 2 };
    for (int i = 0; i < thickness; i++) {
        SDL_RenderDrawLine(renderer, begin.x - halfThickness + i, begin.y,
            end.x + halfThickness - i, end.y);
        SDL_RenderDrawLine(renderer, begin.x, begin.y - halfThickness + i,
            end.x, end.y + halfThickness - i);
    }
}

auto Window::fill_rectangle(Rectangle rectangle, Color color) noexcept -> void
{
    SDL_SetRenderDrawColor(
        renderer, color.red, color.green, color.blue, color.alpha);
    auto sdl_rectangle = SDL_Rect {
        rectangle.x,
        rectangle.y,
        rectangle.width,
        rectangle.height,
    };
    SDL_RenderFillRect(renderer, &sdl_rectangle);
}

auto Window::stroke_rectangle(
    Rectangle rectangle, Color color, int thickness) noexcept -> void
{
    SDL_SetRenderDrawColor(
        renderer, color.red, color.green, color.blue, color.alpha);
    for (int i = 0; i < thickness; i++) {
        auto sdl_rectangle = SDL_Rect {
            rectangle.x + i,
            rectangle.y + i,
            rectangle.width - i * 2,
            rectangle.height - i * 2,
        };
        SDL_RenderFillRect(renderer, &sdl_rectangle);
    }
}

auto Window::set_title(std::string_view title) noexcept -> void
{
    SDL_SetWindowTitle(window, std::string { title }.c_str());
}

[[nodiscard]] auto Window::poll_event() noexcept -> std::optional<Event>
{
    return event_handler.pop_event();
}

auto Window::update() noexcept -> void { SDL_RenderPresent(renderer); }

Server::Server() { SDL_Init(SDL_INIT_VIDEO); }

Server::~Server() { SDL_Quit(); }

[[nodiscard]] auto Server::open(int width, int height) noexcept
    -> std::unique_ptr<Window>
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(
        width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
    return std::make_unique<Window>(width, height, window, renderer);
}

auto Server::update() noexcept -> void
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            fmt::print("Got a SDL_QUIT, and you know how these are handled.\n");
            std::exit(0);
        }
    }
}

}
