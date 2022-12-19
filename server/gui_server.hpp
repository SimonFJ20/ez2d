#pragma once

#include "gui.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

namespace GUI {

template <class Window> class Server {
public:
    static_assert(std::is_base_of<GUI::Window, Window>(),
        "Window must derrive from GUI::Window");

    Server() = default;
    virtual ~Server() = default;

    [[nodiscard]] virtual auto open(int width, int height) noexcept
        -> std::unique_ptr<Window>
        = 0;
};

}
