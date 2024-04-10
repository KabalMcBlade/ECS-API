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

**Due the nature of the library and where you can link this against to, you have to add the processor ECS_EXPORTS to avoid linkage error.**<br>
**Also, in case your project is using precompiled headers, remember to add the header file in the translation units component_manager.cpp and entity_manager.cpp**


## How to import in your project

A way is cloning this repository in your project, but in that case need to remove/exclude from your project everything apart the folder ECS-API/ECS, but is not very pretty. Also it needs to change if using precompiled headers or not, which might result in having local changes into your project without been able to submit.<br>
A nice way instead, is simple to download or cloning this project somewhere locally and copy inside your own project the ECS folder containig ONLY the implementation of the API, the [ECS](ECS-API/ECS).


## How to use

There are some steps to do at the beginning and some step to perform at the finalization. These are:

1. Create the Entity Manager and the Component Manager, passing a max entity count and a max component per entity count. These 2 values are the limits of the entities and the limits of component per entity
	```cpp
	ecs::EntityManager::Create(100u);
	ecs::ComponentManager::Create(100u, 32u);
	```
2. Register all the components which are going to be used during the program into the Component Manager, for instance
	```cpp
	ecs::ComponentManager::RegisterComponent<Transform>();
	ecs::ComponentManager::RegisterComponent<Kinematic>();
	ecs::ComponentManager::RegisterComponent<RigidBody>();
	ecs::ComponentManager::RegisterComponent<Health>();
	ecs::ComponentManager::RegisterComponent<Camera>();
	ecs::ComponentManager::RegisterComponent<Render>();
	ecs::ComponentManager::RegisterComponent<...>();
	```
3. After this point, create the entity whenever the program needs, for instance
	```cpp
	ecs::Entity camera = ecs::EntityManager::CreateEntity();
	ecs::Entity player = ecs::EntityManager::CreateEntity();
	ecs::Entity ... = ecs::EntityManager::CreateEntity();
	```

3. Handle the different component per entity, adding, removing, updating, collecting, etc, see the next section functions to more details and the example in the [examples](https://github.com/KabalMcBlade/ECS-API/blob/main/main.cpp)

4. When everything is finished and the program need to shut down, or simple no longer using some of the component or whataver, remember to remove them, for instance
	```cpp
	ecs::ComponentManager::UnregisterComponent<Transform>();
	ecs::ComponentManager::UnregisterComponent<Kinematic>();
	ecs::ComponentManager::UnregisterComponent<RigidBody>();
	ecs::ComponentManager::UnregisterComponent<Health>();
	ecs::ComponentManager::UnregisterComponent<Camera>();
	ecs::ComponentManager::UnregisterComponent<Render>();
	```

5. At shutdown of the program, destroy the Entity Manager and the Component Manager
	```cpp
	ecs::ComponentManager::Destroy();
	ecs::EntityManager::Destroy();
	```


## Functions

Apart the aformetioned functions to initialise, destroy, register and unregister components, this API has:

- `ecs::ComponentManager::AddComponent`<br>
	Is adding a component to an entity, like `ecs::ComponentManager::AddComponent<Render>(player);`
- `ecs::ComponentManager::RemoveComponent`<br>
	Is removing a component from an entity, like `ecs::ComponentManager::RemoveComponent<Render>(player);`
- `ecs::ComponentManager::HasComponents`<br>
	Return true if an entity as a component, lile `const bool hasRender = ecs::ComponentManager::HasComponents<Render>(player);`
- `ecs::ComponentManager::GetComponent`<br>
	Return the reference to the component associated to the entity, like: `Render& render = ecs::ComponentManager::GetComponent<Render>(player);`
- `ecs::ComponentManager::IterateEntitiesWithAll`
	Iterate across all entities having **all/both** the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithAll<Transform, RigidBody, Health>())
	{
		// do something with iterator, which is an Entity
	}
	```
- `ecs::ComponentManager::IterateEntitiesWithAny`
	Iterate across all entities having **any/either** of the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithAny<Transform, RigidBody, Health, Render>())
	{
		// do something with iterator, which is an Entity
	}
	```
- `ecs::ComponentManager::IterateEntitiesWithNot`
	Iterate across all entities **not** having **all/both** of the component/s passed as template argument, and returning each entity, like:
	```cpp
	for (auto iterator : ecs::IterateEntitiesWithNot<Health>())
	{
		// do something with iterator, which is a Entity
	}
	```
- `ecs::ComponentManager::CollectEntitiesWithAll`
	Collect in a std::vector the entities having **all/both** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected;
	...
	ecs::EntityCollector::CollectEntitiesWithAll<Transform, RigidBody, Health>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```
- `ecs::ComponentManager::CollectEntitiesWithAny`
	Collect in a std::vector the entities having **any/either** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected;
	...
	ecs::EntityCollector::CollectEntitiesWithAny<Transform, RigidBody, Health, Render>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```
- `ecs::ComponentManager::CollectEntitiesWithNot`
	Collect in a std::vector the entities **not** having **all/both** the component/s passed as template argument, like
	```cpp
	std::vector<ecs::Entity> entitiesCollected;
	...
	ecs::EntityCollector::CollectEntitiesWithNot<Health>(entitiesCollected);
	for (const ecs::Entity entityCollected : entitiesCollected)
	{
		// do something with entityCollected, the entity collect
	}
	entitiesCollected.clear();
	```

## LICENSE

GPL-3.0


# Build Status

| Platform | Build Status |
|:--------:|:------------:|
| Windows (Visual Studio 2019) | [![Build status](https://ci.appveyor.com/api/projects/status/30qjfjlc7fodhceb?svg=true)](https://ci.appveyor.com/project/KabalMcBlade/ecs-api) |
