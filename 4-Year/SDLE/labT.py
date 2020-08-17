import networkx as nx
import random
import matplotlib.pyplot as plt
import numpy

resultsPref = {}
resultsRand = {}

weights = {}
total_weights = 0
list_nodes = []

def createEdgePref(n_vertices):
    global weights,total_weights,list_nodes

    probs = list(map(lambda x: x / total_weights,weights.values()))
    #v1 = numpy.random.choice(numpy.arange(1, n_vertices+1), p=[0.1, 0.05, 0.05, 0.2, 0.4, 0.2])
    v1 = numpy.random.choice(list_nodes, p=probs)
    v2 = numpy.random.choice(list_nodes, p=probs)

    if v1 == v2:
       return createEdgePref(n_vertices)
    
    total_weights += 2
    weights[v1] += 1
    weights[v2] += 1
    return (v1,v2)


def buildGraphPreferencial(n_vertices):
    G = nx.Graph()
    G.add_nodes_from(list(range(1,n_vertices+1)))

    global weights, total_weights, list_nodes
    weights = {}
    for x in range(1,n_vertices+1):
        weights[x] = 1

    total_weights = n_vertices
    list_nodes = numpy.arange(1, n_vertices+1)
    
    S = 0
    n_edges = 0
    edge = None
    while not (nx.is_connected(G)):
        while True:
            edge = createEdgePref(n_vertices)
            if not G.has_edge(edge[0],edge[1]):
                break
        G.add_edge(edge[0],edge[1])
        G.add_edge(edge[1],edge[0])
        n_edges += 1

    return (n_edges,G)

def Average(lst): 
    return sum(lst) / len(lst) 

def main():
    Max_vertex = int(input('MaxºVertices:'))
    
    res = buildGraphPreferencial(100)[1]

    nx.draw(res)
    plt.show()


main()

