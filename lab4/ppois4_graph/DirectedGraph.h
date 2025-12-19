#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <utility>
#include <functional>

template<typename T>
struct VertexTraits {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    static T default_value() { return T(); }
};

template<typename VertexType>
class Edge {
private:
    size_t from;
    size_t to;
    const VertexType* fromVertex;
    const VertexType* toVertex;

public:
    Edge(size_t f, size_t t, const VertexType* fv = nullptr, const VertexType* tv = nullptr)
        : from(f), to(t), fromVertex(fv), toVertex(tv) {}

    size_t getFrom() const { return from; }
    size_t getTo() const { return to; }

    const VertexType* getFromVertex() const { return fromVertex; }
    const VertexType* getToVertex() const { return toVertex; }

    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to;
    }

    bool operator!=(const Edge& other) const {
        return !(*this == other);
    }

    bool operator<(const Edge& other) const {
        if (from != other.from) return from < other.from;
        return to < other.to;
    }

    bool operator<=(const Edge& other) const {
        return !(other < *this);
    }

    bool operator>(const Edge& other) const {
        return other < *this;
    }

    bool operator>=(const Edge& other) const {
        return !(*this < other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        if (edge.fromVertex && edge.toVertex) {
            os << *edge.fromVertex << " -> " << *edge.toVertex;
        } else {
            os << edge.from << " -> " << edge.to;
        }
        return os;
    }
};

template<typename VertexType, typename Traits = VertexTraits<VertexType>>
class DirectedGraph {
public:
    using value_type = VertexType;
    using reference = typename Traits::reference;
    using const_reference = typename Traits::const_reference;
    using pointer = typename Traits::pointer;
    using const_pointer = typename Traits::const_pointer;
    using vertex_size_type = size_t;
    using edge_type = Edge<VertexType>;

    class vertex_iterator;
    class const_vertex_iterator;
    class edge_iterator;
    class const_edge_iterator;
    class adjacent_iterator;
    class const_adjacent_iterator;

private:
    std::vector<VertexType> vertices;
    std::vector<std::vector<bool>> adjacencyMatrix;

    void checkVertexIndex(vertex_size_type index) const {
        if (index >= vertices.size()) {
            throw std::out_of_range("Vertex index out of range: " + std::to_string(index));
        }
    }

public:


    DirectedGraph() = default;

    DirectedGraph(const DirectedGraph& other)
        : vertices(other.vertices), adjacencyMatrix(other.adjacencyMatrix) {}

    ~DirectedGraph() = default;


    bool empty() const noexcept {
        return vertices.empty();
    }

    void clear() noexcept {
        vertices.clear();
        adjacencyMatrix.clear();
    }


    DirectedGraph& operator=(const DirectedGraph& other) {
        if (this != &other) {
            vertices = other.vertices;
            adjacencyMatrix = other.adjacencyMatrix;
        }
        return *this;
    }


    bool operator==(const DirectedGraph& other) const {
        return vertices == other.vertices && adjacencyMatrix == other.adjacencyMatrix;
    }

    bool operator!=(const DirectedGraph& other) const {
        return !(*this == other);
    }

    bool operator<(const DirectedGraph& other) const {
        if (vertices != other.vertices) return vertices < other.vertices;
        return adjacencyMatrix < other.adjacencyMatrix;
    }

    bool operator<=(const DirectedGraph& other) const {
        return !(other < *this);
    }

    bool operator>(const DirectedGraph& other) const {
        return other < *this;
    }

    bool operator>=(const DirectedGraph& other) const {
        return !(*this < other);
    }


    vertex_size_type addVertex(const VertexType& vertex) {
        vertices.push_back(vertex);

        for (auto& row : adjacencyMatrix) {
            row.push_back(false);
        }

        adjacencyMatrix.emplace_back(vertices.size(), false);

        return vertices.size() - 1;
    }

    bool hasVertex(vertex_size_type index) const {
        return index < vertices.size();
    }

