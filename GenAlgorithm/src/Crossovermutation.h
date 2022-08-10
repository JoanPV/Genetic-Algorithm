

void OnePointCrossover(unsigned long p1, unsigned long p2, unsigned long *f1, unsigned long *f2, int len){
    /* d \in [1, 8*sizeof(unsigned int)-1] */
    unsigned char d = uniform()*(len-1) + 1;
    /* d 0's at the beginning and (8*sizeof(unsigned int) - d) 1's at the end */
    unsigned long int mask = 0xFFFFFFFFFFFFFFFFU << d;
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void TwoPointCrossover(unsigned long p1, unsigned long p2, unsigned long *f1, unsigned long *f2, int len){
    unsigned char p = uniform()*(len-2) + 1; /* p \in [1, len-2] */
    unsigned long mask = (0xFFFFFFFFFFFFFFFFU >> (((unsigned char) uniform()*(len-p-1)) + p+1)) << p;
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void UniformCrossover(unsigned long p1, unsigned long p2, unsigned long *f1, unsigned long *f2, double prob, int len){
    unsigned long int mask = 0UL;
    register unsigned char i;
    for(i = 0 ; i < len ; i++){
    	if(uniform() < prob) {
    		mask = mask | (1UL << i);
    	}
    }
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void mutation1(unsigned long *f, double prob){
    if(uniform() < prob) *f = (*f)^(1UL << ((unsigned char) uniform()*8*sizeof(*f)));
}

void BitFlipMutation(unsigned long *f, double prob, int len){
    register unsigned char i;
    for(i = 0 ; i < len ; i++){
    	if(uniform() < prob) *f = (*f)^(1UL << i);
    }
}
