from rpyc.utils import server
import rpyc

class DoStuff(rpyc.Service):

    def exposed_say_hello(self, pld):
        print("RPyc server in say_hello, pld = ", str(pid))
        return "Hello, %d!" % pid 
    
    def exposed_say_hello_again(self, *args, **kwargs):
        print("RPyC Server in say_hello_again, pid =", str(args[0]))
        return "Hello again, %d!" % args[0]
    
if __name__ == '__main__':
    server.oneShotServer(
        DoStuff,
        hostname='localhost', import=8888
    ).start()

