from lab2 import Node, Sim
from lab1 import buildGraphPreferencial, buildGraphRandom
from enum import Enum
import random
from nanoid import generate
import networkx as nx

#Guião 3 (Parte 2)

#Guião 3 (Parte 3)

TIMEOUT_GOSSIP = 10
TIMEOUT_IHAVE = 5

class Timeout:
    def __init__(self,origin,timeout):
        self.origin = origin
        self.timeout = timeout

class MessageType(Enum):
    GOSSIP = 1
    IHAVE = 2
    ASK = 3

class Message:
    def __init__(self,id,sender,type,content=None):
        self.id = id
        self.sender = sender
        self.type = type
        self.content = content

    @classmethod
    def build(cls, id, sender, content):
        return cls(id,sender,MessageType.GOSSIP,content), cls(id,sender,MessageType.IHAVE)

class Fanout(Node):
    def __init__(self,id, neighbors, fanout):
        self.id = id
        self.type = None
        self.messages = {}
        self.events = {}
        self.neighbors = neighbors
        self.history = set()

        if (len(neighbors) < fanout):
            self.lazy_neighbors = []
            self.eager_neighbors = neighbors
        else:
            self.lazy_neighbors = random.sample(neighbors,fanout)
            self.eager_neighbors = [item for item in neighbors if item not in self.lazy_neighbors]

    def handle(self,ori,msg,timestamp):
        msg_to_send = []
        actual_events = 0
        #primeiro nodo
        if ori is None:
            m_playload, m_ihave = Message.build(msg.id,self.id,msg.content)
            self.messages[msg.id] = m_playload

            for neighbor in self.lazy_neighbors:
                msg_to_send.append((self.id,neighbor,m_ihave))

            for neighbor in self.eager_neighbors:
                msg_to_send.append((self.id,neighbor,m_playload))

        else:
            if (msg.type == MessageType.GOSSIP):
                msg_to_send, actual_events = self.handleGOSSIP(msg,timestamp)
            elif (msg.type == MessageType.IHAVE):
                actual_events = self.handleIHAVE(msg,timestamp)
            else:
                msg_to_send = self.handleASK(msg)

        return (msg_to_send, actual_events)

    def handleIHAVE(self,msg,timestamp):
        ##testar se já passou por aqui
        actual_events = 0
        if (msg.id not in self.messages) and (msg.id not in self.events):
            self.events[msg.id] = Timeout(msg.sender,timestamp+TIMEOUT_IHAVE)
            actual_events += 1


        return actual_events

    def handleGOSSIP(self,msg,timestamp):
        msg_to_send = []
        actual_events = 0
        payload_message, id_message = Message.build(msg.id,self.id,msg.content)

        ##para não evitar loops
        if (msg.id not in self.history):
            self.history.add(msg.id)
            self.messages[msg.id] = payload_message

            for lazy in self.lazy_neighbors:
                if lazy != msg.sender:
                    msg_to_send.append((self.id,lazy,id_message))

            for eager in self.eager_neighbors:
                if eager != msg.sender:
                    msg_to_send.append((self.id,eager,payload_message))

            msg_to_send.append((self.id,msg.sender,id_message))
        else:
            msg_to_send.append((self.id,msg.sender,id_message))


        ##eliminar timeouts caso ele fosse lazy
        if msg.id in self.events:
            del self.events[msg.id]
            actual_events -= 1

        return (msg_to_send,actual_events)

    def handleASK(self,msg):
        msg_to_send = []
        n_msg = Message(msg.id,self.id,MessageType.GOSSIP,msg.content)
        msg_to_send.append((self.id,msg.sender,n_msg))
        return msg_to_send

    def handle_event(self,time):
        lst = []
        lst_keys = []
        events = 0

        for key,value in self.events.items():
            if (value.timeout == time):
                lst.append((self.id,value.origin,Message(key,self.id,MessageType.ASK)))
                lst_keys.append(key)
        for key in lst_keys:
            del self.events[key]

        events = len(lst)

        return (lst,len(lst),events)



def build_data_structure(graph):
    nodes = {}
    dist = {}

    for n in graph.nodes:
        neighbors = [n for n in graph.neighbors(n)]
        nodes[n] = Fanout(n,neighbors,2)

        for neig in neighbors:
            dist[(n,neig)] = graph[n][neig]['weight']

    return (nodes,dist)

def main():

    graph = buildGraphRandom(8)[1]

    nodes, dist = build_data_structure(graph)
    simulator = Sim(nodes,dist)
    simulator.start(Message(generate(),-1,MessageType.GOSSIP,'Hello'),root=1,events=True)

    print(nx.diameter(graph))
    print(simulator.current_time)

    for node in nodes.values():
        print(len(node.messages))
        #if node.type is NodeType.Lazy:
        #    print(node.events)

    '''
    if nx.diameter(graph) < simulator.current_time:
        nx.draw(graph,with_labels=True)
        plt.show()
    '''

main()
