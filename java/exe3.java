public class MyRunnable implements Runnable {
public void run() {
System.out.println("Executing thread 1 "+Thread.currentThread().getName());
}
public static void main(String[] args) throws InterruptedException {
Thread myThread = new Thread(new MyRunnable(), "vikas");
myThread.start();
}
}
