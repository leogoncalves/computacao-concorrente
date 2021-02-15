class QuickSort {
  
  int partition(Integer array[], int begin, int end){
    int pivot = array[end];
    int i = begin - 1;
    for (int j = begin; j < end; j++) {
      if(array[j] < pivot) {
        i++;
        int swap = array[i];
        array[i] = array[j];
        array[j] = swap;
      }
    }
    int swap = array[i+1];
    array[i+1] = array[end];
    array[end] = swap;
    return i + 1;
  }

  void sort(Integer array[], int begin, int end) {
    if(begin < end) {
      int partition = partition(array, begin, end);
      sort(array, begin, partition - 1);
      sort(array, partition + 1, end);
    }
  }
}