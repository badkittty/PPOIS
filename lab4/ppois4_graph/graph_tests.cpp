#include <gtest/gtest.h>
#include "DirectedGraph.h"
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <sstream>


struct CustomVertex {
    int id;
    std::string name;

    CustomVertex(int i = 0, std::string n = "") : id(i), name(n) {}

    bool operator==(const CustomVertex& other) const {
        return id == other.id && name == other.name;
    }

    bool operator<(const CustomVertex& other) const {
        if (id != other.id) return id < other.id;
        return name < other.name;
    }

    friend std::ostream& operator<<(std::ostream& os, const CustomVertex& cv) {
        os << "CV(" << cv.id << ", " << cv.name << ")";
        return os;
    }
};

template<>
struct VertexTraits<CustomVertex> {
    using value_type = CustomVertex;
    using reference = CustomVertex&;
    using const_reference = const CustomVertex&;
    using pointer = CustomVertex*;
    using const_pointer = const CustomVertex*;

    static CustomVertex default_value() { return CustomVertex(0, "default"); }
};


TEST(EdgeTest, ConstructorAndGetters) {
    Edge<int> edge1(1, 2);
    EXPECT_EQ(edge1.getFrom(), 1);
    EXPECT_EQ(edge1.getTo(), 2);
    EXPECT_EQ(edge1.getFromVertex(), nullptr);
    EXPECT_EQ(edge1.getToVertex(), nullptr);

    int from = 10, to = 20;
    Edge<int> edge2(1, 2, &from, &to);
    EXPECT_EQ(edge2.getFrom(), 1);
    EXPECT_EQ(edge2.getTo(), 2);
    EXPECT_EQ(edge2.getFromVertex(), &from);
    EXPECT_EQ(edge2.getToVertex(), &to);
}

TEST(EdgeTest, ComparisonOperators) {
    Edge<int> edge1(1, 2);
    Edge<int> edge2(1, 2);
    Edge<int> edge3(1, 3);
    Edge<int> edge4(2, 1);

    EXPECT_TRUE(edge1 == edge2);
    EXPECT_FALSE(edge1 == edge3);

    EXPECT_FALSE(edge1 != edge2);
    EXPECT_TRUE(edge1 != edge3);

    EXPECT_TRUE(edge1 < edge3);  // (1,2) < (1,3)
    EXPECT_TRUE(edge1 < edge4);  // (1,2) < (2,1)

    EXPECT_TRUE(edge1 <= edge2);
    EXPECT_TRUE(edge1 <= edge3);
    EXPECT_FALSE(edge3 <= edge1);

    EXPECT_TRUE(edge3 > edge1);
    EXPECT_TRUE(edge4 > edge1);

    EXPECT_TRUE(edge1 >= edge2);
    EXPECT_FALSE(edge1 >= edge3);
    EXPECT_TRUE(edge3 >= edge1);
}

TEST(EdgeTest, OutputOperator) {
    Edge<int> edge1(1, 2);
    std::stringstream ss1;
    ss1 << edge1;
    EXPECT_EQ(ss1.str(), "1 -> 2");

    int from = 10, to = 20;
    Edge<int> edge2(1, 2, &from, &to);
    std::stringstream ss2;
    ss2 << edge2;
    EXPECT_EQ(ss2.str(), "10 -> 20");
}


TEST(DirectedGraphTest, EmptyGraph) {
    DirectedGraph<int> graph;

    EXPECT_TRUE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);

    // Проверка итераторов на пустом графе
    EXPECT_EQ(graph.begin_vertex(), graph.end_vertex());
    EXPECT_EQ(graph.begin_edge(), graph.end_edge());
    EXPECT_EQ(graph.cbegin_vertex(), graph.cend_vertex());
    EXPECT_EQ(graph.cbegin_edge(), graph.cend_edge());
}

TEST(DirectedGraphTest, Clear) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(0, 1);

    EXPECT_FALSE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 2);
    EXPECT_EQ(graph.edgeCount(), 1);

    graph.clear();

    EXPECT_TRUE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);
}


TEST(DirectedGraphTest, AddVertices) {
    DirectedGraph<std::string> graph;

    auto v0 = graph.addVertex("A");
    auto v1 = graph.addVertex("B");
    auto v2 = graph.addVertex("C");

    EXPECT_EQ(v0, 0);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);

    EXPECT_EQ(graph.vertexCount(), 3);
    EXPECT_FALSE(graph.empty());
    EXPECT_EQ(graph.getVertex(0), "A");
    EXPECT_EQ(graph.getVertex(1), "B");
    EXPECT_EQ(graph.getVertex(2), "C");
}

