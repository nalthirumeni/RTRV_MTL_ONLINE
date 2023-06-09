
/*
			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

							      NAL_RV

			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ONLINE MONITORING with test code


// This is Run Time monitoring software tool, for monitoring RT systems to be used along with NAL_MTL specifications.

// Developed by P Thirumeni -203085002, PhD, student IIT Dharwad India.

// Under the guidance of PhD. Supervisors, Dr.Rajshekar.K (IIT-Dh), Dr.C.M.Ananda (CSIR-NAL)

//   P THIRUMENI  date :24.04.23

// Functions : G,F,U,S,W,GF,FG,V,AP_update,Write_ASC,Get_ASC


/**************************************************************************************************************************************************************/




#include <stdio.h>  // for standard io operations
#include <stdlib.h> // for use malloc
#include <string.h> // for string processing


#define MAX_ROWS 7200  // define the no. of total mission time units here, for example 2 hours is 7200 sec. if the time granularity considered is 1 sec.
#define NUM_COLS 64    // Number of atomic propositions to be monitored is to be defined here.
#define NUM_APS 64          // Number of APs planned for monitoring.
#define FORMULA_COUNT 128 // number of RVRT_MTL formulas.
#define MAX_SEG 64      // number segment max. variables in longest RVRT_MTL formulas (S1,S2..)
#define ASC_COUNT 256 // general purpose asserts

#define ASC_MAX_ROWS 10  // This metric to be arrived after the intermediate NAL_MTL definitions.
#define ASC_NUM_COLS 72  // this is the worse case up/low time limit that can be specified by the user or equals to the mission time units(max).

#define MAX_ROWS 10  // define the no. of total mission time units here, for example 2 hours is 7200 sec. if the time granularity considered is 1 sec.
#define NUM_COLS 72   // Number of atomic propositions to be monitored is to be defined here.


char data_1[ASC_MAX_ROWS][ASC_NUM_COLS] = {{1}};

char data[MAX_ROWS][NUM_COLS] = {{0}};



///////////////////////////////////


//DATA STRUCTURES

unsigned int AP_UPDATE [NUM_APS+1] = {1};                  // FOR EVERY TIME STAMP UPDATE

unsigned int ASC_UPDATE [ASC_COUNT] = {1};                  // general asserts list, for manupulation suing verdict operator

unsigned int GF_kj_array [FORMULA_COUNT][MAX_SEG][6] = {{{0}}};  // three dimensional array for handling k and j values during GF operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)
// GF_kj_array[x][y][z] = [for index] [seg index] [z],Z=5 (S_TS, E_TS, k , j, GF_percent,GF_result); S_TS = starting time stamp,E_TS= ending time stamp

unsigned int FG_ji_array [FORMULA_COUNT][MAX_SEG][6] = {{{0}}};  // three dimensional array for handling k and j  and i etc., values during FG operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)
 //FG_kj_array[FORMULA_COUNT][MAX_SEG][6] = {{{}}}; // [for index] [seg index] [z],Z=6 (S_TS, E_TS,j, i,GF_percent,GF_result); S_TS = starting time stamp,E_TS= ending time stamp

unsigned int U_array[FORMULA_COUNT][MAX_SEG][4] = {{{}}};// [for index] [seg index] [z],Z=4 (0=S_TS, 1=E_TS,2=U_percent,3=result,i)
unsigned int S_array[FORMULA_COUNT][MAX_SEG][9] = {{{}}};// //z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4= found j, 5=first time - q holder for (p0,q1)case, 6=k_p (p * prev_P),7=m_q (q * prev_q),8=n_q (prev_q for success case time stamps p=0 q=1)

unsigned int W_array[FORMULA_COUNT][MAX_SEG][5] = {{{}}};// //z=5; index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4=all_p

unsigned int G_array [FORMULA_COUNT][MAX_SEG][5] ={{{0}}};
//[for_index] [seg_index] [z],Z=5     // 0=S_TS, 1=E_TS, 2=prev_g , 3=G_percent,4=G_result);

unsigned int F_array [FORMULA_COUNT][MAX_SEG][5] ={{{0}}};
//[for_index] [seg_index] [z],Z=5     // 0=S_TS, 1=E_TS, 2=prev_f , 3=F_percent,4=F_result);

////////////////


int timestamp;
int row_index;
int v;
char line[200];
char headers[NUM_COLS][500];

float G_realise_percent;
float F_realise_percent;
float GF_realise_percent;
float FG_realise_percent;
float U_realise_percent;
float S_realise_percent;
float W_realise_percent;


// Function prototypes.
//////////////////////////////////////////

// Function to get AP by column from DAQ csv file. - for testing
int* get_data_by_AP(int col_index, int start_TS, int end_TS);

//Global [S_t,E_t]
int NAL_MTL_G(int p,unsigned int form_count,unsigned int seg_count);

//Finally [S_t,E_t]
int NAL_MTL_F(int p,unsigned int form_count,unsigned int seg_count);

//GF [S_t,E_t]
int NAL_MTL_GF(int p,unsigned int form_count,unsigned int seg_count);

//FG [S_t,E_t]
int NAL_MTL_FG(int p,unsigned int form_count,unsigned int seg_count);

//p U[S_t,E_t] q
int NAL_MTL_U(int p,int q, int form_count,int seg_count);

//p S[S_t,E_t] q
int NAL_MTL_S(int p,int q, int form_count,int seg_count);

//p W[a,b] q
int NAL_MTL_W(int p,int q, int form_count,int seg_count);

//verdict Vi
int NAL_MTL_Verdict (int logic_operator, unsigned int p, int q);










int main()

    {


    ////////////////////


    printf("\n\n");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n                                               NAL_RV ONLINE MONITORING ");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n\n");



    //array dimention 3 index detail -  0=S_TS, 1=E_TS, 2=prev_g , 3=G_percent,4=G_result);
    // fill start and end time and call G_func.



   // p =1; // for G to be tested
//    p =0; // for F to be tested
   // int start_time = 3;
 //   int end_time = 10;

 //   for (int i=3 ; i<=10 ; i++)
 //   {
 //   sleep(1);
 //   timestamp =i;

  //  if (i == 7)
   // {
      //  p=0; // for G test
  //      p=1; // for F test
  //  }

   // if (G_array[5][10][4] == 0)
  // printf("befor if F result = %d",G_array[5][10][4]);
   // if (G_array[5][10][4] == 1)
  //  {
    //    break;

      //  printf("G result = %d",G_array[5][10][4]);
  //      printf("F result = %d",G_array[5][10][4]);
  //  }

    // Testing G operator.
    // int ss = NAL_MTL_G(p,5,10);
    // Testing F operator
   // int ss = NAL_MTL_F(p,5,10); // formula 5 and segment 10
   //  printf("\nsending p = %d, at timestamp %d \n ", p,timestamp);
   // }




    // Testing GF operator
    ///////////////
    //read data from csv file

    // Function to get the data row corresponding for a given timestamp

 char filename[] = "testcsv.txt";
    FILE *fp;

    int row = 0, col = 0, i, j;

    fp = fopen(filename, "r");
    if (fp == NULL)
        {
        printf("Could not open csv file %s", filename);
        return 1;
    }

    // Read header row
    if (fgets(line, sizeof(line), fp) != NULL)
        {
        char *token = strtok(line, ",");
        while (token != NULL) {
            strcpy(headers[col], token);
            token = strtok(NULL, ",");
            col++;
        }
    }

    // Read data from file
    row = 0;
    col = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
        {
        char *token = strtok(line, ",");
        while (token != NULL) {
            data[row][col++] = atoi(token);
            token = strtok(NULL, ",");
        }
        row++;
        col = 0;
    }

    fclose(fp);
