                            /*       NAL_RV       */

                            // P26 FOR ONLINE MONITORING


// This is Run Time monitoring software tool, for monitoring RT systems to be used along with NAL_MTL specifications.

// Developed by P Thirumeni -203085002, PhD, student IIT Dharwad India.
// Under the guidance of PhD. Supervisors, Dr.Rajshekar.K (IIT-Dh), Dr.C.M.Ananda (CSIR-NAL)

// Name of the software : NAL_RV
// Formalism to be used : RTRV_MTL

// working P25 P THIRUMENI  date :11.04.23

// implementation under progress for  Online mode of operation//

// P26 FOR ONLINE MONITORING

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


//////////////////////////////////////

//  G operator.

        int NAL_MTL_G(int row_index , int AP_index)  //

        {
          Globally =  (data [row_index] [AP_index]);
          return (Globally);

            }



////////////////////////


//////////////////////////////////////

//  G_SEG operator.

        int NAL_MTL_G_SEG (int ASR_row_index , int SEG_index )  //

        {
          Globally_SEG =   (data_1[ASR_row_index] [SEG_index ]);
          return (Globally_SEG);
            }



//////




////////////////////////////////////////



//  F_seg  operator.

        int NAL_MTL_F_SEG(int ASR_row_index , int SEG_index)

        {

          Finally_SEG =   (data_1[ASR_row_index] [SEG_index ]);
          return (Finally_SEG);

            }

/////////////////////////////////////////////

// F operator

 int NAL_MTL_F(int row_index , int ap_index)

        {

          Finally_SEG =   (data[row_index] [ap_index ]);
          return (Finally);

            }


/////////////////////////////////////////////////

// GF_operator

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


// Function for NAL_MTL_FG - the eventually forever operator.



   int NAL_MTL_FG(int start_time, int end_time, int ASR_ID)

        {
          FG_realise_percent =0;
          int aa =start_time;  // -1 since C arrays starts from 0 and time stamps are starts from 1
          int bb =end_time;
          int cc=bb-aa;
          int jj;

          printf("\n\n\n Eventually Forever of %d \n",ASR_ID);
          printf("Start time, End time %d,%d, \n ",aa,bb);

          int* row_data_3 = get_data_ASR_arr(ASR_ID);



    for (int i = bb; i >= aa; i--)   // for loop i

    {
        if (row_data_3[i] == 0)
            {
            jj = i;  // If value seen as 0, break the for loop(i) and set jj equal to index i,i.e from here till b all values are 1.
            break;
        }
    }


    for (int t = aa; t < jj; t++)

    {
        if (row_data_3[t] == 0 && row_data_3[t] == 1)  // Check for 0 or 1 in range [a, j),other symbol like 'x' is an error or missing value/data
            {
            printf("Start time, End time %d,%d, invalid data found at %d, exiting NAL_FG evaluation \n ",aa,bb,row_data_3[t]);
            return 1; // Formula does not hold
        }
    }

   FG_realise_percent =(((bb-jj)/cc) * 100); // how soon property FG holds i.e the eventualy forever happens - how quickly,
                                             //so if jj occurs early near  'aa' then it is a better percentage at which the property holds.
                                             // the FG realise % output produced for success of property FG.

    return 0; // here checks pass, FG formula holds
}





//////////////////////


