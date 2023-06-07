
# RTRV_MTL_ONLINE
Functions for Online monitoring of RTRV_MTL

The monitoring software tool is a Time triggered, Unobtrusive system, which monitors an unaugmented code (of the system under monitor).
The monitor consumes atomic propositions for every time trigger received and maintains a minimum memory overhead to store only few bits/bytes (in arrays) of information for continuty in processing.

'ABSOLUTE TIME LIMITS'*  based Metric Temporal Logic can be used to draft specifications.

The current implementation in C is run on PC, for embedded system's the code will be revamped to optimise the memory use by bit manupulation to update assertions, thus each byte can store upto 8 bit of information, currently for each AP, one 'unsiged int' is used in the array.

Also specific to the target device the size of 'int' may vary (Example:Atmega 328 - int is 2 byte and in SAM3X8E boards and other 32 bit devices typically int or unsiged int takes 32 bits).



P Thirumeni
203085002 - IITDH



+++++++++++++++++++++++++++++++++++++++

* - Time reference from start of the mission (t0),also the evaluation is not recurring, this is in contrast to the classical MTL time reference of 'from now'.  For recurring continuous evalaution either multiple formula with continuing/different time window or use of the MTL_RAW operator functions. 

