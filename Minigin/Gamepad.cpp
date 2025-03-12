#include "Gamepad.h"
#include <windows.h>
#include <XInput.h>
#include <SDL.h>

class Gamepad::Impl
{
public:
    Impl(int index) : m_Index(index) {}

    bool IsConnected() const
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        return XInputGetState(m_Index, &state) == ERROR_SUCCESS;
    }

    bool GetButtonState(int button) const
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        if (XInputGetState(m_Index, &state) != ERROR_SUCCESS)
            return false;

        return (state.Gamepad.wButtons & button) != 0;
    }

private:
    int m_Index;
};

Gamepad::Gamepad(int index) : m_Impl(std::make_unique<Impl>(index)) {}
Gamepad::~Gamepad() = default;

bool Gamepad::IsConnected() const { return m_Impl->IsConnected(); }
bool Gamepad::GetButtonState(int button) const { return m_Impl->GetButtonState(button); }
