//
// Created by Chi Zhang on 8/24/19.
//

#include "common.hpp"
#include "ValueIterationAgent.hpp"
#include <stdlib.h> 
#include <time.h>

using namespace std;

ValueIterationAgent::ValueIterationAgent(FrozenLakeMDP const &mdp, double gamma, int iterations, double threshold) :
        ValueEstimateAgent(gamma, iterations, threshold), m_mdp(mdp) {
    MSG("Training Value Iteration Agent on " << m_mdp.getName());
    MSG("Initializing Value Iteration Agent");
    initialize();
    MSG("Solving...");
    solve();
}

double ValueIterationAgent::getValue(const GameState &state) {
    // TODO
    return values[state];
}

double ValueIterationAgent::getQValue(const GameState &state, const Action &action) {
    // TODO
    return 0.0;
}

Action ValueIterationAgent::getPolicy(const GameState &state) {
    // TODO
    return actions[state];
}


void ValueIterationAgent::solve() { 
    int counter = 0;
    while(counter != 1000){
        // TODO. Implement Value Iteration here
        set<GameState> allPossibleStates = m_mdp.getStates();
        set<GameState>::iterator it;
        it = allPossibleStates.begin();
        //double FrozenLakeEnv::getReward(const GameState &state, const Action &action, const GameState &nextState) const {
        for(int x = 0; it != allPossibleStates.end(); it++){
            GameState state = *it;
            if(state.getName() == "H"){
                continue;
            }
            vector<Action> action = m_mdp.getPossibleActions(*it);
            vector<double>valuesTemp;
            //cout << "Current state: " << state.getName() << endl;
            double min = -55555;
            Action act;
            for(int i = 0; i < action.size(); i++){
                // return a map from nextState to probability that (state, action) -> nextState
                map<GameState, double> prob = m_mdp.getTransitionStatesAndProbs(state, action[i]);
                /*
                if(action[i] == 0){
                    cout << "Current action: LEFT" << endl;
                }else if(action[i] == 1){
                    cout << "Current action: DOWN" << endl;
                }else if(action[i] == 2){
                    cout << "Current action: RIGHT" << endl;
                }else{
                    cout << "Current action: UP" << endl;
                }
                */
                map<GameState, double>::iterator it2;
                double val = 0;
                int max = 0;
                GameState s2;
                for(it2 = prob.begin(); it2 != prob.end(); it2++){
                
                    //cout << it2->first.getName() << " with prob " << it2->second << " and reward " << m_mdp.getReward(state, action[i], it2->first) << endl;
                    val += it2->second * ( m_mdp.getReward(state, action[i], it2->first) + m_gamma*values[it2->first]);
                }
                /*
                if(m_mdp.isTerminal(s2)){
                    act = action[i];
                    min = val;
                    values[state] = min;
                    actions[state] = act;
                    break;
                }
                */
                if(val > min){
                    act = action[i];
                    /*
                    if(act == 0){
                        cout << "Choosen action: LEFT";
                    }else if(act == 1){
                        cout << "Choosen action: DOWN";
                    }else if(act == 2){
                        cout << "Choosen action: RIGHT";
                    }else{
                        cout << "Choosen action: UP";
                    }
                    cout << " with value " << val << endl;
                    */
                    min = val;
                    values[state] = min;
                    actions[state] = act;
                
                }
            }
            /*
            //now take the max
            int min = 0;
            for(int j = 0; j < valuesTemp.size(); j++){
                if(valuesTemp[j] > min){
                    min = valuesTemp[j];
                }
            }
            */
            //now assing the max
            //    LEFT, DOWN, RIGHT, UP
            //cout << min << endl;
            //cout << act << endl;
            
            
            
            //cout << endl;
        }
        counter++;
    }
    /*
    set<GameState> allPossibleStates3 = m_mdp.getStates();
    set<GameState>::iterator it3 = allPossibleStates3.begin();
    for(int i = 0; it3 != allPossibleStates3.end(); it3++){
        GameState g = *it3;
        double move = getPolicy(g);

        cout << g.getName() << " ";
        if(move == 0){
                    cout << "Choosen action: LEFT";
                }else if(move == 1){
                    cout << "Choosen action: DOWN";
                }else if(move == 2){
                    cout << "Choosen action: RIGHT";
                }else{
                    cout << "Choosen action: UP";
                }
                cout << " " << getValue(g);
                cout << endl;
    }
*/
    /*
       //v(s) = max SIGMA p(s′|s, a)(r(s, a, s′) + γv(s′)),
    GameState state = *it;
    vector<Action> action = m_mdp.getPossibleActions(*it);
    map<GameState, double> prob = m_mdp.getTransitionStatesAndProbs(state, action[1]);
    map<GameState, double>::iterator it2 = prob.begin();
    for(it2 = prob.begin(); it2 != prob.end(); it2++){
        cout << it2->first.getLoc() << " " << it2->second << endl;

    }
    */
}

void ValueIterationAgent::initialize() {
    // TODO. Initialize your data structure.
    srand (time(NULL));
    set<GameState> allPossibleStates = m_mdp.getStates();
    set<GameState>::iterator it;
    for(it = allPossibleStates.begin(); it != allPossibleStates.end();  it++){
        //double randomValue = ((double) rand() / (RAND_MAX)) + 1;
        double randomValue = rand() % 10 + 1;
        GameState state = *it;
        values[state] = 0;
    }

}