TEST(DirectedGraphTest, HasVertex) {
    DirectedGraph<int> graph;

    graph.addVertex(10);
    graph.addVertex(20);

    EXPECT_TRUE(graph.hasVertex(0));
    EXPECT_TRUE(graph.hasVertex(1));
    EXPECT_FALSE(graph.hasVertex(2));
    EXPECT_FALSE(graph.hasVertex(100));
}

TEST(DirectedGraphTest, GetVertex) {
    DirectedGraph<int> graph;
    graph.addVertex(10);
    graph.addVertex(20);

    EXPECT_EQ(graph.getVertex(0), 10);
    EXPECT_EQ(graph.getVertex(1), 20);

    // Проверка константной версии
    const DirectedGraph<int>& constGraph = graph;
    EXPECT_EQ(constGraph.getVertex(0), 10);
    EXPECT_EQ(constGraph.getVertex(1), 20);
}

TEST(DirectedGraphTest, RemoveVertexByIndex) {
    DirectedGraph<int> graph;
    graph.addVertex(1); // 0
    graph.addVertex(2); // 1
    graph.addVertex(3); // 2
    graph.addVertex(4); // 3

    graph.addEdge(0, 1);  // 1->2
    graph.addEdge(1, 2);  // 2->3
    graph.addEdge(2, 3);  // 3->4
    graph.addEdge(3, 0);  // 4->1
    graph.addEdge(0, 2);  // 1->3

    EXPECT_EQ(graph.vertexCount(), 4);
    EXPECT_EQ(graph.edgeCount(), 5);

    graph.removeVertex(1);

    EXPECT_EQ(graph.vertexCount(), 3);
    EXPECT_EQ(graph.getVertex(0), 1);
    EXPECT_EQ(graph.getVertex(1), 3);
    EXPECT_EQ(graph.getVertex(2), 4);

    EXPECT_TRUE(graph.hasEdge(1, 2));

    EXPECT_TRUE(graph.hasEdge(2, 0));

    EXPECT_TRUE(graph.hasEdge(0, 1));

    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_FALSE(graph.hasEdge(0, 2));

    EXPECT_EQ(graph.edgeCount(), 3);
}


TEST(DirectedGraphTest, AddAndRemoveEdges) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 2));
    EXPECT_TRUE(graph.hasEdge(2, 0));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_FALSE(graph.hasEdge(1, 0));
    EXPECT_FALSE(graph.hasEdge(2, 1));

    EXPECT_EQ(graph.edgeCount(), 3);

    graph.removeEdge(0, 1);
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.edgeCount(), 2);
}

TEST(DirectedGraphTest, EdgeCount) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    EXPECT_EQ(graph.edgeCount(), 0);

    graph.addEdge(0, 1);
    EXPECT_EQ(graph.edgeCount(), 1);

    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    EXPECT_EQ(graph.edgeCount(), 3);

    graph.removeEdge(1, 2);
    EXPECT_EQ(graph.edgeCount(), 2);

    graph.addEdge(0, 1);
    EXPECT_EQ(graph.edgeCount(), 2);
}


TEST(DirectedGraphTest, VertexDegrees) {
    DirectedGraph<char> graph;

    graph.addVertex('A'); // 0
    graph.addVertex('B'); // 1
    graph.addVertex('C'); // 2
    graph.addVertex('D'); // 3

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);
    graph.addEdge(3, 2);

    EXPECT_EQ(graph.outDegree(0), 2);
    EXPECT_EQ(graph.inDegree(0), 2);
    EXPECT_EQ(graph.degree(0), 4);

    EXPECT_EQ(graph.outDegree(1), 1);
    EXPECT_EQ(graph.inDegree(1), 1);
    EXPECT_EQ(graph.degree(1), 2);

    EXPECT_EQ(graph.outDegree(2), 1);
    EXPECT_EQ(graph.inDegree(2), 2);
    EXPECT_EQ(graph.degree(2), 3);

    EXPECT_EQ(graph.outDegree(3), 2);
    EXPECT_EQ(graph.inDegree(3), 1);
    EXPECT_EQ(graph.degree(3), 3);
}

TEST(DirectedGraphTest, DegreesWithIsolatedVertex) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.outDegree(2), 0);
    EXPECT_EQ(graph.inDegree(2), 0);
    EXPECT_EQ(graph.degree(2), 0);
}


