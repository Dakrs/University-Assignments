from enum import Enum

class MessageType(Enum):
    Payload = 1
    Id = 2
    Get = 3

class Message:
    def __init__(self,id,sender,type,content=None):
        self.id = id
        self.sender = sender
        self.type = type
        self.content = content

    @classmethod
    def build(cls, id, sender, content):
        return cls(id,sender,MessageType.Payload,content), cls(id,sender,MessageType.Id)

    def print(self):
        print((self.id,self.sender,self.type))


def main():
    payload_message, id_message = Message.build(1,2,'Testing')


main()