//////////////////////////////////
/*
    ///////////////
    // format int* col_data = get_data_by_AP(col_index, start_row, end_row);


    //////////////////

    ////// Testing G operator

    // Testing G operator.
    // int ss = NAL_MTL_G(p,5,10);
    int p,q;
    int start_TS=3;
    int end_TS=12;
    int col_index =4;//ap index



 // timestamp=3;// initial timestamp.
  //  F_array[5][10][0] =3; // start timestamp
  //  F_array[5][10][1] =10; // end timestamp
  //  F_array[5][10][4] =2; // dummy result



        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

    G_array[5][10][0] =start_TS; // start timestamp
    G_array[5][10][1] =end_TS; // end timestamp
    G_array[5][10][4] =2; // dummy result

    for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);
        timestamp =i+1;
        p= data[i][col_index];
        //printf("AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,i, col_data_1[i - start_TS]);//here p =col_data_1[i - start_TS], considerring array index starting at 0
         printf("\n G testing - Sending - AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test G here...

        int ss = NAL_MTL_G(p,5,10);


       // printf("befor if G result = %d",G_array[5][10][4]);
       if (G_array[5][10][4] == 0)
        {
        break;
        printf("G result = %d",G_array[5][10][4]);
        }

        } // G test for loop ends

////////////////////

*/

  ////// Testing F operator

  /*

    // int ss = NAL_MTL_F(p,5,10);
    int p,q;
    int start_TS=1;
    int end_TS=10;
    int col_index =5;//ap index




        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

    F_array[5][10][0] =start_TS; // start timestamp
    F_array[5][10][1] =end_TS; // end timestamp
    F_array[5][10][4] =2; // dummy result

    for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);
        timestamp =i+1;
        p= data[i][col_index];
        //printf("AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,i, col_data_1[i - start_TS]);//here p =col_data_1[i - start_TS], considerring array index starting at 0
         printf("\n F testing - Sending - AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test F here...

        int ss = NAL_MTL_F(p,5,10);


       printf("before if F result = %d",F_array[5][10][4]);
       if (F_array[5][10][4] == 1)
        {
        break;
        printf("F result = %d",F_array[5][10][4]);
        }

        } // F test for loop ends

////////////////////



    // input for GF here...

        int start_TS=3;
        int end_TS=14;
        int col_index =5;//ap index

        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;
        int p;
        int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        GF_kj_array[5][10][0] =start_TS; // start timestamp
        GF_kj_array[5][10][1] =end_TS; // end timestamp


         printf("\n\n\n GF function - Sending the values of P per every 1 sec now....\n");

         printf("\n start time - GF_kj_array[5][10][0] = %d, end time - GF_kj_array[5][10][1] = %d\n",GF_kj_array[5][10][0],GF_kj_array[5][10][1]);

        for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);
        timestamp =i+1;
        p= data[i][col_index]; // -1 considering that arrays stats with 0 index.
        //printf("AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,i, col_data_1[i - start_TS]);//here p =col_data_1[i - start_TS], considerring array index starting at 0
         printf("Sending - AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test GF here...
        //int NAL_MTL_GF(int p,unsigned int form_count,unsigned int seg_count)

        int ss = NAL_MTL_GF(p,5,10);

        } // for loop ends

*/

///////////////////////////////////////
/*
 // Testing FG operator

 // input for FG here...

        int start_TS=1;
        int end_TS=10;
        int col_index =5;//ap index

        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;
        int p;
        int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        FG_ji_array[5][10][0] =start_TS; // start timestamp
        FG_ji_array[5][10][1] =end_TS; // end timestamp


         printf("\n\n\n FG function - Sending the values of P%d per every 1 sec now....\n",col_index);

         printf("\n start time -   = %d, end time -   = %d\n",FG_ji_array[5][10][0],FG_ji_array[5][10][1]);

        for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);
        timestamp =i+1;
        p= data[i][col_index]; // -1 considering that arrays stats with 0 index.
        //printf("AP id/index = %d, time stamp= %d: AP value p = %d\n", col_index,i, col_data_1[i - start_TS]);//here p =col_data_1[i - start_TS], considerring array index starting at 0
     //    printf("\nSending - AP_id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test FG here...

        //int NAL_MTL_FG(int p,unsigned int form_count,unsigned int seg_count)

        int ss = NAL_MTL_FG(p,5,10); // ap index, formula index and segment index

        } // for loop ends

        */
//////////////////////////////

/*

// U operator testing....

//int NAL_MTL_U(int p,int q, int form_count,int seg_count);

// input for U here...

        int start_TS=1;
        int end_TS=10;
        int col_index =4;//ap index
        int col_index2 =5;//ap index
        int p,q;
        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        // get p
      //  int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        //get q
    //    int* col_data_2 = get_data_by_AP(col_index2, start_in_array, end_in_array); // ap index, start time, end time


    //z=4, index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.

        U_array[5][10][0] =start_TS; // start timestamp
        U_array[5][10][1] =end_TS; // end timestamp
        U_array[5][10][2] =end_TS; // U assumed at end time stamp
        U_array[5][10][3] =2; // dummy result


      //   printf("\n\n\n FG function - Sending the values of P per every 1 sec now....\n");

        printf("\n start time -   = %d, end time -   = %d, result =%d\n",U_array[5][10][0],U_array[5][10][1],U_array[5][10][3]);

        for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);
        timestamp =i+1;
        p= data[i][col_index]; // -1 considering that arrays stats with 0 index.
        q= data[i][col_index2]; // -1 considering that arrays stats with 0 index.
        printf("Until AP id/index of p= %d,column index of q=%d, time stamp= %d: AP value p =%d, q =%d\n",col_index,col_index2,timestamp, p,q);//here p =col_data_1[i - start_TS], considerring array index starting at 0
      //  printf("\nSending - AP_id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test U here...

        //format  int NAL_MTL_U(int p,int q, int form_count,int seg_count)



        //z=4 index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.
     //   printf(" \nbefore call func.timestamp=%d,U_array[5][10][3]=%d\n",timestamp,U_array[5][10][3]);
        int ss = NAL_MTL_U(p,q,5,10); // p,q, formula index, seg. index.
    //    printf(" after call func.timestamp=%d,U_array[5][10][3]=%d\n",timestamp,U_array[5][10][3]);
        if (U_array[5][10][3] != 2)
        {
        break;
   //     printf("break at timestamp=%d,U_array[5][10][3]=%d",timestamp,U_array[5][10][3]);
        }



        } // U testing for loop ends

//////////////////////////////

*/


//////////////////////////////

