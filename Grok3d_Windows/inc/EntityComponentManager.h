/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYCOMPONENENTMANAGER__H
#define __ENTITYCOMPONENENTMANAGER__H

#include "grok3d_types.h"

#include "Entity/EntityHandle.h"

#include "Component/TransformComponent.h"
#include "Component/GameLogicComponent.h"
#include "Component/ComponentHandle.h"

#include "System/SystemManager.h"

#include "bidir_map.h"
#include "tupleextensions.h"

#include <vector>
#include <tuple>
#include <unordered_map>
#include <functional>

namespace Grok3d
{
    template<class... ComponentTypes>
    class GRK_EntityComponentManager__
    {
    private:
        using GRK_Entity = Grok3d::Entities::GRK_Entity;
        using GRK_EntityHandle = Grok3d::Entities::GRK_EntityHandle;
        using GRK_Component = Grok3d::Components::GRK_Component;
        template<class ComponentType>
        using GRK_ComponentHandle = Grok3d::Components::GRK_ComponentHandle<ComponentType>;
        using GRK_ComponentBitMask = Grok3d::Components::GRK_ComponentBitMask;
        using GRK_SystemManager = Grok3d::Systems::GRK_SystemManager;

        using ComponentTuple = std::tuple<ComponentTypes...>;
        using ComponentStoreTuple = std::tuple<std::vector<ComponentTypes>...>;


    public:
        GRK_EntityComponentManager__() :
            m_deletedUncleanedEntities(std::vector<GRK_Entity>()),
            m_entityComponentsBitMaskMap(std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(INITIAL_ENTITY_ARRAY_SIZE)),
            m_entityComponentIndexMaps(std::vector<notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>>())
        {
            m_deletedUncleanedEntities.reserve(INITIAL_ENTITY_ARRAY_SIZE / 4);

            static_assert(notstd::ensure_parameter_pack_unique<ComponentTypes...>::value, "The template arguments to GRK_EntityComponentManager__ must all be unique");

            setup_component_stores(*this, m_componentStores);
        }

        GRK_Result Initialize(GRK_SystemManager* systemManager)
        {
            m_systemManager = systemManager;
            m_isInitialized = true;

            return GRK_Result::Ok;
        }

        GRK_EntityHandle CreateEntity()
        {
            //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities    
            GRK_Entity id = s_NextEntityId++;

            m_entityComponentsBitMaskMap[id] = 0;

            this->AddComponent(id, Grok3d::Components::GRK_TransformComponent());

            return GRK_EntityHandle(this, id);
        }

        GRK_ComponentBitMask GetEntityComponentsBitMask(const GRK_Entity entity) const
        {
            if (entity != 0)
            {
                return m_entityComponentsBitMaskMap.at(entity);
            }
            else
            {
                return 0;
            }
        }

        GRK_Result DeleteEntity(GRK_Entity entity)
        {
            GRK_Result result = GRK_Result::Ok;

            //ComponentManager's deleting their components is handled by GarbageCollection
            m_deletedUncleanedEntities.push_back(entity);

            // send a component bit mask of 0 to all Systems, no components means any system will unregister
            m_systemManager->UnregisterEntity(GRK_EntityHandle(this, entity));

            return result;
        }

        std::vector<GRK_Entity>& GetDeletedUncleanedEntities()
        {
            return m_deletedUncleanedEntities;
        }

        template<class ComponentType>
        static constexpr size_t GetComponentTypeAccessIndex()
        {
            return notstd::type_to_index<ComponentType, ComponentTuple>::value;
        }

