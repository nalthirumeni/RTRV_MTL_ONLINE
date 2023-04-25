                                  
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

*/



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
 //FG_kj_array[FORMULA_COUNT][MAX_SEG][5] = {{{}}}; // [for index] [seg index] [z],Z=6 (S_TS, E_TS,j, i,GF_percent,GF_result); S_TS = starting time stamp,E_TS= ending time stamp

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

//Global
int NAL_MTL_G(int p,unsigned int form_count,unsigned int seg_count);

//Finally
int NAL_MTL_F(int p,unsigned int form_count,unsigned int seg_count);

//GF
int NAL_MTL_GF(int p,unsigned int form_count,unsigned int seg_count);



int main()

    {


    ////////////////////


    printf("\n\n");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n                                               NAL_RV ONLINE MONITORING ");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n\n");

    timestamp=3;// initial timestamp.

    //array dimention 3 index detail -  0=S_TS, 1=E_TS, 2=prev_g , 3=G_percent,4=G_result);
    // fill start and end time and call G_func.

    G_array[5][10][0] =3; // start timestamp
    G_array[5][10][1] =18; // end timestamp


    F_array[5][10][0] =3; // start timestamp
    F_array[5][10][1] =18; // end timestamp

    int p =0; // to be tested
    int start_time = 3;
    int end_time = 18;

    for (int i=3 ; i<=18 ; i++)
    {
    sleep(1);
    timestamp =i;
    if (i == 17)
    {
        p=1;
    }
    printf("\nsending p = %d, at timestamp %d \n ", p,timestamp);
    // Testing G operator.
  //  int ss = NAL_MTL_G(p,5,10);
     // Testing F operator

    int ss = NAL_MTL_F(p,5,10); // formula 5 and segment 10


    //if (ss == 0)
    if (ss == 1)
    {
        break;
    }

    }

    return 0;
    }







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

