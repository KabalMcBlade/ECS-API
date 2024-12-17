#include <stdio.h>
#include <iostream>
#include <random>

// the only actual library for ECS
#include "ECS/ecs.h"

#include "data.h"


// utility to print the example!
namespace
{
	std::ostream& operator<< (std::ostream& _os, const Vector& _v)
	{
		_os << "(" << _v.m_x << ", " << _v.m_y << ", " << _v.m_z << ")";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Matrix& _m)
	{
		_os << "[" << std::endl;
		_os << _m.m_i << std::endl;
		_os << _m.m_j << std::endl;
		_os << _m.m_k << std::endl;
		_os << "]";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Quaternion& _q)
	{
		_os << "{" << _q.m_w << ", " << _q.m_real << "}";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Transform& _t)
	{
		_os << "transform (" << std::endl;
		_os << '\t' << "rotation: " << _t.m_rotation << std::endl;
		_os << '\t' << "position: " << _t.m_position << std::endl;
		_os << ")";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Kinematic& _k)
	{
		_os << "kinematic (" << std::endl;
		_os << '\t' << "source: " << _k.m_source << std::endl;
		_os << '\t' << "target: " << _k.m_target << std::endl;
		_os << '\t' << "maxSpeed: " << _k.m_maxSpeed << std::endl;
		_os << '\t' << "maxAcceleration: " << _k.m_maxAcceleration << std::endl;
		_os << '\t' << "maxRotation: " << _k.m_maxRotation << std::endl;
		_os << '\t' << "time: " << _k.m_time << std::endl;
		_os << ")";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const RigidBody& _r)
	{
		_os << "rigid-body (" << std::endl;
		_os << '\t' << "position: " << _r.m_position << std::endl;
		_os << '\t' << "linearVelocity: " << _r.m_linearVelocity << std::endl;
		_os << '\t' << "angularVelocity: " << _r.m_angularVelocity << std::endl;
		_os << '\t' << "intertiaTensor: " << _r.m_intertiaTensor << std::endl;
		_os << '\t' << "invIntertiaTensor: " << _r.m_invIntertiaTensor << std::endl;
		_os << '\t' << "mass: " << _r.m_mass << std::endl;
		_os << '\t' << "principalMomentsIntertia: " << _r.m_principalMomentsIntertia << std::endl;
		_os << ")";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Health& _h)
	{
		_os << "health (" << std::endl;
		_os << '\t' << "maxValue: " << _h.m_maxValue << std::endl;
		_os << '\t' << "currentValue: " << _h.m_currentValue << std::endl;
		_os << ")";

		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Camera& _c)
	{
		return _os;
	}

	std::ostream& operator<< (std::ostream& _os, const Render& _r)
	{
		return _os;
	}
}



#define MAX_ENTITY_COUNT 100u
#define MAX_COMPONENT_PER_ENTITY_COUNT 32u

int main()
{
	// test purpose only:
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.1f, 1.0f);

	std::vector<ecs::Entity> entitiesCollected;


	//////////////////////////////////////////////////////////////////////////
	// 1. Get the managers
	ecs::ComponentManager& componentManager = ecs::GetComponentManager();
	ecs::EntityManager& entityManager = ecs::GetEntityManager();


	// 2. Create the Entity Manager and the Component Manager
	entityManager.Create(MAX_ENTITY_COUNT);
	componentManager.Create(MAX_ENTITY_COUNT, MAX_COMPONENT_PER_ENTITY_COUNT);

	// 3. Register all the components into the Component Manager
	componentManager.RegisterComponent<Transform>();
	componentManager.RegisterComponent<Kinematic>();
	componentManager.RegisterComponent<RigidBody>();
	componentManager.RegisterComponent<Health>();
	componentManager.RegisterComponent<Camera>();
	componentManager.RegisterComponent<Render>();

	// 4. From this point on is more "game side" related, we create a bunch of entities
	ecs::Entity camera = entityManager.CreateEntity();
	ecs::Entity player = entityManager.CreateEntity();
	ecs::Entity npc0 = entityManager.CreateEntity();
	ecs::Entity npc1 = entityManager.CreateEntity();
	ecs::Entity npc2 = entityManager.CreateEntity();