    const_reference getVertex(vertex_size_type index) const {
        checkVertexIndex(index);
        return vertices[index];
    }

    reference getVertex(vertex_size_type index) {
        checkVertexIndex(index);
        return vertices[index];
    }

    vertex_size_type vertexCount() const noexcept {
        return vertices.size();
    }


    void addEdge(vertex_size_type from, vertex_size_type to) {
        checkVertexIndex(from);
        checkVertexIndex(to);
        adjacencyMatrix[from][to] = true;
    }

    void removeEdge(vertex_size_type from, vertex_size_type to) {
        checkVertexIndex(from);
        checkVertexIndex(to);
        adjacencyMatrix[from][to] = false;
    }

    bool hasEdge(vertex_size_type from, vertex_size_type to) const {
        if (from >= vertices.size() || to >= vertices.size()) {
            return false;
        }
        return adjacencyMatrix[from][to];
    }

    vertex_size_type edgeCount() const {
        vertex_size_type count = 0;
        for (const auto& row : adjacencyMatrix) {
            count += std::count(row.begin(), row.end(), true);
        }
        return count;
    }


    vertex_size_type outDegree(vertex_size_type vertex) const {
        checkVertexIndex(vertex);
        return std::count(adjacencyMatrix[vertex].begin(),
                         adjacencyMatrix[vertex].end(), true);
    }

    vertex_size_type inDegree(vertex_size_type vertex) const {
        checkVertexIndex(vertex);
        vertex_size_type count = 0;
        for (vertex_size_type i = 0; i < vertices.size(); ++i) {
            if (adjacencyMatrix[i][vertex]) {
                ++count;
            }
        }
        return count;
    }

    vertex_size_type degree(vertex_size_type vertex) const {
        return outDegree(vertex) + inDegree(vertex);
    }


    void removeVertex(vertex_size_type index) {
        checkVertexIndex(index);

        vertices.erase(vertices.begin() + index);
        adjacencyMatrix.erase(adjacencyMatrix.begin() + index);

        for (auto& row : adjacencyMatrix) {
            row.erase(row.begin() + index);
        }
    }

    vertex_iterator removeVertex(vertex_iterator it) {
        if (it == end_vertex()) {
            return it;
        }
        vertex_size_type index = it - begin_vertex();
        removeVertex(index);
        return vertex_iterator(vertices.begin() + index);
    }

    edge_iterator removeEdge(edge_iterator it) {
        if (it == end_edge()) {
            return it;
        }
        auto edge = *it;
        removeEdge(edge.getFrom(), edge.getTo());
        return ++it;
    }


    class vertex_iterator {
    private:
        typename std::vector<VertexType>::iterator it;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = VertexType;
        using difference_type = std::ptrdiff_t;
        using pointer = typename Traits::pointer;
        using reference = typename Traits::reference;

        explicit vertex_iterator(typename std::vector<VertexType>::iterator iter) : it(iter) {}

        reference operator*() const { return *it; }
        pointer operator->() const { return &(*it); }

        vertex_iterator& operator++() { ++it; return *this; }
        vertex_iterator operator++(int) { vertex_iterator tmp = *this; ++it; return tmp; }

        vertex_iterator& operator--() { --it; return *this; }
        vertex_iterator operator--(int) { vertex_iterator tmp = *this; --it; return tmp; }

        bool operator==(const vertex_iterator& other) const { return it == other.it; }
        bool operator!=(const vertex_iterator& other) const { return it != other.it; }

        vertex_iterator& operator+=(difference_type n) { it += n; return *this; }
        vertex_iterator& operator-=(difference_type n) { it -= n; return *this; }

        vertex_iterator operator+(difference_type n) const { return vertex_iterator(it + n); }
        vertex_iterator operator-(difference_type n) const { return vertex_iterator(it - n); }

        difference_type operator-(const vertex_iterator& other) const { return it - other.it; }

