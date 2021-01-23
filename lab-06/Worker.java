import java.util.*;

class Worker {
  private int numberOfTerms;
  private int numberOfThreads;
  private Vetor A;
  private Vetor B;
  private Vetor C;

  Worker(int numberOfTerms, int numberOfThreads, Vetor A, Vetor B, Vetor C) {
    this.numberOfTerms = numberOfTerms;
    this.numberOfThreads = numberOfThreads;
    this.A = A;
    this.B = B;
    this.C = C;
  }

   public synchronized void sumElement(int id) {
    int ini, end;
    int blockSize = this.numberOfTerms / this.numberOfThreads;
    ini = id * blockSize;
    end = id == numberOfThreads - 1 ? numberOfTerms : ini + blockSize;

    for(int idx = ini; idx < end; idx++) {
      int value = this.A.getItem(idx) + this.B.getItem(idx);
      this.C.setItem(idx, value);
    }
  }
}