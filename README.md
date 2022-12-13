
# ez2d

Easiest way to draw colored pixels and rectangles cross-platform in any language.

## Server

Hosts a connectable TCP socket on a specified port.

Can facilitate multiple clients, with multiple windows each.

## Client

Connects to a server.

Client can tell server to:
- Open window with size
- Draw pixel at location with color
- Draw rectangle at location with color

Client can ask server about:
- Window size
- Mouse position and button states
- Keyboard key states
- Occured events

## Events

Includes:
- Exit event
- Resize event
- Mouse events
- Keyboard events

