class Main {
  public static void main(String[] args) {
    Buffer buffer = new Buffer(5);
    int numberOfProducer = 2;
    int numberOfConsumer = 2;
    int i;
    int numberOfThreads = numberOfConsumer + numberOfProducer;

    Thread[] threads = new Thread[numberOfThreads];

    for(i = 0; i < numberOfProducer; i++) {
      // System.out.println("Criando Produtor " + i);
      threads[i] = new Producer(i, buffer);
    }
    
    for(i = numberOfProducer; i < numberOfThreads; i++) {
      // System.out.println("Criando Consumidor " + i);
      threads[i] = new Consumer(i, buffer);
    }

    for(i = 0; i < numberOfThreads; i++) {
      // System.out.println("Iniciando thread " + i);
      threads[i].start();
    }

    // for(i = 0; i < numberOfThreads; i++) {
    //   try{
    //     threads[i].join();
    //   } catch(InterruptedException IE){
    //     System.out.println(IE.toString());
    //   }
    // }
  }
}