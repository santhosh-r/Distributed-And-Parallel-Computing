package worker;

import java.util.Arrays;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

public class MergeSort {

    public class MergeSortAction extends RecursiveAction {
        
        private static final long serialVersionUID = 6129705971373310633L;
        double[] array;
        int from;
        int to;

        public MergeSortAction(double[] array, int from, int to) {
            this.array = array;
            this.from = from;
            this.to = to;
        }
        
        @Override
        protected void compute() {
            if (this.from >= this.to) {
                return;
            }
            if (to - from < 128) {
                double[] temp = Arrays.copyOfRange(this.array, from, to + 1);
                Arrays.sort(temp);
                for (int i = 0; i < temp.length; i++) {
                    this.array[from + i] = temp[i];
                }
                return;
            }
            int mid = (this.from + this.to)/2;
            invokeAll(new MergeSortAction(this.array, this.from, mid),
                new MergeSortAction(this.array, mid + 1, this.to));
            merge(this.array, this.from, mid, this.to);
        }

        public void merge(double[] array, int from, int mid, int to) {
            double[] arrayA = new double[mid - from + 1];
            double[] arrayB = new double[to - mid];
            for (int i = 0; i < arrayA.length; i++) {
                arrayA[i] = array[from + i];        
            }
            for (int i = 0; i < arrayB.length; i++) {
                arrayB[i] = array[mid + 1 + i];        
            }
            int i = 0, j = 0, k = from;
            while (i < arrayA.length && j < arrayB.length) {
                if (arrayA[i] < arrayB[j]) {
                    array[k++] = arrayA[i++];
                } else {
                    array[k++] = arrayB[j++];
                }
            }
            while (i < arrayA.length) {
                array[k++] = arrayA[i++];
            }
            while (j < arrayB.length) {
                array[k++] = arrayB[j++];
            }
        }
    }

    public MergeSort(double[] unsortedArray) throws InterruptedException {
        int processorCount = Runtime.getRuntime().availableProcessors();
        ForkJoinPool pool = new ForkJoinPool(processorCount);    
        pool.invoke(new MergeSortAction(unsortedArray, 0, unsortedArray.length - 1));    
    }
}