#ifdef _DEBUG
	// the index is never updated, is unique, the version instead get updated when the entity get destroyed and created again, filling the spot left, and is wrapping around the value
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl;

	std::cout << std::endl;
#endif // _DEBUG

	// Phase one -  Adding component to entities
	
	// Transform to:
	componentManager.AddComponent<Transform>(camera, Transform{ Quaternion { 0.989f, Vector{ -0.065f, -0.010f, -0.130f } }, Vector { 0.0f, 4.0f, -5.0f }});
	componentManager.AddComponent<Transform>(player, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 0.0f, 0.0f, 0.0f }});
	componentManager.AddComponent<Transform>(npc0, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 4.0f, 0.0f, 6.0f } });
	componentManager.AddComponent<Transform>(npc1, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 8.0f, 0.0f, 10.0f } });
	componentManager.AddComponent<Transform>(npc2, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 15.0f, 0.0f, 5.0f } });
	
	// Kinematic to:
	componentManager.AddComponent<Kinematic>(camera, Kinematic{ Vector{ 0.0f, 0.0f, 0.0f}, Vector{ 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 0.0f });
	componentManager.AddComponent<Kinematic>(player, Kinematic{ Vector{ 0.0f, 0.0f, 0.0f}, Vector{ 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 0.0f });
	componentManager.AddComponent<Kinematic>(npc0, Kinematic{ Vector{ 0.0f, 0.0f, 0.0f}, Vector{ 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 0.0f });
	componentManager.AddComponent<Kinematic>(npc1, Kinematic{ Vector{ 0.0f, 0.0f, 0.0f}, Vector{ 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 0.0f });
	componentManager.AddComponent<Kinematic>(npc2, Kinematic{ Vector{ 0.0f, 0.0f, 0.0f}, Vector{ 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 0.0f });

	// RigidBody to:
	componentManager.AddComponent<RigidBody>(npc0);
	componentManager.AddComponent<RigidBody>(npc1);
	componentManager.AddComponent<RigidBody>(npc2);

	// Health to:
	componentManager.AddComponent<Health>(player, Health{1.0f, dist(mt) });
	componentManager.AddComponent<Health>(npc0, Health{ 1.0f, dist(mt) });
	componentManager.AddComponent<Health>(npc1, Health{ 1.0f, dist(mt) });
	componentManager.AddComponent<Health>(npc2, Health{ 1.0f, dist(mt) });

	// Camera to (notice has no data for test purpose):
	componentManager.AddComponent<Camera>(camera);
	componentManager.AddComponent<Camera>(player);

	// Render to (notice has no data for test purpose):
	componentManager.AddComponent<Render>(player);
	componentManager.AddComponent<Render>(npc0);
	componentManager.AddComponent<Render>(npc1);
	componentManager.AddComponent<Render>(npc2);

	
	//////////////////////////////////////////////////////////////////////////
	// TEST 1: Checking data manually entered (Transform and Health) knowing the entities where they belong to
	Transform& camera_transform = componentManager.GetComponent<Transform>(camera);
	Transform& player_transform = componentManager.GetComponent<Transform>(player);
	Transform& npc0_transform = componentManager.GetComponent<Transform>(npc0);
	Transform& npc1_transform = componentManager.GetComponent<Transform>(npc1);
	Transform& npc2_transform = componentManager.GetComponent<Transform>(npc2);

	Health& player_health = componentManager.GetComponent<Health>(player);
	Health& npc0_health = componentManager.GetComponent<Health>(npc0);
	Health& npc1_health = componentManager.GetComponent<Health>(npc1);
	Health& npc2_health = componentManager.GetComponent<Health>(npc2);


#ifdef _DEBUG
	std::cout << "TEST 1: Checking data manually entered (Transform and Health) knowing the entities where they belong to" << std::endl << std::endl;

	std::cout << "TRANSFORM: " << std::endl << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl << camera_transform << std::endl << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl << player_transform << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl << npc0_transform << std::endl << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl << npc1_transform << std::endl << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl << npc2_transform << std::endl << std::endl;

	std::cout << "HEALTH: " << std::endl << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl << player_health << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl << npc0_health << std::endl << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl << npc1_health << std::endl << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl << npc2_health << std::endl << std::endl;

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 2: Every positions are multiplied by 100, knowing the entities where they belong to
	camera_transform.m_position.m_x *= 100.0f;
	camera_transform.m_position.m_y *= 100.0f;
	camera_transform.m_position.m_z *= 100.0f;
	player_transform.m_position.m_x *= 100.0f;
	player_transform.m_position.m_y *= 100.0f;
	player_transform.m_position.m_z *= 100.0f;
	npc0_transform.m_position.m_x *= 100.0f;
	npc0_transform.m_position.m_y *= 100.0f;
	npc0_transform.m_position.m_z *= 100.0f;
	npc1_transform.m_position.m_x *= 100.0f;
	npc1_transform.m_position.m_y *= 100.0f;
	npc1_transform.m_position.m_z *= 100.0f;
	npc2_transform.m_position.m_x *= 100.0f;
	npc2_transform.m_position.m_y *= 100.0f;
	npc2_transform.m_position.m_z *= 100.0f;


#ifdef _DEBUG
	std::cout << "TEST 2: Every positions are multiplied by 100, knowing the entities where they belong to" << std::endl << std::endl;

	std::cout << "TRANSFORM UPDATED: " << std::endl << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl << camera_transform << std::endl << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl << player_transform << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl << npc0_transform << std::endl << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl << npc1_transform << std::endl << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl << npc2_transform << std::endl << std::endl;

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 3: Remove component Transform from NPCs and adding back

	bool npc0_hasTransform = componentManager.HasComponents<Transform>(npc0);
	bool npc1_hasTransform = componentManager.HasComponents<Transform>(npc1);
	bool npc2_hasTransform = componentManager.HasComponents<Transform>(npc2);

#ifdef _DEBUG
	std::cout << "TEST 3: Check that NPCs get Transform component attached" << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc0_hasTransform << std::endl;
	std::cout << "npc1 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc1_hasTransform << std::endl;
	std::cout << "npc2 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc2_hasTransform << std::endl;

	std::cout << std::endl << std::endl;
#endif

	componentManager.RemoveComponent<Transform>(npc0);
	componentManager.RemoveComponent<Transform>(npc1);
	componentManager.RemoveComponent<Transform>(npc2);

	npc0_hasTransform = componentManager.HasComponents<Transform>(npc0);
	npc1_hasTransform = componentManager.HasComponents<Transform>(npc1);
	npc2_hasTransform = componentManager.HasComponents<Transform>(npc2);

#ifdef _DEBUG
	std::cout << "TEST 3 Cont'd: The Transform component has been removed, check if it is gone" << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc0_hasTransform << std::endl;
	std::cout << "npc1 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc1_hasTransform << std::endl;
	std::cout << "npc2 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc2_hasTransform << std::endl;

	std::cout << std::endl << std::endl;
#endif

	componentManager.AddComponent<Transform>(npc0, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 7.0f, 2.0f, 7.0f } });
	componentManager.AddComponent<Transform>(npc1, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 2.0f, 1.0f, 9.0f } });
	componentManager.AddComponent<Transform>(npc2, Transform{ Quaternion { 1.0f, Vector{ 0.0f, 0.0f, 0.0f } }, Vector { 5.0f, 4.0f, 2.0f } });

	npc0_hasTransform = componentManager.HasComponents<Transform>(npc0);
	npc1_hasTransform = componentManager.HasComponents<Transform>(npc1);
	npc2_hasTransform = componentManager.HasComponents<Transform>(npc2);

#ifdef _DEBUG
	std::cout << "TEST 3 Cont'd: The Transform component has been added back, check if is back again" << std::endl << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc0_hasTransform << std::endl;
	std::cout << "npc1 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc1_hasTransform << std::endl;
	std::cout << "npc2 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << " has Transform component? " << npc2_hasTransform << std::endl;

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 4: Iterating every entities having BOTH Transform, RigidBody and Health

#ifdef _DEBUG
	std::cout << "TEST 4: Iterating every entities having BOTH Transform, RigidBody and Health, which should be: " << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	for (auto iterator : ecs::IterateEntitiesWithAll<Transform, RigidBody, Health>())
	{
		std::cout << "iterator -> [Entity " << iterator.GetIndex() << ":" << iterator.GetVersion() << "]" << std::endl;
	}

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 5: Iterating every entities having EITHER Transform, RigidBody and Health

#ifdef _DEBUG
	std::cout << "TEST 5: Iterating every entities having EITHER Transform, RigidBody and Health, which should be: " << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	for (auto iterator : ecs::IterateEntitiesWithAny<Transform, RigidBody, Health>())
	{
		std::cout << "iterator -> [Entity " << iterator.GetIndex() << ":" << iterator.GetVersion() << "]" << std::endl;
	}

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 6: Iterating every entities having NOT Health and NOT RigidBody

#ifdef _DEBUG
	std::cout << "TEST 6: Iterating every entities having NOT Health and NOT RigidBody, which should be: " << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	for (auto iterator : ecs::IterateEntitiesWithNot<Health, RigidBody>())
	{
		std::cout << "iterator -> [Entity " << iterator.GetIndex() << ":" << iterator.GetVersion() << "]" << std::endl;
	}

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 7: Collecting every entities having BOTH Transform, RigidBody and Health

#ifdef _DEBUG
	std::cout << "TEST 7: Collecting every entities having BOTH Transform, RigidBody and Health, which should be: " << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	ecs::EntityCollector::CollectEntitiesWithAll<Transform, RigidBody, Health>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		std::cout << "entityCollected -> [Entity " << entityCollected.GetIndex() << ":" << entityCollected.GetVersion() << "]" << std::endl;
	}
	entitiesCollected.clear();

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 8: Collecting every entities having EITHER Transform, RigidBody and Health