TEST(DirectedGraphTest, VertexIterators) {
    DirectedGraph<int> graph;

    graph.addVertex(10);
    graph.addVertex(20);
    graph.addVertex(30);

    std::vector<int> vertices;
    for (auto it = graph.begin_vertex(); it != graph.end_vertex(); ++it) {
        vertices.push_back(*it);
    }

    EXPECT_EQ(vertices.size(), 3);
    EXPECT_EQ(vertices[0], 10);
    EXPECT_EQ(vertices[1], 20);
    EXPECT_EQ(vertices[2], 30);

    std::vector<int> constVertices;
    for (auto it = graph.cbegin_vertex(); it != graph.cend_vertex(); ++it) {
        constVertices.push_back(*it);
    }

    EXPECT_EQ(constVertices, vertices);
}

TEST(DirectedGraphTest, VertexIteratorOperations) {
    DirectedGraph<int> graph;

    graph.addVertex(10);
    graph.addVertex(20);
    graph.addVertex(30);
    graph.addVertex(40);
    graph.addVertex(50);

    auto it = graph.begin_vertex();

    EXPECT_EQ(*it, 10);
    EXPECT_EQ(it[0], 10);
    EXPECT_EQ(it[1], 20);
    EXPECT_EQ(it[4], 50);

    ++it;
    EXPECT_EQ(*it, 20);

    it += 2;
    EXPECT_EQ(*it, 40);

    it--;
    EXPECT_EQ(*it, 30);

    auto it2 = it + 1;
    EXPECT_EQ(*it2, 40);

    EXPECT_EQ(it2 - it, 1);

    EXPECT_TRUE(it < it2);
    EXPECT_TRUE(it <= it2);
    EXPECT_TRUE(it2 > it);
    EXPECT_TRUE(it2 >= it);
    EXPECT_FALSE(it == it2);
    EXPECT_TRUE(it != it2);
}

TEST(DirectedGraphTest, ConstVertexIterators) {
    DirectedGraph<int> graph;
    graph.addVertex(10);
    graph.addVertex(20);
    graph.addVertex(30);

    const DirectedGraph<int>& constGraph = graph;

    std::vector<int> vertices;
    for (auto it = constGraph.begin_vertex(); it != constGraph.end_vertex(); ++it) {
        vertices.push_back(*it);
    }

    EXPECT_EQ(vertices.size(), 3);
    EXPECT_EQ(vertices[0], 10);
    EXPECT_EQ(vertices[1], 20);
    EXPECT_EQ(vertices[2], 30);
}

TEST(DirectedGraphTest, ReverseVertexIterators) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    std::vector<int> reverseVertices;
    for (auto it = graph.rbegin_vertex(); it != graph.rend_vertex(); ++it) {
        reverseVertices.push_back(*it);
    }

    EXPECT_EQ(reverseVertices.size(), 3);
    EXPECT_EQ(reverseVertices[0], 3);
    EXPECT_EQ(reverseVertices[1], 2);
    EXPECT_EQ(reverseVertices[2], 1);
}

TEST(DirectedGraphTest, ConstReverseVertexIterators) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    const DirectedGraph<int>& constGraph = graph;

    std::vector<int> reverseVertices;
    for (auto it = constGraph.rbegin_vertex(); it != constGraph.rend_vertex(); ++it) {
        reverseVertices.push_back(*it);
    }

    EXPECT_EQ(reverseVertices.size(), 3);
    EXPECT_EQ(reverseVertices[0], 3);
    EXPECT_EQ(reverseVertices[1], 2);
    EXPECT_EQ(reverseVertices[2], 1);
}


TEST(DirectedGraphTest, EdgeIterators) {
    DirectedGraph<char> graph;

    graph.addVertex('A');
    graph.addVertex('B');
    graph.addVertex('C');

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);

    std::vector<Edge<char>> edges;
    for (auto it = graph.begin_edge(); it != graph.end_edge(); ++it) {
        edges.push_back(*it);
    }

    EXPECT_EQ(edges.size(), 3);

    bool has01 = false, has12 = false, has20 = false;
    for (const auto& edge : edges) {
        if (edge.getFrom() == 0 && edge.getTo() == 1) has01 = true;
        if (edge.getFrom() == 1 && edge.getTo() == 2) has12 = true;
        if (edge.getFrom() == 2 && edge.getTo() == 0) has20 = true;
    }

    EXPECT_TRUE(has01);
    EXPECT_TRUE(has12);
    EXPECT_TRUE(has20);
}

TEST(DirectedGraphTest, ConstEdgeIterators) {
    DirectedGraph<char> graph;

    graph.addVertex('A');
    graph.addVertex('B');
    graph.addVertex('C');

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);

    const DirectedGraph<char>& constGraph = graph;

    std::vector<Edge<char>> edges;
    for (auto it = constGraph.begin_edge(); it != constGraph.end_edge(); ++it) {
        edges.push_back(*it);
    }

    EXPECT_EQ(edges.size(), 3);
}

