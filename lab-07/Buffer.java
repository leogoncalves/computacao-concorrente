import java.util.Arrays;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

class Buffer {

  private BlockingQueue<Integer> buffer;

  Buffer(int size) {
    this.buffer = new ArrayBlockingQueue<Integer>(size, true);
  }

  public synchronized void printElements() {
    System.out.println(this.buffer.toString());
  }

  public synchronized void addElement(int tid, int value) {
    try {
      while(this.buffer.remainingCapacity() == 0) {
        System.out.println("Thread Produtora " + tid + " chegou, mas buffer está cheio");
        this.printElements();
        wait();
      }
      this.buffer.put(value);
      System.out.println("Thread Produtora: " + tid + " inseriu " + value);
      this.printElements();
      notifyAll();
    } catch (InterruptedException e) {
      System.out.println(e);
    }
  }

  public synchronized void removeElement(int tid) {
    try {
      while (this.buffer.isEmpty()) {
        System.out.println("Thread Consumidora " + tid + " chegou, mas o buffer está vazio");
        wait();
      }
      int removed = this.buffer.take();
      System.out.println("Thread Consumidora " + tid + " removeu " + removed);
      System.out.println("Novo estado do buffer");
      this.printElements();
      notifyAll();
    } catch (InterruptedException e) {
      System.out.println(e);
    }
  }
}