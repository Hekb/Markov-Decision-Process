//
// Created by Chi Zhang on 8/26/19.
//

#include "PolicyIterationAgent.hpp"
#include "common.hpp"
#include <cmath>

using namespace std;

PolicyIterationAgent::PolicyIterationAgent(const FrozenLakeMDP &mdp, double gamma, int iterations, double threshold) :
        ValueEstimateAgent(gamma, iterations, threshold), m_mdp(mdp) {
    MSG("Training Policy Iteration Agent on " << m_mdp.getName());
    MSG("Initializing Policy Iteration Agent");
    initialize();
    MSG("Solving...");
    solve();
}

double PolicyIterationAgent::getQValue(const GameState &state, const Action &action) {
    // TODO
    return 0.0;
}

double PolicyIterationAgent::getValue(const GameState &state) {
    // TODO
    map<GameState, double> values = evaluateCurrentPolicy();
    return values[state];
}

Action PolicyIterationAgent::getPolicy(const GameState &state) {
    // TODO
    return m_policy[state];
}

/*
 * Evaluate the current policy by returning V(s), which is represented as a map,
 * where key is GameState and value is double.
 */
std::map<GameState, double> PolicyIterationAgent::evaluateCurrentPolicy() {
    // TODO
    //it2->second * ( m_mdp.getReward(state, action[i], it2->first) + m_gamma*values[it2->first]);
    //run m_iteration
    map<GameState, double> values;
    for(int i = 0; i < PolicyIterationAgent::m_iterations; i++){
        set<GameState> allPossibleStates = m_mdp.getStates();
        set<GameState>::iterator it;
        for(it = allPossibleStates.begin(); it != allPossibleStates.end();  it++){
            GameState state = *it;
            if(m_mdp.isTerminal(state)){
                continue;
            }
            Action a = m_policy[state];
            map<GameState, double> prob = m_mdp.getTransitionStatesAndProbs(state, a);
            map<GameState, double>::iterator it2;
            double val = 0;
            for(it2 = prob.begin(); it2 != prob.end(); it2++){
                val += it2->second * ( m_mdp.getReward(state, a, it2->first) + m_gamma*values[it2->first] );
            }
            values[state] = val;
        }
    }
    return values;
}

//Don't include the terminal states in m_policy
void PolicyIterationAgent::solve() {
    for(int i = 0; i < PolicyIterationAgent::m_iterations; i++){
        //now update the m_policy
        set<GameState> allPossibleStates = m_mdp.getStates();
        set<GameState>::iterator it;
        //cout <<"iteration: " << i << endl;
        //cout <<"-------------------" << endl;
        map<GameState, double> before = evaluateCurrentPolicy();
        for(it = allPossibleStates.begin(); it != allPossibleStates.end();  it++){
            GameState state = *it;
            if(m_mdp.isTerminal(state)){
                continue;
            }
            //cout << "State: " << state.getName() << endl;
            vector<Action> possibleActions = m_mdp.getPossibleActions(state);
            double val = -55555;
            Action temp;
            for(int j = 0; j < possibleActions.size(); j++){
                Action a = possibleActions[j];
                m_policy[state] = a;
                map<GameState, double> values = evaluateCurrentPolicy();
                /*
                string move2 = "";
                if(a == LEFT){
                    move2 = "left";
                }else if(a == UP){
                    move2 = "up";
                }else if(a == DOWN){
                    move2 = "down";
                }else{
                    move2 = "right";
                }
                cout << "action " << move2 << " with value: " << values[state] << endl;
                */
                if(values[state] > val){
                    val = values[state];
                    temp = a;
                }
            }
            m_policy[state] = temp;
            /*
            string move = "";
            if(temp == LEFT){
                move = "left";
            }else if(temp == UP){
                move = "up";
            }else if(temp == DOWN){
                move = "down";
            }else{
                move = "right";
            }
            cout << "Policy assinged: " << move << endl;
            cout << endl;
            */
        }
        //check if error value is less if so then terminate
        map<GameState, double> after = evaluateCurrentPolicy();
        set<GameState> allPossibleStates2 = m_mdp.getStates();
        set<GameState>::iterator it2;
        bool condition = true;
        for(it2 = allPossibleStates2.begin(); it2 != allPossibleStates2.end();  it2++){
            GameState state = *it2;
            double val1 = before[state];
            double val2 = after[state];
            double result = abs(val1 - val2);
            if( !(result < PolicyIterationAgent::m_threshold) ){
                condition = false;
                break;
            }
        }
        if(condition){
            break;
        }

    }
    // TODO
}

void PolicyIterationAgent::initialize() {
    // TODO
    //Initialize the data structure with random policies
    set<GameState> allPossibleStates = m_mdp.getStates();
    set<GameState>::iterator it;
    srand (time(NULL));
    for(it = allPossibleStates.begin(); it != allPossibleStates.end();  it++){
        int action = rand() % 3;
        GameState state = *it;
        if(m_mdp.isTerminal(state)){
            continue;
        }
        vector<Action> a = m_mdp.getPossibleActions(state);
        Action ac = a[0];
        m_policy[state] = ac;
    }
}


