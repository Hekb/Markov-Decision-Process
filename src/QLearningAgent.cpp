//
// Created by Chi Zhang on 8/27/19.
//

#include "common.hpp"
#include "third_party/random.hpp"
#include "QLearningAgent.hpp"
#include <iostream>
#include <fstream>

using namespace std;

QLearningAgent::QLearningAgent(FrozenLakeEnv &env, double gamma, int iterations, double alpha, double epsilon) :
        ValueEstimateAgent(gamma, iterations, 0.0), m_alpha(alpha), m_epsilon(epsilon), m_env(env) {
    MSG("Training Q Learning Agent on " << m_env.getName());
    MSG("Initializing Q Learning Agent");
    initialize();
    MSG("Solving...");
    solve();
}

double QLearningAgent::getValue(const GameState &state) {
    // TODO
    //Return the max
    /*
    vector<pair<Action, double>> data = qValues[state];
    double max = 0;
    Action i;
    for(int i = 0; i < data.size(); i++){
        if(data[i].second > max){
            max = data[i].second;
            i = data[i].first;
        }
    }
    */
    return 0.0;
}

/*
double max;
    for(int action = 0; action < qValues[state].size(); action++){
        if(qValues[state][action] > max){
            max = qValues[state][action];
        }
    }
    return max;

*/

double QLearningAgent::getQValue(const GameState &state, const Action &action) {
    // TODO
    //q(s,a)+βN(s,a)−1
    //x-n = 1 / xn
    if(nValues[state][action] == 0){
        return 1;
    }
    double q = qValues[state][action] + ( 500 * ( 1 / (pow(nValues[state][action], 0.5)) ) );
    return q;
}
//./frozen_lake --agent q --map ../maps/bridge.json --epsilon 0.99 --alpha 1.0 --iteration 500 --gamma 0.99
// The final policy without exploration. Used for evaluation.
//return random possilbe action
Action QLearningAgent::getPolicy(const GameState &state) {
    // TODO
    if ( m_env.isTerminal(state)) {
        return LEFT;
    }
    double max = INT_MIN;
    int index;
    vector<Action> allPossible = m_env.getPossibleActions(state);
    for(int action = 0; action < allPossible.size(); action++){
        double q = qValues[state][allPossible[action]];
        if(q > max){
            max = q;
            index = action;
        }
    }
    
    return allPossible[index];
}

// you should use getAction in solve instead of getPolicy and implement your exploration strategy here.
Action QLearningAgent::getAction(const GameState &state) {
    // TODO
    // less than ep then random else
    // smaller range random
    // larger range optimal
    
    //genrate a random val
    /*
    random_device d;
    default_random_engine generator{d()};
    uniform_real_distribution<double> distribution(0.0,1.0);
    double number = distribution(generator);
    
    //if  < e then select a random
    Action a;
    vector<Action> allPossible = m_env.getPossibleActions(state);
    */
    /*
    for(int k = 0; k < allPossible.size(); k++){
        if(m_env.getNextState(state, allPossible[k]).getName() == "N"){
            allPossible.erase(allPossible.begin() + k);
        }
    }
    */
    /*
    if(number < m_epsilon){
        //generate a random num
        random_device d;
        default_random_engine generator2{d()};
        uniform_int_distribution<int> distribution(0,allPossible.size() - 1);
        int number2 = distribution(generator2);
        a = allPossible[number2];
    }else{ //else select the optimal
        //argMax( a executable in S) q(S,A)
        a = getPolicy(state);
    }
    //return the choice
    */

    //Counting based
    Action a;
    vector<Action> allPossible = m_env.getPossibleActions(state);
    int min = INT_MIN;
    
    for(int i = 0; i < allPossible.size(); i++){
        double q = getQValue(state, allPossible[i]);
        if(q > min){
            min = q;
            a = allPossible[i];
        }
    }
    return a;
}
///frozen_lake --agent q --map ../maps/bridge.json --epsilon 0.99 --alpha 1.0 --iteration 500 --gamma 0.99
void QLearningAgent::update(const GameState &state, const Action &action, const GameState &nextState, double reward) {
    // TODO
    //max q is the next possible action q
    //q(s,a) = q(s,a) + alpha*(reward + epsilon*max(a/ ex in s') - q(s,a));
        
        double qsa = qValues[state][action];
        double qsa2 = qValues[nextState][getPolicy(nextState)];

        qsa = qsa + m_alpha*(reward + m_gamma*qsa2 - qsa);


        //qsa = qsa + m_alpha*(reward + (m_gamma*qsa2) - qsa);
        //qsa = (1 - m_alpha)*qsa + m_alpha*(reward + m_gamma*qsa2);
        

        qValues[state][action] = qsa;
        nValues[state][action] += 1;

    /*
    vector<pair<Action, double>> qsa = qValues[state];
    for(int i = 0; i < qsa.size(); i++){
        pair<Action, double> temp = qsa[i];
        //get the max
        vector<pair<Action,double>> qsa2 = qValues[nextState];
        double max = -555555;
        for(int j = 0; j < qsa2.size(); j++){
            pair<Action, double> temp2 = qsa2[j];
            if(temp2.second > max){
                max = temp2.second;
            }
        }
        temp.second = temp.second + m_alpha*(reward + (m_gamma*max) - temp.second);
        qsa[i] = temp;
    }
    qValues[state] = qsa;
    */
    return;
}

void QLearningAgent::solve() {
    // output a file for plotting
    std::ofstream outFile;
    outFile.open("result.csv");
    outFile << "Episode,Reward" << std::endl;

    int maxEpisodeSteps = 100;

    Action temp1;
    GameState temp2;
    bool cond = false;
    // collect m_iterations trajectories for update
    for (int i = 0; i < m_iterations; i++) {
        int numSteps = 0;
        GameState state = m_env.reset();
        while (!m_env.isTerminal(state)) {
            Action action = getAction(state);
            GameState nextState = m_env.getNextState(state, action);

            
           
            if ( qValues.find(nextState) == qValues.end() ) {
                vector<Action> allPossible = m_env.getPossibleActions(nextState);
                for(int i = 0; i < allPossible.size(); i++){
                    qValues[nextState][allPossible[i]] = 0;
                    nValues[nextState][allPossible[i]] = 1;
                }
            }

            double reward = m_env.getReward(state, action, nextState);
            /*
            cout << "Current state: " << state << " next state: " << nextState
            << " action: " << action << " reward: " << reward << endl;
            */
            update(state, action, nextState, reward);
            state = nextState;
            numSteps += 1;
            if (numSteps >= maxEpisodeSteps) break;  // avoid infinite loop in some cases.
        }

        // evaluate for 100 episodes using the current optimal policy. You can't change this line.
        double episodeReward = m_env.runGame(*this, 100, m_gamma, false).first;
        std::cout << "Evaluating episode reward at learning iteration " << i << " is " << episodeReward << std::endl;
        outFile << i << "," << episodeReward << std::endl;
    }
    outFile.close();
}

void QLearningAgent::initialize() {
    // TODO
    GameState state = m_env.reset();
    vector<Action> allPossible = m_env.getPossibleActions(state);
    for(int i = 0; i < allPossible.size(); i++){
        qValues[state][allPossible[i]] = 0;
        nValues[state][allPossible[i]] = 1;
    }
    return;
}
