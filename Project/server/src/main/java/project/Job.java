package project;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import lombok.Data;

enum JobType {
    MERGE_SORT, MONTE_CARLO;
}

@Data // using lombok.Data to take care of getter and setter methods
@Entity
public class Job {
    
    @Id
    @GeneratedValue(strategy=GenerationType.AUTO)
    private int id;
    private JobType jobType;
    private String inputFilename;
    private String outputFilename;
    private long extraData;
    private String status;
    private int[] assignedWorkerIds;

    public Job() {
        this.status = "Submitted";
    }

    public int getNumberOfWorkers() {
        if (this.assignedWorkerIds == null) {
            return 0;
        }
        return this.assignedWorkerIds.length;
    }

    public String getInputFilePath() {
        Path rootLocation = Paths.get("uploads");
        return rootLocation.resolve(this.inputFilename).toString();
    }

    public String getOutputFilePath() {
        Path rootLocation = Paths.get("downloads");
        try {
            Files.createDirectories(rootLocation);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return rootLocation.resolve(this.outputFilename).toString();
    }
}