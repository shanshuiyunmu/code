import socket
import enum
import threading
import struct
import chatmysql
import time

#消息类型
class Type(enum.Enum):
    VIRTUSKILL=0
    COMMITVIRTUS=1

#聊天室类
class Mychat(object):
    #构造函数,绑定服务器套接字ip与端口
    def __init__(self):
        try:
            #创建套接字对象
            self.socket=socket.socket()
            #绑定
            self.socket.bind(('127.0.0.1',6666))
            #监听
            self.socket.listen(socket.SOMAXCONN)
            #创建mysql类对象
            self.mysqlchat = chatmysql.chatMysql()
        except Exception as info:
            print(info)
#接收信息
    def accept(self):
        while True:
            try:
                #等待客户端连接
                client,address=self.socket.accept()
                print(address,'进入了服务器')
                #为每一个客户端创建线程
                threading.Thread(target=self.receive,args=(client,)).start()
            except Exception as info:
                print(info)
    #线程响应函数
    def receive(self,client):
        while True:
            try:
                #设置最大接收大小
                msg=client.recv(3072)
                #解包,消息类型
                type,=struct.unpack("i",msg[:4])
                #根据消息类型调用处理函数
                Mychat.dic_func[type](self,client,msg[4:])
            except Exception as info:
                print('离开了服务器')
                break

#查杀病毒
    def on_virtuskill(self,client,msg):
        index,virtusname,virtusmd5=struct.unpack("i64s64s",msg[:132])
        virtusname = virtusname.decode("utf16").strip('\x00')
        virtusmd5 = virtusmd5.decode("utf16").strip('\x00')
        num, str = mychat.mysqlchat.select("select * from virtus_table where (virtusname='%s' and virtusmd5='%s')" %
                                           (virtusname, virtusmd5))
        if num>0:
                client.send(struct.pack("ii64s64s",Type.VIRTUSKILL.value,index,virtusname.encode("utf16"),
                                        "病毒".encode("utf16")))
        else:
            client.send(struct.pack("ii64s64s", Type.VIRTUSKILL.value, index, virtusname.encode("utf16"),
                                    "通过".encode("utf16")))
#提交样本
    def on_commitvirtus(self,client,msg):
        index, virtusname, virtusmd5 = struct.unpack("i64s64s", msg[:132])
        virtusname = virtusname.decode("utf16").strip('\x00')
        virtusmd5 = virtusmd5.decode("utf16").strip('\x00')
        num, str = self.mysqlchat.select("select * from virtus_table where virtusmd5='%s';" % (virtusmd5))
        if num > 0:
            client.send(struct.pack("ii64s64s", Type.COMMITVIRTUS.value, index, virtusname.encode("utf16"),
                                    "病毒已存在".encode("utf16")))
        else:
            self.mysqlchat.insert("INSERT INTO virtus_table(virtusname,virtusmd5) value ('%s','%s');"
                                  % (virtusname, virtusmd5))
            client.send(struct.pack("ii64s64s", Type.COMMITVIRTUS.value, index, virtusname.encode("utf16"),
                                    "提交成功".encode("utf16")))

    #处理函数字典
    dic_func={
        Type.VIRTUSKILL.value:on_virtuskill,
        Type.COMMITVIRTUS.value:on_commitvirtus,
    }

if __name__=="__main__":
    mychat=Mychat()
    mychat.accept()