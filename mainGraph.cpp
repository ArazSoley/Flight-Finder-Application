#include "MultiGraph.h"
#include "Exceptions.h"
#include <iostream>

void InsertEdge(MultiGraph& graph,
                const std::string& E,
                const std::string& V0,
                const std::string& V1,
                float w0, float w1)
{
    std::cout << "Inserting \"" << E << "\" (w0: "
              << w0 << ", w1: " << w1 << ") to "<< V0 << "->" << V1 << std::endl;
    graph.AddEdge(E, V0, V1, w0, w1);
}

int main()
{
    std::cout << "Testing Max Depth Via Edge" << std::endl;
    std::cout << std::endl;

    MultiGraph graph;

    std::cout << "Inserting \"A\"" << std::endl;
    graph.InsertVertex("A");
    std::cout << "Inserting \"B\"" << std::endl;
    graph.InsertVertex("B");
    std::cout << "Inserting \"C\"" << std::endl;
    graph.InsertVertex("C");
    std::cout << "Inserting \"D\"" << std::endl;
    graph.InsertVertex("D");
    std::cout << "Inserting \"E\"" << std::endl;
    graph.InsertVertex("E");
    std::cout << "Inserting \"F\"" << std::endl;
    graph.InsertVertex("F");

    InsertEdge(graph, "E01", "A", "B", 2.3f, 4.12f);
    InsertEdge(graph, "E02", "A", "B", 2.3f, 4.12f);
    InsertEdge(graph, "E02", "B", "C", 1.2f, 2.12f);
    InsertEdge(graph, "E02", "C", "E", 0.2f, 0.02f);
    InsertEdge(graph, "E01", "E", "F", 0.1f, 4.122f);

    graph.PrintEntireGraph();

    std::cout << "Calling max depth via edge name \"A\" and \"E01\"" << std::endl;
    int depth = graph.MaxDepthViaEdgeName("A", "E01");
    std::cout << "Return value: " << depth << std::endl;

    std::cout << "-------------------------------" << std::endl;

    std::cout << "Calling max depth via edge name \"A\" and \"E02\"" << std::endl;
    depth = graph.MaxDepthViaEdgeName("A", "E02");
    std::cout << "Return value: " << depth << std::endl;

	return 0;
}