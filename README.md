# Markov Learning Process
Class project for CSCI 360 at usc.

An implementation of Markov Decision Process reinforced learning using Value, Policy and Q iteraton. An agent learns how to navigate from start to end considering the possiblities of making it from one block to another maximizing total rewards.<br>


**How to run**
Map folder contains different maps with the states and rewards of each state.<br>
To run value iteration, execute this command inside the src folder<br>
`./frozen_lake --agent v --map ../maps/bridge.json --epsilon 0.99 --alpha 1.0 --iteration 500 --gamma 0.99`<br>
You can change the agent from v to p to run the policy iteration and q to run the policy iterations.<br>
You can also change the parameters to experiment with different paramters.<br>


