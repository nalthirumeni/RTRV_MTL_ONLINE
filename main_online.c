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
//DATA STRUCTURES
char AP_UPDATE [NUM_APS] = {1};                  // FOR EVERY TIME STAMP UPDATE
char data[MAX_ROWS][NUM_COLS] = {{1}};           // FOR RETRIVING ANY TIMESTAMPED ROW DATA AND TO RETRIEVE ANY COLUM TIME STAMPED DATA(LOW AND HIGH TIME LIMITS)
char data_1[ASR_MAX_ROWS][ASR_NUM_COLS] = {{1}}; // FOR MANUPULATING SEGMENTS RESULTS AND RETRIVING SEGMENT DATA PER FORMULA BASED ON FORMULA INDEX (ID)
char GF_kj_array [FORMULA_COUNT][MAX_SEG][6] = {{{0}}};  // three dimensional array for handling k and j values during GF operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)
// GF_kj_array[x][y][z] = [for index] [seg index] [z],Z=5 (S_TS, E_TS, k , j, GF_percent,GF_result); S_TS = starting time stamp,E_TS= ending time stamp

char FG_ji_array [FORMULA_COUNT][MAX_SEG][6] = {{{0}}};  // three dimensional array for handling k and j  and i etc., values during FG operations,x= formula count.y=max. segment count found in any one of the formula.Z=5 (S_TS, E_TS, k, j, GF_result)
 //FG_kj_array[FORMULA_COUNT][MAX_SEG][5] = {{{}}}; // [for index] [seg index] [z],Z=6 (S_TS, E_TS,j, i,GF_percent,GF_result); S_TS = starting time stamp,E_TS= ending time stamp

//char ASR [FORMULA_COUNT] [MAX_SEG] ={{0}};
//char FG_ji_array [10][10][10]={{{}}};


char U_array[FORMULA_COUNT][MAX_SEG][4] = {{{}}};// [for index] [seg index] [z],Z=3 (0=S_TS, 1=E_TS,2=U_percent,3=result,i)
char S_array[FORMULA_COUNT][MAX_SEG][4] = {{{}}};// [for index] [seg index] [z],Z=3 (0=S_TS, 1=E_TS,2=U_percent,3=result,i)



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
// GF_kj_array[x][y][z] = [for index] [seg index] [z],Z=5 (S_TS, E_TS, k , j, GF_percent,GF_result)
//char GF_kj_array [FORMULA_COUNT][MAX_SEG][5]


   int NAL_MTL_GF(int p,int form_count,int seg_count,int kj_index,int GF_kj_array[form_count][seg_count][kj_index]) // formula id, segment_id, kj index, 3 d array

        {

          if (GF_kj_array[seg_count][seg_count][0] == timestamp) //checking current time equals to the start time of the segment
          {
           GF_kj_array[form_count][seg_count][2]=GF_kj_array[form_count][seg_count][0]; // initialise k = start time
          }


            if (p == 1 )
                {
                
                GF_kj_array[form_count][seg_count][2]=timestamp;  // k = timestamp, till this time point GF holding true...
                GF_kj_array[form_count][seg_count][3] = 1; // j =1
                }

            else if (p == 0 )
                {
                GF_kj_array[form_count][seg_count][3] = 0; // j =0

                }

// GF_kj_array[x][y][z] = [for index] [seg index] [z],Z=5 (S_TS, E_TS, k , j, GF_percent,GF_result)

            if (GF_kj_array[form_count][seg_count][1] == timestamp; ) //checking current time equals to the end time of the segment

             {
                GF_kj_array[form_count][seg_count][5] = GF_kj_array[form_count][seg_count][3]; //result of GF operation,result = last value of j
                int GF_a =GF_kj_array[form_count][seg_count][0]; // start time
                int GF_b =GF_kj_array[form_count][seg_count][1];  // end time
                int GF_c=b-a; // total time duration for this property to evaluate.
                GF_realise_percent =((GF_kj_array[form_count][seg_count][2] - a) /c) * 100); // (k-time stamp - start time / time duration * 100 %)
                GF_kj_array[form_count][seg_count][4] == GF_realise_percent; // realise percentage

                printf("\n\n\n GF realise percentage %d \n",GF_realise_percent);
                printf("\n\n\n GF result %d \n",GF_kj_array[form_count][seg_count][5]);
                return GF_kj_array[form_count][seg_count][5];
             }


    return 0; //


            }  // GF function ends

