/*****************************************************************************
/* This program can 
/* 1. generate m out of n combinations
/* 2. For all the combinations, check how many of them can route through
/*    a specified matrix using matching algorithm.
/* 3. Generate a connection matrix which can route all combinations.
/*    Each source needs to connect to one source as initial input.
/*
/* command line invocation:
/*
/*     pgmname -c source destination
/*         to list all combinations
/*     pgmname -m filename 
/* 	   to route all combinations and report combinations not routed
/*     pgmname -m -s filename
/* 	   to route all combinations and report statistics only
/*     pgmname -g filename\n");
/* 	   to generate a 100 per cent routing matrix
/*
/*****************************************************************************/


#include <stdio.h>
#include <string.h>
#define   CONNECTED    1
#define   NOT_USED    -1
#define   SUCCESS      1
#define   FAIL         0


int tot_src, tot_dest;   /* number of source and destination */
                         /* command line input */
 
int **conn_tbl;          /* connection table, two dimentional array */
 
int *curr_dest_used;     /* contains how the current destination is used*/
int *augment_dest_used;  /* augment destination assignment, a work place*/

int *src_out_count;      /* pmatch: count number of output for each source */
int tot_conn;            /* pmatch: total number of connections */
int low_cost_src;        /* pmatch, src node with the lowest # of connection */


/**********************************************************************
/* print connection matrix
/*********************************************************************/
void print_connection_tbl() {
int i,j;

        printf("\n");
        printf("Connection Matrix with %d source and %d destination\n",tot_src,tot_dest);
 
        for(i=0; i < tot_src; i++) {
                for(j=0; j < tot_dest; j++)
                        printf("%2d", *(conn_tbl[i]+j));
                printf("\n");
        }
        printf("\n%d total connections\n",tot_conn);
        printf("\n\n");
}
 

/**********************************************************************
/* dest is an I/O variable, search start from *dest+1, next dest is    
/* stored in dest and returne 
/*********************************************************************/
int find_next_dest(int src, int *dest) {
        int j;
        for(j=(*dest)+1; j < tot_dest; j++)
                if(*(conn_tbl[src]+j) == CONNECTED) {
                        *dest = j;
                        return SUCCESS;
                }
 
        return FAIL;
}
 
 
/**********************************************************************
/* A recursive routine to build alternate path  
/*********************************************************************/
int build_alternate_path(int src) {
int i,dest = -1;

        /* record src with min number of connections for pmatch */
        if(src_out_count[src] < src_out_count[low_cost_src])
                low_cost_src = src;

	while(find_next_dest(src, &dest)) {
		if(augment_dest_used[dest]== NOT_USED) {
			augment_dest_used[dest] = src;
			if(curr_dest_used[dest] == NOT_USED) {
				for(i=0; i < tot_dest; i++) {
					augment_dest_used[i] = NOT_USED;
				}
				curr_dest_used[dest] = src;
				return SUCCESS;
			}
			if(build_alternate_path(curr_dest_used[dest])) {
				curr_dest_used[dest] = src;
				return SUCCESS;
			}
			else 
				augment_dest_used[dest] = NOT_USED;
		}
	} /* while */
	return FAIL;
}


/********************************************************************** 
/* go through each input element and build an alternate path for
/* each element. The input combination cannot be routed if any one 
/* invocation of build_alternate_path() failed.
/*********************************************************************/
int match(int comb_input[]) {
int i;
        for(i=0; i < tot_dest; i++)
                curr_dest_used[i] = augment_dest_used[i] = NOT_USED;

        for(i=0; i < tot_dest; i++)
                if(!build_alternate_path(comb_input[i]))  
                        return FAIL;
        return SUCCESS;

}


/********************************************************************** 
/* There is only one unused destination */
/*********************************************************************/
int find_unused_dest() {
int i, unused_dest;

        for(i=0; i < tot_dest; i++)
                if(curr_dest_used[i] == NOT_USED) {
                        unused_dest = i;
                        break;
                }

        if(i == tot_dest) {
                printf("Did not find unused destnation for construction new conn\n");
                exit(1);
        }

        return unused_dest;
}


