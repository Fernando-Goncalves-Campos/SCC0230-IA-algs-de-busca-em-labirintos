#pragma once

#include <iostream>
#include <concepts>
#include <type_traits>
#include <vector>
#include <map>

template <typename T>
concept isString = std::is_same<T, std::string>::value;

template <typename T>
constexpr std::string getQuotes(){
    if constexpr(std::is_same<T, std::string>::value){
        return "\"";
    }
    else if constexpr(std::is_same<T, char>::value){
        return "\'";
    }
    return "";
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& buffer){
    bool following = false;
    out << '[';
    for(const T& it : buffer){
        if(following){
            out << ", ";
        }
        else{
            following = true;
        }

        out << getQuotes<T>();
        out << it;
        out << getQuotes<T>();
    }
    out << ']';
    return out;
}

template <typename T, typename Alloc, template < typename, typename > class Container>
requires (!isString<Container<T, Alloc>>)
std::ostream& operator<<(std::ostream& out, const Container<T, Alloc>& buffer){
    bool following = false;
    out << '[';
    for(const T& it : buffer){
        if(following){
            out << ", ";
        }
        else{
            following = true;
        }

        out << getQuotes<T>();
        out << it;
        out << getQuotes<T>();
    }
    out << ']';
    return out;
}

template <typename T, typename Alloc1, typename Alloc2,
          template < typename, typename > class Container,
          template < class, typename > class Matrix>
requires (!isString<Container<T, Alloc1>>)
std::ostream& operator<<(std::ostream& out, const Matrix<Container<T, Alloc1>, Alloc2>& buffer){
    bool following = false;
    out << '[';
    for(const Container<T, Alloc1>& it : buffer){
        if(following){
            out << "\n ";
        }
        else{
            following = true;
        }
        out << it;
    }
    out << ']';
    return out;
}

template <typename T, typename Alloc1, typename Alloc2, typename Alloc3,
          template < typename, typename > class Container,
          template < class, typename > class Matrix,
          template < class, typename > class Cube>
requires (!isString<Container<T, Alloc1>>)
std::ostream& operator<<(std::ostream& out, const Cube<Matrix<Container<T, Alloc1>, Alloc2>, Alloc3>& buffer){
    bool following = false;
    out << '[';
    for(const Matrix<Container<T, Alloc1>, Alloc2>& it : buffer){
        if(following){
            out << "\n\n";
        }
        else{
            following = true;
        }
        out << it;
    }
    out << ']';
    return out;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& buffer){
    out << '(';
    
    out << getQuotes<T1>();
    out << buffer.first;
    out << getQuotes<T1>();

    out << ", ";
    
    out << getQuotes<T2>();
    out << buffer.second; 
    out << getQuotes<T2>();

    out << ')';
    return out;
}

template <typename T1, typename T2, typename Cmp, typename Alloc,
          template < typename, typename, typename, typename > class Container>
requires (!isString<Container<T1, T2, Cmp, Alloc>>)
std::ostream& operator<<(std::ostream& out, const Container<T1, T2, Cmp, Alloc>& buffer){
    bool following = false;
    out << '[';
    for(const std::pair<T1, T2>& it : buffer){
        if(following){
            out << ", ";
        }
        else{
            following = true;
        }

        out << it;
    }
    out << ']';
    return out;
}