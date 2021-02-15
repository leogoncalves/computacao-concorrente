import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class ParallelQuickSort {
  
  /**
   * 
  */
  private static final Integer NTHREADS = Runtime.getRuntime().availableProcessors();

  /**
   *
  */
  private static Executor pool = Executors.newFixedThreadPool(NTHREADS);

  /**
   * @param array
  */
  public static void quicksort(Integer[] array) {
    final AtomicInteger count = new AtomicInteger(1);
    pool.execute(new QuickSortRunnable(array, 0, array.length - 1, count));
    try {
      synchronized (count) {
        count.wait();
      }
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  /**
   *
  */
  public static class QuickSortRunnable implements Runnable {
    private final Integer[] array;
    private final Integer begin;
    private final Integer end;
    private final AtomicInteger count;

    /**
    *
    */
    public QuickSortRunnable(Integer[] array, Integer begin, Integer end, AtomicInteger count) {
      this.array = array;
      this.begin = begin;
      this.end = end;
      this.count = count;
    }

    /**
    *
    */
    @Override
    public void run() {
      quicksort(begin, end);
      synchronized (count){ 
        if (count.getAndDecrement() == 1) {
          count.notify();
        }
      }
    }

    /**
    *
    */
    private void quicksort(Integer begin, Integer end) {
      if (begin < end) {
        Integer index = partition(begin, end);
        if (count.get() >= NTHREADS) {
          quicksort(begin, index - 1);
          quicksort(index + 1, end);
        } else {
          count.getAndAdd(2);
          pool.execute(new QuickSortRunnable(array, begin, index - 1, count));
          pool.execute(new QuickSortRunnable(array, index + 1, end, count));
        }
      }
    }

    /**
    *
    */
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

    /**
    *
    */
    private void swap(Integer a, Integer b) {
      Integer value = array[a];
      array[a] = array[b];
      array[b] = value;
    }
  }
}