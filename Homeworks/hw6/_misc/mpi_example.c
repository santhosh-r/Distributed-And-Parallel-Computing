#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

struct data{
  int n;
  int array[3];
}data;

struct dynamicData{
  int n;
  int *array;
}dynamicData;

int main(int argc, char *argv[])
{
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    int i;
    for(i=1;i<size;i++)
    {
      struct data myData;
      myData.n = 10;
      int sendSize = sizeof(myData);
      printf("\nMaster - Sending struct of size %d", sendSize);
      MPI_Send(&myData, sendSize, MPI_BYTE, i, 0, MPI_COMM_WORLD);

      //dynamic allocated struct and array
      struct dynamicData *myDData;
      myDData = malloc(sizeof(struct dynamicData));
      myDData->array = malloc(5 * sizeof(int));
      myDData->array[0] = 99;
      myDData->n = -10;
      sendSize = sizeof(struct dynamicData);

      printf("\nMaster - Sending dynamic struct of size %d\n", sendSize);
      //send the struct
      MPI_Send(myDData, sendSize, MPI_BYTE, i, 0, MPI_COMM_WORLD);
      //send the array
      MPI_Send(myDData->array, 5, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }
  else {
    struct data myData;
    int receivingSize;

    MPI_Recv(&myData, sizeof(myData), MPI_BYTE, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received number %d from process 0\n",
           rank, myData.n);

    //Probe and get the size of struct that is being sent
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_BYTE, &receivingSize);
    //you dont need to do the above because receivingSize = sizeof(dynamicData)

    struct dynamicData *myDData = malloc(receivingSize);
    //receive the struct
    //This will not get you the dynamic array
    MPI_Recv(myDData, (receivingSize) , MPI_BYTE, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    //Probe and get the size of array
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &receivingSize);
    //malloc the array
    myDData->array = malloc(receivingSize*sizeof(int));
    //receive the array
    MPI_Recv(myDData->array, receivingSize , MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    printf("Process %d received array[0] %d and number %d from process 0\n",
           rank, myDData->array[0], myDData->n);
  }
  MPI_Finalize();
}
