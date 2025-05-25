#pragma once
#include "GameObject.h"
#include "Component.h"
#include <memory>

namespace dae
{
	class PowerPelletComponent : public Component
	{
    public:
        PowerPelletComponent(GameObject* owner) : Component(owner) {};
    private:
    };
}