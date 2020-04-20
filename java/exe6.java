class BankAccount {
   public double balance;
   public BankAccount(double bal) { balance = bal; }
   
   
}

class Async_Producer extends Thread {
   private BankAccount account;
   public Async_Producer(BankAccount acct) { account = acct; }
   public void run() {
   System.out.println("before deposit balance " + account.balance);
   account.balance=account.balance+10; 
   System.out.println("after deposit balance " + account.balance);
  
   }
}


class sync_Producer extends Thread {
   private BankAccount account;
   public sync_Producer(BankAccount acct) { account = acct; }
   public void run() {
   System.out.println("before deposit balance " + account.balance);
   synchronized(account) { account.balance=account.balance+10; }
   System.out.println("after deposit balance " + account.balance);
  
   }
}

class Async_Consumer extends Thread {
   private BankAccount account;
   public Async_Consumer(BankAccount acct) { account = acct; }
   public void run() {
      
     
   System.out.println("before withdraw balance " + account.balance);
   if(account.balance>10) {account.balance=account.balance-10;} 

   System.out.println("after withdraw balance " + account.balance);

   }
}

class sync_Consumer extends Thread {
   private BankAccount account;
   public sync_Consumer(BankAccount acct) { account = acct; }
   public void run() {
      
     
   System.out.println("before withdraw balance " + account.balance);
   synchronized(account) {if(account.balance>10) {account.balance=account.balance-10;} }

   System.out.println("after withdraw balance " + account.balance);

   }
}

public class Bank {
   public static void main(String[] args) {
      BankAccount account = new BankAccount(100);
      
      int threadCount = 11;


      System.out.println("Asynchronized output ");

 System.out.println();
      Thread[] Async_threads = new Thread[threadCount];
      for(int i = 0; i < threadCount; i++) {
         if (i % 2 == 0) {
            Async_threads[i] = new Async_Producer(account);
         } else {
            Async_threads[i] = new Async_Consumer(account);
         }
      }
      for(int i = 0; i < threadCount; i++) {
         Async_threads[i].start();
      }
      for(int i = 0; i < threadCount; i++) {
         
            try {
            Async_threads[i].join();
         } catch(InterruptedException ie) {
               System.err.println(ie.getMessage());
                           
         }
         
      }
      System.out.print("final balance = ");
      System.out.println("$" + account.balance);

 System.out.println();
 account = new BankAccount(100);
      System.out.println("synchronized output ");
      Thread[] sync_threads = new Thread[threadCount];
      for(int i = 0; i < threadCount; i++) {
         if (i % 2 == 0) {
            sync_threads[i] = new sync_Producer(account);
         } else {
            sync_threads[i] = new sync_Consumer(account);
         }
      }
      for(int i = 0; i < threadCount; i++) {
         sync_threads[i].start();
      }
      for(int i = 0; i < threadCount; i++) {
         
            try {
            sync_threads[i].join();
         } catch(InterruptedException ie) {
               System.err.println(ie.getMessage());
                           
         }
         
      }
      System.out.print("final balance = ");
      System.out.println("$" + account.balance);
   }
}