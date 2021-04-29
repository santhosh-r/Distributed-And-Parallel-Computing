package worker;

import project.Message;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.management.ManagementFactory;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Arrays;

import com.sun.management.OperatingSystemMXBean;

public class App {
    static final OperatingSystemMXBean osBean = (com.sun.management.OperatingSystemMXBean) ManagementFactory.getOperatingSystemMXBean();

    private static void waitUntilIdle() {
        long currentTime, startTime = System.currentTimeMillis();
        do {
            try {
                Thread.sleep(1000);
            } catch (Exception e) {
                System.err.println("Thread Sleep Error");
            }
            if (osBean.getSystemCpuLoad() >= 0.10) { // Reset time if CPU usage > 10%
                startTime = System.currentTimeMillis();
            }
            currentTime = System.currentTimeMillis();
        } while (currentTime - startTime <= (15 * 1000));
        System.out.println("System has been idle (<= 10% cpu usage) for >=15 seconds. Working...");
    }

    public static void main(String[] args) throws Exception, IOException, ClassNotFoundException {
        // Check arguments: server ip address and port
        InetAddress address;
        int regPort, workerPort, jobPort, runningStatusPort, exitStatusPort, workerId;
        boolean done = false;
        
        try {
            if (args.length < 2) {
                throw new Exception();
            }
            address = InetAddress.getByName(args[0]);
            regPort = Integer.parseInt(args[1]);
        } catch (Exception e) {
            System.err.println("Invalid arguments.");
            throw e;
        }

        // Register worker with server
        Socket socket = new Socket(address, regPort);
        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
        Message m = (Message) ois.readObject();
        int message = m.getMessage();
        workerId = (int) m.getData()[0];
        runningStatusPort = (int) m.getData()[1];
        exitStatusPort = (int) m.getData()[2];
        workerPort = (int) m.getData()[3];
        jobPort = (int) m.getData()[4];
        socket.close();

        System.out.println("Worker ID: " + workerId);
        System.out.println("\tStatus Ports: " + runningStatusPort + ", " + exitStatusPort);
        System.out.println("\tWorker Ports: " + workerPort + ", " + jobPort);
    
        // Handle Messages
        while (!done) {
            try {
                Thread.sleep(1000);
            } catch (Exception e) {
                System.err.println("Sleep Error");
                // System.out.println(e.toString());
                // e.printStackTrace();
            }
            
            try {
                socket = new Socket(address, runningStatusPort);
                ois = new ObjectInputStream(socket.getInputStream());
                m = (Message) ois.readObject();
                message = m.getMessage();
                if (message == Message.MESSAGE_CHECK) {
                    // System.out.println("MESSAGE_CHECK");
                }
                socket.close();
            } catch (Exception e) {
                // TODO: handle exception
                System.out.print("!");
            }

            try {
                socket = new Socket(address, workerPort);
                ois = new ObjectInputStream(socket.getInputStream());
                m = (Message) ois.readObject();
                message = m.getMessage();
                if (message == Message.MESSAGE_MERGE_SORT) {
                    System.out.println("MESSAGE_MERGE_SORT " + m.getData().length + " element(s)");
                    socket.close();
                    waitUntilIdle();
                    double[] array = m.getData();
                    if (array != null) {
                        new MergeSort(array);
                    }
                    socket = new Socket(address, jobPort);
                    ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
                    oos.writeObject(new Message(Message.MESSAGE_RESULTS, m.getJobId(), array));
                    System.out.println("Job with id " + m.getJobId() + " and type MERGE_SORT results uploaded");
                } else if (message == Message.MESSAGE_MONTE_CARLO) {
                    System.out.println("MESSAGE_MONTE_CARLO " + Arrays.toString(m.getData()));
                    socket.close();
                    waitUntilIdle();
                    long numberOfRandomPoints = (long) m.getData()[0];
                    MonteCarlo mc = new MonteCarlo(numberOfRandomPoints);
                    long pointsInside = mc.getPointsInside();
                    socket = new Socket(address, jobPort);
                    ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
                    oos.writeObject(new Message(Message.MESSAGE_RESULTS, m.getJobId(), new double[] {pointsInside}));
                    System.out.println("Job with id " + m.getJobId() + " and type MONTE_CARLO results uploaded");
                }
                socket.close();
            } catch (Exception e) {
                // TODO: handle exception
                // System.out.print("^");
            }

            try {
                socket = new Socket(address, exitStatusPort);
                ois = new ObjectInputStream(socket.getInputStream());
                m = (Message) ois.readObject();
                message = m.getMessage();
                if (message == Message.MESSAGE_DONE) {
                    System.out.println("Server sent DONE message. Exiting...");
                    done = true;
                }
                socket.close();
            } catch (Exception e) {
                // TODO: handle exception
                // System.out.print(".");
            }
        }

        System.exit(0);
    }
}