// Function for NAL_MTL_Until - the until operator.

       // format  p U[a,b] q

   int NAL_MTL_U(int start_time, int end_time,int p,int q, int ASR_ID)

        {
          U_realise_percent =0;
          int a =start_time;  // -1 since C arrays starts from 0 and time stamps are starts from 1
          int b =end_time;
          int c=b-a;
          int j=0;
          int found_j;


          printf("\n\n\n p Until q of %d \n",ASR_ID);
          printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_4 = get_data_ASR_arr(ASR_ID);

        found_j =0;

                     if ( (row_data_4[a] == p) && (row_data_4[a] == q) )

            {
                found_j =1;
                U_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true
                return 1;
           }




        for (j = a; j < b; j++)   // for loop j

            {

             if ( (row_data_4[j] == p) && (row_data_4[j+1] != q) )

            {
                found_j =0;


             }

        if ( (row_data_4[j] == p) && (row_data_4[j+1] == q) )

                {
                    found_j =1;
                    break;

                    }

                    if ( (found_j == 0) && (row_data_4[j] != p) )

                {
                    found_j =0;
                    break;

                    }



             /* both p=q=1 @j, this is allowed in LTL so allowing here also
             this i       if ( (row_data_4[j+1] == p) && (row_data_4[j+1] == q) )

                {
                    found_j =0;
                    break;

                    }

                    */

       } // for loop for j ends here



            U_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true

            return found_j;

    }  // until operator ends





//////////////////////////////



// Function for NAL_MTL_Strong Until - the until operator.

       // format  p U[a,b] q

   int NAL_MTL_S(int start_time, int end_time,int p,int q, int ASR_ID)

        {
          S_realise_percent =0;
          int a =start_time;  // -1 since C arrays starts from 0 and time stamps are starts from 1
          int b =end_time;
          int c=b-a;
          int j=0;
          int found_j;
          int after_found_j;


          printf("\n\n\n p Until q of %d \n",ASR_ID);
          printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_4 = get_data_ASR_arr(ASR_ID);

        found_j =0;

                     if ( (row_data_4[a] == p) && (row_data_4[a] == q) )

            {
                found_j =1;
                S_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true
                return 1;
           }


        after_found_j=0;

        for (j = a; j < b; j++)   // for loop j

            {

                if (after_found_j == 0)

                {



             if ( (row_data_4[j] == p) && (row_data_4[j+1] != q) )

            {
                found_j =0;


             }

        if ( (row_data_4[j] == p) && (row_data_4[j+1] == q) )

                {
                    found_j =1;
                    j=j+1;


                    }

                }
                    if (  ( (found_j ==1) && (row_data_4[j] != p) && (row_data_4[j] == q) )!= 1 )

                {
                    after_found_j =1;

                    break;


                    }





             /* both p=q=1 @j, this is allowed in LTL so allowing here also

                    if ( (row_data_4[j+1] == p) && (row_data_4[j+1] == q) )

                {
                    found_j =0;
                    break;

                    }

                    */


       } // for loop for j ends here


            S_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true

            return found_j;

    }  // strong until operator ends



///////////////////////////////


// Function for NAL_MTL_Weak_Until - the weak until operator.

       // format  p W[a,b] q

   int NAL_MTL_W(int start_time, int end_time,int p,int q, int ASR_ID)

        {
          W_realise_percent =0;
          int a =start_time;  //
          int b =end_time;
          int c=b-a;
          int j=0;
          int found_j;
          int W_result;
          int no_p=1;
          int all_p;

          printf("\n\n\n p Until q of %d \n",ASR_ID);
          printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_5 = get_data_ASR_arr(ASR_ID);

        found_j =0;
        all_p =0;

                     if ( (row_data_5[a] == p) && (row_data_5[a] == q) )

            {
                found_j =1;
                U_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true
                return 1;
           }




        for (j = a; j < b; j++)   // for loop j

            {

             if ( (row_data_5[j] == p) && (row_data_5[j+1] != q) )

            {
                found_j =0;


             }

        if ((row_data_5[j] == p) && (row_data_5[j+1] == q))

                {
                    found_j =1;
                    break;

                    }

        if ( (row_data_5[j] == p) && (row_data_5[j+1] != p)  ) // all positions p holding true  will make the formula evaluate to true

                {
                    all_p =0;

                    }


                    if ( (found_j == 0) && (row_data_5[j] != p) )

                {
                    no_p =0;
                    break;

                    }



             /* both p=q=1 @j, this is allowed in LTL so allowing here also
             this i       if ( (row_data_4[j+1] == p) && (row_data_4[j+1] == q) )

                {
                    found_j =0;
                    break;

                    }

                    */

       } // for loop for j ends here



            W_realise_percent =(((j-a)/c) * 100); // how soon Until operator becomes true

            W_result = ( (found_j || all_p)  && (no_p) ) ;

            return W_result;

    }  // weak until operator ends

