package project;

import java.io.Serializable;
import lombok.Data;

@Data
public class Message implements Serializable {
    private static final long serialVersionUID = 4785233908324407025L;
    public static final int MESSAGE_INIT = 0x0110011;
    public static final int MESSAGE_DONE = 0x0011011;
    public static final int MESSAGE_CHECK = 0x0101101;
    public static final int MESSAGE_MERGE_SORT = 0x0101010;
    public static final int MESSAGE_MONTE_CARLO = 0x0010101;
    public static final int MESSAGE_RESULTS = 0x1010101;

    private int message;
    private int jobId;
    private int workerId;
    private double[] data;
    
    public Message(int message) {
        this.message = message;
    }

    public Message(int message, int jobId, double[] data) {
        this.message = message;
        this.jobId = jobId;
        this.data = data;
    }
}