/*

// S operator testing....

//int NAL_MTL_S(int p,int q, int form_count,int seg_count)

//z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4= found j,
// 5=first time - q holder for (p0,q1)case, 6=k_p (p * prev_P),7=m_q (q * prev_q),8=n_q (prev_q for success case time stamps p=0 q=1)

// input for S here...

        int start_TS=1;
        int end_TS=10;
        int col_index =4;//ap index
        int col_index2 =5;//ap index
        int p,q;
        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        // get p
      //  int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        //get q
    //    int* col_data_2 = get_data_by_AP(col_index2, start_in_array, end_in_array); // ap index, start time, end time



        S_array[5][10][0] =start_TS; // start timestamp
        S_array[5][10][1] =end_TS; // end timestamp
        S_array[5][10][3] =2; // dummy result


      //   printf("\n\n\n FG function - Sending the values of P per every 1 sec now....\n");

        printf("\n 1 Strong Until start time -   = %d, end time -   = %d, result =%d\n",S_array[5][10][0],S_array[5][10][1],S_array[5][10][3]);

        for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);

        //printf("\n 2 Strong Until start time -   = %d, end time -   = %d\n",S_array[5][10][0],S_array[5][10][1]);
        timestamp =i+1;
        p= data[i][col_index]; // -1 considering that arrays stats with 0 index.
        q= data[i][col_index2]; // -1 considering that arrays stats with 0 index.
        printf("\nStrong until AP id/index of AP  p %d,column index of AP q%d, time stamp= %d: AP value p =%d, q =%d\n",col_index,col_index2,timestamp, p,q);//here p =col_data_1[i - start_TS], considerring array index starting at 0
      //  printf("\nSending - AP_id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test U here...

        //format  int NAL_MTL_U(int p,int q, int form_count,int seg_count)



        //z=4 index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.
     //   printf(" \nbefore call func.timestamp=%d,S_array[5][10][3]=%d\n",timestamp,S_array[5][10][3]);
        int ss = NAL_MTL_S(p,q,5,10); // p,q, formula index, seg. index.
    //    printf(" after call func.timestamp=%d,S_array[5][10][3]=%d\n",timestamp,S_array[5][10][3]);
        if (S_array[5][10][3] == 0)
        {
        break;
        printf("break at timestamp=%d,S_array[5][10][3]=%d",timestamp,U_array[5][10][3]);
        }



        } // strong until testing.....   for loop ends




      //////


    ////////////////// Weak until testing from here.

*/

////////////////////////////



/*
//int NAL_MTL_W(int p,int q, int form_count,int seg_count)



// input for W here...

        int start_TS=1;
        int end_TS=10;
        int col_index =4;//ap index
        int col_index2 =5;//ap index
        int p,q;
        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        // get p
      //  int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        //get q
    //    int* col_data_2 = get_data_by_AP(col_index2, start_in_array, end_in_array); // ap index, start time, end time



        W_array[5][10][0] =start_TS; // start timestamp
        W_array[5][10][1] =end_TS; // end timestamp
        W_array[5][10][3] =2; // dummy result


      //   printf("\n\n\n FG function - Sending the values of P per every 1 sec now....\n");

        printf("\n 1 Weak  Until start time -   = %d, end time -   = %d, result =%d\n",W_array[5][10][0],W_array[5][10][1],W_array[5][10][3]);

        for (int i = start_in_array; i <= end_in_array; i++)
        {
        sleep(1);

        //printf("\n 2 Strong Until start time -   = %d, end time -   = %d\n",S_array[5][10][0],S_array[5][10][1]);
        timestamp =i+1;
        p= data[i][col_index]; // -1 considering that arrays stats with 0 index.
        q= data[i][col_index2]; // -1 considering that arrays stats with 0 index.
        printf("\n Weak until AP id/index of AP  p %d,column index of AP q%d, time stamp= %d: AP value p =%d, q =%d\n",col_index,col_index2,timestamp, p,q);//here p =col_data_1[i - start_TS], considerring array index starting at 0
      //  printf("\nSending - AP_id/index = %d, time stamp= %d: AP value p = %d\n", col_index,timestamp, p);//here p =col_data_1[i - start_TS], considerring array index starting at 0

        // test W here...


    // format int NAL_MTL_W(int p,int q, int form_count,int seg_count);

     //   printf(" \nbefore call func.timestamp=%d,S_array[5][10][3]=%d\n",timestamp,S_array[5][10][3]);

        int ss = NAL_MTL_W(p,q,5,10); // p,q, formula index, seg. index.

    //    printf(" after call func.timestamp=%d,S_array[5][10][3]=%d\n",timestamp,S_array[5][10][3]);
        if (W_array[5][10][3] != 2)
        {
        break;
        printf("break at timestamp=%d,S_array[5][10][3]=%d",timestamp,W_array[5][10][3]);
        }



        } // weak until test for loop ends


*/

///////////////////////////////

//Testing verdict operator.

/*

// format : int NAL_MTL_Verdict (int logic_operator, unsigned int p, int q)
//locgic code = 1 check ap is 1, 2=cheak ap is 0, 3 = AND , 4= OR , 5 = NOT, 6 = imply,7 = equ.,9 = xor ; default v=3

int logic_code = 0;
int p=1;
int q=0;
int ss = NAL_MTL_Verdict (logic_code,p,q); // calling verdict function

printf("\n V =%d\n",v);
printf("\nverdict result, p=%d,q=%d, Logic code = %d, result V=%d",p,q,logic_code,v);

*/

printf("\nNAL_RV_End of main here....");

    return 0;

    }


///////////////// main ends here.



// Function Definitions from here.....

//////////////////////////////////


int* get_data_by_timestamp(int timestamp)

    {
    static int row_data[NUM_COLS];
    int i;
    row_index = -1;
    for (i = 0; i < MAX_ROWS; i++) {
        if (data[i][0] == timestamp) // search for the time stamp from the first column
            {
            row_index = i;
            break;
        }
    }
    if (row_index == -1) {
        printf("No data row found with timestamp %d\n", timestamp);
        return NULL;
    }
    for (int u = 0; u < NUM_COLS; u++) {
        row_data[u] = data[row_index][u];
    }
    return row_data;
}

/////////////////


///////////////////////////////////

int* get_data_by_AP(int col_index, int start_TS, int end_TS) //get columnwise data for the given time range

{

   // printf("Inside column scan func.\n");
    printf(" \n");
    int* col_data_1 = (int*)malloc((end_TS - start_TS + 1) * sizeof(int)); // allocate memory for column data,  size based on input range
    if (col_data_1 == NULL) {
        printf("Memory allocation failed for column data.\n");
        return NULL;
    }
    printf(" \n");

    for (int i = start_TS; i <= end_TS; i++) {
        col_data_1[i - start_TS] = data[i][col_index];
      //  printf("AP id/index = %d, time stamp= %d: AP value = %d\n", col_index,i, col_data_1[i - start_TS]);
    }

    return col_data_1;
}


//////////////////////////////////




