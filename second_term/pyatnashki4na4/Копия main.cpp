//
//  main.cpp
//  pyatnashki
//
//  Created by Simon Fedotov on 17.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <set>
#include <map>
#include <chrono>
//#include "Fenwick.cpp"


//FinalPosition =
/*
    0 1 2 3
    4 5 6 7
    8 9 10 11
    12 13 14 15
*/
int FieldSize = 16;
int RowNumber = 4;

std::vector<int> LinConflicts;

void Random(std::vector<int> &ToShuffle) {
    srand(time(0));
    for(int i = 0; i < 1000000 ; ++i) {
        

        int WhereIsNull = std::find(ToShuffle.begin(), ToShuffle.end(), 0) - ToShuffle.begin();
        int bubu = rand() % 4;
        if(bubu == 0 && WhereIsNull % RowNumber != RowNumber - 1) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull + 1]);
        }
        else if(bubu == 1 && WhereIsNull % RowNumber != 0) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull - 1]);
        }
        else if(bubu == 2 && WhereIsNull / RowNumber != 0) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull - RowNumber]);
        }
        else if(bubu == 3 && WhereIsNull / RowNumber != RowNumber - 1) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull + RowNumber]);
        }
    }
    
    for(int i = 0; i < ToShuffle.size(); ++i) {
        std::cout << ToShuffle[i] << " ";
        if(i % RowNumber == RowNumber - 1)
            std::cout << "\n";
    }
    std::cout << "\n\n";
}


class FieldPosition { //Это Класс нашего положения в Игре
    //private:
public:
    std::vector<int> Position; //Та ситуация, в которой мы находимся
    int Level;
    int DatDistance;
    std::vector<int> OurParent; //Родительская ситуация
    
    bool IsWinPosition();
    int CountDistance();
    int GetWeight();
    int CountDistanceNew();
    void CheckNewPosition(FieldPosition &Minimal, std::set<FieldPosition> &Opened, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &DatMap);
    FieldPosition MakeNewNeighbor(const int &WhereIsNull, const short &Shift) const;
    void FindTheWay(std::vector<std::string> &OurWay, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &OurMap);
    
    
    
    
    
    FieldPosition(std::vector<int> &ToCreate);
    FieldPosition(std::vector<int> &ToCreate, const int &NewLevel, const std::vector<int> &Parent);
    
    bool operator <(const FieldPosition &ToCompare) const;
};

class GameSolver{
private:
    std::vector<std::string> OurWay;
    std::set<FieldPosition> OpenedPositions;
    std::map<std::vector<int>, std::pair<int, std::vector<int>>> OurMap;
public:
    void Run(FieldPosition& StartPosition);
};



void GameSolver::Run(FieldPosition& StartPosition) {
    
    OpenedPositions.insert(StartPosition);
    OurMap.insert(std::make_pair(StartPosition.Position, std::make_pair(StartPosition.GetWeight(), StartPosition.OurParent)));
    
    while (!OpenedPositions.empty()) {
        FieldPosition Minimal = *OpenedPositions.begin();
        OpenedPositions.erase(OpenedPositions.begin());
        
       

        
        
        if(Minimal.IsWinPosition()) {
            Minimal.FindTheWay(OurWay, OurMap);
            break;
        }
        
        int WhereIsNull = std::find(Minimal.Position.begin(), Minimal.Position.end(), 0) - Minimal.Position.begin();
        
        if(WhereIsNull % RowNumber != RowNumber - 1) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, 1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull % RowNumber != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / RowNumber != RowNumber - 1) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, RowNumber);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / RowNumber != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -RowNumber);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
    }
    
    std::reverse(OurWay.begin(), OurWay.end());
    for(int i = 0; i < OurWay.size(); ++i) {
        
        std::cout << OurWay[i] << "\n";
    }
    
}


bool Find(const int &ToFind) {
    for(int i = 0; i < LinConflicts.size(); ++i) {
        if(LinConflicts[i] == ToFind)
            return true;
    }
    return false;
}



int LinearConflict(std::vector<int>& ToCount) { //Подправить
    LinConflicts.clear();
    int Result = 0;
    for(int i = 0 ; i < FieldSize; ++i) {
        if (ToCount[i] == 0)
            continue;
        if(ToCount[i] / RowNumber == i / RowNumber) {
            for(int j = i - i % RowNumber; j < i + RowNumber; ++j) {
                if(j != i && ToCount[j] / RowNumber == j / RowNumber && ToCount[i] > ToCount[j] && i < j) {
                    Result += 2;
                    LinConflicts.push_back(ToCount[i]);
                }
            }
        }
        
        if(ToCount[i] % RowNumber == i % RowNumber && !Find(ToCount[i])) {
            for(int j = i % RowNumber; j <= i % RowNumber + RowNumber * (RowNumber - 1); j += RowNumber) {
                if(j != i && ToCount[j] % RowNumber == j % RowNumber && ToCount[i] > ToCount[j] && i < j) {
                    Result += 2;
                    LinConflicts.push_back(ToCount[i]);
                }
            }
        }
        
    }
    return Result;
}


