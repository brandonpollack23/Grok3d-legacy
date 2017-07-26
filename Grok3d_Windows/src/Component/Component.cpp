#include "Component/Component.h"

using namespace Grok3d;
using namespace Grok3d::Components;

//Every class will have a new instance of this template function
//each new template function will make a new static variable myBitShift in the order
//the GetComponentTypeAccessIndex functions were called, these functions are needed to access/add components so they weill be called
//when a component is added to the system
//this bit is also the left shift amount for the mask in entityhandle to determine if an entity has what components
template<class ComponentType>
GRK_Component::ComponentTypeAccessIndex GRK_Component::GetComponentTypeAccessIndex()
{
    static const GRK_Component::ComponentTypeAccessIndex myComponentTypeAccessIndex = _offsetCounter++;
    return myBitShift;
}
