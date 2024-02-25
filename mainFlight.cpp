#include "CENGFlight.h"
#include "Exceptions.h"
#include <iostream>

void PrintArray(const std::vector<std::string>& arr)
{
    std::cout << "{";
    for(const std::string& a : arr)
    {
        std::cout << a << ", ";
    }
    std::cout << "}" << std::endl;
}

int main()
{
    std::cout << "Testing Find Sister Airlines" << std::endl;
    std::cout << std::endl;

    try
    {
        CENGFlight flightSystem("turkey_flight.map");
        std::cout << "Loading Map" << std::endl;

        flightSystem.PrintMap();

        std::vector<std::string> result;
        std::cout << "--------------------" << std::endl;
        std::cout << "Sister Airlines of BoraJet From Istanbul(IST)" << std::endl;
        flightSystem.FindSisterAirlines(result, "BoraJet", "Istanbul(IST)");
        std::cout << "Result is: " << std::endl;
        PrintArray(result);
    }
    catch(SameNamedEdgeException& e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }
    catch(DuplicateVertexException& e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }
    catch(VertexNotFoundException& e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }

	return 0;
}