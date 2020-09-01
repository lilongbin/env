
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
2. get HSM_State_Definition_T object (variable) to be used by the statechart engine for the statechart by get_STATECHARTNAME_Defn().
3. Implement the guard and action functions used by the statechart.
4. create HSM_Engine object and Call its start() function to initialize the hsm engine and to cause the statechart 
   to enter its initial state.
5. Call HSM_Engine object's proccessMessage() when events occur that need to be handled by the statechart engine.