/////////////////////////////////

// Now operator – N [a,b]

int NAL_MTL_Now(int start_time, int end_time,int p, int AP_ASR_ID)

        {


          if (AP_ASR_ID == 1) // AP is the argument
          {
          Now =  (data [row_index] [start_time]);
          }

          if (AP_ASR_ID == 0) // SEG is the argument
          {
          Now =  (data_1 [row_index] [start_time]);
          }

          return (Now);


    }  // NOW operator ends


///////////////////////////////

/////////////////////////////////

// End operator – E [a,b]

int NAL_MTL_End(int start_time, int end_time,int p, int AP_ASR_ID)

        {

          if (AP_ASR_ID == 1) // AP is the argument
          {
          End =  (data [row_index] [end_time]);
          }

          if (AP_ASR_ID == 0) // SEG is the argument
          {
          End =  (data_1 [row_index] [end_time]);
          }

          return (End);

    }  // End operator ends


///////////////////////////////

/////////////////////////////////

// Next operator – N [a,b]

int NAL_MTL_Next(int start_time, int end_time,int p,int x_count, int AP_ASR_ID)

        {
          int a =start_time;  //
          int b =end_time;

          printf("\n\n\n Next operator %d \n",AP_ASR_ID);
          printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_9 = get_data_ASR_arr(AP_ASR_ID);

          if  (row_data_9[a+x_count] == p)

            {
                return 1;
            }

            else
            {

                return 0;

            }

    }  // Next operator ends


///////////////////////////////


// Before operator – Y [a,b]

int NAL_MTL_Before(int start_time, int end_time,int p,int y_count, int ASR_ID)

        {
          int a =start_time;  //
          int b =end_time;

          printf("\n\n\n Before operator %d \n",ASR_ID);
          printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_10 = get_data_ASR_arr(ASR_ID);

          if  (row_data_10[b-y_count] == p)

            {
                return 1;
            }

            else
            {

                return 0;

            }

    }  // before operator ends

///////////////////////////////

//////////////////////////////

int read_AP_usetimestamp (int ap_index,int timestamp) //use full row to get AP value

        {

           int ap;

          printf("\n reading AP");


          int* row_data_ap_row = get_data_by_timestamp(timestamp);


          ap = row_data_ap_row [ap_index];


          return ap;

        }
/////////////////////////

int read_AP (int ap_index) // caution  - this function to be used, only when the row data is read and available for this function.

        {

        //   int ap;


         // ap = ap_row_data [ap_index];  //  here ap_row_data is a array of APs (1 or 0) at a particular time stamp.


          return ap_row_data [ap_index];

        }



/////////////////////////////////

// Verdict operator – N [a,b]

