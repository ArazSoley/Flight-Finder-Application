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
        {
            CENGFlight flightSystem("turkey_flight.map");
            std::cout << "Loading Map" << std::endl;

            flightSystem.PrintMap();

            std::vector<std::string> result;
            std::cout << "--------------------" << std::endl;
            std::cout << "Sister Airlines of Pegasus From Ankara(ESB)" << std::endl;
            flightSystem.FindSisterAirlines(result, "Pegasus", "Ankara(ESB)");

            std::cout << "Result is:" << std::endl;
            PrintArray(result);
        }
        std::cout << "=================================" << std::endl;
        {
            CENGFlight flightSystem("basic_flight.map");
            std::cout << "Loading Basic Map" << std::endl;

            flightSystem.PrintMap();

            std::vector<std::string> result;
            std::cout << "--------------------" << std::endl;
            std::cout << "Sister Airlines of EDGE0 From A" << std::endl;
            flightSystem.FindSisterAirlines(result, "EDGE0", "A");

            std::cout << "Result is:" << std::endl;
            PrintArray(result);
        }
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