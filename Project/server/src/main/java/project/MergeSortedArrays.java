package project;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.Stream;

public class MergeSortedArrays {
    private double[] mergedArray;

    protected class Merge implements Callable<Double[]> {
        double[] arrayA;
        double[] arrayB;
        Double[] sorted;

        public Merge(double[] arrayA, double[] arrayB) {
            this.arrayA = arrayA;
            this.arrayB = arrayB;
            this.sorted = new Double[arrayA.length + arrayB.length];
        }

        @Override
        public Double[] call() throws Exception {
            int i = 0, j = 0, k = 0;
            while (i < this.arrayA.length && j < this.arrayB.length) {
                if (this.arrayA[i] < this.arrayB[j]) {
                    this.sorted[k++] = this.arrayA[i++];
                } else {
                    this.sorted[k++] = this.arrayB[j++];
                }
            }
            while (i < this.arrayA.length) {
                this.sorted[k++] = this.arrayA[i++];
            }
            while (j < this.arrayB.length) {
                this.sorted[k++] = this.arrayB[j++];
            }
            return this.sorted;
        }
    }

    public MergeSortedArrays(double[][] sortedArrays) {
        int length = sortedArrays.length;
        int processorCount = Runtime.getRuntime().availableProcessors();
        ExecutorService pool = Executors.newFixedThreadPool(processorCount);
        while (length > 1) {
            int newLength = (int) Math.ceil((double) length/2);
            double[][] mergedArrays = new double[newLength][];
            if (length % 2 == 1) {
                mergedArrays[newLength - 1] = sortedArrays[length - 1];
            }
            List<Future<Double[]>> list = new ArrayList<Future<Double[]>>();
            for (int i = 0; i < length - 1; i += 2) {
                Future<Double[]> future = pool.submit(new Merge(sortedArrays[i], sortedArrays[i + 1]));
                list.add(future);
            }
            int i = 0;
            for (Future<Double[]> future : list){
                try {
                    mergedArrays[i++] = Stream.of(future.get()).mapToDouble(Double::doubleValue).toArray();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            sortedArrays = mergedArrays;
            length = newLength;
        }
        pool.shutdown();
        this.mergedArray = sortedArrays[0];
    }

    double[] getMergedArray() {
        return this.mergedArray;
    }
}