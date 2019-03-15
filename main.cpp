#include <cstddef>
#include <cstdlib>
#include <string>

#include<chrono>
#include<iostream>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinkedList = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;

  LinkedList<std::string> list;
  for(std::size_t i=0;i<repeatCount;++i)
  list.append("xyz");
  auto start = std::chrono::system_clock::now();
  list.append("zyx");
  auto done = std::chrono::system_clock::now();
  std::cout<<"LinkedList append time: "<<(done-start).count()<<'\n';
  Vector<std::string> vect;
  for(std::size_t i=0;i<repeatCount;++i)
  vect.append("xyz");
   start = std::chrono::system_clock::now();
  vect.append("zyx");
  done = std::chrono::system_clock::now();
  std::cout<<"Vector append time: "<<(done-start).count()<<'\n';


  return 0;
}