//Globally

   int NAL_MTL_G(int p,unsigned int form_count,unsigned int seg_count)  //

        {

          printf("\n Receiving inside G_function  p=%d and time stamp = %d\n", p,timestamp);
          if (G_array[form_count][seg_count][0] == timestamp) // checking start time
          {
           printf("\n Inside G init before  value of prev_g =%d,start time=%d,end time=%d\n", G_array[form_count][seg_count][2],G_array[form_count][seg_count][0],G_array[form_count][seg_count][1]);
           G_array[form_count][seg_count][2]=1; // initialise prev_g =1
           printf("\n Inside G init after value of prev_g =%d \n", G_array[form_count][seg_count][2]);
          }

             G_array[form_count][seg_count][4]=G_array[form_count][seg_count][2] && p ; // prev_g && current g
             printf("\n current evaluated G = %d\n\n", G_array[form_count][seg_count][4]);

            if (G_array[form_count][seg_count][4] == 0)

            {
                printf("\n Inside when result of G Failed ");
                int a = G_array[form_count][seg_count][0];
                int b = G_array[form_count][seg_count][1];
                float c = b-a;
                float d = ((timestamp-a)/c) *100;

                printf("\n Current timestamp=%d,start time of G = %d ,End time of G = %d, Total Duration = %f,",timestamp,a,b,c);
                G_array[form_count][seg_count][3] = (int)d; // failure case % at which the G operation failed
                printf("\n\n\n G realise percentage %d, failure timestamp= %d \n",G_array[form_count][seg_count][3],timestamp);
                printf("\n\n\n G result %d \n",G_array[form_count][seg_count][4]);
                return (G_array[form_count][seg_count][4]); // failure case result at any point in time
            }


          if (G_array[form_count][seg_count][1] == timestamp) // checking end time
            {
                printf("\n Inside at end time point %d\n",timestamp);
                printf("\n start time=%d,end time=%d\n",G_array[form_count][seg_count][0],G_array[form_count][seg_count][1]);
                int a = G_array[form_count][seg_count][0];
                int b = G_array[form_count][seg_count][1];
                float c = b-a;
                float d = ((timestamp-a)/c) *100;

                printf("\n Current timestamp=%d,start time of G = %d ,End time of G = %d, Total Duration = %f,",timestamp,a,b,c);
                G_array[form_count][seg_count][3] = (int)d; // G success 100% at the end
                printf("\n\n\n G realise percentage %d \n",G_array[form_count][seg_count][3]);
                printf(" G result %d \n",G_array[form_count][seg_count][4]);
                return (G_array[form_count][seg_count][4]); // success G - at the end of time window
            }

         }

         // G operator ends.

///////////////////////////////

int NAL_MTL_F(int p,unsigned int form_count,unsigned int seg_count)

 {
// F_array[x][y][z] = [for index] [seg index] [z],Z=5 (S_TS, E_TS, prev_f , F_percent,F_result); S_TS = starting time stamp,E_TS= ending time stamp

          printf("\n Receiving inside F_function  p=%d and time stamp = %d\n", p,timestamp);
          if (F_array[form_count][seg_count][0] == timestamp) // checking start time
          {
           printf("\n Inside F init before  value of prev_f =%d \n", F_array[form_count][seg_count][2]);
           F_array[form_count][seg_count][2]=0; // initialise prev_f =1
           printf("\n Inside F init after value of prev_f =%d \n", F_array[form_count][seg_count][2]);
          }

           F_array[form_count][seg_count][4]=F_array[form_count][seg_count][2] || p ; // prev_p && current p for f
           printf("\n evaluated F = %d\n\n", F_array[form_count][seg_count][4]);

            if (F_array[form_count][seg_count][4] == 1) // success case

            {
                printf("\n F-Success !! Evaluated F = %d\n\n", F_array[form_count][seg_count][4]);
                int a = F_array[form_count][seg_count][0];
                int b = F_array[form_count][seg_count][1];
                float c = b-a;
                float d = ((timestamp-a)/c) *100;
                F_array[form_count][seg_count][3] = (int)d; // success case % at which the p becomes 1
                printf("\n Current timestamp=%d,start time of F = %d ,End time of F = %d, Total Duration = %f,",timestamp,a,b,c);
                printf("\n\n\n F realise percentage %d \n",F_array[form_count][seg_count][3]);
                printf(" F result %d \n",F_array[form_count][seg_count][4]);
                return (F_array[form_count][seg_count][4]); // sucess case result at any point in time
            }


          if (F_array[form_count][seg_count][1] == timestamp) // checking end time
            {
                int a = F_array[form_count][seg_count][0];
                int b = F_array[form_count][seg_count][1];
                float c = b-a;
                float d = ((timestamp-a)/c) *100;
                F_array[form_count][seg_count][3] = (int)d; // // failure case all 'p's are 0
                printf("\n Current timestamp=%d,start time of F = %d ,End time of F = %d, Total Duration = %f,",timestamp,a,b,c);
                printf("\n\n\n F realise percentage %d \n",F_array[form_count][seg_count][3]);
                printf(". F result %d \n",F_array[form_count][seg_count][4]);
                return (F_array[form_count][seg_count][4]); // failure case result at end time
            }

         }

 ////////////////////////  F operator ends.


///////////////////////////////////
// GF_operator - Infinitely Often

//     ∀k ≥ a ∧ k ≤ b . ∃j ≥ k ∧ j ≤ b : (Aj ⊨ p)  where a,b are low and up time limit and 'p' is the Atomic proposition(AP) of a RTRV_MTL subformula to be evaluated.


// Function for NAL_MTL_ GF - the infinitely often operator

//unsigned int GF_kj_array [FORMULA_COUNT][MAX_SEG][6] = {{{0}}};  // three dimensional array for handling k and j values during GF operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)
//Z=6 (0=S_TS, 1=E_TS, 2=k , 3=j, 4=GF_percent,5=GF_result); S_TS = starting time stamp,E_TS= ending time stamp

  int NAL_MTL_GF(int p,unsigned int form_count,unsigned int seg_count)

        {

           printf("\n Receiving inside GF_function  p=%d and time stamp = %d,start time=%d\n", p,timestamp,GF_kj_array[form_count][seg_count][0]);
          // printf("\n GF_kj_array[5][10][0] = %d, GF_kj_array[5][10][1] = %d\n",GF_kj_array[5][10][0],GF_kj_array[5][10][1]);
          if (GF_kj_array[form_count][seg_count][0] == timestamp) //checking current time equals to the start time of the segment
          {
           printf("\n Inside GF before init k as start time =%d \n", GF_kj_array[form_count][seg_count][2]);
           GF_kj_array[form_count][seg_count][2]=GF_kj_array[form_count][seg_count][0]; // initialise k = start time
           printf("\n After init. in GF - k as start time =%d \n", GF_kj_array[form_count][seg_count][2]);
          }


            if (p == 1 )
                {


                GF_kj_array[form_count][seg_count][2]=timestamp;  // k = timestamp, till this time point GF holding true...
                GF_kj_array[form_count][seg_count][3] = 1; // j =1
                printf("\n while p =1,found j =1, and  updated k to current timestamp k =%d\n",GF_kj_array[form_count][seg_count][2]);
                }

            else if (p == 0 )
                {
                GF_kj_array[form_count][seg_count][3] = 0; // j =0
                printf("\n while p =0,found j=0, and   k - timestamp (last time j=0), k =%d\n\n",GF_kj_array[form_count][seg_count][2]);

                }

// GF_kj_array[x][y][z] = [for index] [seg index] [z],Z=6 (S_TS, E_TS, k , j, GF_percent,GF_result)

            if (GF_kj_array[form_count][seg_count][1] == timestamp ) //checking current time equals to the end time of the segment

             {
                printf("\n at the end time - GF_kj_array[5][10][0] = %d, GF_kj_array[5][10][1] = %d\n",GF_kj_array[5][10][0],GF_kj_array[5][10][1]);
                GF_kj_array[form_count][seg_count][5] = GF_kj_array[form_count][seg_count][3]; //result of GF operation,result = last value of j
                //even only the last value of p =1 then GF is success
                int GF_a =GF_kj_array[form_count][seg_count][0]; // start time
                int GF_b =GF_kj_array[form_count][seg_count][1];  // end time
                int GF_c=GF_b-GF_a; // total time duration for this property to evaluate.
                int d = GF_kj_array[form_count][seg_count][2];
                float e=(((float)(d-GF_a))/((float) (GF_c))) *100;
               // printf("a=%d,b=%d,c=%d,d=%d,e=%f\n",GF_a,GF_b,GF_c,d,e);
                GF_kj_array[form_count][seg_count][4] = ((int)e); // realise percentage
                printf("GF_kj_array[form_count][seg_count][4]=%d\n",GF_kj_array[form_count][seg_count][4]);
                printf("\n reached end of the GF time window.\n ");
                printf("\nLast time j=1 at time stamp = %d\n",GF_kj_array[form_count][seg_count][2]);//k value
                printf("\n Current timestamp=%d,start time of GF = %d ,End time of GF = %d, Total Duration = %d,",timestamp,GF_a,GF_b,GF_c);
                printf("\n\n\n GF realise percentage %d \n",GF_kj_array[form_count][seg_count][4]);
                printf(" GF result %d \n",GF_kj_array[form_count][seg_count][5]);
                return GF_kj_array[form_count][seg_count][5]; // note the calling function should check this as the return result
             }


    return 0; //


            }  // GF function ends

