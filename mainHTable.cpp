#include "HashTable.h"
#include <iostream>

void PrintIntArray(const std::vector<int>& arr)
{
    std::cout << "{";
    for(size_t i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i];
        if(i != arr.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}";
}

int main(int argc, const char* argv[])
{
    HashTable<11> table;

    std::vector<int> input;
    input.push_back(1);
    input.push_back(2);
    input.push_back(3);

    int lru0 = table.Insert(input, true);

    std::cout << "LRU0:" << lru0 << std::endl;
    std::cout << "-------------------" << std::endl;
    table.PrintTable();

    int lru1 = table.Insert(input, false);

    std::cout << "LRU1:" << lru1 << std::endl;
    std::cout << "-------------------" << std::endl;
    table.PrintTable();

    //int lru2 = table.Insert(input, true);
    //std::cout << "LRU2:" << lru2 << std::endl;
    std::cout << "-------------------" << std::endl;
    table.PrintTable();

    std::vector<int> input2;
    input2.push_back(909);
    input2.push_back(23);
    input2.push_back(311);

    lru1 = table.Insert(input2, false);
    lru1 = table.Insert(input2, false);
    lru1 = table.Insert(input2, false);

    table.PrintTable();

    std::vector<int> result;
    table.GetMostInserted(result);

    std::cout << "result: ";
    PrintIntArray(result);
    std::cout << std::endl;

    std::cout << "-------------------" << std::endl;
    table.PrintSortedLRUEntries();

    table.PrintTable();

    std::cout << "-------------------" << std::endl;
    table.RemoveLRU(2);
    std::cout << "-------------------" << std::endl;
    table.PrintTable();

    table.Remove(result, 2909, 311, false);

    std::cout << "-------------------" << std::endl;
    table.PrintTable();

    std::cout << "-------------------" << std::endl;
    lru1 = table.Insert(input, false);

    try
    {
        lru1 = table.Insert(std::vector<int>{{1,2,2}}, false);
        lru1 = table.Insert(std::vector<int>{{1,2,3}}, false);
        lru1 = table.Insert(std::vector<int>{{1,3,4}}, false);
        lru1 = table.Insert(std::vector<int>{{1,2,3}}, true);


        lru1 = table.Insert(std::vector<int>{{6,2,5}}, true);
        lru1 = table.Insert(std::vector<int>{{4,2,2}}, true);
        lru1 = table.Insert(std::vector<int>{{3,2,1}}, true);

    }
    catch(TableCapFullException& e)
    {
        std::cerr << e.ToString() << '\n';
    }

    table.PrintTable();

    return 0;
}