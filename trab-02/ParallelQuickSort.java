import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class ParallelQuickSort {
  
  /**
   * Número de threads que serão utilizadas pela aplicação.
  */
  private static final Integer NTHREADS = Runtime.getRuntime().availableProcessors();

  /**
   * Pool de threads que serão utilizadas pela aplicação. Criada com um valor fixo de 
   * threads
  */
  private static Executor pool = Executors.newFixedThreadPool(NTHREADS);

  /**
   * @param array Vetor de elementos que serão ordenados
  */
  public static void quicksort(Integer[] array) {
    // Variável contadora que utilizamos para controlar o número de threads em execução
    final AtomicInteger count = new AtomicInteger(1);
    
    // Invoca um executor (elemento da pool) e atribui a ele uma carga de trabalho. 
    // No caso, criamos uma nova instancia da nossa classe que implementa Runnable
    // O construtor recebe o array de elementos a ser ordenado, a posição inicial 
    // e final do array e uma variável de controle (em ordem)
    pool.execute(new QuickSortRunnable(array, 0, array.length - 1, count));
    try {
      // avalia se count != 0
      synchronized (count) {
        // poe a thread em espera
        count.wait();
      }
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  /**
   * @param array Vetor de elemetos a ser ordenado
   * @param begin posição do primeiro elemento do array
   * @param end posição do último elemento do array
   * @param count Variável de controle para checar quantas threads estão ativas
   * 
  */
  public static class QuickSortRunnable implements Runnable {
    private final Integer[] array;
    private final Integer begin;
    private final Integer end;
    private final AtomicInteger count;

    /**
    * Construtor da classe, recebe os parametros
    * 
    * @param array Vetor de inteiros que será ordenado
    * @param begin Posição inicial do vetor
    * @param end Posição final do vetor
    * @param count Variável de controle para checar quantas threads estão ativas
    */
    public QuickSortRunnable(Integer[] array, Integer begin, Integer end, AtomicInteger count) {
      this.array = array;
      this.begin = begin;
      this.end = end;
      this.count = count;
    }

    /**
    * Método executado pela thread
    * Como implementamos a interface Runnable, precisamos
    * implementar o método run na nossa classe.
    * Aqui, fazemos a chamada ao nosso quicksort, que recebe
    * como parametro os valores de begin e end do nosso vetor
    * a ser ordenado.
    * Temos um bloco syncronized para verificar se nossa variável
    * de controle chegou a 1. Caso isso aconteça, ela deve enviar um sinal 
    * para a fila de threads.
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
    * @param begin posição inicial do vetor
    * @param end posição final do vetor.
    * Executa a chamada recursiva    
    * 
    */
    private void quicksort(Integer begin, Integer end) {
      // Caso begin < end, calculamos uma nova partição. 
      if (begin < end) {
        
        // fazemos uma nova partição e encontramos um pivo
        Integer index = partition(begin, end);
        
        // Caso a variável de controle seja maior ou igual ao número de threads, fazemos a chamada recursiva
        if (count.get() >= NTHREADS) {
          quicksort(begin, index - 1);
          quicksort(index + 1, end);
        } else {
          // Caso não tenhamos atingido o número máximo de threads
          
          // incrementamos o valor da variável de controle em 2
          count.getAndAdd(2);
          
          // Lançamos duas novas threads
          pool.execute(new QuickSortRunnable(array, begin, index - 1, count));
          pool.execute(new QuickSortRunnable(array, index + 1, end, count));
        }
      }
    }

    /**
    * @param begin posição inicial
    * @param end posição final
    * Função utilizada para criar uma partição para o 
    * quicksort. 
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
    * @param a Elemento do vetor
    * @param b Elemento do vetor
    * Realiza a troca de posição entre 
    * dois elementos de um vetor
    */
    private void swap(Integer a, Integer b) {
      Integer value = array[a];
      array[a] = array[b];
      array[b] = value;
    }
  }
}