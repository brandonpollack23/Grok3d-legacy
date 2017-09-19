#include "grok3d.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

class MoveBackAndForthBehavour : GRK_GameBehaviourBase
{
    public:
        MoveBackAndForthBehavour(GRK_Entity entity, GRK_EntityComponentManager* entityComponentManger) : 
            GRK_GameBehaviourBase(entity, entityComponentManger),
            m_transform(m_entityComponentManager->GetComponent<GRK_TransformComponent>(entity))
        {
        }

        void Update(float dt) override
        {
            const float speed = 1 / 3.0;

            float direction = 1;

            float translationX = direction * speed * dt;

            glm::vec3 worldPosition = m_transform->GetWorldPosition();

            if (worldPosition.x >= 25)
            {
                direction = -direction;
            }

            m_transform->TranslateLocal(translationX, 0, 0);
        }

    private:
        GRK_ComponentHandle<GRK_TransformComponent> m_transform;
};

int main()
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto pointEntity = ecm.CreateEntity();
        MoveBackAndForthBehavour moveBehaviour(pointEntity, &ecm);
        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();

    return 0;
}
