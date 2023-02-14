
#ifndef NFA_H
#define NFA_H
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <string>
#include <tuple>
#include <queue>
#include <algorithm>
#include "namespace_std.h"

#include "Common.h"

#include "DFA.h"

// typedef std::string T; // type for values in Sigma
// typedef std::string V; // type for values in Q

class InvalidNFAConstruction
{};

class BAD_CONVERSION_TO_DFA
{};

class BAD_NFA_TRANSITION
{};

template < typename T, typename V >
class NFA
{
public:
    // typedef std::pair< V, T > V_T;//type for values in domain of delta
    // typedef std::unordered_map< V_T, V > D_t; // type for delta
    typedef std::unordered_map < std::pair < std::unordered_set < V >, std::unordered_set< T > >, std::unordered_set < std::unordered_set < V > > > D_t;

    NFA(std::unordered_set< std::unordered_set< T > > S, std::unordered_set< std::unordered_set< V > > Q, std::unordered_set < std::unordered_set< V > > q0,
        std::unordered_set< std::unordered_set< V > > F,
        D_t delta)
        : S_(S), Q_(Q), q0_(q0), F_(F), delta_(delta)
    {
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        if (S_.find(epsilon) == S_.end())
        {
            S_.insert(epsilon);
        }
        if (!is_NFA())
        {
            throw InvalidNFAConstruction();
        }
        current_state_ =  *(q0_.begin());
    }
    
    NFA(const NFA< T, V > & N)
        : S_(N.get_sigma()),  Q_(N.get_states()), q0_({N.get_initial_state()}), F_(N.get_final_states()),
          delta_(N.get_delta())
    {}

    ~NFA()
    {}

    NFA< T, V > & operator=(const NFA< T, V > & N)
    {
        S_ = N.get_sigma();
        Q_ = N.get_states();
        q0_ = {N.get_initial_state()};
        F_ = N.get_final_states();
        delta_ = N.get_delta();
        return *this;
    }
    
    std::unordered_set< std::unordered_set< V > > get_states() const
    {
        return Q_;
    }

    std::unordered_set< std::unordered_set< V > > get_final_states() const
    {
        return F_;
    }

    std::unordered_set< V > get_initial_state() const
    {
        return (*q0_.begin());
    }

    std::unordered_set< std::unordered_set< T > > get_sigma() const
    {
        return S_;
    }

    D_t get_delta() const
    {
        return delta_;
    }

    bool is_NFA() const
    {
        // std::cout << "HERE 1\n";
        // this function will be called in our constructor. It
        // validates the constructed NFA
        for (auto state : F_)
        {
            if (Q_.find(state) == Q_.end())
            {
                //std::cout << "BAD 1\n";
                return false;
            }
        }
        
        if (delta_.size() < Q_.size() - 1)
        {
            //std::cout << "BAD 2\n";
            return false;
        }
        // now we need to make sure each state has atleast one transition to it
        // from another state
        std::unordered_map< std::unordered_set< V >, int > state_transitions;
        // std::cout << "HERE 2\n";
        for (std::unordered_set< V > state : Q_)
        {
            // we get our states and put them into our bag with init count of 0 except our
            // initial state, which starts at 1
            if (state != (*q0_.begin()))
            {
                state_transitions[state] = 0;
            }
            else
            {
                state_transitions[state] = 1;
            }
        }
        state_transitions[(*q0_.begin())] = 1;
        // we need to now go through our bag and add transitions to each state as they appear in our delta
        // we need to make a copy of delta_ though to keep the function const
        D_t c_delta = delta_;
        // std::cout << "HERE 3\n";
        for (auto transition : c_delta)
        {
            // we get the state that is transitioned to and then we add one to its count
            // but make sure that the state is not transitioning to itself
            std::pair< std::unordered_set< V >, std::unordered_set< T > > temp = transition.first;
            std::unordered_set< std::unordered_set< V > > states = transition.second;
            for (std::unordered_set< V > state : states)
            {
                if (temp.first != state)
                {
                    int new_count = state_transitions[state] + 1;
                    state_transitions[state] = new_count;
                }
            }
        }

        // now we make sure that every state is visited atleast once
        for (auto state_visits : state_transitions)
        {
            //std::cout << "T:" << to_string(state_visits) << '\n';
            if (state_visits.second <= 0)
            {
                // std::cout << to_string(state_visits.first) << '\n';
                // std::cout << "BAD 3\n";
                return false;
            }
        }
        return true;
    }
    
