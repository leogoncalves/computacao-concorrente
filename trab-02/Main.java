import java.util.Arrays;
import java.util.Random;
import java.time.Instant;
import java.time.Duration;

class Main {
  public static void main(String[] args) {
    for(int i = 0; i < 5; i++) {
        Integer array[] = createRandomIntegerArrayInteger(10000000);
        Integer array2[] = array.clone();
        Duration interval;
        Instant start, end;

        QuickSort sorter = new QuickSort();
        start = Instant.now();
        sorter.sort(array, 0, array2.length - 1);
        end = Instant.now();    
        interval = Duration.between(start, end);
        System.out.println("Tempo de execução do algoritmo sequencial (em segundos): " + interval);
        

        start = Instant.now();
        ParallelQuickSort.quicksort(array2);
        end = Instant.now();    
        interval = Duration.between(start, end);
        System.out.println("Tempo de execução do algoritmo paralelo (em segundos): " + interval);

        boolean result = Arrays.equals(array, array2);

        if(result) {
        System.out.println("Arrays são iguais");
        } else {
        System.out.println("Arrays são diferentes");
        }
    }
  }

  public static int[] createRandomIntegerArray(int size) {
    int[] array = new int[size];
    Random random = new Random();
    for (int i = 0; i < array.length; i++) {
      array[i] = random.nextInt(100);
    }
    return array;
  }

  public static Integer[] createRandomIntegerArrayInteger(Integer size) {
    Integer[] array = new Integer[size];
    Random random = new Random();
    for (int i = 0; i < array.length; i++) {
      array[i] = random.nextInt(size);
    }
    return array;
  }
}