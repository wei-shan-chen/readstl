#ifndef SPEEDUP_H
#define SPEEDUP_H
#include <glm/glm.hpp>
#include <iostream>

class SpeedUp{
public:
    struct Node{
        glm::vec3 pos;
        Node *next;
        Node *prev;
    };
    struct Nodenum{
        int num;
        Nodenum *next;
        Nodenum *prev;
        Node *right;
    };
    SpeedUp(){
        Nodenum head;
        Nodenum now;
        now = head;
        now.num =0;
        now.next = NULL;
        now.prev = NULL;
        now.right = NULL;

        for(int i = 1; i < 27;i++){
            Nodenum tmpNum;
            tmpNum.num = i;
            tmpNum.next = NULL;
            tmpNum.prev = now;
            tmpNum.right = NULL;

            now.next = tmpNum;
            now = now.next;

        }

    }
    



};


#endif