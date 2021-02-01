class Main {
  public static void main(String[] args) {
    if ( args.length < 3 ) {
            System.out.println("Digite: java Main [tamanho do buffer] [quantidade de threads produtoras] [quantidade de threads consumidoras]");
            System.exit(-1);
        }

    int bufferCapacity = Integer.parseInt(args[0]);
    int numberOfProducer = Integer.parseInt(args[1]);
    int numberOfConsumer = Integer.parseInt(args[2]);

    Buffer buffer = new Buffer(bufferCapacity);
    int i;
    int numberOfThreads = numberOfConsumer + numberOfProducer;
    Thread[] threads = new Thread[numberOfThreads];
    System.out.println("Capacidade do Buffer: " + bufferCapacity + " Consumidoras: " + numberOfConsumer + "  Produtoras: " + numberOfProducer );
    for(i = 0; i < numberOfProducer; i++) {
      threads[i] = new Producer(i, buffer);
    }
    
    for(i = numberOfProducer; i < numberOfThreads; i++) {
      threads[i] = new Consumer(i, buffer);
    }

    for(i = 0; i < numberOfThreads; i++) {
      threads[i].start();
    }
  }
}