TEST(DirectedGraphTest, EdgeIteratorOperations) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);

    auto it = graph.begin_edge();

    EXPECT_EQ((*it).getFrom(), 0);
    EXPECT_EQ((*it).getTo(), 1);

    ++it;
    EXPECT_EQ((*it).getFrom(), 0);
    EXPECT_EQ((*it).getTo(), 2);

    auto it2 = it;
    ++it2;
    EXPECT_NE(it, it2);

    EXPECT_EQ(graph.end_edge(), graph.end_edge());
}

TEST(DirectedGraphTest, EdgeIteratorsEmptyGraph) {
    DirectedGraph<int> graph;

    EXPECT_EQ(graph.begin_edge(), graph.end_edge());

    graph.addVertex(1);
    EXPECT_EQ(graph.begin_edge(), graph.end_edge());
}


TEST(DirectedGraphTest, AdjacentIterators) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    graph.addVertex(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);

    std::vector<size_t> adjacent;
    for (auto it = graph.begin_adjacent(0); it != graph.end_adjacent(0); ++it) {
        adjacent.push_back(*it);
    }

    EXPECT_EQ(adjacent.size(), 3);

    std::sort(adjacent.begin(), adjacent.end());
    EXPECT_EQ(adjacent[0], 1);
    EXPECT_EQ(adjacent[1], 2);
    EXPECT_EQ(adjacent[2], 3);

    std::vector<size_t> adjacent2;
    for (auto it = graph.begin_adjacent(2); it != graph.end_adjacent(2); ++it) {
        adjacent2.push_back(*it);
    }

    EXPECT_EQ(adjacent2.size(), 1);
    EXPECT_EQ(adjacent2[0], 4);

    std::vector<size_t> adjacent3;
    for (auto it = graph.begin_adjacent(4); it != graph.end_adjacent(4); ++it) {
        adjacent3.push_back(*it);
    }

    EXPECT_TRUE(adjacent3.empty());
}

TEST(DirectedGraphTest, ConstAdjacentIterators) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);

    const DirectedGraph<int>& constGraph = graph;

    std::vector<size_t> adjacent;
    for (auto it = constGraph.begin_adjacent(0); it != constGraph.end_adjacent(0); ++it) {
        adjacent.push_back(*it);
    }

    EXPECT_EQ(adjacent.size(), 2);
}

TEST(DirectedGraphTest, AdjacentIteratorOperations) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);

    auto it = graph.begin_adjacent(0);

    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    auto it2 = it;
    ++it2;
    EXPECT_EQ(*it2, 3);

    EXPECT_NE(it, it2);
}

TEST(DirectedGraphTest, RemoveVertexByIterator2) {
    DirectedGraph<std::string> graph;

    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    graph.addVertex("D");

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);

    auto it = graph.begin_vertex();
    ++it;

    auto newIt = graph.removeVertex(it);

    EXPECT_EQ(graph.vertexCount(), 3);
    EXPECT_EQ(graph.getVertex(0), "A");
    EXPECT_EQ(graph.getVertex(1), "C");
    EXPECT_EQ(graph.getVertex(2), "D");

    EXPECT_EQ(*newIt, "C");
}

TEST(DirectedGraphTest, RemoveEdgeByIterator) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);

    auto it = graph.begin_edge();
    EXPECT_EQ((*it).getFrom(), 0);
    EXPECT_EQ((*it).getTo(), 1);

    auto newIt = graph.removeEdge(it);

    EXPECT_EQ(graph.edgeCount(), 2);
    EXPECT_FALSE(graph.hasEdge(0, 1));

    EXPECT_EQ((*newIt).getFrom(), 0);
    EXPECT_EQ((*newIt).getTo(), 2);
}


