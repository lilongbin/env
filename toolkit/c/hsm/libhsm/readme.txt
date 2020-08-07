

use step
1.	Define the statechart's elements for the statechart engine.
2.	Define the HSM_Statechart_T object (variable) to be used by the statechart engine for the statechart.
3.	Implement the guard and action functions used by the statechart.
4.	Call the HSM_Start() function to initialize the HSM_Statechart_T object and to cause the statechart to enter its initial state.
5.	Call HSM_Process_Event() when events occur that need to be handled by the statechart engine.


