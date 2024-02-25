#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    return PRIMES[0] * startInt + PRIMES[1] * endInt + PRIMES[2] * (int) isCostWeighted;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        table[i].lruCounter = 0;
        table[i].sentinel = EMPTY_MARK;
    }
    elementCount = 0;
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    if (intArray.size() < 1)
        throw InvalidTableArgException();
    
    int key = Hash(intArray.front(), intArray.back(), isCostWeighted) % MAX_SIZE;

    for (int i = 0, new_key = key; i < MAX_SIZE * MAX_SIZE; i++, new_key = (key + i * i) % MAX_SIZE)
    {
        if (table[new_key].sentinel == OCCUPIED_MARK && 
            table[new_key].endInt == intArray.back() && 
            table[new_key].startInt == intArray.front() && 
            table[new_key].isCostWeighted == isCostWeighted)
        {
            table[new_key].lruCounter++;
            return table[new_key].lruCounter - 1;
        }

        else if ((table[new_key].sentinel == EMPTY_MARK) || (table[new_key].sentinel == SENTINEL_MARK))
            if (elementCount > MAX_SIZE / 2)
                throw TableCapFullException(elementCount);
            else
            {
                table[new_key].intArray = intArray;
                table[new_key].sentinel = OCCUPIED_MARK;
                table[new_key].startInt = intArray.front();
                table[new_key].endInt = intArray.back();
                table[new_key].isCostWeighted = isCostWeighted;
                table[new_key].lruCounter = 1;
                elementCount++;
                return 0;
            }

    }
    if (elementCount > MAX_SIZE / 2)
        throw TableCapFullException(elementCount);
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    int key = Hash(startInt, endInt, isCostWeighted) % MAX_SIZE;

    for (int i = 0, new_key = key; i < MAX_SIZE * MAX_SIZE; i++, new_key = (key + i * i) % MAX_SIZE)
    {
        if (table[new_key].sentinel == OCCUPIED_MARK && 
            table[new_key].endInt == endInt && 
            table[new_key].startInt == startInt && 
            table[new_key].isCostWeighted == isCostWeighted)
        {
            if (incLRU)
                table[new_key].lruCounter++;

            intArray = table[new_key].intArray;
            return true;
        }

        else if (table[new_key].sentinel == EMPTY_MARK)
            return false;

    }
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        table[i].sentinel = EMPTY_MARK;
        table[i].lruCounter = 0;
    }
    elementCount = 0;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    int max_idx = -1, max_lru = -1;
    for (int i = 0; i < MAX_SIZE; i++)
        if (table[i].sentinel == OCCUPIED_MARK && table[i].lruCounter > max_lru)
        {
            max_idx = i;
            max_lru = table[i].lruCounter;
        }
    intArray = table[max_idx].intArray;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    int key = Hash(startInt, endInt, isCostWeighted) % MAX_SIZE;

    for (int i = 0, new_key = key; i < MAX_SIZE * MAX_SIZE; i++, new_key = (key + i * i) % MAX_SIZE)
    {
        if (table[new_key].sentinel == OCCUPIED_MARK && 
            table[new_key].endInt == endInt && 
            table[new_key].startInt == startInt && 
            table[new_key].isCostWeighted == isCostWeighted)
        {
            intArray = table[new_key].intArray;
            table[new_key].sentinel = SENTINEL_MARK;
            table[new_key].lruCounter = 0;
            elementCount--;
            return;
        }

        else if (table[new_key].sentinel == EMPTY_MARK)
            return;

    }
    return;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    MinPairHeap<int, int> minHeap;

    for (int i = 0; i < MAX_SIZE; i++)
        if (table[i].sentinel == OCCUPIED_MARK)
            minHeap.push(Pair<int, int> {table[i].lruCounter, i});

    while (!minHeap.empty() && lruElementCount > 0)
    {
        table[minHeap.top().value].sentinel = SENTINEL_MARK;
        table[minHeap.top().value].lruCounter = 0;
        elementCount--;

        minHeap.pop();

        lruElementCount--;
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    MaxPairHeap<int, int> maxHeap;

    for (int i = 0; i < MAX_SIZE; i++)
        if (table[i].sentinel == OCCUPIED_MARK)
            maxHeap.push(Pair<int, int> {table[i].lruCounter, i});

    while (!maxHeap.empty())
    {
        PrintLine(maxHeap.top().value);
        maxHeap.pop();
    }
}

#endif // HASH_TABLE_HPP