        reference operator[](difference_type n) const { return it[n]; }

        bool operator<(const vertex_iterator& other) const { return it < other.it; }
        bool operator<=(const vertex_iterator& other) const { return it <= other.it; }
        bool operator>(const vertex_iterator& other) const { return it > other.it; }
        bool operator>=(const vertex_iterator& other) const { return it >= other.it; }
    };

    class const_vertex_iterator {
    private:
        typename std::vector<VertexType>::const_iterator it;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const VertexType;
        using difference_type = std::ptrdiff_t;
        using pointer = typename Traits::const_pointer;
        using reference = typename Traits::const_reference;

        explicit const_vertex_iterator(typename std::vector<VertexType>::const_iterator iter) : it(iter) {}

        reference operator*() const { return *it; }
        pointer operator->() const { return &(*it); }

        const_vertex_iterator& operator++() { ++it; return *this; }
        const_vertex_iterator operator++(int) { const_vertex_iterator tmp = *this; ++it; return tmp; }

        const_vertex_iterator& operator--() { --it; return *this; }
        const_vertex_iterator operator--(int) { const_vertex_iterator tmp = *this; --it; return tmp; }

        bool operator==(const const_vertex_iterator& other) const { return it == other.it; }
        bool operator!=(const const_vertex_iterator& other) const { return it != other.it; }

        const_vertex_iterator& operator+=(difference_type n) { it += n; return *this; }
        const_vertex_iterator& operator-=(difference_type n) { it -= n; return *this; }

        const_vertex_iterator operator+(difference_type n) const { return const_vertex_iterator(it + n); }
        const_vertex_iterator operator-(difference_type n) const { return const_vertex_iterator(it - n); }

        difference_type operator-(const const_vertex_iterator& other) const { return it - other.it; }

        reference operator[](difference_type n) const { return it[n]; }

        bool operator<(const const_vertex_iterator& other) const { return it < other.it; }
        bool operator<=(const const_vertex_iterator& other) const { return it <= other.it; }
        bool operator>(const const_vertex_iterator& other) const { return it > other.it; }
        bool operator>=(const const_vertex_iterator& other) const { return it >= other.it; }
    };

    vertex_iterator begin_vertex() { return vertex_iterator(vertices.begin()); }
    vertex_iterator end_vertex() { return vertex_iterator(vertices.end()); }
    const_vertex_iterator begin_vertex() const { return const_vertex_iterator(vertices.begin()); }
    const_vertex_iterator end_vertex() const { return const_vertex_iterator(vertices.end()); }
    const_vertex_iterator cbegin_vertex() const { return const_vertex_iterator(vertices.begin()); }
    const_vertex_iterator cend_vertex() const { return const_vertex_iterator(vertices.end()); }

    std::reverse_iterator<vertex_iterator> rbegin_vertex() {
        return std::reverse_iterator<vertex_iterator>(end_vertex());
    }
    std::reverse_iterator<vertex_iterator> rend_vertex() {
        return std::reverse_iterator<vertex_iterator>(begin_vertex());
    }
    std::reverse_iterator<const_vertex_iterator> rbegin_vertex() const {
        return std::reverse_iterator<const_vertex_iterator>(end_vertex());
    }
    std::reverse_iterator<const_vertex_iterator> rend_vertex() const {
        return std::reverse_iterator<const_vertex_iterator>(begin_vertex());
    }
    std::reverse_iterator<const_vertex_iterator> crbegin_vertex() const {
        return std::reverse_iterator<const_vertex_iterator>(end_vertex());
    }
    std::reverse_iterator<const_vertex_iterator> crend_vertex() const {
        return std::reverse_iterator<const_vertex_iterator>(begin_vertex());
    }


    class edge_iterator {
    private:
        const DirectedGraph* graph;
        vertex_size_type currentVertex;
        vertex_size_type currentEdge;