/********************************************************************** 
/* connect low_cost_src with the unused destination, for pmatch only 
/*********************************************************************/
void make_new_connection(int comb_input[]) {
int i,dest;
 
        dest = find_unused_dest();
 
        if(*(conn_tbl[low_cost_src]+dest) == CONNECTED) {
                printf("Internal error: connection already exist\n");
                exit(1);
        }
 
        *(conn_tbl[low_cost_src]+dest) = CONNECTED;

        ++src_out_count[low_cost_src];
        ++tot_conn;
}



/********************************************************************** 
/* a match routine used to generate 100% match (pmatch)
/*********************************************************************/
int pmatch(int comb_input[]) {
int i;
int status = SUCCESS;

        for(i=0; i < tot_dest; i++)
                curr_dest_used[i] = augment_dest_used[i] = NOT_USED;

        for(i=0; i < tot_dest; i++)   {
                low_cost_src = comb_input[i];    /* for pmatch */
                if(!build_alternate_path(comb_input[i]))
                        status = FAIL;
        }

        if(status == FAIL)
                make_new_connection(comb_input);
}


/**********************************************************************
/* process input arguments and allocate space for tables
/*********************************************************************/
void process_input_and_setup(int argc, char *argv[], int *do_comb, int *do_match,
		int *do_generate, int *summary_only, int **buf1, int **buf2)
{
int i,j,c,temp;
FILE *fp;

	if(argc < 2) {   
            printf("Enter one of the following valid inputs:\n\n");
            printf("pgmname -c source destination\n"); 
	    printf("    to list all combinations\n");
            printf("pgmname -m filename \n");
	    printf("    to route all combinations and report combinations not routed\n");
            printf("pgmname -m -s filename\n");
	    printf("    to route all combinations and report statistics only\n");
            printf("pgmname -g filename\n");
	    printf("    to generate a 100 per cent routing matrix\n\n");
            exit(1); 
	}

	/* read all arguments with - in front */
	while(--argc > 0  && ((*++argv)[0] == '-'))
		while( c = *(++argv[0]))
			switch (c) {
				case 'c':
					*do_comb = 1;
					break;
				case 'm':
					*do_match = 1;
					break;
				case 's': 
                                        *summary_only = 1;
                                        break; 
				case 'g': 
					*do_generate = 1;
                                        break; 
			}

	if(*do_comb) {
		if (argc != 2) {
                        printf("Invalid input, need source and destination\n\n");
                        exit(1); 
		}

        	tot_src = atoi(*argv++); 
        	tot_dest = atoi(*argv++); 

		if(tot_dest > tot_src) {  /* make sure dest <= src */
			temp = tot_src;
			tot_src = tot_dest;
			tot_dest = temp;	
		}
        }

	if(*do_match || *do_generate) {
        	if(argc != 1) {
               		printf("Invalid input, need input filename\n\n");
                	exit(1);
        	}

        	if((fp = fopen(*argv, "r")) == NULL) {
                	printf("Cannot open file %s\n",*argv);
                	exit(1);
        	}
 
        	fscanf(fp, "%d", &tot_src);
        	fscanf(fp, "%d", &tot_dest);
 
                if(tot_dest > tot_src) {  /* make sure dest <= src */
                        temp = tot_src;
                        tot_src = tot_dest;
                        tot_dest = temp;
                }

        	/* allocate space to set up connection table */
        	conn_tbl = (int **) calloc(tot_src, sizeof(int));
        	for(i=0; i<tot_src; i++)
                	conn_tbl[i] = (int *) calloc(tot_dest, sizeof(int));
 
                /* allocate space for the work array */
                curr_dest_used = (int *) calloc(tot_dest, sizeof(int));
                augment_dest_used = (int *) calloc(tot_dest, sizeof(int));

		/* allocate space to count number of connections for each src node */
		tot_conn = 0;
                src_out_count = (int *) calloc(tot_src, sizeof(int));
		for(i=0; i < tot_src; i++)
			src_out_count[i] = 0;


        	/* read data into the connection table */
		if(*do_match) {
        		for(i=0; i< tot_src; i++)
               			for(j=0; j<tot_dest; j++)
                       			fscanf(fp, "%d", (conn_tbl[i]+j));
		}
		else {  /* to generate matrix for perfect match, init count tbl */
                        for(i=0; i< tot_src; i++)
                                for(j=0; j<tot_dest; j++) {
                                        fscanf(fp, "%d", (conn_tbl[i]+j));
					if(*(conn_tbl[i]+j) == CONNECTED) {
        					++src_out_count[i];
						++tot_conn;   
					}
				}	
		}
 
	}


	*buf1 = (int *) calloc(tot_dest, sizeof(int));
	*buf2 = (int *) calloc(tot_dest, sizeof(int));

}


