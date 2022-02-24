#include "clovepch.h"
#include "Entity.h"


namespace Clove {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_handle(handle), m_scene(scene)
	{  }

}