        void findNextEdge() {
            while (currentVertex < graph->vertices.size()) {
                while (currentEdge < graph->vertices.size()) {
                    if (graph->adjacencyMatrix[currentVertex][currentEdge]) {
                        return;
                    }
                    ++currentEdge;
                }
                ++currentVertex;
                currentEdge = 0;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = edge_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const edge_type*;
        using reference = const edge_type&;

        edge_iterator(const DirectedGraph* g, vertex_size_type v, vertex_size_type e)
            : graph(g), currentVertex(v), currentEdge(e) {
            if (graph && currentVertex < graph->vertices.size()) {
                findNextEdge();
            }
        }

        edge_type operator*() const {
            return edge_type(currentVertex, currentEdge,
                           graph ? &graph->vertices[currentVertex] : nullptr,
                           graph ? &graph->vertices[currentEdge] : nullptr);
        }

        edge_iterator& operator++() {
            if (currentVertex < graph->vertices.size()) {
                ++currentEdge;
                findNextEdge();
            }
            return *this;
        }

        edge_iterator operator++(int) {
            edge_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const edge_iterator& other) const {
            return graph == other.graph &&
                   currentVertex == other.currentVertex &&
                   currentEdge == other.currentEdge;
        }

        bool operator!=(const edge_iterator& other) const {
            return !(*this == other);
        }
    };

    class const_edge_iterator {
    private:
        const DirectedGraph* graph;
        vertex_size_type currentVertex;
        vertex_size_type currentEdge;

        void findNextEdge() {
            while (currentVertex < graph->vertices.size()) {
                while (currentEdge < graph->vertices.size()) {
                    if (graph->adjacencyMatrix[currentVertex][currentEdge]) {
                        return;
                    }
                    ++currentEdge;
                }
                ++currentVertex;
                currentEdge = 0;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = edge_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const edge_type*;
        using reference = const edge_type&;

        const_edge_iterator(const DirectedGraph* g, vertex_size_type v, vertex_size_type e)
            : graph(g), currentVertex(v), currentEdge(e) {
            if (graph && currentVertex < graph->vertices.size()) {
                findNextEdge();
            }
        }

        edge_type operator*() const {
            return edge_type(currentVertex, currentEdge,
                           graph ? &graph->vertices[currentVertex] : nullptr,
                           graph ? &graph->vertices[currentEdge] : nullptr);
        }

        const_edge_iterator& operator++() {
            if (currentVertex < graph->vertices.size()) {
                ++currentEdge;
                findNextEdge();
            }
            return *this;
        }

        const_edge_iterator operator++(int) {
            const_edge_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_edge_iterator& other) const {
            return graph == other.graph &&
                   currentVertex == other.currentVertex &&
                   currentEdge == other.currentEdge;
        }

        bool operator!=(const const_edge_iterator& other) const {
            return !(*this == other);
        }
    };

    edge_iterator begin_edge() { return edge_iterator(this, 0, 0); }
    edge_iterator end_edge() { return edge_iterator(this, vertices.size(), 0); }
    const_edge_iterator begin_edge() const { return const_edge_iterator(this, 0, 0); }
    const_edge_iterator end_edge() const { return const_edge_iterator(this, vertices.size(), 0); }
    const_edge_iterator cbegin_edge() const { return const_edge_iterator(this, 0, 0); }
    const_edge_iterator cend_edge() const { return const_edge_iterator(this, vertices.size(), 0); }

    std::reverse_iterator<edge_iterator> rbegin_edge() {
        std::vector<edge_type> edges;
        for (auto it = begin_edge(); it != end_edge(); ++it) {
            edges.push_back(*it);
        }
        return std::reverse_iterator<edge_iterator>(edge_iterator(this, vertices.size(), 0));
    }

    std::reverse_iterator<edge_iterator> rend_edge() {
        return std::reverse_iterator<edge_iterator>(begin_edge());
    }

    class adjacent_iterator {
    private:
        const DirectedGraph* graph;
        vertex_size_type source;
        vertex_size_type current;

        void findNextAdjacent() {
            while (current < graph->vertices.size()) {
                if (graph->adjacencyMatrix[source][current]) {
                    return;
                }
                ++current;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = vertex_size_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const vertex_size_type*;
        using reference = vertex_size_type;

        adjacent_iterator(const DirectedGraph* g, vertex_size_type s, vertex_size_type c)
            : graph(g), source(s), current(c) {
            if (graph && current < graph->vertices.size()) {
                findNextAdjacent();
            }
        }

        vertex_size_type operator*() const {
            return current;
        }

        adjacent_iterator& operator++() {
            if (current < graph->vertices.size()) {
                ++current;
                findNextAdjacent();
            }
            return *this;
        }

        adjacent_iterator operator++(int) {
            adjacent_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const adjacent_iterator& other) const {
            return graph == other.graph &&
                   source == other.source &&
                   current == other.current;
        }

        bool operator!=(const adjacent_iterator& other) const {
            return !(*this == other);
        }
    };

    class const_adjacent_iterator {
    private:
        const DirectedGraph* graph;
        vertex_size_type source;
        vertex_size_type current;

        void findNextAdjacent() {
            while (current < graph->vertices.size()) {
                if (graph->adjacencyMatrix[source][current]) {
                    return;
                }
                ++current;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = vertex_size_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const vertex_size_type*;
        using reference = vertex_size_type;

        const_adjacent_iterator(const DirectedGraph* g, vertex_size_type s, vertex_size_type c)
            : graph(g), source(s), current(c) {
            if (graph && current < graph->vertices.size()) {
                findNextAdjacent();
            }
        }

        vertex_size_type operator*() const {
            return current;
        }

        const_adjacent_iterator& operator++() {
            if (current < graph->vertices.size()) {
                ++current;
                findNextAdjacent();
            }
            return *this;
        }

        const_adjacent_iterator operator++(int) {
            const_adjacent_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_adjacent_iterator& other) const {
            return graph == other.graph &&
                   source == other.source &&
                   current == other.current;
        }

        bool operator!=(const const_adjacent_iterator& other) const {
            return !(*this == other);
        }
    };

    adjacent_iterator begin_adjacent(vertex_size_type vertex) {
        return adjacent_iterator(this, vertex, 0);
    }

    adjacent_iterator end_adjacent(vertex_size_type vertex) {
        return adjacent_iterator(this, vertex, vertices.size());
    }

    const_adjacent_iterator begin_adjacent(vertex_size_type vertex) const {
        return const_adjacent_iterator(this, vertex, 0);
    }

    const_adjacent_iterator end_adjacent(vertex_size_type vertex) const {
        return const_adjacent_iterator(this, vertex, vertices.size());
    }

    const_adjacent_iterator cbegin_adjacent(vertex_size_type vertex) const {
        return const_adjacent_iterator(this, vertex, 0);
    }

    const_adjacent_iterator cend_adjacent(vertex_size_type vertex) const {
        return const_adjacent_iterator(this, vertex, vertices.size());
    }


    friend std::ostream& operator<<(std::ostream& os, const DirectedGraph& graph) {
        os << "Directed Graph with " << graph.vertices.size() << " vertices and "
           << graph.edgeCount() << " edges\n";

        os << "Vertices: ";
        std::for_each(graph.vertices.begin(), graph.vertices.end(),
                      [&os](const VertexType& v) { os << v << " "; });
        os << "\n";

        os << "Edges:\n";
        std::vector<edge_type> edges;
        for (auto it = graph.begin_edge(); it != graph.end_edge(); ++it) {
            edges.push_back(*it);
        }
        std::for_each(edges.begin(), edges.end(),
                      [&os](const edge_type& e) { os << "  " << e << "\n"; });

        return os;
    }
};

#endif // DIRECTEDGRAPH_H