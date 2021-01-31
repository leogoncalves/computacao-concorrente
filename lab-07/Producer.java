class Producer extends Thread {
  private int tid;
  private Buffer buffer;

  Producer(int tid, Buffer buffer) {
    this.tid = tid;
    this.buffer = buffer;
  }

  public void run() {
    while(true) {
      this.buffer.addElement(this.tid, this.tid);
    }
  }
}