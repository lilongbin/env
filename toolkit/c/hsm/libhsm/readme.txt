
use steps:
Draw the statechart in EA.
Check the model using EA's model validation. Fix any issues.
Export the model to an XML file
Run gen_hsm on the XML file. Fix any issues.

Integrate the generated code into the project
--Integrate any new events into the system's event list
--Implement any new action or guard functions
--Rename any existing action or guard functions that were renamed in the model

1. Define the statechart's elements for the statechart engine.
2. Define the HSM_Statechart_T object (variable) to be used by the statechart engine for the statechart.
3. Implement the guard and action functions used by the statechart.
4. Call the HSM_Init() and HSM_Begin() function to initialize the HSM_Statechart_T object and to cause the statechart 
   to enter its initial state.
5. Call HSM_Process_Event() when events occur that need to be handled by the statechart engine.


