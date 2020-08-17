from lab2 import Node, Sim
from lab1 import buildGraphPreferencial, buildGraphRandom
from enum import Enum
import random
from nanoid import generate
import networkx as nx
import matplotlib.pyplot as plt

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
    ACK = 4

class Message:
    def __init__(self,id,sender,type,content=None):
        self.id = id
        self.sender = sender
        self.type = type
        self.content = content

    @classmethod
    def build(cls, id, sender, content):
        return cls(id,sender,MessageType.GOSSIP,content), cls(id,sender,MessageType.IHAVE), cls(id,sender,MessageType.ACK)

class Fanout(Node):
    def __init__(self,id, neighbors, fanout):
        self.id = id
        self.type = None
        self.messages = {}
        self.events = {}
        self.neighbors = neighbors
        self.acks = {}
        self.gossip_timeout = {}
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
            m_playload, m_ihave, m_ack = Message.build(msg.id,self.id,msg.content)
            self.messages[msg.id] = m_playload

            self.acks[msg.id] = set()
            for neighbor in self.lazy_neighbors:
                msg_to_send.append((self.id,neighbor,m_ihave))
                self.acks[msg.id].add(neighbor)

            for neighbor in self.eager_neighbors:
                msg_to_send.append((self.id,neighbor,m_playload))
                self.acks[msg.id].add(neighbor)

            self.gossip_timeout[msg.id] = timestamp + TIMEOUT_GOSSIP
            actual_events += 1
            self.history.add(msg.id)

        else:
            if (msg.type == MessageType.GOSSIP):
                msg_to_send, actual_events = self.handleGOSSIP(msg,timestamp)
            elif (msg.type == MessageType.IHAVE):
                msg_to_send,actual_events = self.handleIHAVE(msg,timestamp)
            elif (msg.type == MessageType.ASK):
                msg_to_send = self.handleASK(msg,timestamp)
            else:
                self.handleACK(msg,timestamp)

        return (msg_to_send, actual_events)

    def handleACK(self,msg,timestamp):
        if (msg.id in self.messages):
            if msg.sender in self.acks[msg.id]:
                self.acks[msg.id].remove(msg.sender)

    def handleIHAVE(self,msg,timestamp):
        ##testar se já passou por aqui
        actual_events = 0
        msg_to_send = []
        if (msg.id not in self.messages) and (msg.id not in self.events) and (msg.id not in self.history):
            self.events[msg.id] = Timeout(msg.sender,timestamp+TIMEOUT_IHAVE)
            actual_events += 1
        elif (msg.id not in self.events):
            msg_to_send = [(self.id,msg.sender,Message(msg.id,self.id,MessageType.ACK))]

        '''
        if (msg.id in self.messages):
            if msg.sender in self.acks[msg.id]:
                self.acks[msg.id].remove(msg.sender)
        '''


        return msg_to_send,actual_events

    def handleGOSSIP(self,msg,timestamp):
        msg_to_send = []
        actual_events = 0
        payload_message, id_message, m_ack = Message.build(msg.id,self.id,msg.content)

        ##para não evitar loops
        if (msg.id not in self.history):
            self.history.add(msg.id)
            self.messages[msg.id] = payload_message

            if msg.id in self.events:
                actual_events -= 1
                del self.events[msg.id]

            self.acks[msg.id] = set()
            self.gossip_timeout[msg.id] = timestamp + TIMEOUT_GOSSIP
            actual_events += 1
            for lazy in self.lazy_neighbors:
                if lazy != msg.sender:
                    msg_to_send.append((self.id,lazy,id_message))
                    self.acks[msg.id].add(lazy)

            for eager in self.eager_neighbors:
                if eager != msg.sender:
                    msg_to_send.append((self.id,eager,payload_message))
                    self.acks[msg.id].add(eager)

            msg_to_send.append((self.id,msg.sender,m_ack)) #ACK
        else:
            msg_to_send.append((self.id,msg.sender,m_ack))


        ##eliminar timeouts caso ele fosse lazy Provavelmente erro aqui
        if msg.id in self.events:
            del self.events[msg.id]
            actual_events -= 1

        return (msg_to_send,actual_events) #mensagens, nºeventos criados

    def handleASK(self,msg,timestamp):
        msg_to_send = []
        n_msg = Message(msg.id,self.id,MessageType.GOSSIP,msg.content)
        #refresh do timeout
        self.gossip_timeout[msg.id] = timestamp + TIMEOUT_GOSSIP
        self.acks[msg.id].add(msg.sender)
        msg_to_send.append((self.id,msg.sender,n_msg))
        return msg_to_send

    def handle_event(self,time):
        lst = []
        lst_keys = []
        events = 0

        for key,value in self.events.items():
            if (value.timeout == time):
                lst.append((self.id,value.origin,Message(key,self.id,MessageType.ASK)))
                lst_keys.append((key,value.origin))
        for key,val in lst_keys:
            self.events[key] = Timeout(val,time+TIMEOUT_IHAVE)

        events = len(lst)
        ev2, up2 = self.handle_gossip_timeout(time)
        lst.extend(ev2)

        return (lst,len(lst),up2+0) # mensagens, nºmensagens, nºeventos concluidos

    def handle_gossip_timeout(self,time):
        lst = []
        lst_keys_del = []
        lst_keys_upt = []
        events = 0

        for key,value in self.gossip_timeout.items():
            if (value == time):
                neighbors_to_ack = self.acks[key]
                if len(neighbors_to_ack) > 0:
                    n_msg = Message(key,self.id,MessageType.GOSSIP,self.messages[key])
                    n_ihave = Message(key,self.id,MessageType.IHAVE)
                    for node in neighbors_to_ack:
                        if node in self.lazy_neighbors:
                            lst.append((self.id,node,n_ihave))
                        else:
                            lst.append((self.id,node,n_msg))
                    lst_keys_upt.append(key)
                    print(f'Nodo {self.id} timeout')
                else:
                    events += 1
                    lst_keys_del.append(key)


        for key in lst_keys_del:
            del self.messages[key]
            del self.acks[key]
            del self.gossip_timeout[key]

        for key in lst_keys_upt:
            self.gossip_timeout[key] = time + 10

        return (lst,events)

    def checkup(self):
        print(f'Node: {self.id} message: {self.messages} events: {self.events} history: {self.history} lazy:{self.lazy_neighbors} eager:{self.eager_neighbors} timeouts: {self.gossip_timeout} ack: {self.acks}');





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

    graph = buildGraphRandom(16)[1]

    nodes, dist = build_data_structure(graph)
    simulator = Sim(nodes,dist)
    simulator.start(Message(generate(),-1,MessageType.GOSSIP,'Hello'),root=1,events=True,error=70)

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
