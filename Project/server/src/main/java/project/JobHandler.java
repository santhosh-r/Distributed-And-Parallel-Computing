package project;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;

import javax.annotation.PreDestroy;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.task.TaskExecutor;
import org.springframework.stereotype.Component;

@Component
public class JobHandler implements Runnable {
    @Autowired
    private JobRepository jobRepo;

    @Autowired
    private WorkerRepository workerRepo;

    @Autowired
    private FileService fileService;

    @Autowired
    private TaskExecutor taskExecutor;

    private static final Object lock = new Object();

    protected class Delegator implements Runnable {
        @Override
        public void run() {
            while (true) {
                try {
                    Thread.sleep(1000);
                } catch (Exception e) {
                    System.err.println("Delegator: Thread Sleep Error");
                }

                List<Job> jobs = jobRepo.findByStatus("Submitted");
                List<Worker> workers = workerRepo.findByConnected(true);
                if (!jobs.isEmpty() && !workers.isEmpty()) {
                    Job job = jobs.get(0);
                    int message = 0;
                    double[][] portions = {};
                    String inputFilePath = fileService.getInputFileFullPath(job.getInputFilename());
                    String inputFileContents;
                    try {
                        inputFileContents = new String(Files.readAllBytes(Paths.get(inputFilePath)));
                    } catch (Exception e) {
                        System.err.println("Delegator: Input File Error");
                        job.setStatus("Input File Error");
                        jobRepo.save(job);
                        continue;
                    }
                    // System.out.println(inputFileContents);
                    String[] tokens = inputFileContents.split(" ");
                    double[] input = new double[tokens.length];
                    for (int i=0; i<tokens.length; i++) {
                        input[i] = Double.parseDouble(tokens[i]); 
                    }
                    
                    if (job.getJobType() == JobType.MERGE_SORT) {
                        message = Message.MESSAGE_MERGE_SORT;
                        portions = new double[workers.size()][];
                        int portionSize = input.length/workers.size();
                        int remainder = input.length % workers.size();
                        if (portionSize > 0) {
                            for (int i = 0, j = 0; i < input.length; j++){
                                int portionEnd = Math.min(input.length, i + portionSize);
                                if (j < remainder) {
                                    portionEnd++;
                                }
                                portions[j] = Arrays.copyOfRange(input, i, portionEnd);
                                i = portionEnd;
                            }
                            job.setExtraData(0L);
                        } else {
                            portions[0] = input;
                            job.setExtraData(1L);
                        }
                    } else if (job.getJobType() == JobType.MONTE_CARLO) {
                        message = Message.MESSAGE_MONTE_CARLO;
                        job.setExtraData((long) input[0]);
                        portions = new double[workers.size()][1];
                        double portionData = input[0]/workers.size();
                        for (int i = 0; i < workers.size(); i++) {
                            portions[i][0] = portionData;
                        }
                    }
                    synchronized (lock) { // Serialize Job Delegation
                        for (int i = 0; i < workers.size(); i++) {
                            try (ServerSocket server = new ServerSocket(10424)) {
                                Socket s = server.accept();
                                Message m = new Message(message);              
                                m.setData(portions[i]);
                                m.setJobId(job.getId());
                                ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
                                oos.writeObject(m);
                                s.close();
                            } catch (Exception e) {
                                System.err.println("Delegator: Socket Error");
                                e.printStackTrace();
                            }
                        }
                    }
                    job.setStatus("Delegated");
                    System.out.println("Job with id " + job.getId() + " and type " + job.getJobType() + " delegated");
                    int[] assignedWorkerIds = new int[workers.size()];
                    for (int i = 0; i < assignedWorkerIds.length; i++) {
                        assignedWorkerIds[i] = workers.get(i).getId();
                    }
                    job.setAssignedWorkerIds(assignedWorkerIds);
                    jobRepo.save(job);
                }
            }
        }
    }

