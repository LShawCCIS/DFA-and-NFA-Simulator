#ifndef DFA_H
#define DFA_H
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <string>
#include <tuple>
#include <cctype>
#include "namespace_std.h"
#include "Common.h"
#include "NFA.h"

class InvalidDFAConstruction
{};

class DFA_EPSILON_IN_SIGMA
{};

template < typename T, typename V >
class DFA
{
public:
    typedef std::unordered_map < std::pair < std::unordered_set < V >, std::unordered_set< T > >, std::unordered_set < std::unordered_set < V > > > D_t;
    
    DFA(std::unordered_set< std::unordered_set< T > > S, std::unordered_set< std::unordered_set< V > > Q,
        std::unordered_set < std::unordered_set< V > > q0,
        std::unordered_set< std::unordered_set< V > > F,
        D_t delta)
        : S_(S), Q_(Q), q0_(q0), F_(F), delta_(delta)
    {
        try
        {
            if (!is_DFA())
            {
                throw InvalidDFAConstruction();
            }
            current_state_ = *(q0_.begin());
        }
        catch (DFA_EPSILON_IN_SIGMA & e)
        {
            std::cout << "ERROR: EPSILON CANNOT BE IN THE DFA SIGMA\n";
        }
    }

    DFA(const DFA< T, V > & M)
        : S_(M.get_sigma()),  Q_(M.get_states()), q0_({M.get_initial_state()}), F_(M.get_final_states()),
          delta_(M.get_delta())
    {}

    ~DFA()
    {}

    std::unordered_set< std::unordered_set< T > > get_sigma() const
    {
        return S_;
    }

    std::unordered_set < std::unordered_set< V > > get_states() const
    {
        return Q_;
    }

    std::unordered_set < std::unordered_set< V > > get_final_states() const
    {
        return F_;
    }

    std::unordered_set < V > get_initial_state() const
    {
        return *(q0_.begin());
    }

    D_t get_delta() const
    {
        return delta_;
    }
    
    DFA< T, V > & operator=(const DFA< T, V > & M)
    {
        S_ = M.get_sigma();
        Q_ = M.get_states();
        q0_ = {M.get_initial_state()};
        F_ = M.get_final_states();
        delta_ = M.get_delta();
        return *this;
    }
    
    std::vector< std::unordered_set < V > >  generate_state_list()const
    {
        std::vector< std::unordered_set < V > > state_list;
        bool inserted;
        for (auto state : Q_)
        {
            if (state_list.size() == 0)
            {
                state_list.push_back(state);
            }
            else
            {
                inserted = false;
                for (int i = 0; i < state_list.size(); ++i)
                {
                    if (less(to_string(state, less), to_string(state_list[i], less)))
                    {
                        state_list.insert(state_list.begin() + i, state);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted)
                    state_list.push_back(state);
            }
            
        }
        return state_list;
    }

    std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > generate_ptable
    (std::vector< std::unordered_set < V > > state_list) const
    {
        std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > pair_table;
        std::pair<std::unordered_set < V >, std::unordered_set < V >> state_pairs; // we use this to make our pairings
        bool found_state = false, found_state2 = false;
        for (std::unordered_set < V > state : state_list)
        {
            found_state = false;
            if (F_.find(state) != F_.end())
                found_state = true;
            for (std::unordered_set < V > s : state_list)
            {
                found_state2 = false;
                if (state == s)
                    break;
                else
                {
                    // we "mark" pairs that have one state in F_ and one
                    // state not in F_
                    if (F_.find(s) != F_.end())
                        found_state2 = true;
                    if ((found_state && found_state2) || (!found_state && !found_state2))
                    {
                        state_pairs = std::make_pair(std::unordered_set < V >(s), std::unordered_set < V >(state));
                        pair_table.push_back(state_pairs);
                    }
                }
            }
        }
        return pair_table;
    }

    void get_unmarked_states(std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > & pair_table)
    {
        if (pair_table.size() == 0)
            return;
        bool marked = true;
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (int i = 0; i < pair_table.size(); ++i)
            {
                // for each pair we look at first and second delta's for a given symbol in our Sigma
                for (std::unordered_set< T > symbol : S_)
                {
                    marked = true;
                    std::unordered_set< std::unordered_set < V > > visited_state = delta_[{pair_table[i].first, symbol}];
                    std::unordered_set< std::unordered_set < V > > visited_state2 = delta_[{pair_table[i].second, symbol}];
                    std::pair< std::unordered_set < V >, std::unordered_set < V > > p;
                    p = std::make_pair(std::unordered_set < V >(*visited_state.begin()),
                                       std::unordered_set < V >(*visited_state2.begin()));
                    if (p.first == p.second)
                    {
                        marked = false;
                        break;
                    }
                    for (std::pair< std::unordered_set < V >, std::unordered_set < V > > compare_p : pair_table)
                    {
                        if (to_string(compare_p, less).compare(to_string(p, less)) == 0)
                        {
                            marked = false;
                            break;
                        }
                    }
                    if (marked)
                    {
                        changed = true;
                        pair_table.erase(pair_table.begin() + i);
                        --i;
                    }
                }
               
            }
        }
    }