int NAL_MTL_Verdict_2(int logic_operator, int i,int j, int ASR_ID_i,int ASR_ID_j)

        {

            int v;
            int p,q;

          printf("\n\n\n On after delay ");
         // printf("Start time, End time %d,%d, \n ",a,b);

          int* row_data_11 = get_data_ASR_arr(ASR_ID_i);
          int* row_data_12 = get_data_ASR_arr(ASR_ID_j);

          p=row_data_11 [i];
          q=row_data_12 [j];


          switch (logic_operator)

          {
        case 1:     // If input is 1, check if p is equal to 1 ; also used to read the atomic proposition as a 'bit' from the array, here q is irrelevant

            if (p == 1)
            {


                printf("p == 1\n");
                v=1;
            }

            else if (p == 0)
            {

             printf("p != 1\n");
                v=0;
            }
            break;
        case 0:
            // If input is 0, check if p is equal to 0
            if (p == 0)
                {
                printf("p == 0\n");
                v=1;
                }
            else if (p == 1)
                {
                printf("p != 0\n");
                v=0;
                }

            break;
        case 3:
            // If input is 3, perform logical AND operation on p and q
            v = p && q;
            printf("p && q = %d\n", v);
            break;
        case 4:
            // If input is 4, perform logical OR operation on p and q
            v = p || q;
            printf("p || q = %d\n", v);
            break;
        case 5:
            // If input is 5, perform logical NOT operation on p
            v = !p;
            printf("!p = %d\n", v);
            break;
        case 6:
            // If input is 6, perform logical implication operation on p and q
            v = (!p) || q;
            printf("(!p) || q = %d\n", v);
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

            printf("(p iff q) = %d\n", v);
            break;
        case 9:
            // If input is 9, perform logical XOR operation on p and q
            v = p ^ q;
            printf("p ^ q = %d\n", v);
            break;
        default:
            // If input is not valid, print error message
            v=3;
            printf("Invalid logic request input!\n");

            return v;
    }



    }  // Verdict operator_2 ends


/////////////////////////////////////////


int NAL_MTL_Verdict (int logic_operator, int i,int j)   // use this function to process the APs directly.

        {


         int  p=ap_row_data [i];
         int q=ap_row_data [j];
         int v;

          switch (logic_operator)

          {
        case 1:     // If input is 1, check if p is equal to 1 ; also used to read the atomic proposition as a 'bit' from the array, here q is irrelevant

            if (p == 1)
            {


                printf("p == 1\n");
                v=1;
            }

            else if (p == 0)
            {

             printf("p != 1\n");
                v=0;
            }
            break;
        case 2:
            // If input is 0, check if p is equal to 0
            if (p == 0)
                {
                printf("p == 0\n");
                v=1;
                }
            else if (p == 1)
                {
                printf("p != 0\n");
                v=0;
                }

            break;
        case 3:
            // If input is 3, perform logical AND operation on p and q
            v = p && q;
            printf("p && q = %d\n", v);
            break;
        case 4:
            // If input is 4, perform logical OR operation on p and q
            v = p || q;
            printf("p || q = %d\n", v);
            break;
        case 5:
            // If input is 5, perform logical NOT operation on p
            v = !p;
            printf("!p = %d\n", v);
            break;
        case 6:
            // If input is 6, perform logical implication operation on p and q
            v = (!p) || q;
            printf("(!p) || q = %d\n", v);
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

            printf("(p iff q) = %d\n", v);
            break;
        case 9:
            // If input is 9, perform logical XOR operation on p and q
            v = p ^ q;
            printf("p ^ q = %d\n", v);
            break;
        default:
            // If input is not valid, print error message
            v=3;
            printf("Invalid logic request input!\n");

            return v;
    }



    }  // Verdict operator ends


/////////////////////////////////////////




//////////////////////////////////////////

// ON_after delay operator – V[ i ] Pi  !SR<t V[ j ] Pj


int NAL_MTL_ON_after_delay(int a, int t, int i,int j,int ON_after_delay_global_1,int stimulas_global_1, int ASR_ID_i,int ASR_ID_j) // t is upper time limit, a start time, i,j are APs index,ASD ids are ref to the data arrays...

        {

            int p,q;

          printf("\n\n\n ON_after delay operator   \n" );
         // printf("Start time, End time %d,%d, \n ",a,b);

          int* col_data_14 = get_data_ASR_arr(ASR_ID_i);
          int* col_data_15 = get_data_ASR_arr(ASR_ID_j);

          p=col_data_14 [i]; // takes current bool value of the AP-p for each update
          q=col_data_15 [j]; // takes current bool value of the AP-p for each update


 int limit=a+t;

     if (p == 0)  // first instance if stimulus is zero at specified time
     {
         ON_after_delay_global_1=0;
         stimulas_global_1=0;
         return 0;
     }

     else if  (p == 1)

     {
       stimulas_global_1=1;
     }


    if (    (stimulas_global_1==1) &&(curr_timestamp <= limit) && (ON_after_delay_global_1==1) )

        {

        if (q == 1)

            {
            // If q becomes 1 (true),
            printf("q became true within the time window.failure case\n");
            ON_after_delay_global_1=0;
             return 0;



        }

    }


    return 1; // return value does not have any importance, the result meaning is in the global variable ON_after_delay_global_1


}  // ON_after delay complete


