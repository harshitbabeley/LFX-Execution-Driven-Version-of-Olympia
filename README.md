# LFX-Execution-Driven-Version-of-Olympia
## Pattern

I used a parent interface class and registry for diffferent shape that inhertit the basic methods from the parent shape class.

## Description

The program lets the user ask about an object (circle, square, rectangle) from the command line.
Each object knows how to:

1. Describe itself

2. Compute its own values (perimeter, area, etc.)

The main function does not care what the object is. It just decribes the object and coputes the object paramters.

## Method

1. Define a base interface (Shape) that exposes only what the main program needs

2. Implement actual object behavior in a generic implementation class namely ShapeDef

3. Store object creators in a registry (keyed by object name)

4. Create the correct object at runtime using the registry

5. Call common functions (describe, compute) through polymorphism

There are no if, else, or switch statements based on object type.

## Usage

1. Build: `g++ describe_object.cpp -o describe_object`
2. Run : <br>`./describe_object circle 2` <br>





