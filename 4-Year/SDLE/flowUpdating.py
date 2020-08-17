from lab2 import Node, Sim
from lab1 import buildGraphPreferencial, buildGraphRandom
from enum import Enum
import random
from nanoid import generate
import networkx as nx
import matplotlib.pyplot as plt

class Timeout:
    def __init__(self,origin,timeout):
        self.origin = origin
        self.timeout = timeout

class Message:
    def __init__(self,id,sender,flow,estimate):
        self.id = id
        self.sender = sender
        self.flow = flow
        self.estimate = estimate

    @classmethod
    def build(cls, id, sender, flow, estimate):
        return cls(id,sender,flow,estimate)

class Flow(Node):
    def __init__(self,id, neighbors):
        self.id = id
        self.messages = {}
        self.neighbors = neighbors
        self.input = self.id
        self.aggregation = self.input
        self.estimates = {}
        self.flows = {}

        for neighbor in self.neighbors:
            self.estimates[neighbor] = 0
            self.flows[neighbor] = 0

    def handle(self,ori,msg,timestamp):
        msg_to_send = []
        actual_events = 0
        #primeiro nodo
        if ori is None:
            #message-generation
            m = Message.build(msg.id,self.id,self.input,0)

            for neighbor in self.neighbors:
                msg_to_send.append((self.id,neighbor,m))

        else:
            #state-transition
            self.flows[msg.sender] = float(-msg.flow)
            self.estimates[msg.sender] = msg.estimate
            vflows = sum(self.flows.values())
            vestimates = sum(self.estimates.values())
            ntotal = (len(self.neighbors)+1)
            self.aggregation = (self.input - vflows + vestimates) / ntotal

            for neighbor in self.neighbors:
                self.flows[neighbor] += (self.aggregation - self.estimates[neighbor])
                self.estimates[neighbor] = self.aggregation
                new_m = Message.build(msg.id,self.id,self.flows[neighbor],self.estimates[neighbor])
                msg_to_send.append((self.id,neighbor,new_m))

        return (msg_to_send, 0)

    def handle_event(self,timestamp):
        return ([],0,0)

    def checkup(self):
        print(f'Node: {self.id} message: {self.messages} aggregation: {self.aggregation} flows: {self.flows} estimates: {self.messages} neighbors:{self.neighbors}');



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

    graph = buildGraphRandom(8)[1]

    nodes, dist = build_data_structure(graph)
    simulator = Sim(nodes,dist)
    simulator.start(Message(generate(),-1,0,0))

    print(nx.diameter(graph))
    print(simulator.current_time)

    for node in nodes.values():
        #print(len(node.history))
        node.checkup()
        #if node.type is NodeType.Lazy:
        #    print(node.events)

    '''
    if nx.diameter(graph) < simulator.current_time:
        nx.draw(graph,with_labels=True)
        plt.show()
    '''
    if simulator.current_time > 1000:
        print('deadlock')
        nx.draw(graph,with_labels=True)
        plt.show()
    else:
        nx.draw(graph,with_labels=True)
        plt.show()

main()
