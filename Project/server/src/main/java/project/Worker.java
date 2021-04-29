package project;

import java.net.InetAddress;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import lombok.Data;

@Data // using lombok.Data to take care of getter and setter methods
@Entity
public class Worker {

    @Id
    @GeneratedValue(strategy=GenerationType.AUTO)
    private int id;
    private InetAddress address;
    private int port;
    private boolean connected;

    public Worker() {}

    public Worker(InetAddress address, int port) {
        this.address = address;
        this.port = port;
        this.connected = false;
    }

    public String getAddressString() {
        return this.address.getHostAddress() + ":" + this.port;
    }
}