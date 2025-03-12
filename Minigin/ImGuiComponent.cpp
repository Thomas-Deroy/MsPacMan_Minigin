#include "ImGuiComponent.h"
#include "GameObject.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>


struct Transform
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D
{
public:
    Transform transform;
    int ID = 0;
};

class GameObject3DAlt
{
public:
    std::vector<Transform> transforms;
    std::vector<int> IDs;

    GameObject3DAlt(int arrSize) : transforms(arrSize), IDs(arrSize, 1) {}
};

namespace dae {

    ImGuiComponent::ImGuiComponent(GameObject* owner, const int exercise)
        : Component(owner) {
		m_Exercise = exercise;
        m_WindowTitle = "Exercise #" + std::to_string(exercise);
    }

    void ImGuiComponent::Update(float) {
    }

    void ImGuiComponent::Render() const {
        if (!ImGui::GetCurrentContext()) return;

        if (m_ShowWindow) {
            ImGui::Begin(m_WindowTitle.c_str());

            ImGui::InputInt("# samples", &m_Value);

            if (m_Exercise == 1) {
                if (ImGui::Button("Trash The Cache")) {
                    RunCacheTest();
                }
            }
            else if (m_Exercise == 2) {
                if (ImGui::Button("Trash The Cache with GameObject3D")) {
                    RunCacheTestGameObject3D();
                }
                if (ImGui::Button("Trash The Cache with GameObject3DAlt")) {
                    RunCacheTestGameObject3DAlt();
                }
            }

            if (!m_Durations.empty()) {
                ImGui::Text(" ");
                ImGui::PlotLines(" ", m_Durations.data(), static_cast<int>(m_Durations.size()), 0,
                    nullptr, 0.0f, *std::max_element(m_Durations.begin(), m_Durations.end()),
                    ImVec2(300, 150));
            }

            if (!m_Durations3D.empty()) {
                ImGui::Text("GameObject3D");
                ImGui::PlotLines("##GameObject3D", m_Durations3D.data(), static_cast<int>(m_Durations3D.size()), 0,
                    nullptr, 0.0f, *std::max_element(m_Durations3D.begin(), m_Durations3D.end()),
                    ImVec2(300, 150));
            }

            if (!m_Durations3DAlt.empty()) {
                ImGui::Text("GameObject3DAlt");
                ImGui::PlotLines("##GameObject3DAlt", m_Durations3DAlt.data(), static_cast<int>(m_Durations3DAlt.size()), 0,
                    nullptr, 0.0f, *std::max_element(m_Durations3DAlt.begin(), m_Durations3DAlt.end()),
                    ImVec2(300, 150));
            }

            if (!m_StepSizes3D.empty() && !m_Durations3D.empty() &&
                !m_StepSizes3DAlt.empty() && !m_Durations3DAlt.empty()) {

                ImGui::Text("Combined Graph:");

                if (ImPlot::BeginPlot("##CombinedGraph", ImVec2(400, 200))) {
                    // Find axis limits
                    float minX = std::min(*std::min_element(m_StepSizes3D.begin(), m_StepSizes3D.end()),
                        *std::min_element(m_StepSizes3DAlt.begin(), m_StepSizes3DAlt.end()));
                    float maxX = std::max(*std::max_element(m_StepSizes3D.begin(), m_StepSizes3D.end()),
                        *std::max_element(m_StepSizes3DAlt.begin(), m_StepSizes3DAlt.end()));

                    float minY = std::min(*std::min_element(m_Durations3D.begin(), m_Durations3D.end()),
                        *std::min_element(m_Durations3DAlt.begin(), m_Durations3DAlt.end()));
                    float maxY = std::max(*std::max_element(m_Durations3D.begin(), m_Durations3D.end()),
                        *std::max_element(m_Durations3DAlt.begin(), m_Durations3DAlt.end()));

                    maxY = minY + (maxY - minY) * 0.4f; 
                    maxX = minX + (maxX - minX) * 0.4f;  

                    ImPlot::SetupAxes("Step Size", "Duration", ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickLabels);
                    ImPlot::SetupAxesLimits(minX, maxX, minY, maxY, ImGuiCond_Always);

                    ImPlot::PlotLine("GameObject3D",
                        m_StepSizes3D.data(), m_Durations3D.data(),
                        static_cast<int>(m_StepSizes3D.size()));

                    ImPlot::PlotLine("GameObject3DAlt",
                        m_StepSizes3DAlt.data(), m_Durations3DAlt.data(),
                        static_cast<int>(m_StepSizes3DAlt.size()));

                    ImPlot::EndPlot();
                }
            }

            ImGui::End();
        }
    }

    void ImGuiComponent::RunCacheTest() const {
        m_StepSizes.clear();
        m_Durations.clear();

        int ARRSIZE = m_Value * VALUE_MULTIPLYER;
        std::vector<int> arr(ARRSIZE, 1);

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
            const auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < ARRSIZE; i += stepsize) {
                arr[i] *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            m_StepSizes.push_back(static_cast<float>(stepsize));
            m_Durations.push_back(static_cast<float>(duration.count()));
        }
    }

    void ImGuiComponent::RunCacheTestGameObject3D() const {
        m_StepSizes3D.clear();
        m_Durations3D.clear();

        int arrSize = m_Value * VALUE_MULTIPLYER;
        std::vector<GameObject3D> arr(arrSize);

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
            const auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arrSize; i += stepsize) {
                arr[i].ID *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            m_StepSizes3D.push_back(static_cast<float>(stepsize));
            m_Durations3D.push_back(static_cast<float>(duration.count()));
        }
    }

    void ImGuiComponent::RunCacheTestGameObject3DAlt() const {
        m_StepSizes3DAlt.clear();
        m_Durations3DAlt.clear();

        int arrSize = m_Value * VALUE_MULTIPLYER;
        GameObject3DAlt arrAlt(arrSize);

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
            const auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arrSize; i += stepsize) {
                arrAlt.IDs[i] *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            m_StepSizes3DAlt.push_back(static_cast<float>(stepsize));
            m_Durations3DAlt.push_back(static_cast<float>(duration.count()));
        }
    }

    void ImGuiComponent::SetTitle(const std::string& title) {
        m_WindowTitle = title;
    }

    void ImGuiComponent::ToggleWindow() {
        m_ShowWindow = !m_ShowWindow;
    }


}
