#pragma once
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include "Singleton.h"
#include <windows.h>
#include <XInput.h>
#include "Command.h"
#include <memory>
#include "Gamepad.h"

namespace dae
{
    enum class KeyState
    {
        Up,
        Down,
        Pressed
    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();

        void BindCommand(SDL_Keycode key, KeyState state, std::unique_ptr<Command> command);
        void BindCommand(SDL_GameControllerButton key, KeyState state, std::unique_ptr<Command> command);

        void RemoveBind(SDL_Keycode key, KeyState state);
        void RemoveBind(SDL_GameControllerButton key, KeyState state);

        bool ProcessInput();

    private:
        std::unordered_map<SDL_Keycode, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>> m_KeyboardBindings;
        std::unordered_map<SDL_GameControllerButton, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>> m_ControllerBindings;
        std::unordered_map<SDL_Keycode, bool> previousKeyStates;
        std::unordered_map<SDL_GameControllerButton, bool> previousControllerStates;

        std::vector<std::unique_ptr<Gamepad>> m_Gamepads;
    };
}
