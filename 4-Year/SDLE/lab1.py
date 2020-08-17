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
        G.add_edge(edge[0],edge[1],weight=1)
        G.add_edge(edge[1],edge[0],weight=1)
        n_edges += 1

    return (n_edges,G)

def createEdgeRandom(n_vertices):
    v1 = random.randint(1,n_vertices)
    v2 = random.randint(1,n_vertices)

    if v1 == v2:
       return createEdgeRandom(n_vertices)
    return (v1,v2)


def buildGraphRandom(n_vertices):
    G = nx.Graph()
    G.add_nodes_from(list(range(1,n_vertices+1)))

    S = 0
    n_edges = 0
    edge = None
    while not (nx.is_connected(G)):
        while True:
            edge = createEdgeRandom(n_vertices)
            if not G.has_edge(edge[0],edge[1]):
                break
        G.add_edge(edge[0],edge[1],weight=1)
        G.add_edge(edge[1],edge[0],weight=1)
        n_edges += 1

    return (n_edges,G)

def Average(lst):
    return sum(lst) / len(lst)

'''
def main():
    Max_vertex = int(input('MaxºVertices:'))


    for i in [ 2**j for j in range(1,Max_vertex)]:
        res_aux = [buildGraphPreferencial(i)[0] for x in range(0,5)]
        resultsPref[i] = Average(res_aux)

    axis_x_pref = []
    axis_y_pref = []
    for key,val in resultsPref.items():
        axis_x_pref.append(key)
        axis_y_pref.append(val)



    for i in [ 2**j for j in range(1,Max_vertex)]:
        res_aux = [buildGraphRandom(i)[0] for x in range(0,5)]
        resultsRand[i] = Average(res_aux)

    axis_x_rand = []
    axis_y_rand = []
    for key,val in resultsRand.items():
        axis_x_rand.append(key)
        axis_y_rand.append(val)

    plt.plot(axis_x_pref,axis_y_pref)
    plt.plot(axis_x_rand,axis_y_rand)
    plt.yscale('linear')
    plt.show()


main()
'''
