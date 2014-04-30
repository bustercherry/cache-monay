typedef struct output_t output_t;

struct output_t
{
  cache_t *L1d, *L1i, *L2;
  char *configName;
  unsigned long long numRead, numWrite, numInst, numRef;
  unsigned long long numReadCycles, numWriteCycles, numInstCycles;
  unsigned long long totalTime;
};

output_t init_output(cache_t*, cache_t*, cache_t*, char*);
void print_cache(cache_t*);
void print_output(output_t);
int print_cost(cache_t*, int);

