import thread
import threading
import time
# Define a function for the thread
a=0
threadLock = threading.Lock()

def consumer(threadName, delay,value):
	global a
	#print(a)
	threadLock.acquire()
	#print_time(self.name, self.counter, 3)
	print("before deleting from account the value is ",a)
	if(a>=value):
		a-=value
	print("after deleting from account the value is ",a)
	threadLock.release()
	time.sleep(delay)
def producer(threadName, delay,value):
	global a
	threadLock.acquire()
	print("before adding to account the value is ",a)
	a+=value
	print("after adding to account the value is ",a)
	threadLock.release()
	time.sleep(delay)
#thread1 = myThread1(1, "Thread-1", 1,12)
#thread2 = myThread2(2, "Thread-2", 2,25)

thread.start_new_thread(producer,("thread1",2,12))
thread.start_new_thread(consumer,("thread2",2,2))
thread.start_new_thread(producer,("thread1",2,15))
thread.start_new_thread(consumer,("thread2",2,19))
thread.start_new_thread(producer,("thread1",2,145))
thread.start_new_thread(consumer,("thread2",2,10))
thread.start_new_thread(producer,("thread1",2,120))
thread.start_new_thread(consumer,("thread2",2,29))
y=10
while (y>0):
	y-=1
	time.sleep(2)

