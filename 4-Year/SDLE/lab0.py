import networkx as nx
import random
import matplotlib.pyplot as plt

results = {}

def createEdge(n_vertices):
    v1 = random.randint(1,n_vertices)
    v2 = random.randint(1,n_vertices)

    if v1 == v2:
       return createEdge(n_vertices)
    return (v1,v2)


def buildGraph(n_vertices):
    G = nx.Graph()
    G.add_nodes_from(list(range(1,n_vertices+1)))
    
    S = 0
    n_edges = 0
    edge = None
    while not (nx.is_connected(G)):
        while True:
            edge = createEdge(n_vertices)
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
    
    for i in [ 2**j for j in range(1,Max_vertex)]:
        res_aux = [buildGraph(i)[0] for x in range(0,10)]
        results[i] = Average(res_aux)

    axis_x = []
    axis_y = []
    for key,val in results.items():
        axis_x.append(key)
        axis_y.append(val)
    
    plt.plot(axis_x,axis_y)
    plt.yscale('linear')
    plt.show()

main()

