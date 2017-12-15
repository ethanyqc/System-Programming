/* THIS CODE IS MY OWN WORK. IT WAS WRITTEN WITHOUT CONSULTING
 * A TUTOR OR CODE WRITTEN BY OTHER STUDENTS -YCHE498*/
#define GETINDEX (long)2
#define PRFCT (long)3
#define SENDINDEX (long)4
#define MEMKEY (key_t)40110	/*key for shared memory segment, can't be destoryed */
#define MSGKEY (key_t)23232
#define SIZE 1048576
//define process struct
typedef struct _process{
  pid_t pid;		// process id
  int perfectCt;		// count of perfect numbers found
  int testCt;	// count of numbers tested
  int skipCt;	// count of numbers skipped
} process;

//define shared memory segment
typedef struct _sharemem{
  int bits[SIZE];//bits[], total of 32 segs
  int prfctNms[20];
  process prc[20];
  pid_t mPid;
} shared;

//define msg structure
typedef struct _msg{
  long type;
  int num;
} message;

//define macros
#define   set(A,k)       ( A[(k/32)] |= (1 << (k%32)) )
#define   check(A,k)     (( A[(k/32)] & (1 << (k%32))) != 0 )