    void epsilon_closure(std::unordered_map < std::unordered_set< V >, std::unordered_set< V > > & ep_closures,
                         D_t c_delta) const
    {
        T epsilon;
        std::pair< std::unordered_set< V >, std::unordered_set< T > > p;
        p.second = {epsilon};
        std::queue< std::unordered_set< V > > TODO;
        std::unordered_set< V > combined_states;

        for (std::unordered_set< V > state : Q_)
        {
            TODO.push(state);
        }
        // for every state we have, we need the epsilon closure of it
        while (TODO.size() > 0)
        {
            combined_states = {};
            p.first = TODO.front();
            TODO.pop();
           
            auto transitions = c_delta[p];
            for (auto set : transitions)
            {
                combined_states.insert(set.begin(), set.end());
            }
            ep_closures[p.first] = combined_states;
        }
        for (std::unordered_set< V > state : Q_)
        {
            TODO.push(state);
        }
        while (TODO.size() > 0)
        {
            combined_states = {};
            std::unordered_set< std::unordered_set< V > > found_states;
            std::queue< std::unordered_set< V > > must_complete;
            found_states.insert(TODO.front());
            p.first = TODO.front();

            for (std::unordered_set< V > transitions : c_delta[p])
            {
                // transitions = {{state}, {state}}
                for (V state : transitions)
                {
                    combined_states.insert(state);
                    if (found_states.find({state}) == found_states.end())
                    {
                        must_complete.push({state});
                    }
                }
            }
            // this loop is for all of the states that we haven't explored the closure of
            // our must_complete is a queue of all states we must examine. We the get examined,
            // the states will be moved to found_states
            while (must_complete.size() > 0)
            {
                p.first = must_complete.front();
                found_states.insert(p.first);
                must_complete.pop();
                for (std::unordered_set< V > transitions : c_delta[p])
                {
                    // transitions = {{state}, {state}}
                    for (V state : transitions)
                    {
                        combined_states.insert(state);
                        if (found_states.find({state}) == found_states.end())
                        {
                            must_complete.push({state});
                        }
                    }
                }
            }
            p.first = TODO.front();
            TODO.pop();
            ep_closures[p.first] = combined_states;
        }

    }
    
    // we are going to make a typedef here to make things a lot easier to type. This type represents the
    // delta that we build in this function for our DFA conversion
    
   void get_DFA_states(std::queue< std::unordered_set < V > > & queue,
                       std::vector < std::unordered_set < V > > & Q,
                       D_t c_delta,
                       std::unordered_map < std::unordered_set< V >, std::unordered_set< V > > ep_closures) const
    {
        // std::cout << "Q:" << to_string(Q) << '\n';
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        // the front of our queue is a collection of states from our NFA.
        // we need to examine these states now and record their transitions for each
        // symbol in our Sigma.
        while (queue.size() != 0)
        {
            //std::cout << "STATES:" << to_string(Q) << '\n';
            //std::cout << "Next State\n";
            std::unordered_set< V > qfront = queue.front();
            // std::cout << "FRONT:" << to_string(qfront) << '\n';
            queue.pop();

            for (std::unordered_set < T > symbol : S_)
            {

                std::unordered_set< std::unordered_set < V > > state_transitions;
                if (symbol != epsilon)
                {
                    // std::cout << "SYMBOL:" << to_string(symbol) << '\n';
                    std::pair< std::unordered_set< V >, std::unordered_set< T > > transition;
                    transition.second = symbol;
                    for (V state : qfront)
                    {
                        transition.first = {state};
                        state_transitions.insert(c_delta[transition].begin(), c_delta[transition].end());
                    }

                    std::unordered_set< V > new_state;
                    for (std::unordered_set< V > state : state_transitions)
                    {
                        new_state.insert(state.begin(), state.end());
                        new_state.insert(ep_closures[state].begin(), ep_closures[state].end());
                    }
                    if (!state_exists(new_state, Q) and new_state.size() >= 1)
                    {
                        Q.push_back(new_state);
                        queue.push(new_state);
                    }
                }
            }
        }
    }
    
