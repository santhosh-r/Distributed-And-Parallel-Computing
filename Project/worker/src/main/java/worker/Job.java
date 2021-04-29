// package worker;

// import lombok.Data;

// enum JobType {
//   MERGE_SORT, MONTE_CARLO;
// }

// @Data // using lombok.Data to take care of getter and setter methods
// public class Job {
//   private int jobId;
//   private JobType jobType;
//   // private double[] inputData;
//   // private double[] outputData;
//   private boolean status;

//   public Job(int jobId, JobType jobType) { //, double[] data) {
//     this.jobId = jobId;
//     this.jobType = jobType;
//     // this.inputData = data;
//     this.status = false;
//   }
// }