////////////////////////////////////////////

/////////////////////////////////////////

// ON_within delay operator – V[ i ] Pi  SR<t V[ j ] Pj


int NAL_MTL_ON_within_delay(int a, int t, int i,int j,int ON_within_delay_global_2,int stimulas_global_2, int ASR_ID_i,int ASR_ID_j) // t is upper time limit, a start time, i,j are APs index,ASD ids are ref to the data arrays...

        {

            int p,q;

          printf("\n\n\n ON_within delay operator   \n" );
         // printf("Start time, End time %d,%d, \n ",a,b);

          int* col_data_16 = get_data_ASR_arr(ASR_ID_i);
          int* col_data_17 = get_data_ASR_arr(ASR_ID_j);

          p=col_data_16 [i]; // takes current bool value of the AP-p for each update
          q=col_data_17 [j]; // takes current bool value of the AP-p for each update


 int limit=a+t;

     if (p == 0)  // first instance, of stimulus if zero at specified time
     {
         ON_within_delay_global_2=0;
         stimulas_global_2=0;
         return 0;
     }

     else  if (p == 1)

     {
       stimulas_global_2=1;
     }


    while ( (stimulas_global_2==1) &&(curr_timestamp <= limit) )

        {

        if (q == 1)

            {

            ON_within_delay_global_2=1;
            break;
            return 1;
            // return 0;
        }

        else if (q ==0)
        {
        ON_within_delay_global_2=0;
        }

    }


    return 0; // return value does not have any importance, the result meaning is in the global variable ON_within_delay_global_2


}  // ON_within delay complete


////////////////////////////////////////////

/////////////////////////////////////////

// ON_exact_delay operator – V[ i ] Pi  SRt V[ j ] Pj


int NAL_MTL_ON_exact_delay(int a, int t, int i,int j,int ON_exact_delay_global_3,int stimulas_global_3, int ASR_ID_i,int ASR_ID_j) // t is upper time limit, a start time, i,j are APs index,ASD ids are ref to the data arrays...

        {

            int p,q;

          printf("\n\n\n ON_exact delay operator   \n" );
         // printf("Start time, End time %d,%d, \n ",a,b);

          int* col_data_18 = get_data_ASR_arr(ASR_ID_i);
          int* col_data_19 = get_data_ASR_arr(ASR_ID_j);

          p=col_data_18 [i]; // takes current bool value of the AP-p for each update
          q=col_data_19 [j]; // takes current bool value of the AP-p for each update


 int limit=a+t;

     if (p == 0)  // first instance, of stimulus if zero at specified time
     {
         ON_exact_delay_global_3=0;
         stimulas_global_3=0;
         return 0;
     }
if (p == 1)

     {
       stimulas_global_3=1;
     }


   while ( (stimulas_global_3==1) &&(curr_timestamp == limit) )

        {

        if (q == 1)

            {

            ON_exact_delay_global_3=1;
            break;
            return 1;
            // return 0;
        }

        else if (q ==0)
        {
        ON_exact_delay_global_3=0;
        break;
        return 0;
        }

    }


    return 0; // return value does not have any importance, the result meaning is in the global variable ON_within_delay_global_2


}  // ON_ON_exact_delay complete


////////////////////////////////////////////
int segment_1 (void)

{
if ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1) )

{
data_1[1][1] = NAL_MTL_Verdict (3,1,5);
			}
return  (data_1[1][1]);
}

