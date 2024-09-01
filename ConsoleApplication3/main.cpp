#include <iostream>
#include <vector>
#include <cassert>

#include "SafeCPP.h"

    int main() {
        try {
            std::safe_vector<int> v;
            auto front = v.front();
        }
        catch (std::exception e) {
            std::cout << "calling front() on an empty vector:" << std::endl;
            std::cout << e.what() << std::endl;
        }

        {
            std::safe_vector<int> v = { 1, 2, 3, 4, 5 };
            auto it = v.begin();
            {
                std::safe_vector<int> scopeVector = { 1, 2 };
                it = scopeVector.begin();
            }
            try {
                auto val = *it;
            }
            catch (std::exception e) {
                std::cout << "dereferncing iterator of a destroyed vector:" << std::endl;
                std::cout << e.what() << std::endl;
            }
        }

       try {
           std::safe_vector<int> v = { 1, 2, 3, 4, 5 };
           auto itr = --v.begin();
           auto val = *itr;
       }
       catch (std::exception e) {
           std::cout << "dereferncing out of range iterator:" << std::endl;
           std::cout << e.what() << std::endl;
       }
       {
           std::safe_vector<int> v = { 1, 2, 3, 4, 5 };
           size_t index = 0;
           for (auto itr = v.cbegin(); itr != v.cend(); itr++) {
               assert(*itr == v[index]);
               index++;
           }
           
           for (auto itr = v.crbegin(); itr != v.crend(); itr++) {
               index--;
               assert(*itr == v[index]);
           }
       }

       {
           std::safe_vector<int> v = { 1, 2, 3, 4, 5 };
           auto& val = v[3];
           assert(val == 4);
           auto itr = v.begin();
           itr += 2;
           v.insert(itr, { 100, 99 });
           assert(v[2] == 100);
           assert(v[3] == 99);
           assert(v.size() == 7);
           assert(val == 4); // even after inserting val kept its referenced object.
       }

       {
           struct MyStruct {
               int x;
               int y;
           };

           std::safe_vector<MyStruct> v{ {1,2}, {3,4}, {5,6} };
           v.push_back(MyStruct(7,8));
           v.emplace_back( 9, 10 );
           assert(v.size() == 5);
           assert(v[3].x == 7);
           assert(v[3].y == 8);
           assert(v[4].x == 9);
           assert(v[4].y == 10);

           auto itr = v.begin() + 1;
           v.emplace(itr, 0, 0);
           assert(v.size() == 6);
           assert(v[1].x == 0);
           assert(v[1].y == 0);
       }

    }