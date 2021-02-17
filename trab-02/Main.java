import java.util.Arrays;
import java.util.Random;
import java.time.Instant;
import java.time.Duration;

/** 
* Classe principal da aplicação. Utilizada para a execução da comparação entre os algoritmos
* com recursão simples e com múltiplas threads.
*/
class Main {

  /**
  * Método principal da aplicação. Fazemos 5 execuções de ordenação de um array de 
  * 10^7 elementos inteiros gerados de forma pseudoaleatoria. Cada execução possui uma 
  * nova tomada de tempo
  */
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

  /**
   * @param size O tamanho do array que será gerado
  */
  public static Integer[] createRandomIntegerArrayInteger(Integer size) {
    Integer[] array = new Integer[size];
    Random random = new Random();
    for (int i = 0; i < array.length; i++) {
      array[i] = random.nextInt(size);
    }
    return array;
  }
}