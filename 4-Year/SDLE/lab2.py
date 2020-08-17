import networkx as nx
from lab1 import buildGraphPreferencial, buildGraphRandom
from sortedcontainers import SortedKeyList
import matplotlib.pyplot as plt
from abc import ABC, abstractmethod
import random


class Node(ABC):
    #tem que devolver um tuplo (mensages para enviar, número de eventos criados)
    @abstractmethod
    def handle(self,origem,destino,timestamp):
        pass

    #tem que devolver um tuplo (mensagens para enviar, número de mensagens ,número de eventos concluidos)
    @abstractmethod
    def handle_event(self,timestamp):
        pass


'''
class Broadcast(Node):
    def __init__(self, id, neighbors, fanout=None):
        self.id = id
        self.neighbors = neighbors
        self.state = False
        self.fanout = fanout


    def handle(self,ori,msg,t):
        msg_to_send = []

        if self.state is False:
            if ori is None:
                #envia para todos
                for neighbor in self.neighbors:
                    msg_to_send.append((self.id,neighbor,msg))
            else:
                if (self.fanout is not None):
                    i = 0
                    while(i < self.fanout and i < len(self.neighbors)):
                        neighbor = self.neighbors[i]
                        if neighbor != ori:
                            msg_to_send.append((self.id,neighbor,msg))
                        i += 1
                else:
                    for neighbor in self.neighbors:
                        if neighbor != ori:
                            msg_to_send.append((self.id,neighbor,msg))

            self.state = True

        return (msg_to_send,0)

    def handle_event(self):
        pass
'''

class Sim:
    # - nodes: {0: Node, 1: Node, 2: Node, ...}
    # - distances: {(0,1): 103, (0,2): 40, ...}
    def __init__(self,nodes, distances):
        self.nodes = nodes
        self.distances = distances
        self.current_time = 0
        self.handle_events = False

        #self.pending = [] # [(delay, (src,dst,msg))]
        #self.pending = SortedKeyList(key=lambda x: -x[0])
        self.pending = {} #Formato das mensages: (origem, destino, mensagem a enviar)
        self.pending_msgs = 0
        self.pending_events = 0
        self.error_rate = 0
        self.dynamic_rate = None
        self.dynamic_func = None
        self.godview = None


    def start(self, initial_msg,**options):
        #shedule first event

        root = options.get("root")
        events = options.get("events")
        error_rate = options.get("error")
        dynamic = options.get("dynamic")
        dynamic_rate = options.get("drate")
        godfunc = options.get("godview")

        if callable(dynamic) and isinstance(dynamic_rate,int):
            self.dynamic_rate = dynamic_rate
            self.dynamic_func = dynamic

        if callable(godfunc):
            self.godview = godfunc

        if isinstance(events,bool):
            self.handle_events = events

        if isinstance(root,int):
            #event = (-1, (None, root, initial_msg))
            #self.pending.add(event)
            self.pending[0] = [(None, root, initial_msg)]
            self.pending_msgs += 1
        else:
            initial_messages = []
            for key in self.nodes.keys():
                initial_messages.append((None,key,initial_msg))
                self.pending_msgs += 1
            self.pending[0] = initial_messages

        if isinstance(error_rate,int):
            self.error_rate = error_rate

        self.run_loop()

    def run_loop(self):
        while(self.pending_msgs > 0 or self.pending_events > 0):
            if self.current_time > 5000:
                print('Probably error. Manual Break')
                break
            #msgs_to_send = self.pending[self.current_time]
            msgs_to_send = self.pending.get(self.current_time,[])
            self.pending_msgs -= len(msgs_to_send)

            for data in msgs_to_send:
                if (self.error_rate > 0 and data[0] is not None):
                    ra = random.randint(0, 100)
                    if (ra < self.error_rate):
                        #print(f'Skip: (O: {data[0]},D: {data[1]})')
                        continue
                #print(f'(O: {data[0]},D: {data[1]},Flow: {data[2].flow},Est: {data[2].estimate})')

                dest_node = self.nodes[data[1]]
                new_msgs, update_events = dest_node.handle(data[0],data[2],self.current_time)

                for ori, dest, msg in new_msgs:
                    if (ori,dest) not in self.distances:
                        continue
                    timestamp = self.distances[(ori,dest)] + self.current_time

                    if timestamp not in self.pending:
                        self.pending[timestamp] = []
                    self.pending[timestamp].append((ori,dest,msg))

                self.pending_msgs += len(new_msgs)
                self.pending_events += update_events

            if (self.handle_events):
                self.toggle_events()

            if (self.pending_msgs != 0 or self.pending_events != 0):
                self.current_time += 1

            if (self.dynamic_rate is not None) and self.dynamic_rate > 20:
                if (self.current_time % self.dynamic_rate) == 0:
                    print('View Change')
                    n_nodes, n_dists = self.dynamic_func(self.nodes,self.distances)
                    self.nodes = n_nodes
                    self.distances = n_dists

            if (self.godview and self.godview(self.nodes)):
                print('GodView exiting')
                break

    def toggle_events(self):
        for node in self.nodes.values():
            lst_events, n_messages, events = node.handle_event(self.current_time)

            for ori, dest, msg in lst_events:
                timestamp = self.distances[(ori,dest)] + self.current_time

                if timestamp not in self.pending:
                    self.pending[timestamp] = []
                self.pending[timestamp].append((ori,dest,msg))

            self.pending_msgs += n_messages

            self.pending_events -= events

'''
def build_data_structure(graph):
    nodes = {}
    dist = {}

    for n in graph.nodes:
        neighbors = [n for n in graph.neighbors(n)]
        nodes[n] = Broadcast(n,neighbors)

        for neig in neighbors:
            dist[(n,neig)] = graph[n][neig]['weight']

    return (nodes,dist)



def main():

    graph = buildGraphRandom(8)[1]

    nodes, dist = build_data_structure(graph)
    simulator = Sim(nodes,dist)
    simulator.start('ola',root=1)

    print(nx.diameter(graph))
    print(simulator.current_time)


    if nx.diameter(graph) < simulator.current_time:
        nx.draw(graph,with_labels=True)
        plt.show()

main()

'''
