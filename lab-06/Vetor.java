import java.util.*;

public class Vetor {
  private List<Integer> vetor;
	
  public Vetor() {
    this.vetor = new ArrayList<Integer>();      
  }

  public Vetor(int n) {
    this.vetor = Arrays.asList(new Integer[n]);
    for(int i = 0; i < n; i++) {
      this.vetor.set(i, 0);
    } 
    
  }

  public int getItem(int idx) {
    return this.vetor.get(idx);
  }

  public void setItem(int idx, int item) {
    this.vetor.set(idx, item);
  }

  public void populaVetor(int n){
    for(int i = 0; i < n; i++) {
      this.vetor.add(i+1);
    }
  }

  @Override
  public String toString(){
    return this.vetor.toString(); 
  }
}