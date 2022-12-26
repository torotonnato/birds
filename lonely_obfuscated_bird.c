#include <math.h>
main(f,i,s,o){for (i=o=0;;putchar(s=o>0?127*sin(.43*i-750*cos(142e-6*f*i)/f):0))if(o*o>1)o>1?o--:o++,i++;else o=o>0?-rand()%20000-8000:rand()%10000+2000,f=rand()%10+8;}
