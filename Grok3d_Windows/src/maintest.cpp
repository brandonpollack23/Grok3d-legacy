#include "grok3d.h"

#include <cmath>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

int main()
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto pointEntity = ecm.CreateEntity();
        GRK_GameLogicComponent glc(pointEntity);
        glc.RegisterBehaviour(
            [](GRK_EntityHandle entity, float dt) -> GRK_Result
            {
                static const double speed = 1 / 3.0;

                static float direction = 1;

                auto transform = entity.GetComponent<GRK_TransformComponent>();

                float translationX = direction * speed * dt;

                glm::vec3 worldPosition = transform->GetWorldPosition();

                transform->TranslateLocal(translationX, 0, 0);

                if (abs(worldPosition.x) >= 25)
                {
                    direction = -direction;
                }

                return GRK_Result::Ok;
            });
        pointEntity.AddComponent(glc);
        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();

    return 0;
}
