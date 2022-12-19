#pragma once

#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "gui.hpp"
#include "gui_server.hpp"
#include <cstdint>
#include <memory>
#include <mutex>
#include <string_view>
#include <vector>

namespace GUI::SDL {

class EventHandler {
public:
    EventHandler(uint32_t window_id);
    ~EventHandler();

    auto handle_event(const SDL_Event* event) noexcept -> void;

    [[nodiscard]] auto pop_event() noexcept -> std::optional<Event>;

    [[nodiscard]] auto convert_sdl_event(const SDL_Event* event) noexcept
        -> std::optional<Event>;

    [[nodiscard]] auto convert_sdl_window_event(const SDL_Event* event) noexcept
        -> std::optional<Event>;

    [[nodiscard]] auto convert_sdl_key_event(const SDL_Event* event) noexcept
        -> std::optional<Event>;

private:
    auto push_event(Event event) noexcept -> void;

    uint32_t window_id;
    std::mutex event_mutex;
    std::vector<Event> events;
};

auto event_handler_caller(EventHandler* data, const SDL_Event* event) noexcept
    -> int;

class Window final : public GUI::Window {
public:
    Window(int width, int height, SDL_Window* window, SDL_Renderer* renderer);
    ~Window();

    [[nodiscard]] inline auto width() const noexcept -> int override
    {
        return _width;
    };

    [[nodiscard]] inline auto height() const noexcept -> int override
    {
        return _height;
    };

    auto draw_line(Point begin, Point end, int thickness, Color color) noexcept
        -> void override;

    auto fill_rectangle(Rectangle rectangle, Color color) noexcept
        -> void override;

    auto stroke_rectangle(Rectangle rectangle, Color color,
        int thickness) noexcept -> void override;

    auto set_title(std::string_view title) noexcept -> void override;

    [[nodiscard]] auto poll_event() noexcept -> std::optional<Event> override;

    auto update() noexcept -> void;

private:
    int _width, _height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    EventHandler event_handler;
};

class Server final : public GUI::Server<Window> {
public:
    Server(const Server&) = default;
    Server& operator=(const Server&) = default;

    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    Server();
    ~Server();

    [[nodiscard]] auto open(int width, int height) noexcept
        -> std::unique_ptr<Window> override;

    auto update() noexcept -> void;

private:
};

}