////////////////////////////////////////


///////////////////////////////////////////


///////////////////////////////////

//FG

// Function for NAL_MTL_FG - the eventually forever operator.

//∃j (a ≤ j ≤ b) ∧ (∀i (a ≤ i < j) : (Ai ⊨ p ∨ Ai ⊭ p)  ∧  (∀i (j ≤ i ≤ b) : Ai ⊨ p)

// FG_ji_array [FORMULA_COUNT][MAX_SEG][z=6]

//z index detail: 0=start time, 1= end time, 2=j,3=i(prev.p),4=FG_realise_percent,5=result of FG.



    int NAL_MTL_FG(int p,unsigned int form_count,unsigned int seg_count)

        {

      //  printf("\n Receiving inside FG_function  p=%d and time stamp = %d,start time=%d,end time=%d\n", p,timestamp,FG_ji_array[form_count][seg_count][0],FG_ji_array[form_count][seg_count][1]);
        if (FG_ji_array[form_count][seg_count][0] == timestamp)
          {
           printf("\n Inside FG before init j =%d  and i =%d \n", FG_ji_array[form_count][seg_count][2],FG_ji_array[form_count][seg_count][3]);
           FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1]; // initialise j = end time
           FG_ji_array[form_count][seg_count][3] =1; //// initialise prvious val. og AP(P), i = 1
           printf("\n After init j as end time =%d  and i =%d \n", FG_ji_array[form_count][seg_count][2],FG_ji_array[form_count][seg_count][3]);
          }

//j is the point from which p becomes '1' forever, befre j p can be 0 or 1 arbitrarily...

         if (FG_ji_array[form_count][seg_count][0] == timestamp) //start time

         {

            if  ( (p && FG_ji_array[form_count][seg_count][3]) == 1  )

            {
              FG_ji_array[form_count][seg_count][2]=timestamp; // make j= current timestamp
              printf("\n Found at the start timestamp p=1, so setting j=start time stamp (current timestamp) now p=%d, j=%d and i =%d \n",p, FG_ji_array[form_count][seg_count][2],FG_ji_array[form_count][seg_count][3]);
            }

             else if  ( (p && FG_ji_array[form_count][seg_count][3]) == 0  )

            {

                FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1] ; //  j = end time, hopinf at end p may become 1
            }

         }


         /* after first time stamp do the following

         let p= current value of AP and i= previous value of AP taken from global array

         p=0,i=0 : make j = the end time stamp
         p=0,i=1 : make j = the end time stamp, since p again became 0
         p=1,i=0 : make j = the current time stamp, since p=1 and we found new j point, and p may continue to be 1...
         p=1,i=1 : make j = do not change j let the previous  j time stamp remain as it is, since p = 1 and i=1 continuously..

         P Thirumeni        12.4.23

         */

            if ( (p == 1) && (FG_ji_array[form_count][seg_count][3] == 0) ) // p=1,prev_p =0

                {
                FG_ji_array[form_count][seg_count][2]= timestamp; // make j = current timestamp hoping from this point all future p's will be 1.
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result considering two values at end
                printf("\n At timestamp =%d,Found  p is 1 and prev_p is 0, now p=%d, prev_p=i =%d and updated j=%d \n",timestamp,p, FG_ji_array[form_count][seg_count][3],FG_ji_array[form_count][seg_count][2]);
               // FG_ji_array[form_count][seg_count][3] = 1; // update i =1 ( current i  to previous i)
                }

            if ( (p == 1) && (FG_ji_array[form_count][seg_count][3] == 1) )

                {
                printf("\n At timestamp =%d,Found both p and prev_p are 1,   now p=%d, prev_p=i=%d and j=%d \n",timestamp,p, FG_ji_array[form_count][seg_count][3],FG_ji_array[form_count][seg_count][2]);
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result for the end values 1,1
               // FG_ji_array[form_count][seg_count][3] = 1; // update i =1
                }



            if ( (p == 0) && (FG_ji_array[form_count][seg_count][3] == 0) )
                {


                FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1] ; //  j = end time
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result for the end
                printf("\n At timestamp =%d, Found both p & prev_p=i becoming 0, now p=%d, prev_p=i=%d,j=%d \n",timestamp,p, FG_ji_array[form_count][seg_count][3],FG_ji_array[form_count][seg_count][2]);
              //  FG_ji_array[form_count][seg_count][3] = 0; // update prev_p=i =0
                }

            if ( (p == 0) && (FG_ji_array[form_count][seg_count][3] == 1) )
                {


                FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1] ; //  j = end time
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result for the end
             //   FG_ji_array[form_count][seg_count][3] = 0; // update prev_p=i =0
                printf("\n At timestamp =%d,Found  p is 0 and i=prev_p is 1 , now p=%d, prev_p=i=%d,j=%d \n",timestamp,p, FG_ji_array[form_count][seg_count][3],FG_ji_array[form_count][seg_count][2]);
                }

                  FG_ji_array[form_count][seg_count][3] = p; // update i =p ( current i as the previous i)

            if (FG_ji_array[form_count][seg_count][1] == timestamp) //end time reached

             {

               ///////////////////
                int FG_a =FG_ji_array[form_count][seg_count][0]; // start time
                int FG_b =FG_ji_array[form_count][seg_count][1];  // end time
                int FG_c =FG_b-FG_a; // total time duration for this property to evaluate.
                //int d = FG_b-FG_ji_array[form_count][seg_count][2]; // last time j was 1
                int d = FG_ji_array[form_count][seg_count][2]; // last time j was 1
                float e= (((float)(FG_b-d))/((float) (FG_c))) *100;
                FG_ji_array[form_count][seg_count][4] = ((int)e); // realise percentage of FG, this tells from start point how quickly eventually p became '1', higher the this percentage quicker the eventually forever condition reached.
                printf("\nFG_ji_array[form_count][seg_count][4]=%d\n",FG_ji_array[form_count][seg_count][4]);
                printf("a=%d,b=%d,c=%d,d=%d,e=%f\n",FG_a,FG_b,FG_c,d,e);
                printf("\n\n\n FG realised from timestamp=%d and percentage=%d \n",FG_ji_array[form_count][seg_count][2],FG_ji_array[form_count][seg_count][4]);
                printf(" FG realise result %d \n",FG_ji_array[form_count][seg_count][5]);
                return FG_ji_array[form_count][seg_count][5]; // this si the result of FG operation do not consider fucntion return 0 below from the calling function.
             }


    return 0; // FG_ends
}