TEST(DirectedGraphTest, ComparisonOperators) {
    DirectedGraph<int> graph1;
    graph1.addVertex(1);
    graph1.addVertex(2);
    graph1.addEdge(0, 1);

    DirectedGraph<int> graph2;
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addEdge(0, 1);

    DirectedGraph<int> graph3;
    graph3.addVertex(1);
    graph3.addVertex(3);
    graph3.addEdge(0, 1);

    DirectedGraph<int> graph4;
    graph4.addVertex(1);
    graph4.addVertex(2);
    graph4.addVertex(3);
    graph4.addEdge(0, 1);

    EXPECT_TRUE(graph1 == graph2);
    EXPECT_FALSE(graph1 == graph3);
    EXPECT_FALSE(graph1 == graph4);

    EXPECT_FALSE(graph1 != graph2);
    EXPECT_TRUE(graph1 != graph3);
    EXPECT_TRUE(graph1 != graph4);

    EXPECT_FALSE(graph1 < graph2);
    EXPECT_FALSE(graph2 < graph1);
    EXPECT_TRUE(graph1 < graph3);
    EXPECT_TRUE(graph1 < graph4);

    EXPECT_TRUE(graph1 <= graph2);
    EXPECT_TRUE(graph1 <= graph3);
    EXPECT_FALSE(graph3 <= graph1);

    EXPECT_FALSE(graph1 > graph2);
    EXPECT_TRUE(graph3 > graph1);
    EXPECT_TRUE(graph4 > graph1);

    EXPECT_TRUE(graph1 >= graph2);
    EXPECT_FALSE(graph1 >= graph3);
    EXPECT_TRUE(graph3 >= graph1);
}

TEST(DirectedGraphTest, CopyConstructor) {
    DirectedGraph<std::string> original;
    original.addVertex("A");
    original.addVertex("B");
    original.addVertex("C");
    original.addEdge(0, 1);
    original.addEdge(1, 2);

    DirectedGraph<std::string> copy = original;

    EXPECT_EQ(copy.vertexCount(), original.vertexCount());
    EXPECT_EQ(copy.edgeCount(), original.edgeCount());
    EXPECT_EQ(copy.getVertex(0), original.getVertex(0));
    EXPECT_EQ(copy.getVertex(1), original.getVertex(1));
    EXPECT_EQ(copy.getVertex(2), original.getVertex(2));
    EXPECT_TRUE(copy.hasEdge(0, 1));
    EXPECT_TRUE(copy.hasEdge(1, 2));

    original.addVertex("D");
    EXPECT_EQ(original.vertexCount(), 4);
    EXPECT_EQ(copy.vertexCount(), 3);
}

TEST(DirectedGraphTest, AssignmentOperator) {
    DirectedGraph<std::string> original;
    original.addVertex("A");
    original.addVertex("B");
    original.addVertex("C");
    original.addEdge(0, 1);
    original.addEdge(1, 2);

    DirectedGraph<std::string> assigned;
    assigned.addVertex("X");
    assigned.addVertex("Y");
    assigned.addEdge(0, 1);

    assigned = original;

    EXPECT_EQ(assigned.vertexCount(), original.vertexCount());
    EXPECT_EQ(assigned.edgeCount(), original.edgeCount());
    EXPECT_EQ(assigned.getVertex(0), original.getVertex(0));
    EXPECT_EQ(assigned.getVertex(1), original.getVertex(1));
    EXPECT_EQ(assigned.getVertex(2), original.getVertex(2));
    EXPECT_TRUE(assigned.hasEdge(0, 1));
    EXPECT_TRUE(assigned.hasEdge(1, 2));

    assigned = assigned;
    EXPECT_EQ(assigned.vertexCount(), 3);
    EXPECT_EQ(assigned.edgeCount(), 2);
}


TEST(DirectedGraphTest, OutputOperator) {
    DirectedGraph<int> graph;
    graph.addVertex(100);
    graph.addVertex(200);
    graph.addVertex(300);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    std::stringstream ss;
    ss << graph;

    std::string output = ss.str();

    EXPECT_NE(output.find("Directed Graph"), std::string::npos);
    EXPECT_NE(output.find("3 vertices"), std::string::npos);
    EXPECT_NE(output.find("2 edges"), std::string::npos);
    EXPECT_NE(output.find("100"), std::string::npos);
    EXPECT_NE(output.find("200"), std::string::npos);
    EXPECT_NE(output.find("300"), std::string::npos);
    EXPECT_NE(output.find("Edges:"), std::string::npos);

    bool hasEdges = (output.find("100 -> 200") != std::string::npos) ||
                    (output.find("0 -> 1") != std::string::npos);
    EXPECT_TRUE(hasEdges) << "Output: " << output;
}

TEST(DirectedGraphTest, OutputOperatorEmptyGraph) {
    DirectedGraph<int> graph;

    std::stringstream ss;
    ss << graph;

    std::string output = ss.str();
}


