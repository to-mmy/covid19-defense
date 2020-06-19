//
// Created by Kuo Fu on 2020-06-18.
//
#include "HighScores.h"

HighScoreList::HighScoreList() {
    head = new ListNode; // head points to the sentinel node
    head->next = head;
    head->back = head;
    count = 0;
}


void HighScoreList::insertNode(PlayerData dataIn) {

    ListNode* newNode;  // A new node
    ListNode* pCur;     // To traverse the list
    ListNode* pPre;     // The previous node

    newNode = new ListNode;
    newNode -> pData = dataIn;
    pPre = head;
    pCur = head->next;

    while(pCur != head && pCur->pData < dataIn) {
        pPre = pCur;
        pCur = pCur->next;
    }

    newNode->next = pCur;
    newNode->back = pPre;
    pCur->back = newNode;
    pPre->next = newNode;

    count++;
}

bool HighScoreList::isEmpty() {
    return (head->next == head && head->back == head);
}

HighScoreList::~HighScoreList() {
    ListNode* pCur;   // To traverse the list
    ListNode* pNext;  // To point to the next node

    pCur = head->next;

    while (pCur != head) {
        pNext = pCur -> next;
        pCur->next->back = pNext;
        delete pCur;

        pCur = pNext;
    }
    delete head; // delete the sentinel node
}
void HighScoreList::writeToFile(std::string outFilename) {
    std::ofstream outputFile;
    outputFile.open(outFilename);

    if(outputFile.fail()) {
        std::cout << "ERROR: Cannot open/create outputFile.\n";
        exit(EXIT_FAILURE);
    }

    // traverseForward
    ListNode* pWalk;
    pWalk = head->next;
    while(pWalk != head) {
        outputFile << pWalk->pData << std::endl;
        pWalk = pWalk->next;
    }
    outputFile.close();
}
