# LFX-Execution-Driven-Version-of-Olympia
## Pattern

I used a runtime factory + registry pattern with a common interface.
The idea is simple: objects register themselves, and the program creates the requested object at runtime based on user input.

## Description

The program lets the user ask about an object (circle, square, rectangle) from the command line.
Each object knows how to:

1. describe itself

2. compute its own values (perimeter, area, etc.)

The main function does not care what the object is. It just asks the object to do its job.

## Method

1. Define a base interface (Shape) that exposes only what the main program needs

2. Implement actual object behavior in a generic implementation class

3. Store object creators in a registry (keyed by object name)

4. Create the correct object at runtime using the registry

5. Call common functions (describe, compute) through polymorphism

There are no if, else, or switch statements based on object type.

