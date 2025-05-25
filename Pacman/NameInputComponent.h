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

        void UpdateDisplay();

    private:
        int m_CurrentCharIndex = 0;
        std::vector<char> m_Chars = { '_', '_', '_' };
        TextComponent* m_TextComponent = nullptr;
        int m_LetterPosition = 0;
        int m_Score;
        const std::string m_SaveFile = "highscore.dat";
    };
}