////////////////////////////////////////



///////////////////////////////////

//FG

// Function for NAL_MTL_FG - the eventually forever operator.

//∃j (a ≤ j ≤ b) ∧ (∀i (a ≤ i < j) : (Ai ⊨ p ∨ Ai ⊭ p)  ∧  (∀i (j ≤ i ≤ b) : Ai ⊨ p)

// FG_ji_array [FORMULA_COUNT][MAX_SEG][z=5]

//z index detail: 0=start time, 1= end time, 2=j,3=i,4=FG_realise_percent,5=result of FG.




   int NAL_MTL_FG(int p,int form_count,int seg_count,int ji_index,int FG_ji_array[form_count][seg_count][ji_index]) // formula id, segment_id, kji index, 3 d array


        {


      if (FG_ji_array[form_count][seg_count][0] == timestamp)
          {
           FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1]; // initialise j = end time
           FG_ji_array[form_count][seg_count][3] =1; //// initialise i = 1
          }

//j is the point from which p becomes 1 forever

         if (FG_ji_array[form_count][seg_count][0] == timestamp) //start time

         {
            //FG_ji_array[form_count][seg_count][5] = p; // 

            if  ( (p && FG_ji_array[form_count][seg_count][3]) == 1  )

            {

                FG_ji_array[form_count][seg_count][2]=timestamp; //j
            }

             else if  ( (p && FG_ji_array[form_count][seg_count][3]) == 0  )

            {

                FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1] ; //  j = end time
            }

         }


         /*

         p= current value of AP and i= previous value of AP taken from array

         p=0,i=0 : make j = the end time stamp
         p=0,i=1 : make j = the end time stamp
         p=1,i=0 : make j = the current time stamp
         p=1,i=1 : make j = don not change j let the previous  j time stamp remain

         P Thirumeni        12.4.23

         */

            if ( (p == 1) && (FG_ji_array[form_count][seg_count][3] == 0) )

                {

                FG_ji_array[form_count][seg_count][2]= timestamp; // make j = timestamp hoping from this point all future p's will be 1.
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result considering two values at end
                FG_ji_array[form_count][seg_count][3] = 1; // update i =1
                 }

            if ( (p == 1) && (FG_ji_array[form_count][seg_count][3] == 1) )

                {

                FG_ji_array[form_count][seg_count][3] = 1; // update i =1
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result for the end values 1,1

                }



            else if (p == 0 )
                {


                FG_ji_array[form_count][seg_count][2]=FG_ji_array[form_count][seg_count][1] ; //  j = end time
                FG_ji_array[form_count][seg_count][5] = (p  && FG_ji_array[form_count][seg_count][3]) ; //result for the end
                FG_ji_array[form_count][seg_count][3] = 0; // update i =0
                }


            if (FG_ji_array[form_count][seg_count][1] == timestamp) //end time reached

             {
               // GF_ji_array[form_count][seg_count][5] = GF_ji_array[form_count][seg_count][5]; //result of FG operation,result = last value of i
                int FG_a =FG_ji_array[form_count][seg_count][0]; // start time
                int FG_b =FG_ji_array[form_count][seg_count][1];  // end time
                int FG_c =FG_b-FG_a; // total time duration for this property to evaluate.
                FG_realise_percent =(((FG_b-FG_ji_array[form_count][seg_count][2]) /FG_c) * 100); // ((end time - j_time stamp  ) / (start time - end_time)) * 100 %
                FG_ji_array[form_count][seg_count][4] == FG_realise_percent; // realise percentage of FG, this tells from start point how quickly eventually p became '1', higher the this percentage quicker the eventually forever condition reached.

                printf("\n\n\n GF realise percentage %d \n",FG_realise_percent);
                printf("\n\n\n GF realise result %d \n",FG_ji_array[form_count][seg_count][5]);
                return FG_ji_array[form_count][seg_count][5];
             }


    return 0; // FG_ends
}