    bool state_exists(std::unordered_set< V > new_state,
                      std::vector < std::unordered_set < V > > & Q) const
    {
        bool exists = false;
        for (std::unordered_set< V > set : Q)
        {
            if (new_state.size() < set.size() || new_state.size()  > set.size())
            {
                continue;
            }
            if (to_string(new_state, less).compare(to_string(set, less)) == 0)
            {
                // std::cout << "N:" << to_string(new_state, less) << '\n';
                // std::cout << "S:" << to_string(set, less) << '\n';
                return true;
            }
            
        }
        // std::cout << "N:" << to_string(new_state, less) << '\n';
        return false;
    }
    
    void create_delta(std::vector < std::unordered_set < V > > & Q,
                      std::unordered_set < std::unordered_set < T > > & S,
                      D_t & new_delta,
                      std::unordered_map < std::unordered_set< V >, std::unordered_set< V > > & ep_closures) const
    {
        D_t c_delta = delta_;
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        for (std::unordered_set< T > symbol : S)
        {
            if (symbol != epsilon)
            {
                // std::cout << "SYMBOL:" << to_string(symbol) << '\n';
                std::pair< std::unordered_set< V >, std::unordered_set< T > > p;
                p.second = symbol;
                for (std::unordered_set< V > state : Q)
                {
                    // std::cout << "LOOKING AT STATE:" << to_string(state) << '\n';
                    std::unordered_set< V > transition;
                    for (V v : state)
                    {
                        // std::cout << "STATE:" << to_string(v) << '\n';
                        p.first = {v};
                        // std::cout << "C DELTA WITH p:" << to_string(c_delta[p]) <<'\n';
                        for (auto set : c_delta[p])
                        {
                            transition.insert(set.begin(), set.end());
                            transition.insert(ep_closures[set].begin(), ep_closures[set].end());
                        }
                        transition.insert(ep_closures[{v}].begin(), ep_closures[{v}].end());
                    }
                    // std::cout << "TRANSITIONS:" << to_string(transition) << '\n';
                    p.first = state;
                    new_delta[p] = {transition};
            
                }
            }
        }
    }

    void create_finish_states(std::vector < std::unordered_set < V > > & Q,
                              std::unordered_set < std::unordered_set < V > > & F)const
    {
        for (std::unordered_set< V > state : Q)
        {
            for (V s : state)
            {
                if (F_.find({s}) != F_.end())
                {
                    F.insert(state);
                }
            }
        }

    }

    DFA< T, V > to_DFA() const // DFA< T, D_t > 
    {
        // to create our DFA we need to make all the parts
        // first we get our Sigma, which is the same as the NFA except no empty string
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        std::unordered_set< std::unordered_set < T > > S; // Sigma for DFA
        for (std::unordered_set< T > symbol : S_)
        {
            if (symbol != epsilon)
            {
                S.insert(symbol);
            }
        }
        // we have removed the empty set from our Sigma, now we create our individual states
        // we first have to create our initial state, then work off of that
        // take q0_ and every epsilon from init state to wherever there are no more
        
        std::vector < std::unordered_set < V > > Q; // set of states for DFA
        std::unordered_set< std::unordered_set < V > > F;
        std::unordered_set< std::unordered_set < V > > state_bag; // for each state we look at, we will put possible states here
        D_t c_delta = delta_; // copy of our delta
        std::unordered_map < std::unordered_set< V >, std::unordered_set< V > > ep_closures;
        epsilon_closure(ep_closures, c_delta);
        // std::cout << "CLOSURES:" << to_string(ep_closures) << '\n';
        // ep_closures will hold all of the epsilon closures of our states
        std::queue< std::unordered_set< V > > new_states;
        std::unordered_set< V > temp = (*q0_.begin());
        temp.insert(ep_closures[(*q0_.begin())].begin(), ep_closures[(*q0_.begin())].end());
        // // create our initial state
        std::unordered_set< std::unordered_set < V > > init_state; // get initial state
        init_state.insert(temp);
        new_states.push(*init_state.begin());
        get_DFA_states(new_states, Q, c_delta, ep_closures);
        Q.push_back(*init_state.begin());
        D_t new_delta;

        create_delta(Q, S, new_delta, ep_closures);
        create_finish_states(Q, F);
        std::unordered_set< std::unordered_set< V > > new_Q;
        new_Q.insert(Q.begin(), Q.end());
        // std::cout << "S:" << to_string(S) << '\n';
        // std::cout << "Q:" << to_string(new_Q) << '\n';
        // std::cout << "F:" << to_string(F) << '\n';
        // std::cout << "INIT:" << to_string(init_state) << '\n';
        // std::cout << "DELTA:" << to_string(new_delta) << '\n';
        try
        {
            return DFA<T, V> (S, new_Q, init_state, F, new_delta);
        }
        catch (InvalidDFAConstruction & e)
        {
            std::cout << "ERROR: BAD_CONVERSION_TO_DFA\n";
            throw BAD_CONVERSION_TO_DFA();
        }
    }

