
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
unsigned int S_array[FORMULA_COUNT][MAX_SEG][9] = {{{}}};//  //z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4= found j,5=prev_q holder,6=k_p,7=m_q,8=n_q
unsigned int W_array[FORMULA_COUNT][MAX_SEG][6] = {{{}}};//  //z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result,4= found j,5=all_p

unsigned int G_array [FORMULA_COUNT][MAX_SEG][5] ={{{0}}};
//[for_index] [seg_index] [z],Z=5     // 0=S_TS, 1=E_TS, 2=prev_g , 3=G_percent,4=G_result);

unsigned int F_array [FORMULA_COUNT][MAX_SEG][5] ={{{0}}};
//[for_index] [seg_index] [z],Z=5     // 0=S_TS, 1=E_TS, 2=prev_f , 3=F_percent,4=F_result);

////////////////


int timestamp;
int row_index;
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

    G_array[5][10][0] =3; // start timestamp
    G_array[5][10][1] =18; // end timestamp
    int p,q;
 // timestamp=3;// initial timestamp.
    F_array[5][10][0] =3; // start timestamp
    F_array[5][10][1] =18; // end timestamp
/*
    int p =0; // to be tested
    int start_time = 3;
    int end_time = 18;

    for (int i=3 ; i<=18 ; i++)
    {
    sleep(1);
    timestamp =i;

    if (i == 17)
    {
       // p=1;
    }

    // Testing G operator.
    //  int ss = NAL_MTL_G(p,5,10);
    // Testing F operator
    //int ss = NAL_MTL_F(p,5,10); // formula 5 and segment 10
     printf("\nsending p = %d, at timestamp %d \n ", p,timestamp);
    }


    */

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

    ///////////////
    // format int* col_data = get_data_by_AP(col_index, start_row, end_row);

    // input for GF here...
/*
        int start_TS=3;
        int end_TS=19;
        int col_index =5;//ap index

        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

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

 // Testing FG operator

 // input for FG here...
/*
        int start_TS=2;
        int end_TS=30;
        int col_index =5;//ap index

        int start_in_array = start_TS -1;// -1 considering that arrays stats with 0 index.
        int end_in_array = end_TS -1;

        int* col_data_1 = get_data_by_AP(col_index, start_in_array, end_in_array); // ap index, start time, end time

        FG_ji_array[5][10][0] =start_TS; // start timestamp
        FG_ji_array[5][10][1] =end_TS; // end timestamp


         printf("\n\n\n FG function - Sending the values of P per every 1 sec now....\n");

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

// U operator testing....

//int NAL_MTL_U(int p,int q, int form_count,int seg_count);

// input for U here...

        int start_TS=3;
        int end_TS=12;
        int col_index =4;//ap index
        int col_index2 =5;//ap index

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
        printf("AP id/index of p= %d,column index of q=%d, time stamp= %d: AP value p =%d, q =%d\n",col_index,col_index2,timestamp, p,q);//here p =col_data_1[i - start_TS], considerring array index starting at 0
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



        } // for loop ends

//////////////////////////////





    return 0;

    }


///////////////// main ends here.

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


// Fucntion Definitions from here.....

//Globally

   int NAL_MTL_G(int p,unsigned int form_count,unsigned int seg_count)  //

        {

          printf("\n Receiving inside G_function  p=%d and time stamp = %d\n", p,timestamp);
          if (G_array[form_count][seg_count][0] == timestamp) // checking start time
          {
           printf("\n Inside G init before  value of prev_g =%d \n", G_array[form_count][seg_count][2]);
           G_array[form_count][seg_count][2]=1; // initialise prev_g =1
           printf("\n Inside G init after value of prev_g =%d \n", G_array[form_count][seg_count][2]);
          }

           G_array[form_count][seg_count][4]=G_array[form_count][seg_count][2] && p ; // prev_g && current g
            printf("\n evaluated G = %d\n\n", G_array[form_count][seg_count][4]);

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
                printf("\n Inside at end time point %d",timestamp);
                int a = G_array[form_count][seg_count][0];
                int b = G_array[form_count][seg_count][1];
                float c = b-a;
                float d = ((timestamp-a)/c) *100;

                printf("\n Current timestamp=%d,start time of G = %d ,End time of G = %d, Total Duration = %f,",timestamp,a,b,c);
                G_array[form_count][seg_count][3] = (int)d; // G success 100% at the end
                printf("\n\n\n G realise percentage %d \n",G_array[form_count][seg_count][3]);
                printf("\n\n\n G result %d \n",G_array[form_count][seg_count][4]);
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
                printf("\n\n\n F result %d \n",F_array[form_count][seg_count][4]);
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
                printf("\n\n\n F result %d \n",F_array[form_count][seg_count][4]);
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
                printf("GF_kj_array[form_count][seg_count][4]\n",GF_kj_array[form_count][seg_count][4]);
                printf("\n reached end of the GF time window.\n ");
                printf("\nLast time j=1 at time stamp = %d\n",GF_kj_array[form_count][seg_count][2]);//k value
                printf("\n Current timestamp=%d,start time of GF = %d ,End time of GF = %d, Total Duration = %d,",timestamp,GF_a,GF_b,GF_c);
                printf("\n\n\n GF realise percentage %d \n",GF_kj_array[form_count][seg_count][4]);
                printf("\n\n\n GF result %d \n",GF_kj_array[form_count][seg_count][5]);
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
                printf("\nFG_ji_array[form_count][seg_count][4]\n",FG_ji_array[form_count][seg_count][4]);
                printf("a=%d,b=%d,c=%d,d=%d,e=%f\n",FG_a,FG_b,FG_c,d,e);
                printf("\n\n\n FG realised from timestamp=%d and percentage=%d \n",FG_ji_array[form_count][seg_count][2],FG_ji_array[form_count][seg_count][4]);
                printf("\n\n\n FG realise result %d \n",FG_ji_array[form_count][seg_count][5]);
                return FG_ji_array[form_count][seg_count][5]; // this si the result of FG operation do not consider fucntion return 0 below from the calling function.
             }


    return 0; // FG_ends
}


//////////////////////  FG_ends




//////////////////////

//Until

// NAL_MTL_Until

// format  p U[a,b] q

//∃j (a ≤ j ≤ b) ∧ (∀i (a ≤ i < j) : (Ai ⊨ p ∨ Ai ⊭ p)  ∧  (∀i (j ≤ i ≤ b) : Ai ⊨ p)

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
                printf("\n U result=%d\n",U_array[form_count][seg_count][3]);
                return (U_array[form_count][seg_count][3]); //return result failure
                }




            return 0; // next iteration

    }  // until  operator ends.





//////////////////////////////

