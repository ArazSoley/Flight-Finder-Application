#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    try
    {
        std::vector<float> tmp;
        tmp = navigationMap.haltEdge(airlineName, airportFrom, airportTo);
        haltedFlights.push_back(HaltedFlight {airportFrom, airportTo, airlineName, tmp[0], tmp[1]});
    }
    catch (...)
    {
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
    }
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    try
    {
        for (int i = 0, size = haltedFlights.size(); i < size; i++)
            if (haltedFlights[i].airline == airlineName &&
                haltedFlights[i].airportFrom == airportFrom &&
                haltedFlights[i].airportTo == airportTo)
                {
                    navigationMap.AddEdge(airlineName, airportFrom, airportTo, haltedFlights[i].w0, haltedFlights[i].w1);
                    haltedFlights.erase(haltedFlights.begin() + i);
                    return;
                }
        throw int (1);
    }
    catch (...)
    {
        PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
    }
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    try
    {
        int start = navigationMap.getVertexIndex(startAirportName), end = navigationMap.getVertexIndex(endAirportName);
        std::vector<int> path;


        if ((alpha == 0 || alpha == 1) && lruTable.Find(path, start, end, (alpha ? 0 : 1), true))
        {
            PrintFlightFoundInCache(startAirportName, endAirportName, (alpha ? 0 : 1));
            navigationMap.PrintPath(path, alpha, true);
        }
        else
        {
            if (navigationMap.HeuristicShortestPath(path, startAirportName, endAirportName, alpha))
            {
                if (alpha == 0 || alpha == 1)
                {
                    PrintFlightCalculated(startAirportName, endAirportName, (alpha ? 0 : 1));
                    try 
                    {
                    lruTable.Insert(path, (alpha ? 0 : 1));
                    }
                    catch (TableCapFullException x)
                    {
                        lruTable.RemoveLRU(1);
                        lruTable.Insert(path, (alpha ? 0 : 1));
                    }
                }
                navigationMap.PrintPath(path, alpha, true);
            }
            else
                PrintPathDontExist(startAirportName, endAirportName);
        }

    }
    catch (...)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    try
    {
        int start = navigationMap.getVertexIndex(startAirportName), end = navigationMap.getVertexIndex(endAirportName);
        std::vector<int> path;

        if (navigationMap.FilteredShortestPath(path, startAirportName, endAirportName, alpha, unwantedAirlineNames))       
            navigationMap.PrintPath(path, alpha, true);
        else
            PrintPathDontExist(startAirportName, endAirportName);

    }
    catch (...)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    std::vector<int> visited (navigationMap.getVertexCount(), 0);
    std::vector<std::string> airlines;
    
    try
    {
        int from = navigationMap.getVertexIndex(airportName);
        visited[from] = 1;
        airlines.push_back(startAirlineName);

        FindSisterAirlinesRec(airlines, from, visited);
        airlineNames = airlines;
        airlineNames.erase(airlineNames.begin());
    }
    catch (...)
    {
        PrintSisterAirlinesDontCover(airportName);
    }
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    try
    {
        int res = navigationMap.MaxDepthViaEdgeName(airportName, airlineName);
        return res;
    }
    catch (...)
    {
        return -1;
    }
    
}

void CENGFlight::FindSisterAirlinesRec(std::vector<std::string> &airlines,
                                       int airport,
                                       std::vector<int> &visited) const
{
    bool flag = false;
    std::vector<int> tempVisited (visited.size(), 0);
    
    navigationMap.traverseUsingEdges(airlines, airport, tempVisited);

    for (int i = 0, size = tempVisited.size(); i < size; i++)
    {
        if (tempVisited[i] != visited[i])
            flag = true;

        visited[i] = tempVisited[i];
    }
    bool visitedAll = true;
    for (int i = 0, size = visited.size(); i < size; i++)
        if (!visited[i])
        {
            visitedAll = false;
            break;
        }
    if (visitedAll)
        return;
    
    if (flag)
    {
        int new_airport = navigationMap.mostUnvisitedNeighbor(visited);
        navigationMap.unusedAirline(airlines, new_airport, visited);
        FindSisterAirlinesRec(airlines, new_airport, visited);
        return;
    }
    else
    {
        for (int i = 0, size = visited.size(); i < size; i++)
            if (!visited[i])
                throw int(0);
        return;
    }

}