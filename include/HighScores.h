//
// Created by Kuo Fu.
//

#ifndef FINALPROJECT_HIGH_SCORES_H
#define FINALPROJECT_HIGH_SCORES_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include "PlayerData.h"

class HighScoreList{
private:
    struct ListNode{
        PlayerData pData;
        ListNode *next;
        ListNode *back;
    };
    ListNode *head;
    int count;
    std::string outFilename;

public:
    //Constructor
    HighScoreList();

    // Linked list operations
    void insertNode(PlayerData);
    int getCount() {return count;}
    bool isEmpty();
    void update();
    void writeToFile(std::string outFilename);

    //Destructor
    ~HighScoreList();
};

#endif //FINALPROJECT_HIGH_SCORES_H
