#include "grok3d.h"

using namespace Grok3d;

int main()
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto pointEntity = ecm.CreateEntity();
        //TODO add cpp behaviour that makes it move back and forth
    };
    GRK_Engine engine(engineInitialization);
}