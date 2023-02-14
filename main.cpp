#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <string>
#include "DFA.h"
#include "NFA.h"
#include "namespace_std.h"


int main()
{
    try
    {
        // TEST # 1 DFA -- STRING -------------------------------------------------------------------------------
        typedef std::string S_t; // type for values in Sigma
        typedef std::string Q_t; // type for values in Q
        typedef std::pair< std::unordered_set < Q_t >, std::unordered_set< S_t > > Q_t_S_t; // type for values in domain of delta
        typedef std::unordered_map< Q_t_S_t, std::unordered_set < std::unordered_set< Q_t > > > D_t; // type for delta
        S_t a = "a";
        S_t b = "b";
        std::unordered_set < std::unordered_set< S_t > > S {{a}, {b}}; // Sigma
        Q_t q0 = "q0";
        Q_t q1 = "q1";
        Q_t q2 = "q2";
        std::unordered_set< std::unordered_set< S_t > > Q; // Q
        Q.insert({q0});
        Q.insert({q1});
        Q.insert({q2});
        std::unordered_set< std::unordered_set< S_t > > F;
        F.insert({q1});
        F.insert({q2});
        D_t delta; //delta

        delta[{{q0}, {a}}] = {{q1}};
        delta[{{q0}, {b}}] = {{q0}};
        delta[{{q1}, {a}}] = {{q2}};
        delta[{{q1}, {b}}] = {{q2}};
        delta[{{q2}, {a}}] = {{q2}};
        delta[{{q2}, {b}}] = {{q2}};

        DFA< S_t, Q_t > M(S, Q, {{q0}}, F, delta);
        DFA< S_t, Q_t > M2 = M;
        std::cout << M << "\n\n";
        std::cout << M.complement() << "\n\n";
        std::cout << M.intersection(M2) << "\n\n";
        M.minimization();
        std::cout << M << '\n';
       
        M2 = M.complement();
        std::cout << M2 << "\n\n";
        
        std::vector< std::unordered_set < S_t > > word;
        word.push_back({a});
        word.push_back({b});
        std::cout << M(word) << '\n';
        // END TEST # 1 DFA -- STRING ---------------------------------------------------------------------------

        
        // TEST # 2 DFA -- NON-STRING ----------------------------------------------------------------------------
        // typedef std::pair< int, int > S_t; // type for values in Sigma
        // typedef std::unordered_set < double > Q_t; // type for values in Q
        // typedef std::pair< std::unordered_set < Q_t >, std::unordered_set< S_t > > Q_t_S_t; // type for values in domain of delta
        // typedef std::unordered_map< Q_t_S_t, std::unordered_set < std::unordered_set< Q_t > > > D_t; // type for delta
        // S_t a = std::make_pair(int(5), int(7));
        // S_t b = std::make_pair(int(2), int(3));
        // std::unordered_set < std::unordered_set< S_t > > S {{a}, {b}}; // Sigma
        // Q_t q0 = {1.1, 3.3};
        // Q_t q1 = {9.1, 1.2};
        // Q_t q2 = {4.4, 5.6};
        // std::unordered_set < std::unordered_set< Q_t > > Q; // Q
        // Q.insert({q0});
        // Q.insert({q1});
        // Q.insert({q2});
        // std::unordered_set < std::unordered_set< Q_t > > F;
        // F.insert({q1});
        // F.insert({q2});
        // D_t delta; //delta

        // delta[{{q0}, {a}}] = {{q1}};
        // delta[{{q0}, {b}}] = {{q0}};
        // delta[{{q1}, {a}}] = {{q2}};
        // delta[{{q1}, {b}}] = {{q2}};
        // delta[{{q2}, {a}}] = {{q2}};
        // delta[{{q2}, {b}}] = {{q2}};

        // DFA< S_t, Q_t > M(S, Q, {{q0}}, F, delta);
        // DFA< S_t, Q_t > M2 = M;
        
        // std::cout << M << "\n\n";
        // std::cout << M.complement() << "\n\n";
        // std::cout << M.intersection(M2) << "\n\n";
        // M.minimization();
        // std::cout << M << '\n';

        // END TEST # 2 DFA -- NON STRING ----------------------------------------------------------------


        // TEST # 3 NFA -- STRING --------------------------------------------------------------------
        // typedef std::string S_t; //type for values in Sigma
        // typedef std::string Q_t; //type for values in Q
        // typedef std::pair< std::unordered_set < Q_t >, std::unordered_set< S_t > > Q_t_S_t; //type for values in domain of delta
        // typedef std::unordered_map< Q_t_S_t, std::unordered_set < std::unordered_set< Q_t > > > D_t; //type for delta
        // S_t a = "a";
        // S_t b = "b";
        // S_t e;
        // std::unordered_set < std::unordered_set< S_t > > S {{a}, {b}, {e}}; // Sigma
        // Q_t q0 = "q0";
        // Q_t q1 = "q1";
        // Q_t q2 = "q2";
        // Q_t q3 = "q3";
        // Q_t q4 = "q4";
        // Q_t q5 = "q5";
        // std::unordered_set< std::unordered_set< S_t > > Q;  // Q
        // Q.insert({q0});
        // Q.insert({q1});
        // Q.insert({q2});
        // std::unordered_set< std::unordered_set< S_t > > F; // finish states
        // F.insert({q1});
        // F.insert({q2});
        // D_t delta;  // // delta

        // delta[{{q0}, {a}}] = {{q1}};
        // delta[{{q0}, {b}}] = {{q2}};
        // delta[{{q0}, {e}}] = {{q2}};
        // delta[{{q1}, {a}}] = {{q2}};
        // delta[{{q1}, {e}}] = {{q0}};
        // delta[{{q2}, {b}}] = {{q1}};
        // delta[{{q2}, {e}}] = {{q2}};

        // NFA< S_t, Q_t > N(S, Q, {{q0}}, F, delta);
        // NFA< S_t, Q_t > N2 = N;
        // N2 = N;
        // std::cout << N << "\n\n";
        // std::cout << N2 << "\n\n";

        // std::unordered_set< std::unordered_set< S_t > > Q2; // Q
        // Q2.insert({q3});
        // Q2.insert({q4});
        // Q2.insert({q5});
        // std::unordered_set< std::unordered_set< S_t > > F2;
        // F2.insert({q4});
        // F2.insert({q5});
        // D_t delta2;  // delta

        // delta2[{{q3}, {a}}] = {{q4}};
        // delta2[{{q3}, {b}}] = {{q3}};
        // delta2[{{q4}, {a}}] = {{q5}};
        // delta2[{{q4}, {b}}] = {{q5}};
        // delta2[{{q5}, {a}}] = {{q5}};
        // delta2[{{q5}, {b}}] = {{q5}};

        // NFA< S_t, Q_t > N3(S, Q2, {{q3}}, F2, delta2);

        
        // std::cout << N.concat(N3) << "\n\n";
        // std::unordered_set< Q_t > q00 = {"q-1"};
        // std::cout << N.kleen_star(q00) << "\n\n";
        // std::cout << N.make_union(N3, q00) << "\n\n";
        // std::cout << N.to_DFA() << "\n\n";
        // std::cout << N << "\n\n";
        // std::vector< std::unordered_set < S_t > > word;
        // word.push_back({a});
        // word.push_back({b});
        // std::cout << (N(word)) << '\n';
        // END TEST # 3 NFA -- STRING ----------------------------------------------------------------------------


        // TEST # 4 NFA -- NON-STRING ----------------------------------------------------------------------------
        // typedef std::pair< int, int > S_t; // type for values in Sigma
        // typedef std::unordered_set < double > Q_t; // type for values in Q
        // typedef std::pair< std::unordered_set < Q_t >, std::unordered_set< S_t > > Q_t_S_t; // type for values in domain of delta
        // typedef std::unordered_map< Q_t_S_t, std::unordered_set < std::unordered_set< Q_t > > > D_t; // type for delta
        // S_t a = std::make_pair(int(5), int(7));
        // S_t b = std::make_pair(int(2), int(3));
        // S_t e;
        // std::unordered_set < std::unordered_set< S_t > > S {{a}, {b}, {e}}; // Sigma
        // Q_t q0 = {1.1, 3.3};
        // Q_t q1 = {9.1, 1.2};
        // Q_t q2 = {4.4, 5.6};
        // Q_t q3 = {6.4, 7.6};
        // Q_t q4 = {7.4, 8.6};
        // Q_t q5 = {8.4, 9.6};
        // std::unordered_set < std::unordered_set< Q_t > > Q; // Q
        // Q.insert({q0});
        // Q.insert({q1});
        // Q.insert({q2});
        // std::unordered_set < std::unordered_set< Q_t > > F;
        // F.insert({q1});
        // F.insert({q2});
        // D_t delta; //delta

        // delta[{{q0}, {a}}] = {{q1}};
        // delta[{{q0}, {b}}] = {{q2}};
        // delta[{{q0}, {e}}] = {{q2}};
        // delta[{{q1}, {a}}] = {{q2}};
        // delta[{{q1}, {e}}] = {{q0}};
        // delta[{{q2}, {b}}] = {{q1}};
        // delta[{{q2}, {e}}] = {{q2}};

        // NFA< S_t, Q_t > N(S, Q, {{q0}}, F, delta);
        // NFA< S_t, Q_t > N2 = N;
        // N2 = N;
        // std::cout << N << "\n\n";
        // std::cout << N2 << "\n\n";

        // std::unordered_set< std::unordered_set< Q_t > > Q2;
        // Q2.insert({q3});
        // Q2.insert({q4});
        // Q2.insert({q5});
        // std::unordered_set< std::unordered_set< Q_t > > F2;
        // F2.insert({q4});
        // F2.insert({q5});
        // D_t delta2;

        // delta2[{{q3}, {a}}] = {{q4}};
        // delta2[{{q3}, {b}}] = {{q3}};
        // delta2[{{q4}, {a}}] = {{q5}};
        // delta2[{{q4}, {b}}] = {{q5}};
        // delta2[{{q5}, {a}}] = {{q5}};
        // delta2[{{q5}, {b}}] = {{q5}};

        // NFA< S_t, Q_t > N3(S, Q2, {{q3}}, F2, delta2);

        
        // std::cout << N.concat(N3) << "\n\n";
        // std::unordered_set< Q_t > q00 = {{-5, -4}};
        // std::cout << N.kleen_star(q00) << "\n\n";
        // std::cout << N.make_union(N3, q00) << "\n\n";
        // std::cout << N.to_DFA() << "\n\n";

        // std::vector< std::unordered_set < S_t > > word;
        // word.push_back({a});
        // word.push_back({b});
        // std::cout << (N(word)) << '\n';
        
        // END TEST # 4 NFA -- NON-STRING ------------------------------------------------------------------

        // DFA< S_t, Q_t > M(S, Q, {q0}, F, delta);

        // DFA< S_t, Q_t > M2 = M;

        //M2 = M;

        // std::cout << M << "\n\n";
        //std::cout << M.complement() << "\n\n";
        //std::cout << M.intersection(M2) << "\n\n";
        //M.minimization();
        //std::cout << M << '\n';
        //M = M.complement();
        // std::cout << M2 << "\n\n";

        // M2 = M.complement();
        // std::cout << M2 << "\n\n";

        
        // delta[{q2, a}] = {q2};
        // delta[{q2, b}] = {q2};
        // std::vector< S_t > word;
        // word.push_back(std::make_pair(int(5), int(7)));
        // word.push_back(std::make_pair(int(2), int(3)));
        // std::cout << M(word) << '\n';

        //M.minimization();
        //M = M.intersection(M2);

        // D_t delta2;
        // delta2[{q0, a}] = {q0};
        // delta2[{q0, b}] = {q1};
        // delta2[{q1, a}] = {q1};
        // delta2[{q1, b}] = {q2};
        // delta2[{q2, a}] = {q2};
        // delta2[{q2, b}] = {q2};
        // delta[{q3, a}] = {q4};
        // delta[{q3, b}] = {q5};
        // delta[{q4, a}] = {q1};
        // delta[{q4, b}] = {q3};
        // delta[{q5, a}] = {q1};
        // delta[{q5, b}] = {q4};

        // delta[{q0, a}] = {q1};
        // delta[{q0, b}] = {q2};
        // delta[{q1, a}] = {q3};
        // delta[{q1, b}] = {q2};
        // delta[{q2, a}] = {q3};
        // delta[{q2, b}] = {q0};
        // delta[{q3, a}] = {q1};
        // delta[{q3, b}] = {q0};

        // delta[{q0, a}] = {q1};
        // delta[{q0, b}] = {q2};
        // delta[{q1, a}] = {q0};
        // delta[{q1, b}] = {q3};
        // delta[{q2, a}] = {q4};
        // delta[{q2, b}] = {q5};
        // delta[{q3, a}] = {q4};
        // delta[{q3, b}] = {q5};
        // delta[{q4, a}] = {q4};
        // delta[{q4, b}] = {q5};
        // delta[{q5, a}] = {q5};
        // delta[{q5, b}] = {q5};

        // delta[{q0, a}] = {q1};
        // delta[{q0, b}] = {q2};
        // delta[{q0, e}] = {q2};
        // delta[{q1, a}] = {q2};
        // delta[{q1, e}] = {q0};
        // delta[{q2, b}] = {q1};
        // delta[{q2, e}] = {q2};
        
        // delta[{q0, a}] = {q0, q1};
        // delta[{q0, b}] = {q0, q1};
        // delta[{q0, e}] = {q3};
        // delta[{q1, b}] = {q2};
        // delta[{q3, a}] = {q4};
        // delta[{q3, e}] = {q1};
        
        // delta[{q0, e}] = {q1};
        // delta[{q0, a}] = {q2};
        // delta[{q1, a}] = {q0};
        // delta[{q2, a}] = {q1};
        // delta[{q2, b}] = {q1, q2};

        
        // DFA< S_t, Q_t > M(S, Q, q0, F, delta);
        // DFA< S_t, Q_t > M2(S, Q, q0, F, delta2);
        // std::cout << M << "\n\n";
        // M = M.intersection(M2);
        // std::cout << M << "\n\n";
        //M.minimization();
        //std::cout << M << '\n';
        //NFA< S_t, Q_t > N = M.to_NFA();
        //std::cout << N << '\n';
        // std::cout << M << '\n';
        // std::vector< S_t > word = {a,a,a,b,a,a,a,b,b,b,b,b,a,a,b};
        // std::cout << M(word);
        
        //DFA< S_t, Q_t > M2(S, Q, q2, F, delta);
        //std::cout << '\n' << M2 << '\n';
        // M2 = M;
        // std::cout << '\n' << M2 << '\n';
        // NFA< S_t, Q_t > N(S, Q, q0, F, delta);
        // N = N.kleen_star(N);
        // std::cout << N << '\n';
        // std::cout << N << '\n';
        // DFA< S_t, Q_t > test = N.to_DFA();
        // std::cout << "TEST NFA TO DFA\n" << test << '\n';
        // std::string input;
        // std::vector< S_t > word;

        // for (int i = 0; i < 10; ++i)
        // {
        //     input = "";
        //     std::getline(std::cin, input);
        //     while (input.compare("-1") != 0)
        //     {   
        //         word.push_back(input);
        //         std::cout << to_string(word) << '\n';
        //         std::getline(std::cin, input);
        //     }
        //     std::cout << N(word) << '\n';
        //     word.clear();
        // }
        
    }
    catch(InvalidDFAConstruction & e)
    {
        std::cout << "ERROR: INVALID DFA CONSTRUCTION\n";
    }
    // catch(InvalidNFAConstruction & e)
    // {
    //     std::cout << "ERROR: INVALID NFA CONSTRUCTION\n";
    // }

    
    /*
    std::pair <int, std::string > p = {42, "pee"};
    std::cout << "Testing std::to_string\n";
    std::cout << "pair: " << std::to_string(p) << '\n';
    std::vector< int > t;
    t.push_back(1);
    t.push_back(23);
    t.push_back(-2);
    t.push_back(44);
    t.push_back(19);
    //std::cout << "Testing std::to_string\n";
    std::cout << "vector: " << std::to_string(t) << '\n';

    std::cout << "set: " <<
        std::to_string(std::unordered_set< int >{1, 2, 3}) << '\n'; // prints {1, 2, 3}
    std::cout << "map: " <<
        std::to_string(std::unordered_map< int, int >{{1,2}, {3,4}}) << '\n';
    std::cout << "Testing std::to_string\n";
    std::cout << "int: " << std::to_string(42) << '\n';
    std::cout << "double: " << std::to_string(3.14) << '\n';
    std::cout << "char: " << std::to_string('a') << '\n';
    std::cout << "bool: " << std::to_string(true) << '\n';
    std::cout << "unsigned int: " << std::to_string((unsigned int)(-1)) << '\n';
    std::cout << "std::string: " << std::to_string("hello world") << '\n';
    std::cout << "std::pair< int, int >: "
              << std::to_string(std::make_pair(42, 43)) << '\n';
    std::cout << "std::pair< int, double >: "
              << std::to_string(std::make_pair(42, 3.14)) << '\n';
    std::cout << "std::pair< int, char >: "
              << std::to_string(std::make_pair(42, 'a')) << '\n';
    std::cout << "std::pair< int, bool >: "
              << std::to_string(std::make_pair(42, true)) << '\n';
    std::cout << "std::pair< int, unsigned int>: "
              << std::to_string(std::make_pair(-1, (unsigned int)(-1))) << '\n';
    std::cout << "std::pair< std::pair< int, int > int >: "
              << std::to_string(std::make_pair(std::make_pair(42, 43), 44)) << '\n';
    std::cout << "std::pair< int, std::pair< int, int > >: "
              << std::to_string(std::make_pair(42, std::make_pair(43, 44))) << '\n';
    std::cout << "std::vector< int >: "
              << std::to_string(std::vector< int >{2, 3, 5}) << '\n';
    std::cout << "std::vector< double >: "
              << std::to_string(std::vector< double >{1.2, 3.4, 5.6}) << '\n';
    std::cout << "std::vector< char >: "
              << std::to_string(std::vector< char >{'a', 'b', 'c'}) << '\n';
    std::cout << "std::vector< std::string >: "
              << std::to_string(std::vector< std::string >{"ab", "cd", "ef"}) << '\n';
    std::cout << "std::pair< int, int >: "
              << std::to_string(std::pair< int, int >{42, 43}) << '\n';
    std::cout << "std::unordered_map< int, int >: "
              << std::to_string(std::unordered_map< int, int>{{42,1}, {43,2}, {44,3}}) << '\n';
    std::cout << "std::unordered_map< std::pair< int, int >, int>: "
              << std::to_string(std::unordered_map< std::pair< int, int>, int >{
                      {{42,1},2}, {{43,2},3}, {{44,3},4} }) << '\n';

     std::cout << "std::unordered_map< std::unordered_map< int, std::unordered_set >, int>: "
              << std::to_string(std::unordered_map<
                                std::unordered_set< int >, int >
                                {     {{42},4}    }) << '\n';

     std::unordered_set<int> test0 = {1,2,3,4,42};
     LESS less;
     std::cout << "test0: " << to_string(test0) << '\n';
     std::cout << "test0 LESS: " << to_string(test0, less) << '\n';
    */
    return 0;
}