//////////////////////  FG_ends




//////////////////////

//Until

// NAL_MTL_Until

// format  p U[a,b] q

////φ1 U [a,b] φ2  iff ∃j (a ≤ j < b) ∧ ∀i (a ≤ i < j : (Ai ⊨ φ1 ∧ Ai ⊭ φ2)) ∧ (Ai ⊨ φ2 : i=j) ∧ ∀i (j+1 ≤ i ≤ b : ((Ai ⊨ φ2) ∨ (Ai ⊨ φ1)) ∨ ((Ai ⊭ φ2) ∨ (Ai ⊭ φ1)))

// U_array [FORMULA_COUNT][MAX_SEG][z=5]

//z=4, index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.

   int NAL_MTL_U(int p,int q, int form_count,int seg_count) //

        {

        /*
        // j is the time point at which q holds true.
          p_now = 0,q_now=0 ; until failed.
          p_now = 1,q_now=0 ; proceed to next iteration
          p_now = 0,q_now=1 ; found j . after found j no further iteration needed in thi type of Until
          p_now = 1,q_now=1 ; found j . after found j no further iteration needed in thi type of Until
         P Thirumeni        12.4.23
         */

             printf("\n Inside U function.\n");
             if ( p==0 && q == 0) // p does not hold true till j

             {
                U_array[form_count][seg_count][3] = 0; // result - until failed

                /////////


                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_b-U_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-U_a))/ ((float)(U_c))) *100;
                U_array[form_count][seg_count][2] = (int)(e);
                printf("Until failed at time stamp=%d,when p=%d,q=%d\n",timestamp,p,q);
                printf("\n U realise percentage=%d at timestamp=%d \n",U_array[form_count][seg_count][2],timestamp);
                printf("\n U result=%d\n",U_array[form_count][seg_count][3]);
                return (U_array[form_count][seg_count][3]); //return result failure
             }

//z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.

             if ( q == 1 )

            {
               // U_array[form_count][seg_count][4]=timestamp // j found at current time stamp, let us avoid this index to save memory
                U_array[form_count][seg_count][3] = q; // result
//////////
                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_b-U_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-U_a))/ ((float)(U_c))) *100;
                U_array[form_count][seg_count][2] = (int) (e);
                printf("\n\nUntil succesful !! at time stamp=%d,when p=%d,q=%d\n",timestamp,p,q);
                printf("\n U realise percentage %d at timestamp=%d \n",U_array[form_count][seg_count][2],timestamp);
                printf("\n U result=%d\n",U_array[form_count][seg_count][3]);
                return (U_array[form_count][seg_count][3]); //return result failure
///////////



             }

              if ( (q==0) && (p == 1) )

            {
               U_array[form_count][seg_count][2]= U_array[form_count][seg_count][1]; // hoping U holds at end time, move to nest iteration.
                                                                                     // nothing to do, move to next iteration

             }

               if ( (q==0) && (p == 1) && (U_array[form_count][seg_count][1] == timestamp) ) //  end time reached till p is 0

                {
                 U_array[form_count][seg_count][3] = 0; // result - until failed

                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_b-U_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-U_a))/ ((float)(U_c))) *100;
                U_array[form_count][seg_count][2] = (int) (e);
                printf("\n U realise percentage %d at time stamp=%d\n",U_array[form_count][seg_count][2],timestamp);
                printf("\n Until failed since All values of p are '1', Until Result=%d\n",U_array[form_count][seg_count][3]);
                return (U_array[form_count][seg_count][3]); //return result failure
                }




            return 0; // next iteration

    }  // until operator ends




//////////////////////////////

/////Strong Until
/////////////////////////////




// Function for NAL_MTL_Strong Until - the until operator.

       // format  p S[a,b] q
