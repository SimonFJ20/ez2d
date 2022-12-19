#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

namespace GUI {

struct Color {
    uint8_t red, green, blue, alpha;
};

struct Point {
    int x, y;
};

struct Rectangle {
    int x, y, width, height;
};

enum class MouseButtons {
    Left,
    Middle,
    Right,
};

enum class KeyActions {
    Press,
    Release,
};

enum class Events {
    MouseButtonEvent,
    KeyboardEvent,
    ResizeEvent,
    CloseEvent,
};

struct MouseButtonEvent {
    Point location;
    MouseButtons button;
    KeyActions action;
};

struct KeyboardEvent {
    uint16_t key;
    KeyActions action;
};

struct ResizeEvent {
    int newWidth, newHeigh;
};

struct CloseEvent { };

struct Event {
    Events type;
    time_t timestamp;
    std::variant<MouseButtonEvent, KeyboardEvent, ResizeEvent, CloseEvent> data;
};

class Window {
public:
    Window() = default;
    virtual ~Window() = default;

    [[nodiscard]] virtual auto width() const noexcept -> int = 0;
    [[nodiscard]] virtual auto height() const noexcept -> int = 0;

    virtual auto draw_line(
        Point begin, Point end, int thickness, Color color) noexcept -> void
        = 0;

    virtual auto fill_rectangle(Rectangle rectangle, Color color) noexcept
        -> void
        = 0;

    virtual auto stroke_rectangle(
        Rectangle rectangle, Color color, int thickness) noexcept -> void
        = 0;

    virtual auto set_title(std::string_view title) noexcept -> void = 0;

    [[nodiscard]] virtual auto poll_event() noexcept -> std::optional<Event>
        = 0;
};

}
