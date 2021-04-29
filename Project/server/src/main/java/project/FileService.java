package project;

import java.io.InputStream;
import java.nio.file.*;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;
import org.springframework.web.multipart.MultipartFile;

@Service
public class FileService {

    public String uploadLocation = "uploads";
    public String downloadLocation = "downloads";

    public void uploadFile(MultipartFile file) {
    Path rootLocation = Paths.get(uploadLocation);
    String filename = StringUtils.cleanPath(file.getOriginalFilename());
    try {
        Files.createDirectories(rootLocation);
            if (file.isEmpty()) {
                throw new Exception("Empty file");
            }
            InputStream inputStream = file.getInputStream();
            Files.copy(inputStream, rootLocation.resolve(filename), StandardCopyOption.REPLACE_EXISTING);
        } catch (Exception e) {
        e.printStackTrace();
    }
    }

    public String getInputFileFullPath(String filename) {
    Path rootLocation = Paths.get(uploadLocation);
    return rootLocation.resolve(filename).toString();
    }

    public String getOutputFileFullPath(String filename) {
    Path rootLocation = Paths.get(downloadLocation);
    try {
        Files.createDirectories(rootLocation);
    } catch (Exception e) {
        e.printStackTrace();
    }
    return rootLocation.resolve(filename).toString();
    }
}