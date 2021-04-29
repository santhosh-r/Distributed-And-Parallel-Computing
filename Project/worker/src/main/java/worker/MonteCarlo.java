package worker;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MonteCarlo {
    long pointsInside;
    double calculatedPI;
    CountDownLatch latch;
    static final Object lock = new Object();

    protected class ComputePI implements Runnable {
        long totalPoints;
        
        public ComputePI(long totalPoints) {
            this.totalPoints = totalPoints;
        }

        @Override
        public void run() {
            long inside = 0L;
            for (int i = 0; i < totalPoints; i++) {
                double x = Math.random();
                double y = Math.random();
                if (x*x + y*y <= 1.0) {
                    inside++;       
                }
            }
            synchronized (lock) {
                pointsInside += inside;
            }
            latch.countDown();
        }
    }

    public MonteCarlo(long numberOfRandomPoints) throws InterruptedException {
        int processorCount = Runtime.getRuntime().availableProcessors();
        latch = new CountDownLatch(processorCount);
        ExecutorService pool = Executors.newFixedThreadPool(processorCount);
        this.pointsInside = 0L;
        for (int i = 0; i < processorCount; i++) {
            pool.execute(new ComputePI(numberOfRandomPoints/processorCount));
        }
        latch.await();
    }
    
    public long getPointsInside() {
        return this.pointsInside;
    }
}