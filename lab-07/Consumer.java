class Consumer extends Thread {
  private int tid;
  private Buffer buffer;
  
  Consumer(int tid, Buffer buffer) {
    this.tid = tid;
    this.buffer = buffer;
  }
  public void run() {
    while(true) {
      this.buffer.removeElement(this.tid);
    }
  }
}