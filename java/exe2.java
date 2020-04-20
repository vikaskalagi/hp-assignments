public class MyThread extends Thread {
public MyThread(String name) {
super(name);
}
@Override
public void run() {
System.out.println("Executing thread "+Thread.currentThread().getName());
}
public static void main(String[] args) throws InterruptedException {
MyThread myThread1 = new MyThread("myThread1");
myThread1.start();
MyThread myThread2 = new MyThread("myThread2");
myThread2.start();
MyThread myThread3 = new MyThread("myThread3");
myThread3.start();
MyThread myThread4 = new MyThread("myThread4");
myThread4.start();
MyThread myThread5 = new MyThread("myThread5");
myThread5.start();
}
}
