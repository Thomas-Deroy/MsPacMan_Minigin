#pragma once
#include <memory>


class Gamepad
{
public:
    Gamepad(int index);
    ~Gamepad();

    bool IsConnected() const;
    bool GetButtonState(int button) const;
    int GetIndex() const;
private:
    class Impl;
    std::unique_ptr<Impl> m_Impl;
};
