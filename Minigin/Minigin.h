#pragma once
#include <string>
#include <functional>

namespace dae
{
    class Minigin
    {
    public:
        Minigin(const std::string& dataPath, int width, int height);
        ~Minigin();
        void Run(const std::function<void()>& load);

        Minigin(const Minigin&) = delete;
        Minigin(Minigin&&) = delete;
        Minigin& operator=(const Minigin&) = delete;
        Minigin& operator=(Minigin&&) = delete;

        float GetFPS() const { return m_FPS; }

    private:
        float m_FPS{ 60.0f };
    };
}