    std::vector < std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >, std::unordered_set < V >>> merge_pairs(       std::vector< std::unordered_set < V > > & merged_pairs,
        const std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > pair_table)
    {
        D_t new_delta;
        std::unordered_set < V > merged_pair;
        std::pair< std::unordered_set < V >, T > p;
        std::vector<
                   std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >, std::unordered_set < V > > > pair_key;
        std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >, std::unordered_set < V > > pairs;
        
        for (int i = 0; i < pair_table.size(); ++i)
        {
            std::unordered_set< V > transition;
            merged_pair.insert(pair_table[i].first.begin(), pair_table[i].first.end());
            merged_pair.insert(pair_table[i].second.begin(), pair_table[i].second.end());
            merged_pairs.push_back({merged_pair});
            
            Q_.insert(merged_pair);
            
            if (F_.find(pair_table[i].first) != F_.end() || F_.find(pair_table[i].second) != F_.end())
                F_.insert(merged_pair);
            
            if (q0_.find(pair_table[i].first) != q0_.end() || q0_.find(pair_table[i].second) != q0_.end())
                q0_ = {merged_pair};
            
            pairs.first = pair_table[i];
            pairs.second = {merged_pair};
            pair_key.push_back(pairs);
        }
        return pair_key;
        
    }

    void remove_states(std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > & pair_table)
    {
        for (std::pair< std::unordered_set < V >, std::unordered_set < V > > p : pair_table)
        {
            if (Q_.find(p.first) != Q_.end())
            {
                Q_.erase(p.first);
            }
            if (Q_.find(p.second) != Q_.end())
            {
                Q_.erase(p.second);
            }
            if (F_.find(p.first) != F_.end())
            {
                F_.erase(p.first);
            }
            if (F_.find(p.second) != F_.end())
            {
                F_.erase(p.second);
            }
        }
    }

    std::unordered_set< std::unordered_set < V > > get_correct_state(std::unordered_set< std::unordered_set < V > > s,
                            std::vector< std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >,
                                                                                          std::unordered_set < V > > > & pair_key)
    {
        for (auto p : pair_key)
        {
            for (auto t : s)
            {
                if (p.first.first == t || p.first.second == t)
                {
                    return {p.second};
                }
            }
        }
        return std::unordered_set< std::unordered_set < V >> {};
    }
    
    void get_transitions(D_t & new_delta,
                         std::vector< std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >,
                                                                                       std::unordered_set < V > > > & pair_table,
                         std::vector< std::unordered_set < V > > & merged_pairs)
    {
        for (auto p : delta_)
        {
            if (Q_.find(p.first.first) != Q_.end())
            {
                if (Q_.find((*p.second.begin())) != Q_.end())
                {
                    new_delta[p.first] = p.second;
                }
                else
                {
                    new_delta[p.first] = get_correct_state(p.second, pair_table);
                }
            }
            else
            {
                if (Q_.find((*p.second.begin())) != Q_.end())
                {
                    new_delta[{(*get_correct_state({p.first.first}, pair_table).begin()), p.first.second}] = p.second;
                }
                else
                {
                    new_delta[{(*get_correct_state({p.first.first}, pair_table).begin()), p.first.second}] =
                        get_correct_state(p.second, pair_table);
                }
            }
        }
    }
    
