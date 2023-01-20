#include <math.h>
main(b,i,r,d){for(i=d=0;;putchar(r=d>0?127*sin(.43*i-750*cos(142e-6*b*i)/b):0))if(d*d>1)d>1?d--:d++,i++;else d=d>0?-rand()%20000-8e3:rand()%10000+2e3,b=rand()%10+8;}
