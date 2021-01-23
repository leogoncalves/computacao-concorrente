import java.util.*;
class Main {
	public static void main(String[] args) {
    int numberOfElements = 10;
    int numberOfThreads = 4;
		Vetor A = new Vetor();
    Vetor B = new Vetor();
    Vetor C = new Vetor(numberOfElements);

    A.populaVetor(numberOfElements);
    B.populaVetor(numberOfElements);

    Thread[] threads = new Thread[numberOfThreads];
    Worker worker = new Worker(numberOfElements, numberOfThreads, A, B, C);

    for(int i = 0; i < threads.length; i++) {
      threads[i] = new T(i, worker);
    }

    for(int i = 0; i < threads.length; i++) {
      threads[i].start();
    }

    for(int i = 0; i < threads.length; i++) {
      try{
        threads[i].join();
      }
      catch(InterruptedException IE){
        return;
      }
    }

    System.out.println("Vetor A: " + A.toString());
    System.out.println("Vetor B: " + B.toString());
    System.out.println("Vetor C: " + C.toString());
	}
}