///////////////////////////////////////////
int segment_2 (void)

{

if  ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1)  && (data_1[1][2] ))

{
data_1[1][2]= data_1[1][2] && NAL_MTL_G_SEG (1, 1);
			}
return (data_1[1][2]);

}

//////////////////////////////////////////

int segment_3 (void)

{

if  ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1) )

{
data_1[1][3] = read_AP (9);
		}
return (data_1[1][3]);
}

/////////////////////////////////////////

int segment_4 (void)

{

if  ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1)  && (data_1[1][4] ))

{
data_1[1][4]= data_1[1][4] && NAL_MTL_G_SEG (1, 3);
		}
return (data_1[1][4]);
}

//////////////////////////////////////
int segment_5 (void)

{

if ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1) )

{
data_1[1][5] = NAL_MTL_Verdict (3,2,4);
		}
		return (data_1[1][5]);
		}

///////////////////////////////////

int segment_6 (void)

{

if ( (timestamp >= TS_L_2)  &&  (timestamp <= TS_H_2) )

{
data_1[1][6] = NAL_MTL_Verdict (3,7,1);
		}
return (data_1[1][6]);
		}
////////////////////////////////////////
 int segment_7 (void)

{

if  ( (timestamp >= TS_L_2)  &&  (timestamp <= TS_H_2)  && (data_1[1][7] ==0 ) )

{
data_1[1][7] =  data_1[1][7] && NAL_MTL_F_SEG (1, 7);
		}
return (data_1[1][7]);
}

////////////////////////////////////////
int segment_8 (void)

{

if ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_1) )

{
data_1[1][8] = NAL_MTL_Verdict (6,5,7);
		}
return data_1[1][8];
		}

/////////////////////////////////////////
////// main function goes here.


int main()

{
    char filename[] = "d2.txt";
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

    // Print the entire array
    printf("Data in the csv text file:\n");
    for (i = 0; i < NUM_COLS; i++)
        {
        printf("%s,", headers[i]);
    }
    printf("\n\n\n\n");
    for (i = 0; i < row; i++) {
        for (j = 0; j < NUM_COLS; j++)
        {
            printf("%d,", data[i][j]);
        }
        printf("\n");
    }

    // Get user input for timestamp
    printf("Enter time stamp: ");

    int timestamp;
    scanf("%d", &timestamp);

    // Get the row of data corresponding to the timestamp
    int* row_data = get_data_by_timestamp(timestamp);
    if (row_data != NULL) {

    ts=row_data[0];
    P1=row_data[1];
    P2=row_data[2];
    P3=row_data[3];
    P4=row_data[4];
    P5=row_data[5];


    printf("by value : TIME = %d, P1 = %d, P2 = %d, P3 = %d, P4 = %d, P5 = %d\n",ts,P1, P2, P3, P4, P5);
    printf(" \n");
    printf(" %d", P1);
    printf(" %d ", P2);



        printf(" by array TIME = %d, P1 = %d, P2 = %d, P3 = %d, P4 = %d, P5 = %d\n",
            row_data[0], row_data[1], row_data[2], row_data[3], row_data[4], row_data[5]);
      //  printf("P5 * P4 = %d\n", row_data[5] * row_data[4]);





     //////////////


    }




if ( (timestamp >= TS_L_1)  &&  (timestamp <= TS_H_2) )

{

       call =  segment_1();
       call =  segment_2();
       call =  segment_3();
       call =  segment_4();
       call =  segment_5();
       call =  segment_8();

}

if ( (timestamp >= TS_L_2)  &&  (timestamp <= TS_H_2) )

{

       call =  segment_6 ();
       call =  segment_7 ();
}


        printf ("Evaluated result for formula Index : %d , is %d  \n", formula_id,(data_1[1][8]) );









    return 0;
}


// G[10,25] ( ((G[15,25](P1&P5)) & (G[15,25] (P9|P7))) -> (F[20,25](P7&P1) ) )