int CornerConflict(std::vector<int>& ToCount) {
    int Result = 0;
    
    if(ToCount[1] == 1 && ToCount[RowNumber] == RowNumber && ToCount[0] != 0 && !Find(ToCount[1]) && !Find(ToCount[0]) && !Find(ToCount[RowNumber]))
        Result += 2;
    
    if(ToCount[RowNumber - 2] == RowNumber - 2 && ToCount[2 * RowNumber - 1] == 2 * RowNumber - 1 && ToCount[RowNumber - 1] != RowNumber - 1 && !Find(ToCount[RowNumber - 2]) && !Find(ToCount[2 * RowNumber -1]) && !Find(ToCount[RowNumber - 1]))
        Result += 2;
    
    if(ToCount[RowNumber * (RowNumber - 2)] == RowNumber * (RowNumber - 2) && ToCount[RowNumber * (RowNumber - 1) + 1] == RowNumber * (RowNumber - 1) + 1 && ToCount[RowNumber * (RowNumber - 1)] != RowNumber * (RowNumber - 1) && !Find(ToCount[RowNumber * (RowNumber - 1)]) && !Find(ToCount [RowNumber * (RowNumber - 1) + 1]) && !Find(ToCount[RowNumber * (RowNumber - 2)]))
        Result += 2;
    
    
    if(ToCount[RowNumber * (RowNumber - 1) - 1] == RowNumber * (RowNumber - 1) - 1 && ToCount[RowNumber * RowNumber - 2] == RowNumber * RowNumber - 2 && ToCount[RowNumber * RowNumber - 1] != RowNumber * RowNumber - 1 && !Find(ToCount[RowNumber * (RowNumber - 1) - 1]) && !Find(ToCount[RowNumber * RowNumber - 2]) && !Find(ToCount[RowNumber * RowNumber - 1]))
        Result += 2;
    
    return Result;
}


int TilesOut(std::vector<int> &ToCount) {
    int Result = 0;
    for(int i = 0; i < FieldSize; ++i) {
        if(ToCount[i] / RowNumber != i / RowNumber)
            ++Result;
        if(ToCount[i] % RowNumber != i % RowNumber)
            ++Result;
    }
    return Result;
}



bool FieldPosition::operator<(const FieldPosition &ToCompare) const {
    if(this -> Level + this -> DatDistance == ToCompare.Level + ToCompare.DatDistance) {
        return this -> Position < ToCompare.Position;
    }
    return this -> Level + this -> DatDistance < ToCompare.Level + ToCompare.DatDistance;
}

int FieldPosition::CountDistance() { //Подсчет не стоящих на своем месте
    int Result = 0;
//    for(int TruePosition = 0; TruePosition < FieldSize; ++TruePosition) {
//        if(!((this -> Position)[TruePosition]  == TruePosition))
//            ++Result;
//    }
    
    Result += TilesOut(this -> Position);
    
    Result += CountDistanceNew();
    
    
    
    Result += LinearConflict(this -> Position);
    
    Result += CornerConflict(this -> Position);
    
    
    return Result;
}

int FieldPosition::GetWeight() {
    return this -> DatDistance + this -> Level;
}


FieldPosition::FieldPosition(std::vector<int> &ToCreate, const int &NewLevel, const std::vector<int> &Parent) {
    Position = ToCreate;
    Level = NewLevel;
    DatDistance = CountDistance();
    OurParent = Parent;
}

FieldPosition::FieldPosition(std::vector<int> &ToCreate) {
    Position = ToCreate;
    Level = 0;
    DatDistance = CountDistance();
    OurParent = {};
}


bool FieldPosition::IsWinPosition() {
    if(CountDistance() == 0)
        return true;
    else
        return false;
    
}



int CountDistance(std::vector<int>& ToCount) {
    int Result = 0;
    for(int TruePosition = 0; TruePosition < FieldSize; ++TruePosition) {
        if(!(ToCount[TruePosition]  == TruePosition))
            ++Result;
    }
    return Result;
}


int FieldPosition::CountDistanceNew() {
    int Result = 0;
    std::vector<int> ToCount = this -> Position;
    for(int i = 0; i < FieldSize; ++i) {
        if(ToCount[i] != 0) {
            int RealPosition = ToCount[i] ;
            Result += abs(RealPosition / RowNumber - i / RowNumber) + abs(RealPosition % RowNumber - i % RowNumber);
        }
    }
    return Result;
}

FieldPosition FieldPosition::MakeNewNeighbor(const int &WhereIsNull, const short &Shift) const {
    std::vector<int> NeighborPosition = this -> Position;
    std::swap(NeighborPosition[WhereIsNull], NeighborPosition[WhereIsNull + Shift]);
    FieldPosition Neighbor = FieldPosition(NeighborPosition, this -> Level + 1, this -> Position);
    return Neighbor;
}