    void minimization()
    {
        // this function will minimize our DFA. To do this first we build a table
        // of all of the pairs of states in the DFA. Half of the pairs will be repeatative
        // so we divide the table in half to speed it up.
        // This first part will create our list of states in order. We use this to generate a table
        std::vector< std::unordered_set < V > > state_list = generate_state_list();

        // Our table needs to be able to be read, so we'll just use a vector
        std::vector< std::pair < std::unordered_set < V >, std::unordered_set < V > > > pair_table = generate_ptable(state_list);
        
        // at this point our pair_table contains all of the unmarked pairs
        // we move onto the next step which involves using our delta. We will
        // check umarked pairs (P,Q) and see on transition for x will give us a
        // state that is marked. If it does we remove the pair from our vector as
        // it is now "marked"

        get_unmarked_states(pair_table);

        // at this stage all that is left is our unmarked sets of states which we can now
        // turn into single states.
        D_t new_delta;
        if (pair_table.size() >= 1)
        {
            // we will now create our new states and add them to our Q_, F_, and initial state
            std::vector< std::unordered_set < V > > merged_pairs;
            std::vector< std::pair< std::pair < std::unordered_set < V >, std::unordered_set < V > >,
                                    std::unordered_set < V > > > pair_key;
            
            pair_key = merge_pairs(merged_pairs, pair_table);
                                        
            // now we have our merged pairs, which means we can remove the old used states
            // from our Q_ and F_
            remove_states(pair_table);
            get_transitions(new_delta, pair_key, merged_pairs);
            delta_ = new_delta;
        }
    }
    
    DFA<T, V> complement() const
    {
        // the complement just means that all accept states are now nonaccept states and
        // all nonaccept states are accept states.
        std::unordered_set < std::unordered_set< V > > new_F;
        for (std::unordered_set < V > state : Q_)
        {
            if (F_.find(state) == F_.end())
            {
                new_F.insert(state);
            }
        }
        return DFA<T, V>(S_, Q_, q0_, new_F, delta_);
    }
    
    DFA<T, V> intersection(DFA<T, V> & M)
    {
        // // the intersection means we are going to have to create a new DFA with
        // // a lot more states. First our Q is now Q1 X Q2. We will also give them names
        // // in this process
        std::unordered_set< std::unordered_set< V > > qproduct;
        std::unordered_set< std::unordered_set< V > > fproduct;
        std::unordered_set< V > initial_state;
        bool same_state;
        for (std::unordered_set < V > state : Q_)
        {
            same_state = false;
            for (std::unordered_set < V > state2 : M.get_states())
            {
                std::unordered_set < V > s;
                s.insert(state.begin(), state.end());
                s.insert(state2.begin(), state.end());
                if (state == get_initial_state() && state2 == M.get_initial_state())
                {
                    initial_state.insert(state.begin(), state.end());
                    initial_state.insert(state2.begin(), state2.end());
                }
                if (F_.find(state) != F_.end() && M.get_final_states().find(state2) != M.get_final_states().end())
                {
                    
                    fproduct.insert(s);
                }
                qproduct.insert(s);
            }
        }

        // we now have our accepting and nonaccepting states named. Now we need to get our delta
        D_t c_delta = delta_; // this is so we don't change our current delta
        D_t new_delta; // new delta for new DFA
        std::unordered_set < std::unordered_set< V > > new_states;
        std::unordered_set < std::unordered_set< V > > new_fstates;
        std::pair< std::unordered_set < V > , std::unordered_set< T > > t_state;
        bool finish_state;
        for (auto s : qproduct)
        {
            finish_state = true;
            std::unordered_set< V > new_state;
            for (std::unordered_set< T > symbol : S_)
            {
                std::unordered_set< V > transitioned_to;
                for (auto m : s)
                {
                    new_state.insert(s.begin(), s.end());
                    t_state = std::make_pair(std::unordered_set< V >({m}), std::unordered_set< T >(symbol));
                    auto t = c_delta[t_state];
                    for (std::unordered_set< V > state : c_delta[t_state])
                    {
                        transitioned_to.insert(state.begin(), state.end());
                    }
                    if (F_.find(s) == F_.end())
                        finish_state = false;
                }
                new_delta[{{new_state}, symbol}] = {transitioned_to};
            }
            new_states.insert(new_state);
            if (finish_state)
                new_fstates.insert(new_state);
        }
        return  DFA<T, V>(S_, new_states, {initial_state}, new_fstates, new_delta);
    }

