class T extends Thread {
  private int id;
  private Worker worker;

  public T(int id, Worker worker) {
    this.id = id;
    this.worker = worker;
  }

  public void run(){
    this.worker.sumElement(this.id);
  }
  
 
}