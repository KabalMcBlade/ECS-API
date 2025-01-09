# ECS-API

ECS-API is a ECS framework.
It is build around 3 simple ideas:
1. Optimised for space-time complexity<br>
	It is using the minimal storage space as possible and it using linear search.
2. No boilerplate on the user side<br>
	Althought the API itself has quite of boilerplate, it is not the case for the user.<br>
	This API is very straigforward to use but also has a very powerful and inexpensive implementation.
3. Flexibility in order to deal with different implementations<br>
	This ECS is actually a "EC", because the system are up to the user.<br>
	The API provides functions to add, remove, update, iterate, collect using idioms (any, all, not) the components associated to an entity, thus the entities and the components can be managed by any functions, aka systems, implemented by user side, they do not require any "base class" or whatever.

It is important to note that the entity data structure is a 32bit size, having a index of 24 bits and a version of 8 bits.<br>
Whenever an already created entity got destroyed, when a new one is created, the API will use the "spot" left free by the previous one, re-using the same index but incrementing the version.<br>
When the version reach the limit, simply wrap around and restart.


The actual API can be found here: [ECS](https://github.com/KabalMcBlade/ECS-API/tree/main/ECS)<br>
While here there are some [examples](https://github.com/KabalMcBlade/ECS-API/blob/main/main.cpp) of usage.

In order to use, just include the main [header](https://github.com/KabalMcBlade/ECS-API/tree/main/ECS/ecs.h) and use the namespace `ecs` and you are good to go!

## Important

Is working on **64 bit platform only** due the usage of _BitScanForward64 (or __builtin_ctzll unix systems)


## Important Compilation info

When exporting the ECS library from a DLL project, remember to add the preprocessor macro ECS_DLL_EXPORT.<br>
When importing the ECS library that was exported from a DLL project, remember to add the preprocessor macro ECS_DLL_IMPORT.<br>
If you are not using dynamic libraries at all, such as in the same executable project or a static library, do not add any of the above macros!<br>


## How to use

There are some steps to do at the beginning and some step to perform at the finalization. These are:

1. Get the singletons, which are responsable for everything, much better than keep calling the getter!
	```cpp
	ecs::ComponentManager& componentManager = ecs::GetComponentManager();
	ecs::EntityManager& entityManager = ecs::GetEntityManager();
	```

2. Create the Entity Manager and the Component Manager, passing a max entity count and a max component per entity count. These 2 values are the limits of the entities and the limits of component per entity
	```cpp
	entityManager.Create(100u);
	componentManager.Create(100u, 32u);
	```
3. Register all the components which are going to be used during the program into the Component Manager, for instance
	```cpp
	componentManager.RegisterComponent<Transform>();
	componentManager.RegisterComponent<Kinematic>();
	componentManager.RegisterComponent<RigidBody>();
	componentManager.RegisterComponent<Health>();
	componentManager.RegisterComponent<Camera>();
	componentManager.RegisterComponent<Render>();
	componentManager.RegisterComponent<...>();
	```
4. After this point, create the entity whenever the program needs, for instance
	```cpp
	ecs::Entity camera = entityManager.CreateEntity();
	ecs::Entity player = entityManager.CreateEntity();
	ecs::Entity ... = entityManager.CreateEntity();
	```

5. Handle the different component per entity, adding, removing, updating, collecting, etc, see the next section functions to more details and the example in the [examples](https://github.com/KabalMcBlade/ECS-API/blob/main/main.cpp)

6. When everything is finished and the program need to shut down, or simple no longer using some of the component or whataver, remember to remove them, for instance
	```cpp
	componentManager.UnregisterComponent<Transform>();
	componentManager.UnregisterComponent<Kinematic>();
	componentManager.UnregisterComponent<RigidBody>();
	componentManager.UnregisterComponent<Health>();
	componentManager.UnregisterComponent<Camera>();
	componentManager.UnregisterComponent<Render>();
	```

7. At shutdown of the program, destroy the Entity Manager and the Component Manager
	```cpp
	componentManager.Destroy();
	entityManager.Destroy();
	```


## Functions

Apart the aformetioned functions to initialise, destroy, register and unregister components, this API has:

- `componentManager.AddComponent`<br>
	Is adding a component to an entity, like `componentManager.AddComponent<Render>(player);`
- `componentManager.RemoveComponent`<br>
	Is removing a component from an entity, like `componentManager.RemoveComponent<Render>(player);`
- `componentManager.HasComponents`<br>
	Return true if an entity as a component, lile `const bool hasRender = componentManager.HasComponents<Render>(player);`
- `componentManager.GetComponent`<br>
	Return the reference to the component associated to the entity, like: `Render& render = componentManager.GetComponent<Render>(player);`
- `componentManager.IterateEntitiesWithAll`
	Iterate across all entities having **all/both** the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithAll<Transform, RigidBody, Health>(componentManager))
	{
		// do something with iterator, which is an Entity
	}
	```
- `componentManager.IterateEntitiesWithAny`
	Iterate across all entities having **any/either** of the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithAny<Transform, RigidBody, Health, Render>(componentManager))
	{
		// do something with iterator, which is an Entity
	}
	```
- `componentManager.IterateEntitiesWithNot`
	Iterate across all entities **not** having **all/both** of the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithNot<Health>(componentManager))
	{
		// do something with iterator, which is a Entity
	}
	```
- `componentManager.CollectEntitiesWithAll`
	Collect in a std::vector the entities having **all/both** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected = cs::EntityCollector::CollectEntitiesWithAll<Transform, RigidBody, Health>();
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```
- `componentManager.CollectEntitiesWithAny`
	Collect in a std::vector the entities having **any/either** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected = ecs::EntityCollector::CollectEntitiesWithAny<Transform, RigidBody, Health, Render>();
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```
- `componentManager.CollectEntitiesWithNot`
	Collect in a std::vector the entities **not** having **all/both** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected = ecs::EntityCollector::CollectEntitiesWithNot<Health>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```
- `componentManager.CollectAndGroupEntitiesWithAllByField`
	This method collects the entities having **all/both** the component/s passed as template argument and <br />
	grouping them in an unordered map, by a defined field in the struct having the same value across entities. <br />
	To do so, your struct has to reflect this field such as, for example: <br />
	```cpp
	struct Health
	{
		using FieldType = float;	// want to group by float field type

		float m_maxValue = 0.0f;
		float m_currentValue = 0.0f;
	};
	```
	Afterward, pass as first template argument this struct, so Health, and pass as parameter the field you want, such as m_currentValue:
	```cpp
	auto  entitiesCollectedAndGroupEntitiesWithAllByField = ecs::EntityCollector::CollectAndGroupEntitiesWithAllByField<Health, Render>(entityManager, componentManager, &Health::m_currentValue);
	for (const auto& [key, entities] : entitiesCollectedAndGroupEntitiesWithAllByField)
	{
		// iterate by group
		for (const auto& entityCollected : entities)
		{
			// do something with entityCollected, the entity collect
		}
	}
	entitiesCollectedAndGroupEntitiesWithAllByField.clear();
	```
- `componentManager.CollectAndGroupEntitiesWithAnyByField`
	This method collects the entities having **any/either** the component/s passed as template argument and <br />
	grouping them in an unordered map, by a defined field in the struct having the same value across entities. <br />
	If an entities has not such group (so struct), will be gathered in the output parameter _noGroupedEntities as std::vector<Entity>. <br />
	To do so, your struct has to reflect this field such as, for example: <br />
	```cpp
	struct Health
	{
		using FieldType = float;	// want to group by float field type

		float m_maxValue = 0.0f;
		float m_currentValue = 0.0f;
	};
	```
	Afterward, pass as first template argument this struct, so Health, and pass as parameter the field you want, such as m_currentValue:
	```cpp
	std::vector<ecs::Entity> entitiesWithoutGroupByAny;
	auto  entitiesCollectedAndGroupEntitiesWithAnyByField = ecs::EntityCollector::CollectAndGroupEntitiesWithAnyByField<Health, Transform, Render>(entityManager, componentManager, &Health::m_currentValue, entitiesWithoutGroupByAny);
	for (const auto& [key, entities] : entitiesCollectedAndGroupEntitiesWithAnyByField)
	{
		// iterate by group
		for (const auto& entityCollected : entities)
		{
			// do something with entityCollected, the entity collect
		}
	}
	// Entities having not Health struct
	for (const ecs::Entity entityWithoutGroupByAny : entitiesWithoutGroupByAny)
	{
		std::cout << "entityCollected -> [Entity " << entityWithoutGroupByAny.GetIndex() << ":" << entityWithoutGroupByAny.GetVersion() << "]" << std::endl;
	}
	entitiesWithoutGroupByAny.clear();
	entitiesCollectedAndGroupEntitiesWithAnyByField.clear();
	```


## LICENSE

MIT


# Build Status

| Platform | Build Status |
|:--------:|:------------:|
| Windows (Visual Studio 2019) | [![Build status](https://ci.appveyor.com/api/projects/status/30qjfjlc7fodhceb?svg=true)](https://ci.appveyor.com/project/KabalMcBlade/ecs-api) |
