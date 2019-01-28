// to compile without using SFML, remove GUI imports
// GUI is the only class that references SFML
#include "GUI.hpp"

// allows us to use the WorldState / simulation
#include "World.hpp"
#include "Simulator.hpp"
#include "EntityControllers.hpp"

int main()
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    //auto world = ExampleWorlds::GetGridWorld720(2);
    auto world = ExampleWorlds::GetGetSquareWorld(Vec2(800, 800), 20, 10, 250, 10);

    // create a new simulator with the given world
    Simulator sim(world);

    // set up a GUI to visualize the simulation with a given frame rate limit
    // the frame rate limit should be set at least as high as your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can also be created at any time to start visualizing an ongoing simulation
    GUI gui(sim, 144);

    // determines the amount of 'time' that passes per simulation tick
    // lower is more 'accurate', but 'slower' to get to a given time
    double simulationTimeStep = 1.0;

    // how many simulation ticks are peformed before each world render in the GUI
    double stepsPerRender = 1;

    // run the simulation and gui update() function in a loop
    while (true)
    {
        for (size_t i = 0; i < stepsPerRender; i++)
        {
            // un-comment to update the robots with a sample controller
            for (auto & robot : sim.getWorld()->getEntities("robot"))
            {
                // if the entity doesn't have a controller we can skip it
                if (!robot.hasComponent<CController>()) { continue; }

                // get the action that should be done for this entity
                EntityAction action = robot.getComponent<CController>().controller->getAction();

                // have the action apply its effects to the entity
                action.doAction(robot, simulationTimeStep);
            }

            // call the world physics simulation update
            // parameter = how much sim time should pass (default 1.0)
            sim.update(simulationTimeStep);
        }
    
        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}