void FieldPosition::FindTheWay(std::vector<std::string> &OurWay, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &OurMap) {
    std::cout << Level << "\n";
    std::vector<int> Check = OurParent;
    if(Level == 0)
        return;
    
    int WhereIsNullParent = std::find(Check.begin(), Check.end(), 0) - Check.begin();
    
    if(WhereIsNullParent == 1)
        OurWay.push_back("LEFT");
    else
        OurWay.push_back("UP");
    
    int Counter = Level - 1;
    while (Counter > 0) {
        auto OurElement = OurMap.find(Check);
        
        for(int i = 0; i < FieldSize; ++i) {
            std::cout << Check[i] << " ";
            if(i % RowNumber == RowNumber - 1)
                std::cout << "\n";
        }
        std::cout << "\n";

        int WhereIsNullSon = std::find(Check.begin(), Check.end(), 0) - Check.begin();
        int WhereIsNullParent = std::find((OurElement -> second.second).begin(), (OurElement -> second.second).end(), 0) - (OurElement -> second.second).begin();

        if(WhereIsNullParent - WhereIsNullSon == RowNumber)
            OurWay.push_back("UP");
        else if(WhereIsNullParent - WhereIsNullSon == -RowNumber)
            OurWay.push_back("DOWN");
        else if(WhereIsNullParent - WhereIsNullSon == 1)
            OurWay.push_back("LEFT");
        else if(WhereIsNullParent - WhereIsNullSon == -1)
            OurWay.push_back("RIGHT");
        
        Check = OurElement -> second.second;
        --Counter;
        
    }
}



//Эта функция проверяет, есть ли решение


//bool HasSolution(std::vector<int> &StartPosition) {
//    int Result = 0;
//    std::vector<int> OurVector(FieldSize - 1);
//    FenwickTree OurTree = FenwickTree(FieldSize - 1);
//
//    int OurNumber;
//    for(int i = 0; i < FieldSize; ++i) {
//        if(StartPosition[i] == 0)
//            continue;
//
//        OurNumber = StartPosition[i] ;
//        OurVector[i] = OurNumber;
//    }
//
//    for(int Position = 0; Position < FieldSize - 1; ++Position){
//        Result += Position - OurTree.Sum(OurVector[Position] - 1);
//        OurTree.Update(OurVector[Position] - 1, 1);
//    }
//
//    int WhereIsNull = std::find(StartPosition.begin(), StartPosition.end(), 0) - StartPosition.begin();
//
//    Result += (WhereIsNull / RowNumber) + 1;
//
//    if(Result % 2 == 0)
//        return true;
//    else
//        return false;
//}


void FieldPosition::CheckNewPosition(FieldPosition &Minimal, std::set<FieldPosition> &Opened, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &DatMap) {
    auto OurElement = DatMap.find(Position);
    if(OurElement != DatMap.end()) {
        int OldDistance = OurElement -> second.first;
        int jo;
        
        
        if((jo = GetWeight()) < OldDistance) {
            FieldPosition ToDelete = FieldPosition(Position, OurElement -> second.first - CountDistance(), Minimal.Position);
            
            
            Opened.erase(ToDelete);
            Opened.insert(*this);
            
            OurElement -> second.first = GetWeight();
            OurElement -> second.second = OurParent;
            
        }
    }
    else {
        Opened.insert(*this);
        DatMap.insert(std::make_pair(Position, std::make_pair(GetWeight(), OurParent)));
    }
    
}


int main(int argc, const char * argv[]) {
    
    int GameMode;
    std::vector<int> StartPosition;
    std::cin >> GameMode;
    RowNumber = GameMode;
    

    GameMode *= GameMode;
    FieldSize = GameMode;
    
    
    for(int i = 0; i < FieldSize; ++i) {
        int OurNumber;
        std::cin >> OurNumber;
        StartPosition.push_back(OurNumber);
    }
    
    Random(StartPosition);


    FieldPosition StartField = FieldPosition(StartPosition);
    
    double AVG = 0.0;
    
    std::vector<int> Bu = StartPosition;
    
    for(int i = 0; i < 20; ++i) {
        std::cout << "TestNumber = " << i << "\n";
        Random(Bu);
        
        
        FieldPosition StartField = FieldPosition(Bu);
        GameSolver Bubu;
        
        auto Start = std::chrono::steady_clock::now();
        Bubu.Run(StartField);
        auto End = std::chrono::steady_clock::now();
        
        Bu = StartPosition;
        AVG += std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() * 0.000001;
    }

    
    GameSolver Bubu;
    
    auto Start = std::chrono::steady_clock::now();
    Bubu.Run(StartField);
    auto End = std::chrono::steady_clock::now();

    
    for(int i = 0; i < StartPosition.size(); ++i) {
        std::cout << StartPosition[i] << " ";
        if(i % RowNumber == RowNumber - 1)
            std::cout << "\n";
    }
    std::cout << "\n\n";

    
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() * 0.000001;
    
    std::cout << "Average = " << AVG / 20 << "\n";

    
    return 0;
}

