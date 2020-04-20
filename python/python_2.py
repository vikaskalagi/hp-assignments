#import Queue
from queue import PriorityQueue
import threading
import time
import random
exitFlag = 0

class add (threading.Thread):
	def __init__(self, threadID, name, q):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.q = q
	def run(self):
		#print ("Starting " + self.name)
		queueLock.acquire()
		#data = q.get()
		
		r1 = random.randint(0, 10)
		print("thread %s has added element :%s to queue"%(self.name,r1))
		workQueue.put(r1)
		queueLock.release()
		#process_data(self.name, self.q)
		time.sleep(1)
		#print ("Exiting " + self.name)

class delete (threading.Thread):
	def __init__(self, threadID, name, q):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.q = q
	def run(self):
		#print ("Starting " + self.name)
		process_data(self.name, self.q)
		#print ("Exiting " + self.name)
def process_data(threadName, q):
	while not exitFlag:
		#print(125)
		queueLock.acquire()
		if not workQueue.empty():
			data = q.get()
			queueLock.release()
			print ("%s has deleted element from queue %s" % (threadName, data))
		else:
			queueLock.release()
		time.sleep(1)
threadList = ["Thread-1", "Thread-2", "Thread-3"]
nameList = ["One", "Two", "Three", "Four", "Five"]
queueLock = threading.Lock()
#workQueue = Queue.Queue(10)
workQueue = PriorityQueue()
threads = []
threadID = 1
# Create new threads
for tName in threadList:
	thread = add(threadID, "add_"+tName, workQueue)
	thread.start()
	threads.append(thread)
	threadID += 1
for tName in threadList:
	thread = delete(threadID, "delete_"+tName, workQueue)
	thread.start()
	threads.append(thread)
	threadID += 1
# Fill the queue
# queueLock.acquire()
# for word in nameList:
# 	workQueue.put(word)
# queueLock.release()
# Wait for queue to empty
while not workQueue.empty():
	pass
# Notify threads it's time to exit
exitFlag = 1
# Wait for all threads to complete
for t in threads:
	t.join()
print ("Exiting Main Thread")
