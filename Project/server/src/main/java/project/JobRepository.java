package project;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;

public interface JobRepository extends JpaRepository<Job, Long> {
    public Job findById(int id);
    public List<Job> findByStatus(String status);
}