/**********************************************************************
/* initialization for upper bound array and buffer contains combination
/* the comb_buf does not contain the first combination
/*********************************************************************/
void init_for_combination(int comb_buf[], int up_bound[]) {
int i;

/* init buffer with first valid combination */
for(i=0; i < tot_dest; i++)
        comb_buf[i] = i;

--comb_buf[tot_dest-1];  /* decrement the last element by one */
 

/* set up upper bound array */
up_bound[tot_dest-1] = tot_src - 1;
for(i=tot_dest - 2; i >= 0; i--)
        up_bound[i] = up_bound[i+1] - 1;
 
}


/**********************************************************************
/*********************************************************************/
void print_this_combination(int comb_buf[]) {
int i;

	for(i=0; i<tot_dest; i++)
		printf(" %5d",comb_buf[i]);
	printf("\n");

}



/********************************************************************** 
/* This routine generates all m out of n combinations one by one
/* and place each generated comb in comb_buf[]
/* It will return FAIL when all combinations are generated
/*********************************************************************/ 
int get_next_combination(int comb_buf[], int up_bound[]) {
int i; 
        /* start with last element in buffer, increment by 1,
           and check if in valid range */
        for(i = tot_dest-1; i >= 0; i--)
                if(++comb_buf[i] <= up_bound[i]) break;
 
        if(i < 0) return FAIL;    /* all combinations are generated */
         
        /* adjust value in buffer. No adjustment for i = m-1 */
        for(i = i+1; i < tot_dest; i++)
                comb_buf[i] = comb_buf[i-1] + 1;

	return SUCCESS;
}


/**********************************************************************
/* main control loop
/* based on the options set, execute different part of the code
/*********************************************************************/
void process_combinations(int do_comb, int do_match, int do_generate, int summary_only,
                  int *total_cnt, int *unmatch_cnt, 
                  int comb_buf[], int up_bound[])
{
        init_for_combination(comb_buf, up_bound); 
 
        while(get_next_combination(comb_buf, up_bound)) {
                if(do_comb)
                        print_this_combination(comb_buf);
                          
                if(do_match)
                        if(!match(comb_buf)) {  /* cannot route this combination */
                                ++(*unmatch_cnt);
                                if(!summary_only)  print_this_combination(comb_buf);
                        }
 
                if(do_generate)
                        pmatch(comb_buf);
 
                ++(*total_cnt);             
        }
 
} 


/**********************************************************************
/* main routine
/*********************************************************************/

main(int argc, char *argv[])
{
 
int i, do_comb = 0, do_match = 0, do_generate = 0, summary_only = 0;
FILE *fp;

int   *comb_buf,   /* store the combination */
      *up_bound;   /* upper bound for the combination */

int total_cnt = 0,    /* total number of combinations */
    unmatch_cnt = 0;  /* total number of unmatched comb */


	process_input_and_setup(argc, argv, &do_comb, &do_match, &do_generate,
			        &summary_only, &comb_buf,&up_bound);


	process_combinations(do_comb, do_match, do_generate,  summary_only,
			     &total_cnt, &unmatch_cnt, comb_buf, up_bound);


	/* check the matching percentage for generated connection matrix */
	if(do_generate) { 
		print_connection_tbl();
		do_generate = 0;
		do_match = 1;
		total_cnt = 0;
		unmatch_cnt = 0;
        	process_combinations(do_comb, do_match, do_generate,  summary_only,
                             &total_cnt, &unmatch_cnt, comb_buf, up_bound);
	} 
	
 
	printf("\n\n");
	printf(" %d total combinations (Choose %d out of %d elements)\n",total_cnt,tot_dest,tot_src);
	if(!do_comb) {
		printf(" %d combinations not routed\n",unmatch_cnt);
		printf(" %5.2f percent of the inputs are routed\n", ((float)(total_cnt - unmatch_cnt))/total_cnt*100.00); 
	}
	printf(	"\n\n");
}