        template<class ComponentType>
        GRK_Result AddComponent(GRK_Entity entity, ComponentType&& newComponent)
        {
            static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value, "AddComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            auto componentTypeIndex = GetComponentTypeAccessIndex<ComponentType>();

            //only add a component if one doesnt exist
            if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(componentTypeIndex)) == 0)
            {
                // TODO should be ref but will be updated soon
                auto& componentTypeVector =
                    std::get<std::vector<ComponentType>>(m_componentStores);

                if (componentTypeVector.size() == componentTypeVector.max_size())
                {
                    return GRK_Result::NoSpaceRemaining;
                }
                else
                {
                    //TODO 8 have 2 CVARs that customizes load reallocation size, for now i'm doing 10% larger
                    //SCALE NUM and SCALE DEN
                    //DOCUMENTATION put the formula for scale factor of this in the docs, %scale = (1+NUM/DEN)

                    //resize vector if necessary
                    const auto cap = componentTypeVector.capacity();
                    if (cap == componentTypeVector.size())
                    {
                        componentTypeVector.reserve(cap + cap / 10);
                    }

                    //add the component to the end our vector
                    componentTypeVector.push_back(std::move(newComponent));

                    auto& entityInstanceMap = m_entityComponentIndexMaps.at(componentTypeIndex);

                    //the new size - 1 is the index of the vector the element is stored at
                    entityInstanceMap.put(entity, static_cast<ComponentInstance>(componentTypeVector.size() - 1));

                    m_entityComponentsBitMaskMap[entity] |=
                        IndexToMask(componentTypeIndex);

                    //inform all systems of new component added to this entity
                    m_systemManager->UpdateSystemEntities(GRK_EntityHandle(this, entity));

                    return GRK_Result::Ok;
                }
            }
            else
            {
                return GRK_Result::ComponentAlreadyAdded;
            }
        }

        template<class ComponentType>
        GRK_ComponentHandle<ComponentType> GetComponent(GRK_Entity entity) const
        {
            static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value, "GetComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

            auto componentTypeIndex = GetComponentTypeAccessIndex<ComponentType>();

            const GRK_ComponentBitMask componentMask =
                static_cast<GRK_ComponentBitMask>(IndexToMask(componentTypeIndex));

            if (entity == 0 || (m_entityComponentsBitMaskMap.at(entity) & componentMask) == 0)
            {
                return GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }
            else if ((m_entityComponentsBitMaskMap.at(entity) & componentMask) == componentMask)
            {
                //this is a vector of the type we are trying to remove
                // TODO should be ref but will be changed soon
                const auto& componentTypeVector =
                    std::get<std::vector<ComponentType>>(m_componentStores);

                const auto& entityInstanceMap = m_entityComponentIndexMaps.at(componentTypeIndex);

                //get the instance (index in our vector) from teh entityInstanceMap
                const ComponentInstance instance = entityInstanceMap.at(entity);

                //use the instance to index the array of that componenttype
                const ComponentType* const componentPointer = &(componentTypeVector.at(instance));

                //return it in a handle
                return GRK_ComponentHandle<ComponentType>(this, componentPointer, entity);
            }
            else
            {
                return GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }

        }

        //removes component for specified entity if it exists
        template<class ComponentType>
        GRK_Result RemoveComponent(GRK_Entity entity)
        {
            static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value,
                    "RemoveComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

            const GRK_ComponentBitMask componentMask = static_cast<GRK_ComponentBitMask>(IndexToMask(GetComponentTypeAccessIndex<ComponentType>()));

            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            if ((m_entityComponentsBitMaskMap[entity] & componentMask) > 0)
            {
                //TODO GC this as well, when you remove component you should just remove it from
                //entity mask and store the actual component from the store's ComponentInstance in a
                //m_deletedUncleanedComponents vector to iterate through
                //there may be some overlap with the entity removal pass so use .find(entity) and
                //check to be sure it is already out of the map
                RemoveComponentHelper(entity, GetComponentTypeAccessIndex<ComponentType>());
            }
            else
            {
                return GRK_Result::NoSuchElement;
            }
        }

        // Garbage collects deleted entities (this is Components are always directly deleted as of now)
        void GarbageCollect()
        {
            //TODO dont always do this lets be smarter
            garbage_collect_iter();
        }

    private:
        template<class ComponentType>
        GRK_Result RemoveComponentHelper(GRK_Entity entity)
        {
            static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value, "RemoveComponentHelper Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

            const auto componentAccessIndex = GetComponentTypeAccessIndex<ComponentType>();
            //this is a vector of the type we are trying to remove
            auto& componentTypeVector = std::get<std::vector<ComponentType>>(m_componentStores);

            //this is the map of entity to components for this type
            auto& entityInstanceMap = m_entityComponentIndexMaps.at(componentAccessIndex);

            //check if the elment is in the map
            //and do what we need to if it is not
            if (entityInstanceMap.find(entity) == entityInstanceMap.end())
            {
                return GRK_Result::NoSuchElement;
            }
            else
            {
                //this entity exists so we move the last element of the components vector
                //to the spot that this one was taking up

                // Index of element we are removing
                const auto removeIndex = entityInstanceMap.at(entity);

                // Instance of element we are moving
                auto lastElementEntity = entityInstanceMap.reverse_at(componentTypeVector.size() - 1);

                // if element is not last then we move last into this one's place
                if (lastElementEntity != entity)
                {
                    // Element we are moving into it's old place
                    auto& lastElement = componentTypeVector.back();

                    //use std::move so we cannibilize any allocated components and dont copy them
                    componentTypeVector[removeIndex] = std::move(lastElement);
                }

                //then remove it from the map
                //and shorten our vector
                entityInstanceMap.erase(entity);
                componentTypeVector.pop_back();

                // If there is nothing in componentTypeVector we just erased the last element, so no need to update map
                if (componentTypeVector.size() > 0)
                {
                    entityInstanceMap.reverse_erase(componentTypeVector.size());
                    entityInstanceMap.put(lastElementEntity, removeIndex);
                }

                //remove it from bitmask
                m_entityComponentsBitMaskMap[entity] &= ~(IndexToMask(componentAccessIndex));

                return GRK_Result::Ok;
            }
        }

    private:
        /***Set up Component Stores***/
        template<int index, class... Ts>
        struct setup_component_stores_impl
        {
            void operator()(GRK_EntityComponentManager& ecm, std::tuple<Ts...>& t)
            {
                auto& elem = std::get<index>(t);
                elem.reserve(INITIAL_ENTITY_ARRAY_SIZE);
                ecm.m_entityComponentIndexMaps.push_back(notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>(INITIAL_ENTITY_ARRAY_SIZE));
                setup_component_stores_impl<index - 1, Ts...>{}(ecm, t);
            }
        };
        template<class... Ts>
        struct setup_component_stores_impl<-1, Ts...>
        {
            void operator()(GRK_EntityComponentManager& ecm, std::tuple<Ts...>& t) {}
        };
        template<class... Ts>
        void setup_component_stores(GRK_EntityComponentManager& ecm, std::tuple<Ts...>& t)
        {
            const auto size = sizeof...(Ts);
            setup_component_stores_impl<size - 1, Ts...>{}(ecm, t);
        }

        /***Garbage Collect Iterate through component types by index***/
        template<int ComponentIndex, class... Ts>
        struct garbage_collect_iter_impl
        {
            void operator()(GRK_EntityComponentManager& ecm)
            {
                for(auto& entity : ecm.m_deletedUncleanedEntities)
                {
                    using ComponentType = typename notstd::index_to_type<ComponentIndex, Ts...>::type;
                    if((ecm.m_entityComponentsBitMaskMap[entity] & IndexToMask(ComponentIndex)) > 0)
                    {
                        ecm.template RemoveComponentHelper<ComponentType>(entity);
                    }
                }

                garbage_collect_iter_impl<ComponentIndex - 1, Ts...>{}(ecm);
            }
        };
        template<class... Ts>
        struct garbage_collect_iter_impl<-1, Ts...>
        {
            void operator()(GRK_EntityComponentManager& ecm) {}
        };
        void garbage_collect_iter()
        {
            const auto size = sizeof...(ComponentTypes);
            garbage_collect_iter_impl<size - 1, ComponentTypes...>{}(*this);
            m_deletedUncleanedEntities.clear();
        }

    private:
        bool m_isInitialized = false;

        GRK_SystemManager* m_systemManager;

        ComponentStoreTuple m_componentStores;

        //entity ID
        static GRK_Entity s_NextEntityId;

        //list of deleted entites that need to be GC'd
        std::vector<GRK_Entity> m_deletedUncleanedEntities;

        //index into vector for component
        typedef size_t ComponentInstance;
        //this is a map of entities to a bitmask of their components, used for system registration/component deletion checks etc
        std::unordered_map<GRK_Entity, GRK_ComponentBitMask> m_entityComponentsBitMaskMap;

        //vector of maps from entity to component index into componentStore[ComponentType::Offset]
        mutable std::vector<notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>> m_entityComponentIndexMaps;
    };
} /*Grok3d*/

#endif