//////////////////////

//Until
// NAL_MTL_Until  
// format  p U[a,b] q

//∃j (a ≤ j ≤ b) ∧ (∀i (a ≤ i < j) : (Ai ⊨ p ∨ Ai ⊭ p)  ∧  (∀i (j ≤ i ≤ b) : Ai ⊨ p)

// U_array [FORMULA_COUNT][MAX_SEG][z=3]

//z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.



   int NAL_MTL_U(int p,int q, int form_count,int seg_count,int u_index,int U_array[form_count][seg_count][u_index]) // formula id, segment_id, u_index, 3 d array

        {

        /*

        // j is the time point at which q holds true.


          p_now = 0,q_now=0 ; until failed.
          p_now = 1,q_now=0 ; proceed to next iteration
          p_now = 0,q_now=1 ; found j .
          p_now = 1,q_now=1 ; found j .



         P Thirumeni        12.4.23

         */


             if ( p==0 && q == 0) // p does not hold true till j

             {
                (U_array[form_count][seg_count][3] == 0); // result - until failed

                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_a-U_b; // total time duration for this property to evaluate.
                U_realise_percent =(((timestamp-U_a) /U_c) * 100); // ((end time - j_time stamp  ) / (start time - end_time)) * 100 %
                U_array[form_count][seg_count][2] == U_realise_percent; // realise percentage of U, % of at which p=0, that is until failed at this % of total time

                printf("\n\n\n U realise percentage %d \n",U_array[form_count][seg_count][2]);

                return (U_array[form_count][seg_count][3]); //return result failure
             }

//z index detail: 0=start time, 1= end time, 2=U_realise_percent,3=result.

             if ( q == 1 )

            {
               // U_array[form_count][seg_count][4]=timestamp // j found at current time stamp, let us avoid this index to save memory
               (U_array[form_count][seg_count][3] == q); // result

                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_a-U_b; // total time duration for this property to evaluate.
                U_realise_percent =(((timestamp - U_a)  /U_c) * 100); // ((end time - j_time stamp  ) / (start time - end_time)) * 100 %
                U_array[form_count][seg_count][2] == U_realise_percent; // realise percentage of U, % at which Until got satisfied

                printf("\n\n\n U realise percentage %d \n",U_array[form_count][seg_count][2]);

                return (U_array[form_count][seg_count][3]); //return result success


             }

              if ( (q==0) && (p == 1) )

            {
               U_array[form_count][seg_count][2]= U_array[form_count][seg_count][1]; // hoping U holds at end time, move to nest iteration.
                                                                                     // nothing to do, move to next iteration

             }

               if ( (q==0) && (p == 1) && (U_array[form_count][seg_count][1] == timestamp) )

                {
                 (U_array[form_count][seg_count][3] == 0); // result - until failed

                int U_a =U_array[form_count][seg_count][0]; // start time
                int U_b =U_array[form_count][seg_count][1];  // end time
                int U_c =U_a-U_b; // total time duration for this property to evaluate.
                U_realise_percent =(((timestamp - U_a)  /U_c) * 100); // ((end time - j_time stamp  ) / (start time - end_time)) * 100 %
                U_array[form_count][seg_count][2] == U_realise_percent; // realise percentage of U, % of at which p=0, that is until failed at this % of total time

                printf("\n\n\n U realise percentage %d \n",U_array[form_count][seg_count][2]);


                  return (U_array[form_count][seg_count][3]); //return result for failure
                }




            return 0; // next iteration

    }  // until operator ends





//////////////////////////////

//////////////////////
