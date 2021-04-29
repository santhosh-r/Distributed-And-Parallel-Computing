# ualbany-csi520-project

## About
An example of a distributed application in Java submitted for the Distributed & Parallel Computing course CSI 520 at UAlbany.

Spring framework is used to provide a web based user interface. The user uploads input data and submits jobs to the server. The server then splits the job workload and sends the portions to the connected worker nodes. The workers process their portions when their processors are idle and then send their results back to the server which compiles them together and makes them available to the user.

## Screenshots

Web User Interface

![](images/ui.png?raw=true)

Results

![](images/results.png?raw=true)