#ifdef _DEBUG
	std::cout << "TEST 5: Collecting every entities having EITHER Transform, RigidBody and Health, which should be: " << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl;
	std::cout << "player = [Entity " << player.GetIndex() << ":" << player.GetVersion() << "]" << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;
	std::cout << "npc1 = [Entity " << npc1.GetIndex() << ":" << npc1.GetVersion() << "]" << std::endl;
	std::cout << "npc2 = [Entity " << npc2.GetIndex() << ":" << npc2.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	ecs::EntityCollector::CollectEntitiesWithAny<Transform, RigidBody, Health>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		std::cout << "entityCollected -> [Entity " << entityCollected.GetIndex() << ":" << entityCollected.GetVersion() << "]" << std::endl;
	}
	entitiesCollected.clear();

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 9: Collecting every entities having NOT Health and NOT RigidBody

#ifdef _DEBUG
	std::cout << "TEST 9: Collecting every entities having NOT Health and NOT RigidBody, which should be: " << std::endl;
	std::cout << "camera = [Entity " << camera.GetIndex() << ":" << camera.GetVersion() << "]" << std::endl;

	std::cout << "Checking..." << std::endl;

	ecs::EntityCollector::CollectEntitiesWithNot<Health, RigidBody>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		std::cout << "entityCollected -> [Entity " << entityCollected.GetIndex() << ":" << entityCollected.GetVersion() << "]" << std::endl;
	}
	entitiesCollected.clear();

	std::cout << std::endl << std::endl;
#endif


	//////////////////////////////////////////////////////////////////////////
	// TEST 10: Destroy an entity and create it again, see the version updated


#ifdef _DEBUG
	std::cout << "TEST 9: Destroy an entity and create it again, The current version is: " << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;

	entityManager.DestroyEntity(npc0);

	npc0 = entityManager.CreateEntity();

	std::cout << "The new version is: " << std::endl;
	std::cout << "npc0 = [Entity " << npc0.GetIndex() << ":" << npc0.GetVersion() << "]" << std::endl;

	std::cout << std::endl << std::endl;
#endif


	componentManager.UnregisterComponent<Transform>();
	componentManager.UnregisterComponent<Kinematic>();
	componentManager.UnregisterComponent<RigidBody>();
	componentManager.UnregisterComponent<Health>();
	componentManager.UnregisterComponent<Camera>();
	componentManager.UnregisterComponent<Render>();


	componentManager.Destroy();
	entityManager.Destroy();

	return 0;
}