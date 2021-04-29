package project;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.core.task.TaskExecutor;

@SpringBootApplication
public class Application implements CommandLineRunner{
    @Autowired
    private TaskExecutor taskExecutor;

    @Autowired
    private ApplicationContext appContext;
    
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

    @Override
    public void run(String... args) {
        taskExecutor.execute(appContext.getBean(JobHandler.class));
    }
}
