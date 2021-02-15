class Counter {
    private int counter;

    Counter() {
        this.counter = 1;
    }

    public synchronized void increment(int value) {
        this.counter += value;
    }

    public synchronized void decrement(int value) {
        this.counter -= value;
    }

    public synchronized int getValue() {
        return this.counter;
    }
}