    void combine_sigma(std::unordered_set< std::unordered_set < T > > & N_S, NFA< T, V > & N) const
    {
        // Since the NFAs are of the same type, we just combine the Sigma's
        for (std::unordered_set< T > symbol : S_)
        {
            N_S.insert(symbol);
        }
        for (std::unordered_set< T > symbol : N.get_sigma())
        {
            N_S.insert(symbol);
        }
        //std::cout << "SIGMA:" << to_string(N_S) << '\n';
    }
    

    void combine_states(std::unordered_set< std::unordered_set < V > > & N_Q,
                        std::unordered_set< std::unordered_set < V > > & N_F,
                        const NFA<T, V > & N) const
    {
        // This function is simple. Run over Q_ and N.Q_ and get all the states. If we find final states in N.Q_
        // we add them to our new final states.
        for (std::unordered_set< V > state : Q_)
        {
            N_Q.insert(state);
            if (F_.find(state) != F_.end())
            {
                N_F.insert(state);
            }
        }
        for (std::unordered_set< V > state : N.get_states())
        {
            N_Q.insert(state);
            if (N.get_final_states().find(state) != N.get_final_states().end())
            {
                N_F.insert(state);
            }
        }
    }

    void combine_delta( D_t & N_delta, std::unordered_set< std::unordered_set < V > > & N_Q,
                        std::unordered_set< std::unordered_set < V > > & N_F,
                        const NFA<T, V > & N) const
    {
        D_t c_delta = delta_; // create copy of our delta
        D_t cN_delta = N.get_delta(); // create a copy of N's delta
        // Now we copy everything over and we also add transitions for our final states from N1 to the initial state
        // from N2
        for (auto pair : c_delta)
        {
            N_delta[pair.first] = pair.second;
        }
        for (auto pair : cN_delta)
        {
            N_delta[pair.first] = pair.second;
        }
        std::pair< std::unordered_set< V >, std::unordered_set< T > > p;
        T t;
        p.second = {t};
        for (std::unordered_set< V > state : F_)
        {
            p.first = state;
            N_delta[p] = {N.get_initial_state()};
        }
    }

