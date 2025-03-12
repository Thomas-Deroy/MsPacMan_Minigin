#pragma once
#include "Component.h"
#include <string>
#include "imgui.h"
#include "implot.h" 
#include <vector>

namespace dae {
    class ImGuiComponent : public Component {
    public:
        ImGuiComponent(GameObject* owner, const int exercise = 0);
        void Update(float deltaTime) override;
        void Render() const override;

        void SetTitle(const std::string& title);
        void ToggleWindow();

    private:
        void RunCacheTest() const;
        void RunCacheTestGameObject3D() const;
        void RunCacheTestGameObject3DAlt() const;

        std::string m_WindowTitle;

        mutable bool m_ShowWindow = true;
        mutable int m_Value = 10;
		const int VALUE_MULTIPLYER = 1000000;
        int m_Exercise;

        mutable std::vector<float> m_StepSizes;
        mutable std::vector<float> m_Durations;

        mutable std::vector<float> m_StepSizes3D;
        mutable std::vector<float> m_Durations3D;

        mutable std::vector<float> m_StepSizes3DAlt;
        mutable std::vector<float> m_Durations3DAlt;

        mutable std::vector<float> m_CombinedDurations;
        mutable bool m_HasRun3D{ false };    
        mutable bool m_HasRun3DAlt{ false }; 

    };
}