    protected class ResultsCompiler implements Runnable {
        @Override
        public void run() {
            ServerSocket server;

            try {
                server = new ServerSocket(10842);
                server.setSoTimeout(60 * 1000);
                while (true) {
                    try {
                        Thread.sleep(1000);
                    } catch (Exception e) {
                        System.err.println("ResultsCompiler: Thread Sleep Error");
                    }
                    
                    List<Job> jobs = jobRepo.findByStatus("Delegated");
                    if (jobs.isEmpty()) {
                        continue;
                    }
                    synchronized (lock) {
                        Job job = jobs.get(0);
                        int[] workerIds = job.getAssignedWorkerIds();
                        int numberOfWorkers = workerIds.length;
                        double[][] results = new double[workerIds.length][];
                        long startTime = System.currentTimeMillis();
                        boolean failed = false;
                        for (int i = 0; i < numberOfWorkers; i++) {
                            try {
                                Socket socket = server.accept();
                                ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
                                Message m = (Message) ois.readObject();
                                socket.close();
                                results[i] = m.getData();
                            } catch (Exception e) {
                                System.err.println("ResultsCompiler: Socket Error");
                                e.printStackTrace();
                                failed = true;
                                break;
                            }
                            long currentTime = System.currentTimeMillis();
                            if (i != (numberOfWorkers - 1) && (currentTime - startTime) > (1 * 60 * 1000)) {
                                failed = true;
                                break;
                            }
                        }
                        if (!failed) {
                            String outputFilePath = fileService.getOutputFileFullPath(job.getOutputFilename());
                            PrintWriter writer = new PrintWriter(outputFilePath, "utf-8");
                            if (job.getJobType() == JobType.MONTE_CARLO) {
                                long pointsInside = 0L;
                                for (int i = 0; i < results.length; i++) {
                                    pointsInside += (long) results[i][0];
                                }
                                double calculatedPI = 4.0 * pointsInside/job.getExtraData();
                                writer.println(calculatedPI);
                                job.setStatus("Done");
                                System.out.println("Job with id " + job.getId() + " and type " + job.getJobType() + " finished");
                                jobRepo.save(job);
                            } else if (job.getJobType() == JobType.MERGE_SORT) {
                                double[] mergedArray = {};
                                if (job.getExtraData() == 0L) {
                                    mergedArray = new MergeSortedArrays(results).getMergedArray();
                                } else {
                                    mergedArray = results[0];
                                }
                                if (mergedArray != null) {
                                    String[] array = Arrays.stream(mergedArray).mapToObj(String::valueOf).toArray(String[]::new);
                                    writer.println(String.join(" ", array));
                                    job.setStatus("Done");
                                    System.out.println("Job with id " + job.getId() + " and type " + job.getJobType() + " finished");
                                } else {
                                    job.setStatus("Invalid Input");
                                }
                                jobRepo.save(job);
                            }
                            writer.close();
                        } else {
                            job.setStatus("Failed");
                            jobRepo.save(job);
                        }
                    }
                }
            } catch (Exception e) {
                System.err.println("ResultsCompiler: Server Socket or other Error");
                e.printStackTrace();
            }
        }
    }
    
    protected class WorkerRegister implements Runnable {
        @Override
        public void run() {
            ServerSocket server;

            try {
                server = new ServerSocket(10421);

                while (true) {
                    Socket socket = server.accept();
                    Worker w = workerRepo.findByAddressAndPort(socket.getInetAddress(), socket.getPort());
                    if (w == null) {
                        w = new Worker(socket.getInetAddress(), socket.getPort());
                        workerRepo.save(w);
                    } else {
                        System.err.println("WorkerRegister: Worker " + w.getAddressString() + " already registered");
                    }
                    ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
                    oos.writeObject(new Message(Message.MESSAGE_INIT, 0, new double[] {w.getId(), 10422, 10423, 10424, 10842}));
                    socket.close();
                }
            } catch (Exception e) {
                System.err.println("WorkerRegister: Server/Socket Error");
                e.printStackTrace();
            }
        }
    }

    protected class WorkerCheck implements Runnable {
        @Override
        public void run() {
            while (true) {
                try {
                    Thread.sleep(200);
                } catch (Exception e) {
                    System.err.println("WorkerCheck: Thread Sleep Error");
                    e.printStackTrace();
                }

                List<Worker> workers = workerRepo.findAll();
                for (Worker w : workers) {
                    try (ServerSocket server = new ServerSocket(10422)) {
                        Socket s = server.accept();
                        ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
                        oos.writeObject(new Message(Message.MESSAGE_CHECK));
                        s.close();
                        w.setConnected(true);
                    } catch (Exception e) {
                        w.setConnected(false);
                    }
                    workerRepo.save(w);
                }
            }
        }
    }

    public void run() {
        taskExecutor.execute(new WorkerRegister());
        taskExecutor.execute(new WorkerCheck());
        taskExecutor.execute(new Delegator());
        taskExecutor.execute(new ResultsCompiler());
    }

    @PreDestroy
    public void destroy() {
        System.out.println("\n\nSending DONE messages to clients...");
        List<Worker> workers = workerRepo.findAll();
        for (Worker w : workers) {
            if (w.isConnected()) {
                try (ServerSocket server = new ServerSocket(10423)) {
                    server.setSoTimeout(1000);
                    Socket s = server.accept();
                    ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
                    oos.writeObject(new Message(Message.MESSAGE_DONE));
                    s.close();
                } catch (Exception e) {
                    System.err.println("JobHandler.destroy(): Socket Error");
                    e.printStackTrace();
                }
            }
        }
        System.out.println("...Finished\n\n");
    }
}