TEST(DirectedGraphTest, Exceptions) {
    DirectedGraph<double> graph;
    graph.addVertex(1.5);
    graph.addVertex(2.5);

    EXPECT_THROW(graph.getVertex(5), std::out_of_range);
    EXPECT_THROW(graph.getVertex(2), std::out_of_range);

    EXPECT_THROW(graph.addEdge(0, 5), std::out_of_range);
    EXPECT_THROW(graph.addEdge(5, 0), std::out_of_range);

    EXPECT_THROW(graph.removeEdge(0, 5), std::out_of_range);
    EXPECT_THROW(graph.removeEdge(5, 0), std::out_of_range);

    EXPECT_THROW(graph.outDegree(5), std::out_of_range);
    EXPECT_THROW(graph.inDegree(5), std::out_of_range);

    EXPECT_THROW(graph.removeVertex(5), std::out_of_range);

    EXPECT_NO_THROW(graph.hasEdge(0, 5));
    EXPECT_NO_THROW(graph.hasEdge(5, 0));
    EXPECT_FALSE(graph.hasEdge(0, 5));
}

TEST(DirectedGraphTest, WithCustomTraits) {
    DirectedGraph<CustomVertex, VertexTraits<CustomVertex>> graph;

    auto v0 = graph.addVertex(CustomVertex(1, "Alice"));
    auto v1 = graph.addVertex(CustomVertex(2, "Bob"));
    auto v2 = graph.addVertex(CustomVertex(3, "Charlie"));

    EXPECT_EQ(v0, 0);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);

    EXPECT_EQ(graph.getVertex(0).id, 1);
    EXPECT_EQ(graph.getVertex(0).name, "Alice");
    EXPECT_EQ(graph.getVertex(1).id, 2);
    EXPECT_EQ(graph.getVertex(1).name, "Bob");
    EXPECT_EQ(graph.getVertex(2).id, 3);
    EXPECT_EQ(graph.getVertex(2).name, "Charlie");

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 2));
    EXPECT_FALSE(graph.hasEdge(0, 2));
}


TEST(DirectedGraphTest, DifferentDataTypes) {
    DirectedGraph<int> intGraph;
    intGraph.addVertex(1);
    intGraph.addVertex(2);
    intGraph.addEdge(0, 1);
    EXPECT_EQ(intGraph.edgeCount(), 1);

    DirectedGraph<std::string> stringGraph;
    stringGraph.addVertex("A");
    stringGraph.addVertex("B");
    stringGraph.addEdge(0, 1);
    EXPECT_EQ(stringGraph.edgeCount(), 1);

    DirectedGraph<double> doubleGraph;
    doubleGraph.addVertex(1.5);
    doubleGraph.addVertex(2.5);
    doubleGraph.addEdge(0, 1);
    EXPECT_EQ(doubleGraph.edgeCount(), 1);
}

TEST(DirectedGraphTest, ComplexScenario) {
    DirectedGraph<int> graph;

    for (int i = 0; i < 10; ++i) {
        graph.addVertex(i * 10);
    }

    EXPECT_EQ(graph.vertexCount(), 10);

    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (i != j) {
                graph.addEdge(i, j);
            }
        }
    }

    EXPECT_EQ(graph.edgeCount(), 90);

    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(graph.outDegree(i), 9);
        EXPECT_EQ(graph.inDegree(i), 9);
        EXPECT_EQ(graph.degree(i), 18);
    }

    for (int i = 9; i >= 5; --i) {
        graph.removeVertex(i);
    }

    EXPECT_EQ(graph.vertexCount(), 5);

    EXPECT_LT(graph.edgeCount(), 90);
}

TEST(DirectedGraphTest, SelfLoops) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);

    graph.addEdge(0, 0);
    graph.addEdge(1, 1);

    EXPECT_TRUE(graph.hasEdge(0, 0));
    EXPECT_TRUE(graph.hasEdge(1, 1));
    EXPECT_EQ(graph.edgeCount(), 2);

    EXPECT_EQ(graph.outDegree(0), 1);
    EXPECT_EQ(graph.inDegree(0), 1);
    EXPECT_EQ(graph.degree(0), 2);
}


TEST(DirectedGraphTest, LargeGraph) {
    DirectedGraph<int> graph;

    const int N = 100;

    for (int i = 0; i < N; ++i) {
        graph.addVertex(i);
    }

    EXPECT_EQ(graph.vertexCount(), N);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j += 2) {
            if (i != j) {
                graph.addEdge(i, j);
            }
        }
    }

    EXPECT_GT(graph.edgeCount(), 0);

    int vertexCount = 0;
    for (auto it = graph.begin_vertex(); it != graph.end_vertex(); ++it) {
        ++vertexCount;
    }
    EXPECT_EQ(vertexCount, N);

    int edgeCount = 0;
    for (auto it = graph.begin_edge(); it != graph.end_edge(); ++it) {
        ++edgeCount;
    }
    EXPECT_EQ(edgeCount, graph.edgeCount());
}


