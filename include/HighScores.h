//
// Created by Kuo Fu.
//

#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include "PlayerData.h"
#include <iostream>
#include <fstream>
#include <iomanip>

class HighScoreList {
private:
    struct ListNode {
        PlayerData pData;
        ListNode* next;
        ListNode* back;
    };
    ListNode* head;
    int count;
    std::string outFilename;

public:
    //Constructor
    HighScoreList();

    // Linked list operations
    void insertNode(PlayerData);
    int getCount() { return count; }
    bool isEmpty();
    void writeToFile(std::string outFilename);

    //Destructor
    ~HighScoreList();
};

#endif // HIGHSCORES_H
