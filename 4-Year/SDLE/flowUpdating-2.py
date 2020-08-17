from lab2 import Node, Sim
from lab1 import buildGraphPreferencial, buildGraphRandom
from enum import Enum
import random
from nanoid import generate
import networkx as nx
import matplotlib.pyplot as plt

MAX_ITER = 1000
ITER_INTERVAL = 5
MAX_ITER_WITHOUT_CHANGE = 10
GOD_VIEW = False
N_NODES = 512

class Timeout:
    def __init__(self,origin,timeout):
        self.origin = origin
        self.timeout = timeout

class Message:
    def __init__(self,sender,flow,estimate):
        self.sender = sender
        self.flow = flow
        self.estimate = estimate

    @classmethod
    def build(cls, sender, flow, estimate):
        return cls(sender,flow,estimate)

class Flow(Node):
    def __init__(self,id, neighbors):
        self.id = id
        self.neighbors = set(neighbors)
        self.input = 1 if id == 1 else 0
        self.aggregation = 0
        self.estimates = {}
        self.flows = {}
        self.iters = 0
        self.itersValues = {}
        self.finished = False
        self.acks = set()
        self.tick = 0
        self.timeout = ITER_INTERVAL
        self.nonews = 0

        for neighbor in self.neighbors:
            self.estimates[neighbor] = 0
            self.flows[neighbor] = 0
        self.itersValues[0] = 0;
        self.iters += 1

    def handle(self,ori,msg,timestamp):
        msg_to_send = []
        actual_events = 0
        #primeiro nodo
        if ori is None:
            #message-generation
            m = Message(self.id,0,0)

            for neighbor in self.neighbors:
                msg_to_send.append((self.id,neighbor,m))
            actual_events += 1

          
            self.tick = 0
        else:
            #state-transition
            if msg.sender in self.neighbors:
                self.flows[msg.sender] = float(-msg.flow)
                self.estimates[msg.sender] = msg.estimate
                self.acks.add(msg.sender)

                if len(self.acks) == len(self.neighbors) and not self.finished:
                    msg_to_send, actual_events = self.newround()
                

        return (msg_to_send, actual_events)

    def newround(self):
        msg_to_send = []
        actual_events = 0

        self.aggregation = self.input - sum(self.flows.values())
        average = (self.aggregation + sum(self.estimates.values())) / (len(self.neighbors) + 1)
        if self.stop() or self.iters > MAX_ITER and not GOD_VIEW:
            actual_events -= 1
            self.finished = True
        else:
            for n in self.neighbors:
                self.flows[n] += average - self.estimates[n]
                self.estimates[n] = average
                msg_to_send.append((self.id,n,Message(self.id,self.flows[n],average)))

            self.tick = 0
            self.acks = set()

            self.itersValues[self.iters] = self.aggregation
            self.iters += 1

        return msg_to_send,actual_events

    def handle_event(self,timestamp):
        msg_to_send = []
        actual_events = 0

        self.tick += 1
        if self.tick == self.timeout and not self.finished:
            msg_to_send, actual_events = self.newround()
        return (msg_to_send,len(msg_to_send),abs(actual_events))

    def checkup(self):
        print(f'Node: {self.id} iters: {self.iters}  aggregation: {self.aggregation} flows: {self.flows}');

    def stop(self):
        flag = False
        if (self.itersValues[self.iters-1] == self.aggregation or abs(self.itersValues[self.iters-1] - self.aggregation) < 0.00000001) and self.aggregation!=0:
            self.nonews += 1
        else:
            self.nonews = 0

        if self.nonews >= MAX_ITER_WITHOUT_CHANGE:
            flag = True

      
        return flag

def nodes_convergence_test(val,tol,nodes):
    lst = list(map(lambda x: abs(x.aggregation - val) < tol,list(nodes.values())))
    return all(lst)

def viewChange(graph,dist):
    n_graph = buildGraphRandom(N_NODES)[1]
    n_dist = {}

    for n in n_graph.nodes:
        n_neighbors = set([n for n in n_graph.neighbors(n)])
        node = graph[n]
        for neig in node.neighbors:
            if neig not in n_neighbors:
                del node.estimates[neig]
                del node.flows[neig]
        node.neighbors = n_neighbors
        for neig in node.neighbors:
            if neig not in node.estimates:
                node.estimates[neig] = node.aggregation
                node.flows[neig] = 0

        graph[n] = node

        for neig in n_neighbors:
            n_dist[(n,neig)] = n_graph[n][neig]['weight']

    return graph, n_dist

def build_data_structure(graph):
    nodes = {}
    dist = {}

    for n in graph.nodes:
        neighbors = [n for n in graph.neighbors(n)]
        nodes[n] = Flow(n,neighbors)

        for neig in neighbors:
            dist[(n,neig)] = graph[n][neig]['weight']

    return (nodes,dist)

def main():

    random.seed(100)
    graph = buildGraphRandom(N_NODES)[1]

    nodes, dist = build_data_structure(graph)
    simulator = Sim(nodes,dist)
    #godview  Message(-1,0,0),events=True,godview=lambda x: nodes_convergence_test(valor esperado,tolerancia,x) a flag lá em cima tem que ficar a true
    #viewchange  dynamic=viewChange,drate=50
    #events=True                       ativa eventos
    #erros=X                           injeta perda de mensagem X=20 20% de perdas
    #dynamic=viewChange,drate=X        injeta alterações na rede. dynamic=func Func é a função que altera os nodos drate=X X é a frequência com que altera. Na versão atual os nodos não convergem, não testar com o godview
    #godview=func                      Função que testa se todos os nodos convergiram. Para os nossos nodos func=lambda x: nodes_convergence_test(1/n,tolerancia,x). Flag GOD_VIEW tem que estar a true
    simulator.start(Message(-1,0,0),events=True,error=30)

    print(nx.diameter(graph))
    print(simulator.current_time)

    n_iter = max(map(lambda x: x.iters,list(nodes.values())))

    for node in nodes.values():

        print(f'{round(1/node.aggregation)} iters: {node.iters}')
    
    print(n_iter)
  

main()