TEST(DirectedGraphTest, LargeGraphBoundaryConditions) {
    DirectedGraph<int> graph;

    for (int i = 0; i < 1000; ++i) {
        graph.addVertex(i);
    }

    EXPECT_THROW(graph.getVertex(1000), std::out_of_range);
    EXPECT_THROW(graph.addEdge(999, 1000), std::out_of_range);
    EXPECT_NO_THROW(graph.hasEdge(999, 1000));
}

TEST(DirectedGraphTest, ComplexIteratorOperations) {
    DirectedGraph<int> graph;

    for (int i = 0; i < 10; ++i) {
        graph.addVertex(i);
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if ((i + j) % 3 == 0) {
                graph.addEdge(i, j);
            }
        }
    }

    auto v_it = graph.begin_vertex();
    auto v_end = graph.end_vertex();

    auto middle = v_it + 5;
    EXPECT_EQ(*middle, 5);

    EXPECT_TRUE(v_it < middle);
    EXPECT_TRUE(middle > v_it);
}

TEST(DirectedGraphTest, AllComparisonOperators) {
    DirectedGraph<int> empty1;
    DirectedGraph<int> empty2;

    EXPECT_TRUE(empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);
    EXPECT_FALSE(empty1 < empty2);
    EXPECT_FALSE(empty1 > empty2);
    EXPECT_TRUE(empty1 <= empty2);
    EXPECT_TRUE(empty1 >= empty2);

    DirectedGraph<int> graph1;
    graph1.addVertex(1);
    graph1.addVertex(2);
    graph1.addEdge(0, 1);

    DirectedGraph<int> graph2;
    graph2.addVertex(1);
    graph2.addVertex(3);
    graph2.addEdge(0, 1);

    EXPECT_FALSE(graph1 == graph2);
    EXPECT_TRUE(graph1 != graph2);
    EXPECT_TRUE(graph1 < graph2);    // 2 < 3
    EXPECT_FALSE(graph1 > graph2);
    EXPECT_TRUE(graph1 <= graph2);
    EXPECT_FALSE(graph1 >= graph2);

    DirectedGraph<int> graph3;
    graph3.addVertex(1);
    graph3.addVertex(2);

    EXPECT_FALSE(graph1 == graph3);
    EXPECT_TRUE(graph1 != graph3);
}

TEST(DirectedGraphTest, IteratorBoundaryConditions) {
    DirectedGraph<int> graph;

    EXPECT_EQ(graph.begin_vertex(), graph.end_vertex());
    EXPECT_EQ(graph.begin_edge(), graph.end_edge());
    EXPECT_EQ(graph.cbegin_vertex(), graph.cend_vertex());
    EXPECT_EQ(graph.cbegin_edge(), graph.cend_edge());

    graph.addVertex(1);
    EXPECT_EQ(graph.begin_adjacent(0), graph.end_adjacent(0));
    EXPECT_EQ(graph.cbegin_adjacent(0), graph.cend_adjacent(0));

    graph.addVertex(2);
    graph.addVertex(3);

    auto it = graph.begin_vertex();
    ++it; ++it;
    --it;
    EXPECT_EQ(*it, 2);

    --it;
    EXPECT_EQ(*it, 1);

    EXPECT_EQ(it[0], 1);
    EXPECT_EQ(it[1], 2);
    EXPECT_EQ(it[2], 3);

    auto it2 = it + 2;
    EXPECT_EQ(*it2, 3);

    auto it3 = it2 - 1;
    EXPECT_EQ(*it3, 2);

    EXPECT_EQ(it2 - it, 2);
}

TEST(DirectedGraphTest, RemoveVertexAtEndIterator) {
    DirectedGraph<std::string> graph;

    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");

    auto end_it = graph.end_vertex();

    auto new_it = graph.removeVertex(end_it);
    EXPECT_EQ(new_it, end_it);
    EXPECT_EQ(graph.vertexCount(), 3);
}

TEST(DirectedGraphTest, RemoveEdgeAtEndIterator) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(0, 1);

    auto it = graph.begin_edge();
    auto end_it = graph.end_edge();

    auto new_it = graph.removeEdge(it);
    EXPECT_EQ(new_it, end_it);
    EXPECT_EQ(graph.edgeCount(), 0);

    new_it = graph.removeEdge(end_it);
    EXPECT_EQ(new_it, end_it);
}

