

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class ParallelQuickSortWithCounter {
  private static final Integer NTHREADS = Runtime.getRuntime().availableProcessors();
  private static Executor pool = Executors.newFixedThreadPool(NTHREADS);
  final static Counter counter = new Counter();
  
  public static void quicksort(Integer[] array) {    
    pool.execute(new QuickSortRunnable(array, 0, array.length - 1, counter));
    try {
      synchronized (counter) {
        counter.wait();
      }
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  public static class QuickSortRunnable implements Runnable {
    private final Integer[] array;
    private final Integer begin;
    private final Integer end;
    private final Counter counter;

    public QuickSortRunnable(Integer[] array, Integer begin, Integer end, Counter counter) {
      this.array = array;
      this.begin = begin;
      this.end = end;
      this.counter = counter;
    }

    @Override
    public void run() {
      quicksort(begin, end);
      synchronized (counter) {
        counter.decrement();
        if (counter.getValue() == 1) {
          counter.notify();
        }
      }
    }

    private void quicksort(Integer begin, Integer end) {
      if (begin < end) {
        Integer index = partition(begin, end);
        if (counter.getValue() >= NTHREADS) {
          quicksort(begin, index - 1);
          quicksort(index + 1, end);
        } else {
          counter.increment(2);
          pool.execute(new QuickSortRunnable(array, begin, index - 1, counter));
          pool.execute(new QuickSortRunnable(array, index + 1, end, counter));
        }
      }
    }

    private Integer partition(Integer begin, Integer end) {
      Integer pivot = array[end];
      Integer index = begin;
      for (Integer i = begin; i < end; i++) {
        if (array[i].compareTo(pivot) < 0) {
          swap(i, index);
          index += 1;
        }
      }
      swap(index, end);
      return index;
    }

    private void swap(Integer a, Integer b) {
      Integer value = array[a];
      array[a] = array[b];
      array[b] = value;
    }
  }
}