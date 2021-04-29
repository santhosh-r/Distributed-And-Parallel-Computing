package project;

import java.net.InetAddress;
import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;

public interface WorkerRepository extends JpaRepository<Worker, Long> {

    public Worker findById(int id);
    public Worker findByAddressAndPort(InetAddress address, int port);
    public List<Worker> findByConnected(boolean connected);
}