    NFA<T, V> make_union(NFA<T, V> & N, std::unordered_set< V > new_q0) const
    {
        // THIS FUNCTION IS OUR UNION FUNCTION

        // WARNING #1: THIS FUNCTION DEPENDS ON THE new_q0 SENT IN BY THE USER. THIS MEANS
        // IT IS UPON THE USER TO GIVE A STATE THAT MAKES SENSE WITH THE ORDERING FUNCTION.
        
        // WARNING #2: THIS FUNCTION ONLY CONCAT'S NFAs OF THE SAME TYPE. DO NOT SEND IN AN NFA OF A DIFFERENT TYPE
        // OTHERWISE YOU WILL GET AN ERROR.

        // WARNING #3: THIS FUNCTION EXPECTS YOU TO GIVE IT 2 NFAs THAT HAVE STATES OF DIFFERENT NAMES. IF SOME NFAs
        // SHARE SIMILAR NAMES THE THIS FUNCTION WILL NOT RUN AS EXPECTED. THE REASON FOR THIS IS BECAUSE
        // IF IT WERE TO WORK RENAME YOUR STATES, THE TYPE WOULD CHANGE AND WOULD MAKE IT HARDER ON THE USER TO USE
        // THE CREATED NFA IN THE FUTURE. IF YOU HAVE A TYPE ERROR THE *this WILL BE RETURNED

        try
        {
            // First, we create all of the parts we will need for this function
            std::unordered_set< std::unordered_set < V > > N_Q; // New States
            std::unordered_set< std::unordered_set < V > > N_F; // New Final States
            std::unordered_set< std::unordered_set < T > > N_S; // New Sigma
            std::unordered_set< std::unordered_set < V > > q0 = {new_q0};  // New Initial State
            D_t N_delta;                                        // Our New Delta
            // Next, combine our Sigmas
            combine_sigma(N_S, N);
            // we have combined the Sigma's, now we do the same for the states.
            combine_states(N_Q, N_F, N);
            N_Q.insert(new_q0);
            // Finally combine our deltas
            combine_delta(N_delta, N_Q, N_F, N);
            // now we just give our new initial state epsilon traversals to our previous initial states
            std::unordered_set< T > epsilon;
            std::pair< std::unordered_set< V >, std::unordered_set< T > > p;
            p.second = epsilon;
            p.first = new_q0;
            N_delta[p] = {get_initial_state(), N.get_initial_state()};
            // std::cout << "SIGMA:" << to_string(N_S) << '\n';
            // std::cout << "NEW Q:" << to_string(N_Q) << '\n';
            // std::cout << "NEW F:" << to_string(N_F) << '\n';
            // std::cout << "NEW q0:" << to_string(q0) << '\n';
            // std::cout << "NEW DELTA:" << to_string(N_delta) << '\n';
            return NFA< T, V > (N_S, N_Q, q0, N_F, N_delta);
        }
        catch(const std::exception & e)
        {
            std::cout << "ERROR: NFA TYPE MISMATCH.\n";
        }
        return *this;
    }

    NFA<T, V> concat(NFA<T, V> & N) const
    {
        // WARNING #1: THIS FUNCTION ONLY CONCAT'S NFAs OF THE SAME TYPE. DO NOT SEND IN AN NFA OF A DIFFERENT TYPE
        // OTHERWISE YOU WILL GET AN ERROR.

        // WARNING #3: THIS FUNCTION EXPECTS YOU TO GIVE IT 2 NFAs THAT HAVE STATES OF DIFFERENT NAMES. IF SOME NFAs
        // SHARE SIMILAR NAMES THE THIS FUNCTION WILL NOT RUN AS EXPECTED. THE REASON FOR THIS IS BECAUSE
        // IF IT WERE TO WORK RENAME YOUR STATES, THE TYPE WOULD CHANGE AND WOULD MAKE IT HARDER ON THE USER TO USE
        // THE CREATED NFA IN THE FUTURE. IF YOU HAVE A TYPE ERROR THE *this WILL BE RETURNED
        try
        {
            // First, we create all of the parts we will need for this function
            std::unordered_set< std::unordered_set < V > > N_Q; // New States
            std::unordered_set< std::unordered_set < V > > N_F; // New Final States
            std::unordered_set< std::unordered_set < T > > N_S; // New Sigma
            std::unordered_set< std::unordered_set < V > > q0 = q0_;  // New Initial State
            D_t N_delta;                                        // Our New Delta
            // Next, combine our Sigmas
            combine_sigma(N_S, N);
            // we have combined the Sigma's, now we do the same for the states.
            combine_states(N_Q, N_F, N);
            // well now we have added our N1 final states, which we don't want, so we remove them.
            for (auto state : F_)
            {
                if (N_F.find(state) != N_F.end())
                    N_F.erase(state);
            }
            combine_delta(N_delta, N_Q, N_F, N);
            // std::cout << "SIGMA:" << to_string(N_S) << '\n';
            // std::cout << "NEW Q:" << to_string(N_Q) << '\n';
            // std::cout << "NEW F:" << to_string(N_F) << '\n';
            // std::cout << "NEW q0:" << to_string(q0) << '\n';
            // std::cout << "NEW DELTA:" << to_string(N_delta) << '\n';
            return NFA< T, V > (N_S, N_Q, q0, N_F, N_delta);
        }
        catch(const std::exception & e)
        {
            std::cout << "ERROR: NFA TYPE MISMATCH.\n";
        }
        return *this;
    }
    
