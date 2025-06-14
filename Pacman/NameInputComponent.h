#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"

namespace dae
{
    class TextComponent;

    class NameInputComponent : public Component
    {
    public:
        NameInputComponent(GameObject* owner, int finalScore);

        void SelectNextLetter();    
        void SelectPreviousLetter(); 
        void ConfirmLetter();        

        bool IsDone() const { return m_IsDone; }
        std::string GetName() const { return std::string(m_Chars.begin(), m_Chars.end()); }

        void UpdateDisplay();

    private:
        int m_CurrentCharIndex = 0;
        std::vector<char> m_Chars = { '_', '_', '_' };
        TextComponent* m_TextComponent = nullptr;
        int m_LetterPosition = 0;
        int m_Score;
        bool m_IsDone{ false };
    };
}
