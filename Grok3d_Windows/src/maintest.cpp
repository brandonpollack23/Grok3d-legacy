#include "grok3d.h"

#include <cmath>

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

        void Update(float dt) override
        {
            static const double speed = 1 / 3.0;

            static double direction = 1;

            double translationX = direction * speed * dt;

            glm::vec3 worldPosition = m_transform->GetWorldPosition();

            m_transform->TranslateLocal(translationX, 0, 0);

            if (abs(worldPosition.x) >= 25)
            {
                direction = -direction;
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
        MoveBackAndForthBehavour* moveBehaviour = new MoveBackAndForthBehavour(pointEntity);
        GRK_GameLogicComponent glc;
        glc.RegisterBehaviour(moveBehaviour);
        pointEntity.AddComponent(glc);
        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();

    return 0;
}