    NFA<T, V> kleen_star(const std::unordered_set< V > & new_q0) const
    {
        // WARNING #1: THIS FUNCTION DEPENDS ON THE new_q0 SENT IN BY THE USER. THIS MEANS
        // IT IS UPON THE USER TO GIVE A STATE THAT MAKES SENSE WITH THE ORDERING FUNCTION.
        
        // make an epsilon traversal from every final state to the initial state
        D_t c_delta = delta_;
        T e;
        std::unordered_set< T > empty_set;
        empty_set.insert(e);
        std::pair< std::unordered_set< V >, std::unordered_set< T > > p;
        p.second = empty_set;
        for (std::unordered_set< V > fstate : F_)
        {
            p.first = fstate;
            auto m = c_delta[p];
            m.insert(get_initial_state());
            c_delta[p] = m;
        }
        // now create transitions for our new_q0
        p.first = new_q0;
        c_delta[p] = q0_;
        // finally add the new q0 to our new_Q and new_F and then create our NFA
        std::unordered_set < std::unordered_set< V > > new_F = F_;
        std::unordered_set< std::unordered_set< V > > new_Q = Q_;
        new_F.insert(new_q0);
        new_Q.insert(new_q0);
        return NFA<T, V> (S_, new_Q, {new_q0}, new_F, c_delta);
    }

    bool operator()(std::vector< std::unordered_set< T > > & word) const
    {
        try
        {
            DFA<T, V> d = this->to_DFA();
            std::cout << "in try....\n";
            std::cout << d << "\n\n";
            return d(word);
        }
        catch (InvalidDFAConstruction & e)
        {
            throw BAD_CONVERSION_TO_DFA();
        }
    }

    struct LESS
    {
        bool operator()(const std::string & lhs, const std::string & rhs)const
        {
            bool found_lhs = false, found_rhs = false;
            int i = 0, j = 0;
            while (true)
            {
                if (isdigit(lhs[i]))
                {
                    found_lhs = true;
                }
                if (isdigit(lhs[j]))
                {
                    found_rhs = true;
                }
                if (found_rhs && found_lhs) break;
                else
                {
                    (found_lhs) ? : ++i;
                    (found_rhs) ? : ++j;
                }
            }
            // std::cout << lhs.substr(i, i2) << ' ' << rhs.substr(j, j2) << '\n';
            return (stod(lhs.substr(i)) < stod(rhs.substr(j)));
        }
    };
    
    LESS less;
private:
    std::unordered_set < std::unordered_set< T > > S_;
    std::unordered_set < std::unordered_set< V > > Q_;
    std::unordered_set < std::unordered_set< V > > F_;
    std::unordered_set < std::unordered_set< V > > q0_;
    D_t delta_;
    std::unordered_set< V > current_state_;
    
    
};

template< typename T, typename V >
std::ostream & operator<<(std::ostream & cout, const NFA< T, V > & N)
{
    cout << "NFA N" << '\n';
    std::cout << "The Sigma of NFA N is: "
              << std::to_string(N.get_sigma()) << '\n';
    std::cout << "The States of NFA N are: "
              << std::to_string(N.get_states(), N.less) << '\n';
    std::cout << "The Initial State of NFA N is: "
              << std::to_string(N.get_initial_state()) << '\n';
    std::cout << "The Final States of NFA N are: "
              << std::to_string(N.get_final_states()) << '\n';
    std::cout << "The Delta of NFA N is: "
              << std::to_string(N.get_delta());
    
    return cout;
}

#endif
