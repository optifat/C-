#include "iostream"
#include "CircleList.h"

int main() {
    CircleBuffer<int> myList(4);

    for(int i = 0; i<5; i++)                       ///Shows the correctness of push_back method
        myList.push_back(i);
    std::cout<<myList<<std::endl;

    std::cout<<myList.pop()<<std::endl;            ///Shows the correctness of pop method
    std::cout<<myList<<std::endl;
    myList.push_back(5);
    std::cout<<myList<<std::endl;

    std::cout<<myList.getElementValue(2)<<std::endl;    ///Shows the correctness of getElementValue method
    std::cout<<myList.getElementValue(5)<<std::endl;

    CircleBuffer<char> myList_2(3);                  ///Shows the working ability with another type of data
    myList_2.push_back('a');
    myList_2.push_back('b');
    std::cout<<myList_2<<std::endl;

    myList_2.replaceElement(0, 'c');                 ///Shows the correctness of replaceElementValue method
    std::cout<<myList_2<<std::endl;

    return 0;
}