                            /*       NAL_RV       */

                            // P26 FOR ONLINE MONITORING


// This is Run Time monitoring software tool, for monitoring RT systems to be used along with NAL_MTL specifications.

// Developed by P Thirumeni -203085002, PhD, student IIT Dharwad India.
// Under the guidance of PhD. Supervisors, Dr.Rajshekar.K (IIT-Dh), Dr.C.M.Ananda (CSIR-NAL)
 
// working P26 P THIRUMENI  date :12.04.23

// implementation under progress for  Online mode of operation//

// P26 FOR ONLINE MONITORING


/**********************************************************************************************/

#include <stdio.h>  // for standard io operations
#include <stdlib.h> // for use malloc
#include <string.h> // for string processing


#define MAX_ROWS 7200  // define the no. of total mission time units here, for example 2 hours is 7200 sec. if the time granularity considered is 1 sec.
#define NUM_COLS 64    // Number of atomic propositions to be monitored is to be defined here.
#define ASR_MAX_ROWS 128  // Formula count OR FORMULA INDEX/ID
#define ASR_NUM_COLS 64  // MAX_SEGMENTS - SUB FORMULA RESULT SEGMENTS (EXAMPLE: S1,S2,S3...ETC.,)
#define NUM_APS 64          // Number of APs planned for monitoring.

#define FORMULA_COUNT 128 // number of RVRT_MTL formulas.
#define MAX_SEG 64      // number segment max. variables in longest RVRT_MTL formulas (S1,S2..)




//DATA STRUCTURES

char AP_UPDATE [NUM_APS] = {1};                  // FOR EVERY TIME STAMP UPDATE
char data[MAX_ROWS][NUM_COLS] = {{1}};           // FOR RETRIVING ANY TIMESTAMPED ROW DATA AND TO RETRIEVE ANY COLUM TIME STAMPED DATA(LOW AND HIGH TIME LIMITS)
char data_1[ASR_MAX_ROWS][ASR_NUM_COLS] = {{1}}; // FOR MANUPULATING SEGMENTS RESULTS AND RETRIVING SEGMENT DATA PER FORMULA BASED ON FORMULA INDEX (ID)
char GF_kj_array [FORMULA_COUNT][MAX_SEG][5] = {{{0}}};  // three dimensional array for handling k and j values during GF operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)


//char ASR [FORMULA_COUNT] [MAX_SEG] ={{0}};


char headers[NUM_COLS][500];

char line[200];

int row_index;
int row_index_1;
int ts,P1,P2,P3,P4,P5,t1,t2;
int* ap_row_data;
int *row_data;


int Globally=1;
int Globally_SEG=1;
int Finally_SEG = 1;
int Finally=1;
int InOf=1;
int EvFor=1;
int Until=1;
int Weak_U=1;
int curr_timestamp;
int timestamp;
int formula_id=1;
int TS_L_1=15;
int TS_H_1=25;
int TS_L_2=20;
int TS_H_2=25;
int call;
int Now,Next,End,Bedore;


float G_realise_percent;
float F_realise_percent;
float GF_realise_percent;
float FG_realise_percent;
float U_realise_percent;
float S_realise_percent;
float W_realise_percent;


int exps;

// global to be made extern from other file...
int ON_after_delay_global_1=1;
int stimulas_global_1;
int ON_after_delay_global_2=0;
int stimulas_global_2;

// Functions
 
/////////////////////////////////////////////////

// GF_operator - Infinitely Often

//     ∀k ≥ a ∧ k ≤ b . ∃j ≥ k ∧ j ≤ b : (Aj ⊨ p)  where a,b are low and up time limit and 'p' is the Atomic proposition(AP) of a RTRV_MTL subformula to be evaluated.


// Function for NAL_MTL_ GF - the infinitely often operator
// GF_kj_array,//Z=5 (S_TS, E_TS, k , j, GF_percent,GF_result)

//char GF_kj_array [FORMULA_COUNT][MAX_SEG][5]


   int NAL_MTL_GF(int p,int form_count,int seg_count,int kj_index,int GF_kj_array[form_count][seg_count][kj_index]) // formula id, segment_id, kj index, 3 d array

        {

          if (GF_kj_array[seg_count][seg_count][0] == timestamp)
          {
           GF_kj_array[form_count][seg_count][3]=GF_kj_array[form_count][seg_count][0]; // initialise k = strat time
          }


            if (p == 1 )
                {
                found_j = 1;
                GF_kj_array[form_count][seg_count][2]=time stamp;  // k = timestamp, till this time point GF holding true...
                GF_kj_array[form_count][seg_count][3] = 1; // j =1
                }

            else if (p == 0 )
                {
                GF_kj_array[form_count][seg_count][3] = 0; // j =0

                }


            if (GF_kj_array[form_count][seg_count][1] == timestamp; )

             {
                GF_kj_array[form_count][seg_count][5] = GF_kj_array[form_count][seg_count][3]; //result of GF operation,result = last value of j
                int GF_a =GF_kj_array[form_count][seg_count][0]; // start time
                int GF_b =GF_kj_array[form_count][seg_count][1];  // end time
                int GF_c=b-a; // total time duration for this property to evaluate.
                GF_realise_percent =((GF_kj_array[form_count][seg_count][2] - a) /c) * 100); // k-time stamp - start time / time duration * 100 %
                GF_kj_array[form_count][seg_count][4] == GF_realise_percent; // realise percentage

                printf("\n\n\n GF realise percentage %d \n",GF_realise_percent);
                printf("\n\n\n GF realise percentage %d \n",GF_kj_array[form_count][seg_count][5]);
                return GF_kj_array[form_count][seg_count][5];
             }


    return 0; //


            }  // GF function ends

////////////////////////////////////////

