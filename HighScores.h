#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <string>
#include <set>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>

class Score
{
    char name[16];
    int score;
public:
    Score(const char* n = "no name", int sc = 0);

    char* getName();
    int getScore();
    void setName(const char*);
    void setScore(int);

    bool operator<(const Score& obj) const;
    friend std::ostream& operator<<(std::ostream& out, const Score& obj);
};

Score::Score(const char* n, int sc) : score(sc)
{
#ifdef _MSC_VER                         // for MSVS 2017
    strcpy_s(name, sizeof name, n);
#else
    std::strcpy(name,n);
#endif
}

char* Score::getName() {
    return name;
}

void Score::setName(const char* input) {
#ifdef _MSC_VER                         // for MSVS 2017
    strcpy_s(name, sizeof name, n);
#else
    std::strcpy(name,input);
#endif
}

int Score::getScore() {
    return score;
}

void Score::setScore(int input) {
    score = input;
}

bool Score::operator<(const Score& obj) const
{
    return score > obj.score;  // display in reverse order
}

std::ostream& operator<<(std::ostream& out, const Score& obj)
{
    out << std::left << std::setw(16) << obj.name
        << std::right << std::setw(4) << obj.score
        << std::endl;
    return out;
}

class HighScores
{
public:
    HighScores();
    void WriteHighScoresFile();
    void updateHighScores(const Score&);
protected:

private:
    std::multiset<Score> highScores;
    bool highScoresFileExists;
    static std::string HighScoresFilename;
    friend std::ostream& operator<<(std::ostream& out, const HighScores& scores);

};
std::string HighScores::HighScoresFilename = "highscores.bin";

HighScores::HighScores()
        : highScores(), highScoresFileExists(true)
{
    std::ifstream fin(HighScoresFilename,std::ios_base::binary);
    if (!fin)
    {
        std::cout << "Can't find high scores file, " << HighScoresFilename << ".  I'll create a new one." << std::endl;
        highScoresFileExists = false;
    }
    else
    {
        Score temp;
        while (fin.read(reinterpret_cast<char*>(&temp),sizeof(temp)))
        {
            highScores.insert(temp);
        }
        fin.close();
    }
}

void HighScores::updateHighScores(const Score& obj)
{
    highScores.insert(obj);
    if (highScores.size() > 10)
    {
        highScores.erase(--highScores.end());
    }
}


void HighScores::WriteHighScoresFile()
{
    std::ofstream fout(HighScoresFilename,std::ios_base::binary);

    auto count = 0;
    for (auto it = highScores.cbegin(); it != highScores.cend(); ++it, ++count )
    {
        if (count == 10)
            break;
        fout.write(reinterpret_cast<const char*>(&*it),sizeof(*it));
    }
    fout.close();
    std::cout << "High scores file written...." << std::endl;
}

std::ostream& operator<<(std::ostream& out, const HighScores& scores)
{
    if (scores.highScores.size() == 0)
        out << "No high scores recorded" << std::endl;
    else
    {
        out << "***************** High Scores ******************" << std::endl << std::endl;
        out << "     Name           Score         " << std::endl;
        auto count = 1;
        for (auto it = scores.highScores.cbegin(); it != scores.highScores.cend(); ++it, ++count )
        {
            if (count == 11)
                break;
            out << std::setw(3) << count << "  " << *it;
        }
    }
    return out;
}

#endif // HIGH_SCORES_H
