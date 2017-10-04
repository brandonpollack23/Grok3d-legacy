#include "grok3d.h"

#include <cmath>
#include <iostream>
#include <memory>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

class MoveBackAndForthBehavour : public GRK_GameBehaviourBase
{
    public:
        MoveBackAndForthBehavour(GRK_EntityHandle entity) : 
            GRK_GameBehaviourBase(entity),
            m_transform(entity.GetComponent<GRK_TransformComponent>())
        {
        }

        void Update(double dt) override
        {
            static const float speed = 1.0f / 3.0f;

            float translationX = m_direction * speed * static_cast<float>(dt);

            m_updateCount++;

            m_transform->TranslateLocal(translationX, 0, 0);

            glm::dvec3 worldPosition = m_transform->GetWorldPosition();

            if (abs(worldPosition.x) >= 3)
            {
                m_direction = -m_direction;
            }

            if (m_updateCount == 72)
            { 
                std::cout << "Entity #" << m_owningEntity << ": " << worldPosition.x << ", " << worldPosition.y << ", " << worldPosition.z << std::endl;
                m_updateCount = 0;
            }
        }

    private:
        GRK_ComponentHandle<GRK_TransformComponent> m_transform;
        float m_direction = 1;
        int m_updateCount = 0;
};

GRK_Result CreateAndRegisterMoveTestEntity(GRK_EntityComponentManager& ecm)
{
    auto pointEntity = ecm.CreateEntity();

    auto moveBehaviour = std::make_unique<MoveBackAndForthBehavour>(pointEntity);

    GRK_GameLogicComponent glc;
    glc.RegisterBehaviour(std::move(moveBehaviour));

    return pointEntity.AddComponent(std::move(glc));
}

int main()
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        for (int i = 0; i < 5; i++)
        {
            auto result = CreateAndRegisterMoveTestEntity(ecm);
            if (result != GRK_Result::Ok)
            {
                return result;
            }
        }

        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();

    return 0;
}
