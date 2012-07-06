#include"mpi.h"
#include<stdlib.h>
#include<stdio.h>

#define NUM_DIMS 2

 

int main( int argc, char **argv ){

     

     int rank,size;

     int periods[NUM_DIMS] = {0,0};

     int dims[NUM_DIMS] = {0,0};

     int right_nbr, left_nbr;

     int *data,buf=0;

     int i,flag;

 

     MPI_Comm my_comm;

     MPI_Status status;

     MPI_Request request;

 

     MPI_Init(&argc, &argv);

     MPI_Comm_rank( MPI_COMM_WORLD, &rank );

     MPI_Comm_size(MPI_COMM_WORLD, &size);

     MPI_Dims_create(size, NUM_DIMS, dims);

     MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, 0, &my_comm);

 

     data = (int *)malloc( size * sizeof(int) );

     //初始化data数组

     for(i=0;i<size;i++){

         data[i] = 10*i;

     }

 

     flag = NUM_DIMS;

     while(--flag>=0){

         //flag = 1时，横向,flag = 0时，纵向

         MPI_Cart_shift(my_comm, flag, 1, &left_nbr, &right_nbr);

 

         MPI_Isend(&data[rank], 1, MPI_INT, left_nbr, 0, MPI_COMM_WORLD, &request);

         MPI_Wait(&request, &status);

         MPI_Isend(&data[rank], 1, MPI_INT, right_nbr, 0, MPI_COMM_WORLD, &request);

         MPI_Wait(&request, &status);

 

         if(flag){

              printf("进程[%d]发送数据数据(%d)\n",rank, data[rank]);

         }

     

         if(left_nbr!=-1){

              MPI_Irecv(&buf, 1, MPI_INT, left_nbr, 0, MPI_COMM_WORLD, &request);

              MPI_Wait(&request, &status);

              printf("进程[%d]从%s进程[%d]接收到数据(%d)\n",rank, flag?"左边":"上边", left_nbr, buf);

         }

         if(right_nbr!=-1){

              MPI_Irecv(&buf, 1, MPI_INT, right_nbr, 0, MPI_COMM_WORLD, &request);

              MPI_Wait(&request, &status);

              printf("进程[%d]从%s进程[%d]接收到数据(%d)\n",rank, flag?"右边":"下边", right_nbr, buf);

         }

     }

 

     MPI_Finalize();

} 
