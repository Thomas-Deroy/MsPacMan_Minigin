#include "NameInputComponent.h"
#include "TextComponent.h"
#include "HighScoreManager.h"
#include "EventSystem.h"
#include "GameEvents.h"
#include <iostream>

namespace dae
{
    NameInputComponent::NameInputComponent(GameObject* owner, int finalScore)
        : Component(owner), m_Score(finalScore)
    {
        m_TextComponent = owner->GetComponent<TextComponent>();
        UpdateDisplay();
    }

    void NameInputComponent::SelectNextLetter()
    {
        if (m_LetterPosition >= 3 || m_IsDone) return;

        if (m_Chars[m_LetterPosition] == '_') m_Chars[m_LetterPosition] = 'A';
        else if (m_Chars[m_LetterPosition] < 'Z') ++m_Chars[m_LetterPosition];
        else m_Chars[m_LetterPosition] = 'A';

        UpdateDisplay();
    }

    void NameInputComponent::SelectPreviousLetter()
    {
        if (m_LetterPosition >= 3 || m_IsDone) return;

        if (m_Chars[m_LetterPosition] == '_') m_Chars[m_LetterPosition] = 'Z';
        else if (m_Chars[m_LetterPosition] > 'A') --m_Chars[m_LetterPosition];
        else m_Chars[m_LetterPosition] = 'Z';

        UpdateDisplay();
    }

    void NameInputComponent::ConfirmLetter()
    {
        if (m_IsDone) return;

        if (m_LetterPosition < 3)
        {
            if (m_Chars[m_LetterPosition] == '_')
                m_Chars[m_LetterPosition] = 'A';
            ++m_LetterPosition;
        }

        if (m_LetterPosition == 3)
        {
            std::string name(m_Chars.begin(), m_Chars.end());
            HighScoreManager::GetInstance().SaveHighScore(name, m_Score);
            m_IsDone = true;
            EventSystem::GetInstance().Notify(Event{ NameInputCompleted });
        }

        UpdateDisplay();
    }

    void NameInputComponent::UpdateDisplay()
    {
        if (m_TextComponent)
        {
            std::string display;
            for (int i = 0; i < 3; ++i)
            {
                if (i == m_LetterPosition) display += '[' + std::string(1, m_Chars[i]) + "] ";
                else display += ' ' + std::string(1, m_Chars[i]) + " ";
            }
            m_TextComponent->SetText(display);
        }
    }
}
