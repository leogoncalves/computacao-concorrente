class Counter {
    int counter = 1;

    public synchronized void increment(int value) {
        this.counter += value;
    }

    public synchronized void decrement() {
        this.counter--;
    }

    public synchronized int getValue() {
        System.out.println("getValue " + this.counter);
        return this.counter;
    }
}