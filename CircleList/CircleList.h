//
// Created by pavel on 12/12/17.
//

#ifndef UNTITLED4_CIRCLELIST_H
#define UNTITLED4_CIRCLELIST_H

#include "iostream"

template <typename T>
struct element{                  ///This is the component of list
    T value;
    element *Next;
};

template <typename T>
class CircleBuffer{

private:
    element<T> *head;
    element<T> *end;
    int listCapacity;
    int listSize = 0;

public:
    CircleBuffer(int N = 100){      ///Number N can be changed
        listCapacity = N;
        head = nullptr;
        end = nullptr;
    }

    ~CircleBuffer(){
        for(int i = 0; i < listSize; i++){
            auto *tmp = head->Next;
            delete head;
            head = tmp;
        }
    };

    void push_back(T x){
        if(!listSize){
            listSize ++;
            auto *tmp = new element<T>;
            tmp->value = x;
            tmp->Next = end;
            end = tmp;
            head = end;
        }

        else if (listSize + 1 <= listCapacity) {
            listSize++;
            auto *tmp = new element<T>;
            tmp->value = x;
            end->Next = tmp;
            end = tmp;
            end->Next = head;
        }

        else{
            head->value = x;
            end = head;
            end->Next = head->Next;
            head = head->Next;
        }
    }

    T pop(){
        listSize--;
        T value = head->value;
        auto *tmp = head->Next;
        delete head;
        head = tmp;
        end->Next = head;
        return value;
    }

    int size(){
        return listSize;
    }

    T getElementValue(int index){
        auto *tmp = head;
        for(int i = 0; i < index%listSize; i++)
            tmp = tmp->Next;
        return tmp->value;
    }

    void replaceElement(int index, T change){
        auto *tmp = head;
        for(int i = 0; i < index%listSize; i++)
            tmp = tmp->Next;
        tmp->value = change;
    }

    template <typename type> friend std::ostream &operator<<(std::ostream &out, CircleBuffer<type> &a);
};

template <typename type> std::ostream &operator<<(std::ostream &out, CircleBuffer<type> &a){
    auto *tmp = a.head;
    out<<"[ ";
    do{
        out<<tmp->value<<" ";
        tmp = tmp->Next;
    }
    while(tmp != a.end->Next);
    out<<"]";
    return out;
}
#endif //UNTITLED4_CIRCLELIST_H
