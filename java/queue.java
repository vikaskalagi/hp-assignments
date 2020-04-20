import java.util.*; 

class BankAccount {
   Queue<Integer> Q = new LinkedList<Integer>();
   public BankAccount(Queue bal) { Q = bal; }
   }
   

class Producer extends Thread {
   private BankAccount account;
   public Producer(BankAccount acct) { account = acct; }
   public void run() {
       Random rand = new Random();
               int rand_int1 = rand.nextInt(1000); 
System.out.println("interger added to queue " + rand_int1+" by "+ Thread.currentThread().getName());
    account.Q.add(rand_int1);
    synchronized (account) {
    account.notify();
    }   
   //System.out.println("before deposit balance " + account.balance);
   //account.balance=account.balance+10; 
   //System.out.println("after deposit balance " + account.balance);
  
   }
}   
   

class Consumer extends Thread {
  private BankAccount account;
  public Consumer(BankAccount acct) { account = acct; }
  public void run() {
      synchronized (account) {
try {
account.wait();
} catch (InterruptedException e) {
e.printStackTrace();
}}
if (!account.Q.isEmpty()) {
Integer integer = account.Q.poll();
System.out.println("[" + Thread.currentThread().getName() + "]: " + integer);
}
  //System.out.println("before deposit balance " + account.balance);
  //account.balance=account.balance+10; 
  //System.out.println("after deposit balance " + account.balance);
  
  }
}   
      
   
// class Producer extends Thread {
// public consumer(BankAccount acct) { account = acct; }    
// public void run() {
// synchronized (account) {
// try {
// account.wait();
// } catch (InterruptedException e) {
// e.printStackTrace();
// }
// }
// if (1) {
// Integer integer = account.Q.poll();
// System.out.println("[" + Thread.currentThread(). getName() + "]: " + integer);

// }}
// }
 
public class GFG { 
    public static void main(String[] args) 
        throws IllegalStateException 
    { 
  
        // create object of Queue 
 Queue<Integer> Qu = new LinkedList<Integer>();
        BankAccount Q = new BankAccount(Qu);
        // Add numbers to end of Queue 
        
        System.out.println("synchronized output ");
      Thread[] sync_threads = new Thread[10];
      for(int i = 0; i < 10; i++) {
         if (i % 2 == 0) {
            sync_threads[i] = new Producer(Q);
         } else {
            sync_threads[i] = new Consumer(Q);
         }
      }
      for(int i = 0; i < 10; i++) {
         sync_threads[i].start();
      }
      for(int i = 0; i < 10; i++) {
         
            try {
            sync_threads[i].join();
         } catch(InterruptedException ie) {
               System.err.println(ie.getMessage());
                           
         }
         
      }
       
    }
} 