TEST(DirectedGraphTest, HasEdgeBoundaryConditions) {
    DirectedGraph<int> graph;

    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 0));

    graph.addVertex(1);
    graph.addVertex(2);

    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_FALSE(graph.hasEdge(2, 0));
    EXPECT_FALSE(graph.hasEdge(2, 2));


    graph.addEdge(0, 1);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 0));
}

TEST(DirectedGraphTest, MatrixComparisonInOperators) {
    DirectedGraph<int> graph1;
    graph1.addVertex(1);
    graph1.addVertex(2);
    graph1.addVertex(3);

    graph1.addEdge(0, 1);
    graph1.addEdge(1, 2);

    DirectedGraph<int> graph2;
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addVertex(3);

    graph2.addEdge(0, 1);

    EXPECT_FALSE(graph1 == graph2);
    EXPECT_TRUE(graph1 != graph2);

    EXPECT_FALSE(graph1 < graph2);
    EXPECT_TRUE(graph2 < graph1);
}

TEST(DirectedGraphTest, DegreeWithSelfLoops) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    graph.addEdge(0, 0);
    graph.addEdge(1, 1);
    graph.addEdge(0, 1);
    graph.addEdge(1, 0);
    graph.addEdge(2, 0);

    EXPECT_EQ(graph.outDegree(0), 2);
    EXPECT_EQ(graph.inDegree(0), 3);
    EXPECT_EQ(graph.degree(0), 5);

    EXPECT_EQ(graph.outDegree(1), 2);
    EXPECT_EQ(graph.inDegree(1), 2);
    EXPECT_EQ(graph.degree(1), 4);

    EXPECT_EQ(graph.outDegree(2), 1);
    EXPECT_EQ(graph.inDegree(2), 0);
    EXPECT_EQ(graph.degree(2), 1);
}

TEST(DirectedGraphTest, IteratorFindMethods) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);

    auto edge_it = graph.begin_edge();
    auto edge_end = graph.end_edge();
    EXPECT_EQ(edge_it, edge_end);

    graph.addEdge(0, 2);
    graph.addEdge(2, 1);

    edge_it = graph.begin_edge();
    EXPECT_EQ((*edge_it).getFrom(), 0);
    EXPECT_EQ((*edge_it).getTo(), 2);

    ++edge_it;
    EXPECT_EQ((*edge_it).getFrom(), 2);
    EXPECT_EQ((*edge_it).getTo(), 1);

    ++edge_it;
    EXPECT_EQ(edge_it, edge_end);

    graph.addEdge(0, 1);

    auto adj_it = graph.begin_adjacent(0);
    EXPECT_EQ(*adj_it, 1);

    ++adj_it;
    EXPECT_EQ(*adj_it, 2);

    ++adj_it;
    EXPECT_EQ(adj_it, graph.end_adjacent(0));
}

TEST(DirectedGraphTest, DeepCopyConstructor) {
    DirectedGraph<std::string> original;

    original.addVertex("A");
    original.addVertex("B");
    original.addVertex("C");

    original.addEdge(0, 1);
    original.addEdge(1, 2);
    original.addEdge(2, 0);

    DirectedGraph<std::string> copy = original;

    original.addVertex("D");
    original.addEdge(0, 3);

    EXPECT_EQ(original.vertexCount(), 4);
    EXPECT_EQ(copy.vertexCount(), 3);

    EXPECT_EQ(original.edgeCount(), 4);
    EXPECT_EQ(copy.edgeCount(), 3);

    EXPECT_TRUE(copy.hasEdge(0, 1));
    EXPECT_TRUE(copy.hasEdge(1, 2));
    EXPECT_TRUE(copy.hasEdge(2, 0));
    EXPECT_FALSE(copy.hasEdge(0, 3));
}

TEST(DirectedGraphTest, ComprehensiveCoverage) {
    DirectedGraph<int> graph;

    EXPECT_TRUE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);

    for (int i = 0; i < 5; ++i) {
        graph.addVertex(i * 10);
    }

    EXPECT_EQ(graph.vertexCount(), 5);

    graph.addEdge(0, 0);
    graph.addEdge(1, 1);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 0);

    graph.addEdge(1, 0);
    graph.addEdge(2, 1);

    EXPECT_FALSE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 5);
    EXPECT_GT(graph.edgeCount(), 0);

    int vertex_sum = 0;
    for (auto it = graph.begin_vertex(); it != graph.end_vertex(); ++it) {
        vertex_sum += *it;
    }
    EXPECT_EQ(vertex_sum, 100);

    graph.removeVertex(2);

    EXPECT_EQ(graph.vertexCount(), 4);

    graph.clear();
    EXPECT_TRUE(graph.empty());
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}