// unsigned int S_array[FORMULA_COUNT][MAX_SEG][9] = {{{}}};//

    int NAL_MTL_S(int p,int q, int form_count,int seg_count) //

        {
          /*
        // j is the time point at which q holds true.
          p_now = 0,q_now=0 ; until failed.
          p_now = 1,q_now=0 ; proceed to next iteration
          p_now = 0,q_now=1 ; found j .
          p_now = 1,q_now=1 ; until failed.
          after found j, all points should be p_now=0 and q_now=1
         P Thirumeni        12.4.23
//z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4= found j,
 5=first time - q holder for (p0,q1)case, 6=k_p (p * prev_P),7=m_q (q * prev_q),8=n_q (prev_q for success case time stamps p=0 q=1)
         */
            //  printf("\n 3 Strong Until start time -   = %d, end time -   = %d,\n",S_array[5][10][0],S_array[5][10][1]);
              printf("\n at timestamp = %d, value of p =%d, value of q=%d\n",timestamp,p,q);
         //     printf("\n attatt before init start time S_array[form_count][seg_count][0]=%d\n",S_array[form_count][seg_count][0]);

             if ((S_array[form_count][seg_count][0])== timestamp)
             {

              if ((p==0) && (q == 1))

              {
                printf("\n P has to hold atleast once but at the first instance itself p is  0 and q is 1 , at timestamp = %d, p=%d,q=%d\n",timestamp,p,q);
                (S_array[form_count][seg_count][3] = 0); // result - until failed
                int S_a =S_array[form_count][seg_count][0]; // start time
                int S_b =S_array[form_count][seg_count][1];  // end time
                int S_c =S_b-S_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-S_a))/ ((float)(S_c))) *100;
                S_array[form_count][seg_count][2] = (int) (e);
                printf("\n Strong Until failed - realise percentage=%d and  at timestamp =%d ",S_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",S_array[form_count][seg_count][3]);
                return (S_array[form_count][seg_count][3]); //return result failure
              }



              printf("\n at timestamp = %d, before init prev_p=%d,prev_q=%d,seg[5]=%d \n",timestamp,S_array[form_count][seg_count][6],S_array[form_count][seg_count][7],S_array[form_count][seg_count][5]);
           //   printf("\n start time S_array[form_count][seg_count][0]=%d\n",S_array[form_count][seg_count][0]);
              S_array[form_count][seg_count][6]= 1; // init previous value of p
              S_array[form_count][seg_count][7]= 1; // init previous value of q
              S_array[form_count][seg_count][5]= 0; // init previous value of q holds for (p0,q1) case
              printf("\n at timestamp = %d, AFTER init prev_p=%d,prev_q=%d,seg[5]=%d \n",timestamp,S_array[form_count][seg_count][6],S_array[form_count][seg_count][7],S_array[form_count][seg_count][5]);
          //    printf("\n inside init  before init start time S_array[form_count][seg_count][0]=%d\n",S_array[form_count][seg_count][0]);
             }
          //  printf("\n outside init  before init start time S_array[form_count][seg_count][0]=%d\n",S_array[form_count][seg_count][0]);

                // p,p,p,p,q,q

                //p=0,q=1 first success case here




                // after SU occured p=0 q=1 cases ( success cases)
              if ( (p==0) && (q == 1) &&(S_array[form_count][seg_count][5] ==1) && (S_array[form_count][seg_count][1] != timestamp))

                {

                printf("\n timestamp=%d,p=%d,q=%d,prev_p=%d,prev_q=%d,result=%d\n",timestamp,p,q,S_array[form_count][seg_count][6],S_array[form_count][seg_count][7],S_array[form_count][seg_count][3]);

               (S_array[form_count][seg_count][8]) = (q && (S_array[form_count][seg_count][8])); // q & q_prev
               (S_array[form_count][seg_count][3]) = (q && (S_array[form_count][seg_count][8])); //result

                printf("\n After q holding true and p holding false at timestamp = %d, p=%d,q=%d, found_j =%d,(S_array[form_count][seg_count][5])=%d,(S_array[form_count][seg_count][8])=%d\n",timestamp,p,q,(S_array[form_count][seg_count][4]),(S_array[form_count][seg_count][5]),(S_array[form_count][seg_count][8]));
                printf("\n result S_array[form_count][seg_count][3]=%d",S_array[form_count][seg_count][3]);
                }


                if ( (p==0) && (q == 1) &&(S_array[form_count][seg_count][5] == 0) )

                {
                printf("\n q holds before update timestamp = %d, p=%d,q=%d, found_j =%d,(S_array[form_count][seg_count][5])=%d,(S_array[form_count][seg_count][8])=%d\n",timestamp,p,q,(S_array[form_count][seg_count][4]),(S_array[form_count][seg_count][5]),(S_array[form_count][seg_count][8]));
                S_array[form_count][seg_count][3] = q; // result
               (S_array[form_count][seg_count][4])= timestamp;// first time, found j
               (S_array[form_count][seg_count][5]) = 1;
               (S_array[form_count][seg_count][8]) = q;
                printf("\n First instance after q holding true and p holding false at timestamp = %d, p=%d,q=%d, found_j =%d,(S_array[form_count][seg_count][5])=%d,(S_array[form_count][seg_count][8])=%d\n",timestamp,p,q,(S_array[form_count][seg_count][4]),(S_array[form_count][seg_count][5]),(S_array[form_count][seg_count][8]));
                printf("\n result S_array[form_count][seg_count][3]=%d",S_array[form_count][seg_count][3]);

                }

   //fail cases

             if (( p==0 && q == 0) || ( p==1 && q == 1)) // both fail cases

                {
                 printf("\n Both p and q are 0 or both p and  q are 1 , at timestamp = %d, p=%d,q=%d\n",timestamp,p,q);
                (S_array[form_count][seg_count][3] = 0); // result - until failed
                int S_a =S_array[form_count][seg_count][0]; // start time
                int S_b =S_array[form_count][seg_count][1];  // end time
                int S_c =S_b-S_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-S_a))/ ((float)(S_c))) *100;
                S_array[form_count][seg_count][2] = (int) (e);
                printf("\n Strong Until failed - realise percentage=%d and  at timestamp =%d ",S_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",S_array[form_count][seg_count][3]);


                return (S_array[form_count][seg_count][3]); //return result failure
                }


                if ( ( p==1 && q == 0) && S_array[form_count][seg_count][8] ==1) // q holding already but p holds again
                {
                printf("\n Earlier q was holding at this time stamp (found j) =%d and P holds now at timestamp=%d, hence Strong Until failed.  p=%d,q=%d\n",S_array[form_count][seg_count][4],timestamp,p,q);
                (S_array[form_count][seg_count][3] = 0); // result - until failed
                int S_a =S_array[form_count][seg_count][0]; // start time
                int S_b =S_array[form_count][seg_count][1];  // end time
                int S_c =S_b-S_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-S_a))/ ((float)(S_c))) *100;
                S_array[form_count][seg_count][2] = (int) (e);
                printf("\n Strong Until failed - realise percentage=%d and  at timestamp =%d ",S_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",S_array[form_count][seg_count][3]);
                return (S_array[form_count][seg_count][3]); //return result failure
                }

                if ( timestamp) // for all iteration this update has to be done. // this includeds p=1 and q=0 cases
                {
                (S_array[form_count][seg_count][6])= (p && (S_array[form_count][seg_count][6])); // current p && prev_p
                (S_array[form_count][seg_count][7])= (q && (S_array[form_count][seg_count][7])); // current q && prev_q
                printf("\n at just iterating... timestamp =%d,p=%d and  q=%d\n ",timestamp,p,q);
                printf("\n timestamp=%d,prev_p=%d,prev_q=%d\n ",timestamp,S_array[form_count][seg_count][6],S_array[form_count][seg_count][7]);
                }

            //end time results, if all time instances either p or q iis always 1, its a failure case

             if ((S_array[form_count][seg_count][1])== timestamp) // at end time p=1, q=0 case also covered here...

              {
               if  (((S_array[form_count][seg_count][6]) == 1 ) || ((S_array[form_count][seg_count][7]) == 1 ) ) //fail case

                    {
                printf("\n at timestamp =%d,p=%d and  q=%d ",timestamp,p,q);
                S_array[form_count][seg_count][2] =0; // realise percentage is 0 since all  values are either p holds true or all q holds true
                S_array[form_count][seg_count][3] =0;
                printf("\n Strong Until failed - realise percentage=%d and  at timestamp =%d ",S_array[form_count][seg_count][2],timestamp);
                printf("\n Either All p are 1 or all q are 1, all p = %d,all q=%d",S_array[form_count][seg_count][6],S_array[form_count][seg_count][7]);
                printf("\n Result =%d",S_array[form_count][seg_count][3]);
                return (S_array[form_count][seg_count][3]); //return result
                    }
              }

//z index detail: 0=start time, 1= end time, 2=S_realise_percent,3=result,4= found j,
// 5=first time - q holder for (p0,q1)case, 6=k_p (p * prev_P),7=m_q (q * prev_q),8=n_q (prev_q for success case time stamps p=0 q=1)

        //end time result
             if (( p==0 && q == 1) && (S_array[form_count][seg_count][5] == 1) &&(S_array[form_count][seg_count][1] == timestamp)) // at end time

                {

                (S_array[form_count][seg_count][3]) == (q && (S_array[form_count][seg_count][5]) && (S_array[form_count][seg_count][8])); // q && jfound && prev_q
                //(S_array[form_count][seg_count][4]) == q;
                int S_a =S_array[form_count][seg_count][0]; // start time
                int S_b =S_array[form_count][seg_count][1];  // end time
                int S_c =S_b-S_a; // total time duration for this property to evaluate.
                float e =( ((float)(S_array[form_count][seg_count][4]-S_a))/ ((float)(S_c))) *100;
                S_array[form_count][seg_count][2] = (int) (e);
                printf("timestamp,S_a=%d,S_b=%d,S_c=%d,e=%f",S_a,S_b,S_c,e);
                printf("\n Strong Until Success - realise percentage=%d and  at timestamp =%d ",S_array[form_count][seg_count][2],S_array[form_count][seg_count][4]);
                printf("\n Result =%d",S_array[form_count][seg_count][3]);


                return (S_array[form_count][seg_count][3]); //return result success

                }




            return 2; // next iteration, returning 2 to indicate the formula not eveluated yet

    }

