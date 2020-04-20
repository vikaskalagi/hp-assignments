import time, threading



def basicthreading(threadname,timedelay):
    nooftimes=0
    while nooftimes < 10:
        time.sleep(timedelay)
        nooftimes += 1
        print(threadname,timedelay)

t1 = threading.Thread(None, basicthreading ,"Thread 1",("Thread 1",5))
t2 = threading.Thread(None, basicthreading,"Thread 2",("Thread 2",5))


if __name__ == "__main__":
        t2.start()
        t1.start()