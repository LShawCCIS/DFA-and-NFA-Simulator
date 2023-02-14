#ifndef NAMESPACE_STD_H
#define NAMESPACE_STD_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>

namespace std
{
    std::string to_string(char);
    std::string to_string(const std::string &);
    template < typename S, typename T >
    std::string to_string(const std::pair< S, T > &);
    template < typename T >
    std::string to_string(const std::vector< T > &);
    template < typename S, typename T >
    std::string to_string(const std::unordered_map< S, T > &);
    template < typename T >
    std::string to_string(const std::unordered_set< T > &);
    template < typename T, typename LESS >
    std::string to_string(const std::unordered_set< T > &, const LESS &);
    
    std::string to_string(char c)
    {
        return {c};
    }
    template < typename LESS >
    std::string to_string(double c, const LESS & less)
    {
        return std::to_string(c);
    }
    template < typename LESS >
    std::string to_string(int c, const LESS & less)
    {
        return std::to_string(c);
    }
    std::string to_string(const std::string & x)
    {
        return x;
    }
    template < typename LESS >
    std::string to_string(const std::string & x, const LESS & less)
    {
        return std::to_string(x);
    }
    template < typename S, typename T >
    std::string to_string(const std::pair< S, T > & x)
    {
        return "(" + (std::to_string(x.first)) + ", " +(std::to_string(x.second)) + ")";
    }
    template < typename S, typename T, typename LESS >
    std::string to_string(const std::pair < S, T> & x, const LESS & less)
    {
        std::vector< std::string > t;
        std::string delim = ", ";
        std::string s = "{";
        int size = 0;
        if (less(to_string(x.first), to_string(x.second)))
        {
            t.push_back(to_string(x.first));
            t.push_back(to_string(x.second));
        }
        else
        {
            t.push_back(to_string(x.second));
            t.push_back(to_string(x.first));
        }
        for (int i = 0; i < t.size(); ++i)
        {
            ++size;
            if (size == t.size()) delim = "";
            s += t[i] + delim;
        }
        return s + "}";
    }
    template < typename T >
    std::string to_string(const std::vector< T > & x)
    {
        std::string delim = ", ";
        std::string s = "[";
        for (int i = 0; i < x.size(); ++i)
        {
            if (i == x.size() - 1) delim = "";
            s += std::to_string(x[i]) + delim;
        }
        return s + "]";
    }

    template < typename T >
    std::string to_string(const std::stack< T > & x)
    {
        std::string delim = ", ";
        std::string s = "[";
        std::stack< T > x_copy = x;
        while (x_copy.size() != 0)
        {
            T top = x_copy.top();
            x_copy.pop();
            s += std::to_string(top) + delim;
        }
        return s + "]";
    }

    template < typename T >
    std::string to_string(const std::queue< T > & x)
    {
        std::string delim = ",";
        std::string s = "[";
        std::queue< T > x_copy = x;
        while (x_copy.size() != 0)
        {
            if (x_copy.size() == 1) delim = "";
            T front = x_copy.front();
            x_copy.pop();
            s += std::to_string(front) + delim;
        }
        return s + "]";
    }
    
    
    template < typename S, typename T >
    std::string to_string(const std::unordered_map< S, T > & m)
    {
        std::string s = "{";
        std::string delim = ", ";
        int size = 0;
        for (auto i : m)
        {
            ++size;
            if (size == m.size()) delim = "";
            s += std::to_string(i.first) + ":" + std::to_string(i.second)
                + delim;
        }
                
        return s + "}";
    }

    template < typename T, typename LESS >
    std::string to_string(const std::unordered_set< T > & x, const LESS & less)
    {
        //std::cout << "here\n";
        std::vector< std::string > t;
        std::string delim = ", ";
        std::string s = "{";
        bool inserted = false;
        int size = 0;
        for (auto i : x)
        {
            //std::cout << "I IN FIRST:" << to_string(i) << '\n';
            inserted = false;
            //std::cout << "STILL HERE 1...\n";
            for (int j = 0; j < t.size(); ++j)
            {

                //std::cout << "I:" << to_string(i) << '\n';
                //std::cout << "J:" << to_string(t[j]) << '\n';
                if (less(to_string(i, less), to_string(t[j])))
                {
                    //std::cout << "YEP\n";
                    t.insert(t.begin() + j, to_string(i));
                    inserted = true;
                    break;
                }
            }
            if (!inserted)
            {
                //std::cout << "STILL HERE 2...\n";
                t.push_back(to_string(i, less));
            }
        }
        std::sort(t.begin(), t.end());
        
        for (int i = 0; i < t.size(); ++i)
        {
            ++size;
            if (size == t.size()) delim = "";
            s += t[i] + delim;
        }
        //std::cout << "RET:" << s << '\n';
        return s + "}";
    }
    
    template < typename T >
    std::string to_string(const std::unordered_set< T > & set)
    {
        std::string delim = ", ";
        std::string s = "{";
        int size = 0;
        for (auto i : set)
        {
            ++size;
            if (size == set.size()) delim = "";
            s += std::to_string(i) + delim;
        }
        return s + "}";
    }
    template < typename S, typename T >
    struct hash< std::pair< S, T > >
    {
        size_t operator()(const std::pair< S, T > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };
    

    template < typename T >
    struct hash< std::vector< T > >
    {
        size_t operator()(const std::vector< T > & t) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(t));
        }
    };

    template < typename S, typename T >
    struct hash< std::unordered_map< S, T > >
    {
        size_t operator()(const std::unordered_map< S, T > & m) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(m));
        }
    };

    template < typename T >
    struct hash< std::unordered_set< T > >
    {
        size_t operator()(const std::unordered_set< T > & s) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(s));
        }
    };
}


#endif