//// strong until operator ends

//////////////////////////////

//////////////////////


// weak until

// Function for NAL_MTL_Weak_Until - the weak until operator.

//z=5; index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4=all_p

       // format  p W[a,b] q
   int NAL_MTL_W(int p,int q, int form_count,int seg_count) // formula id, segment_id, kji index, 3 d array


        {

        // weak until,  j is the time point at which q holds true.

        /*
          p_now = 0,q_now=0 ; until failed.
          p_now = 1,q_now=0 ; proceed to next iteration
          p_now = 0,q_now=1 ; found j .exit
          p_now = 1,q_now=1 ; found j .exit
          also from start to end p_now all true ; property true.

         P Thirumeni        12.4.23
         */

                 printf("\n receiving at Weak Until timestamp = %d, value of p =%d, value of q=%d\n",timestamp,p,q);
             if ( (W_array[form_count][seg_count][0]) == timestamp)  // at start initialise..

                 {
                     printf("\n Before init all_p =%d\n",W_array[form_count][seg_count][4]);
                     (W_array[form_count][seg_count][4])=1; //all_p init to 1 if p become 0 at some stage this will become 0
                     printf("\n After init all_p =%d\n",W_array[form_count][seg_count][4]);
                 }


             if ( p==0 && q == 0) // p does not hold - property fails
             {
                (W_array[form_count][seg_count][3] = 0); // result - until failed


                printf("\n at timestamp =%d,p=%d and  q=%d ",timestamp,p,q);
                W_array[form_count][seg_count][2] =0; // realise percentage is 0 since all  values are either p holds true or all q holds true
                int W_a =W_array[form_count][seg_count][0]; // start time
                int W_b =W_array[form_count][seg_count][1];  // end time
                int W_c =W_b-W_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-W_a))/ ((float)(W_c))) *100;
                W_array[form_count][seg_count][2] = (int) (e);

                printf("\n Weak Until failed - realise percentage=%d and  at timestamp =%d ",W_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",W_array[form_count][seg_count][3]);
                return (W_array[form_count][seg_count][3]); //return result

                }

//z index detail: 0=start time, 1= end time, 2=W_realise_percent,3=result.

             if ( q == 1 )

            {
               // W_array[form_count][seg_count][4]=timestamp // j found at current time stamp, let us avoid this index to save memory
               (W_array[form_count][seg_count][3] = q); // result

                printf("\n at timestamp =%d,p=%d and  q=%d ",timestamp,p,q);
                int W_a =W_array[form_count][seg_count][0]; // start time
                int W_b =W_array[form_count][seg_count][1];  // end time
                int W_c =W_b-W_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-W_a))/ ((float)(W_c))) *100;
                W_array[form_count][seg_count][2] = (int) (e);
                printf("\n Weak Until Sucessful !!  - realise percentage=%d and  at timestamp =%d ",W_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",W_array[form_count][seg_count][3]);
                return (W_array[form_count][seg_count][3]); //return result


             }

              if ( (p==1) && (q == 0) )

            {
               W_array[form_count][seg_count][2]= W_array[form_count][seg_count][1]; // hoping U holds at end time, move to nest iteration.
                printf("\n iterating...\n");                                                                     // nothing to do, move to next iteration

             }


//z index detail: 0=start time, 1= end time, 2=W_realise_percent,3=result,4= all_p


        if ( timestamp) // at every time stamp p && prev_p

            {
                (W_array[form_count][seg_count][4])= (p && (W_array[form_count][seg_count][4]));
            }

              if ( (W_array[form_count][seg_count][1] == timestamp) && (W_array[form_count][seg_count][4]) == 1 )

              {

                printf("\n at timestamp =%d,p=%d and  q=%d ",timestamp,p,q);
                int W_a =W_array[form_count][seg_count][0]; // start time
                int W_b =W_array[form_count][seg_count][1];  // end time
                int W_c =W_b-W_a; // total time duration for this property to evaluate.
                float e =( ((float)(timestamp-W_a))/ ((float)(W_c))) *100;
                W_array[form_count][seg_count][2] = (int) (e);
                W_array[form_count][seg_count][3] = W_array[form_count][seg_count][4];
                printf("\n Weak Until Sucessful !!  - realise percentage=%d and  at timestamp =%d ",W_array[form_count][seg_count][2],timestamp);
                printf("\n Result =%d",W_array[form_count][seg_count][3]);
                printf("\n Alll values of P =%d",W_array[form_count][seg_count][4]);
                return (W_array[form_count][seg_count][3]); //return result


              }

            return 2;

    }  // weak until operator ends


////////////////////////////////////////////////////



////////////////////////////////////////////////////

// for single input arguments like not of p or other operation like check p == 1 etc., send q argument as 0 or 1 arbitrary, but q has to be sent

int NAL_MTL_Verdict (int logic_operator, unsigned int p, int q)

        {

        // int v;
          switch (logic_operator)

          {
        case 1:     // If input is 1, check if p is equal to 1 ; also used to read the atomic proposition as a 'bit' from the array, here q is irrelevant

            if (p == 1)
            {


                printf("\np == 1\n");
                v=1;
            }

            else if (p == 0)
            {

             printf("\np != 1\n");
                v=0;
            }
            break;
        case 0:
            // If input is 0, check if p is equal to 0
            if (p == 0)
                {
                printf("\np == 0\n");
                v=1;
                }
            else if (p == 1)
                {
                printf("\np != 0\n");
                v=0;
                }

            break;
        case 3:
            // If input is 3, perform logical AND operation on p and q
            v = p && q;
            printf("\np && q = %d\n", v);
            break;
        case 4:
            // If input is 4, perform logical OR operation on p and q
            v = p || q;
            printf("\np || q = %d\n", v);
            break;
        case 5:
            // If input is 5, perform logical NOT operation on p
            v = !p;
            printf("\n!p = %d\n", v);
            break;
        case 6:
            // If input is 6, perform logical implication operation on p and q
            v = (!p) || q;
            printf("\nLogical implication  (!p) || q = %d\n", v);
            break;
        case 7:
            // If input is 7, perform logical equivalence operation on p and q
           if (p != q)
            {
                v = 0;
            }
           else if (p == q)
            {
              v = 1;
             }

            printf("\nLogical equivalence (p iff q) = %d\n", v);
            break;
        case 9:
            // If input is 9, perform logical XOR operation on p and q
            v = p ^ q;
            printf("\nXOR p ^ q = %d\n", v);
            break;

        default:
            // If input is not valid, print error message
            v=3;
            printf("\nInvalid logic request input!\n");

    }

            return v;

    }  // Verdict operator_2 ends

/////////////////////////////////////////////////



