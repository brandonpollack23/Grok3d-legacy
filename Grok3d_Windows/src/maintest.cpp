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

            static float direction = 1;

            float translationX = direction * speed * static_cast<float>(dt);

            static int updateCount;

            updateCount++;

            m_transform->TranslateLocal(translationX, 0, 0);

            glm::vec3 worldPosition = m_transform->GetWorldPosition();

            if (abs(worldPosition.x) >= 3)
            {
                direction = -direction;
            }


            if (updateCount == 72)
            { 
                std::cout << worldPosition.x << ", " << worldPosition.y << ", " << worldPosition.z << std::endl;
                updateCount = 0;
            }
        }

    private:
        GRK_ComponentHandle<GRK_TransformComponent> m_transform;
};

int main()
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto pointEntity = ecm.CreateEntity();
        auto moveBehaviour = std::make_unique<MoveBackAndForthBehavour>(pointEntity);
        GRK_GameLogicComponent glc;
        glc.RegisterBehaviour(std::move(moveBehaviour));
        pointEntity.AddComponent(std::move(glc));
        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();

    return 0;
}
