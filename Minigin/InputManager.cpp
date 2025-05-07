#include "InputManager.h"
#include <iostream>
#include <glm.hpp>

namespace dae
{
    InputManager::InputManager()
    {
        for (int i = 0; i < 4; ++i)
        {
            auto gamepad = std::make_unique<Gamepad>(i);
            if (gamepad->IsConnected())
            {
                m_Gamepads.emplace_back(std::move(gamepad));
            }
        }
    }

    void InputManager::BindCommand(SDL_Keycode key, KeyState state, std::unique_ptr<Command> command)
    {
        m_KeyboardBindings[key].emplace_back(state, std::move(command));
    }

    void InputManager::BindCommand(SDL_GameControllerButton button, KeyState state, std::unique_ptr<Command> command)
    {
        m_ControllerBindings[button].emplace_back(state, std::move(command));
    }

    void InputManager::RemoveBind(SDL_Keycode key, KeyState state)
    {
        if (m_KeyboardBindings.contains(key))
        {
            auto& bindings = m_KeyboardBindings[key];
            bindings.erase(std::remove_if(bindings.begin(), bindings.end(),
                [state](const auto& pair) { return pair.first == state; }), bindings.end());

            if (bindings.empty())
            {
                m_KeyboardBindings.erase(key);
            }
        }
    }

    void InputManager::RemoveBind(SDL_GameControllerButton button, KeyState state)
    {
        if (m_ControllerBindings.contains(button))
        {
            auto& bindings = m_ControllerBindings[button];
            bindings.erase(std::remove_if(bindings.begin(), bindings.end(),
                [state](const auto& pair) { return pair.first == state; }), bindings.end());

            if (bindings.empty())
            {
                m_ControllerBindings.erase(button);
            }
        }
    }

    bool InputManager::ProcessInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return false;
        }

        const Uint8* state = SDL_GetKeyboardState(nullptr);
        for (auto& [key, bindings] : m_KeyboardBindings)
        {
            bool keyPressed = state[SDL_GetScancodeFromKey(key)];
            bool wasPressed = previousKeyStates.contains(key) ? previousKeyStates[key] : false;

            for (auto& [keyState, command] : bindings)
            {
                if (command)
                {
                    if ((keyState == KeyState::Pressed && !wasPressed && keyPressed) ||
                        (keyState == KeyState::Down && keyPressed) ||
                        (keyState == KeyState::Up && wasPressed && !keyPressed))
                    {
                        command->Execute();
                    }
                }
            }
            previousKeyStates[key] = keyPressed;
        }

        for (const auto& gamepad : m_Gamepads)
        {
            if (!gamepad->IsConnected())
                continue;

            std::vector<std::pair<int, SDL_GameControllerButton>> buttonMappings = {
                { XINPUT_GAMEPAD_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_UP },
                { XINPUT_GAMEPAD_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
                { XINPUT_GAMEPAD_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT },
                { XINPUT_GAMEPAD_DPAD_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
                { XINPUT_GAMEPAD_A, SDL_CONTROLLER_BUTTON_A},
                { XINPUT_GAMEPAD_B, SDL_CONTROLLER_BUTTON_B},
                { XINPUT_GAMEPAD_X, SDL_CONTROLLER_BUTTON_X},
            };

            for (const auto& [xinputButton, sdlButton] : buttonMappings)
            {
                bool buttonPressed = gamepad->GetButtonState(xinputButton);
                bool wasPressed = previousControllerStates.contains(sdlButton) ? previousControllerStates[sdlButton] : false;

                for (auto& [keyState, command] : m_ControllerBindings[sdlButton])
                {
                    if (command)
                    {
                        if ((keyState == KeyState::Pressed && !wasPressed && buttonPressed) ||
                            (keyState == KeyState::Down && buttonPressed) ||
                            (keyState == KeyState::Up && wasPressed && !buttonPressed))
                        {
                            command->Execute();
                        }
                    }
                }

                previousControllerStates[sdlButton] = buttonPressed;
            }
        }

        return true;
    }
}
