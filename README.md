# SpaceShooter_SDL
A shmup game made using a data oriented approach. Uses SDL and GLM, features an ECS system and quadtree collision checking

The controls for playing are:
- WASD or arrow keys for movement.
- SPACE for shooting. hold for automatic fire, tap for semi-automatic.
Colliding with an enemy and dying closes the application.

This project is meant as an exercise in data oriented programming.
The goal was to make a performant 2D game that is able to handle spawning, moving and collisions of multiple entities.

The approaches used to achieve this are as follows:

##Minimize allocations 

In order to help performance I tried to reduce the amount of heap allocations during run time. Most values are stored on the stack and passed by reference instead of copies. Collections that needed to dynamically scale were either replaced with array based classes of fixed maximum size, or reserved their length at the start of the application.

##ECS

In order to keep track of entities and what they are, an Entity Component System was utilized. The entities are simply IDs that can be used to access components. Components are structs that are stored contiguously in type specific arrays. Each array is the length of the max entities allowed in the game. Systems are able to get access to tuples of component references via a SystemView. Systems are then able to manipulate the values in the components, so that when other systems read them outcomes are changed. The SystemView maintains a vector of tuples to references of components. Tuples are added and subtracted as entities with relevant components are created or destroyed.

##QuadTree collisions

A big performance impact is how collisions are handled. The first implementation in the project used the approach of checking every transform against every other transform, resulting in an exponential performance loss. This was instead replaced by a quadtree approach to collision checking. The quadtree approach meant that the screen space was divided into leaf nodes of equal sizes leaving us with a fixed amount of nodes to check. Whenever an entity moved the quadtree updated what leaf node it was in. The collision system traverses the quadtree only calculating collisions if more than one entity is contained in the same node and are of types that can collide. This approach means that the collision checking is not subject to an exponential growth and is therefore able to handle more entities in the world then it would have otherwise.
