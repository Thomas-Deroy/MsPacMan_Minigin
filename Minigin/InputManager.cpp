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
		if (m_ShouldClearBinds)
			m_PendingKeyboardBindings[key].emplace_back(state, std::move(command));
        else 
            m_KeyboardBindings[key].emplace_back(state, std::move(command));
    }

    void InputManager::BindCommand(int controllerIndex, SDL_GameControllerButton button, KeyState state, std::unique_ptr<Command> command)
    {
		if (m_ShouldClearBinds)
			m_PendingControllerBindings[controllerIndex][button].emplace_back(state, std::move(command));
		else
            m_ControllerBindings[controllerIndex][button].emplace_back(state, std::move(command));
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

    void InputManager::RemoveBind(int controllerIndex, SDL_GameControllerButton button, KeyState state)
    {
        if (!m_ControllerBindings.contains(controllerIndex))
            return;

        auto& buttonMap = m_ControllerBindings[controllerIndex];
        if (!buttonMap.contains(button))
            return;

        auto& bindings = buttonMap[button];
        bindings.erase(std::remove_if(bindings.begin(), bindings.end(),
            [state](const auto& pair) { return pair.first == state; }), bindings.end());

        if (bindings.empty())
            buttonMap.erase(button);

        if (buttonMap.empty())
            m_ControllerBindings.erase(controllerIndex);
    }

    void dae::InputManager::ClearAllBinds()
    {
        m_ControllerBindings.clear();
        m_KeyboardBindings.clear();
    }

    void InputManager::RequestClearAllBinds()
    {
        m_ShouldClearBinds = true;
    }

    void InputManager::ApplyPendingBinds()
    {
        for (auto& [key, vec] : m_PendingKeyboardBindings)
        {
            for (auto& [state, command] : vec)
            {
                m_KeyboardBindings[key].emplace_back(state, std::move(command));
            }
        }

        for (auto& [controllerIndex, bindings] : m_PendingControllerBindings)
        {
            for (auto& [button, vec] : bindings)
            {
                for (auto& [state, command] : vec)
                {
                    m_ControllerBindings[controllerIndex][button].emplace_back(state, std::move(command));
                }
            }
        }

        m_PendingKeyboardBindings.clear();
        m_PendingControllerBindings.clear();
    }

    bool InputManager::IsControllerConnected(int controllerIndex) const
    {
        if (controllerIndex < 0 || controllerIndex >= static_cast<int>(m_Gamepads.size()))
            return false;

        return m_Gamepads[controllerIndex] && m_Gamepads[controllerIndex]->IsConnected();
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

            int controllerIndex = gamepad->GetIndex();
            auto& controllerPrevStates = previousControllerStates[controllerIndex];
            auto& bindings = m_ControllerBindings[controllerIndex];

            for (const auto& [xinputButton, sdlButton] : buttonMappings)
            {
                bool buttonPressed = gamepad->GetButtonState(xinputButton);
                bool wasPressed = controllerPrevStates.contains(sdlButton) ? controllerPrevStates[sdlButton] : false;

                for (auto& [keyState, command] : bindings[sdlButton])
                {
                    if (!command) continue;

                    if ((keyState == KeyState::Pressed && !wasPressed && buttonPressed) ||
                        (keyState == KeyState::Down && buttonPressed) ||
                        (keyState == KeyState::Up && wasPressed && !buttonPressed))
                    {
                        command->Execute();
                    }
                }

                controllerPrevStates[sdlButton] = buttonPressed;
            }
        }

        if (m_ShouldClearBinds)
        {
            ClearAllBinds();
            ApplyPendingBinds();
            m_ShouldClearBinds = false;
        }
        return true;
    }
}
