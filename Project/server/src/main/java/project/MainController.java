package project;

import org.springframework.stereotype.Controller;

import java.io.FileInputStream;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

import org.apache.commons.io.IOUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

@Controller
public class MainController {

    @Autowired
    private WorkerRepository workerRepo;

    @Autowired
    private JobRepository jobRepo;

    @Autowired
    private FileService fileService;

    @GetMapping("/")
    public String index(Model model) {
        model.addAttribute("job", new Job());
        return "index";
    }

    @PostMapping("/submitJob")
    public String eventSubmit(@ModelAttribute Job job, Model model) {
        String outputFilename = LocalDate.now().format(DateTimeFormatter.ofPattern("YYYYMMdd"));
        outputFilename += "_" + job.getJobType() + "_" + job.getInputFilename();
        job.setOutputFilename(outputFilename);  
        job = jobRepo.save(job);
        System.out.println("Job with id " + job.getId() + " and type " + job.getJobType() + " submitted");
        return "redirect:/./jobs";
    }

    @GetMapping("/jobs")
    public String jobs(Model model) {
        model.addAttribute("jobs", jobRepo.findAll());
        return "jobs";
    }

    @GetMapping("/workers")
    public String workers(Model model) {
        model.addAttribute("workers", workerRepo.findAll());
        // model.addAttribute("workers", workerRepo.findByConnected(true));
        return "workers";
    }

    @PostMapping("/uploadFile")
    public String uploadFile(@RequestParam("file") MultipartFile file, RedirectAttributes redirectAttributes) {
        fileService.uploadFile(file);
        redirectAttributes.addFlashAttribute("message", "You successfully uploaded " + file.getOriginalFilename() + "!");
        return "redirect:/./";
    }

    @RequestMapping(value="/downloadFile", method = RequestMethod.GET)
    public ResponseEntity<byte[]> getImage(@RequestParam String file) throws Exception {
        FileInputStream in = new FileInputStream(file);
        final HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.TEXT_PLAIN);
        return new ResponseEntity<byte[]>(IOUtils.toByteArray(in), headers, HttpStatus.CREATED);    
    }
}