    bool is_DFA() const
    {
        // this function will be called in our constructor. It
        // validates the constructed DFA
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        for (auto sym : S_)
        {
            if (sym == epsilon)
                throw DFA_EPSILON_IN_SIGMA();
        }
        for (auto state : F_)
        {
            if (Q_.find(state) == Q_.end())
            {
                std::cout << "WRONG 1\n";
                // std::cout << to_string(state) << '\n';
                return false;
            }
        }
        int transitions = Q_.size() * S_.size();
        if (delta_.size() != transitions)
        {
            std::cout << "WRONG 2\n";
            return false;
        }
        for (auto state : Q_)
        {
            for (auto symbol : S_)
            {
                std::unordered_set < V > s = state;
                std::unordered_set< T > s2 = symbol;
                std::pair< std::unordered_set< V >, std::unordered_set< T > > t;
                t.first = s;
                t.second = s2;

                if (delta_.find(t) == delta_.end())
                {
                    std::cout << "WRONG 3\n";
                    return false;
                }
                D_t d = delta_;
                if (d[t].size() == 1)
                {
                    std::cout << "WRONG 4\n";
                    return false;
                }
                std::unordered_set < std::unordered_set< V > > temp = d[t];
                for (std::unordered_set< V > t_state : temp)
                {
                    if (Q_.find(t_state) == Q_.end())
                    {
                        
                        // std::cout << "\n\n" << to_string(t_state) << '\n';
                        // std::cout << to_string(Q_) << '\n';
                        std::cout << "WRONG 5\n";
                        return false;
                    }
                }
            }
        }
        return true;
    }

     NFA<T, V> to_NFA() const
     {
         // turns an nfa from our dfa. just make an nfa out of what we
         // have already. epsilon is automatically thrown into nfa during
         // constructor
         NFA<T, V> nfa = NFA<T, V>(s_, q_, q0_, f_, delta_);
         return nfa;
     }


    bool operator()(std::vector< std::unordered_set< T > > & word)
    {
        typename std::vector< T >::iterator it;
        T e;
        std::unordered_set< T > epsilon;
        epsilon.insert(e);
        //it = word.begin(); it != word.end(); ++it
        if (epsilon == word[0])
        {
            word.erase(word.begin());
        }
        for (std::unordered_set< T > symbol : word)
        {
            // check to make sure symbol is in our Sigma
            if (S_.find(symbol) == S_.end())
            {
                return false;
            }
            // we begin at our initial state, and now we need make sure
            // there exists a transition from intial to another state
            // and also take that transition if it exists
            // We will use our current_state_ attribute
            std::pair< std::unordered_set< V >, std::unordered_set< T > > t;
            t = std::make_pair( std::unordered_set< V >(current_state_), std::unordered_set< T >(symbol));
            // we have our pair of (state, transition_symbol), and we know that it has to exist in
            // our delta because we know the character is in sigma and the checks preformed in our constructor.
            // we take the transition and move states
            current_state_ = (*delta_[t].begin());
            // this gets us to our next state
        }
        // we have processed every char. If we haven't already returned false, we are now either
        // in an accept state or in a non-accept state.
        if (F_.find(current_state_) == F_.end())
        {
            // our current_state_ is not an accept state. We don't accept, and we must reset our current
            // state back to the initial state for processing in the future
            current_state_ = (*q0_.begin());
            return false;
        }
        // our current_state_ is an accept state. We accept, and we must reset our current
        // state back to the initial state for processing in the future
        current_state_ = (*q0_.begin());
        return true;
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
                if (!(found_lhs && found_rhs))
                {++i; ++j;}
                else break;
            }
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
std::ostream & operator<<(std::ostream & cout, const DFA< T, V > & M)
{
    cout << "DFA M" << '\n';
    std::cout << "The Sigma of DFA M is: "
              << std::to_string(M.get_sigma()) << '\n';
    std::cout << "The States of DFA M are: "
              << std::to_string(M.get_states(), M.less) << '\n';
    std::cout << "The Initial State of DFA M is: "
              << std::to_string(M.get_initial_state()) << '\n';
    std::cout << "The Final States of DFA M are: "
              << std::to_string(M.get_final_states()) << '\n';
    std::cout << "The Delta of DFA M is: "
              << std::to_string(M.get_delta());
    
    return cout;
}

#endif
