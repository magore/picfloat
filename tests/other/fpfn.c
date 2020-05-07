
/* fp_quad
 *      Internal routine to split an angle into
 *      a given interval as an integer and fraction part
*/
int FPquad,FPflag;
void fp_quad(double *xp)
{
    FPflag = 0;

	if(*xp < 0.0) {
        FPflag = 0x80;       // Mark Sign
		*xp = -*xp;
	}
	FPquad = *xp;
	*xp = *xp - (double) FPquad;
    FPquad &= 7;
}

double sin_tab[] = {
	5.4589878320e-08 ,
	-3.5955981533e-06 ,
	1.6043892991e-04 ,
	-4.6817533261e-03 ,
	7.9692626107e-02 ,
	-6.4596409750e-01 ,
	1.5707963268e+00 
};
//
// SIN(X) = z*P(z**2), z = x * SIN_SCALE
//
//
#define SIN_SCALE (2.0/M_PI)
double  fp_sincos(double xp, int cos)
{
	double p;
	double *ptr;
	int i;
//
// SIN
// 0	90   	90+		180- 	180		270-  	270  	360-
// 0	1		1		0		0		-1		-1	  	0
//

	xp *= SIN_SCALE;
	
	fp_quad((double *)&xp);	
	if(cos) {
		if(FPflag & 0x80)
			FPquad -= 1;
		else
			FPquad += 1;
	}
	if(FPquad & 1) 
		xp = 1.0-xp;
	if(FPquad & 2) 
		FPflag ^= 0x80;
	ptr = sin_tab;
			
	p = 0;
	for(i=0;i<6;++i) {
		p += *ptr++;
		p *= xp;
		p *= xp;
	}
	p += *ptr++;
	p *= xp;
	xp = fabs(p);

	if(FPflag & 0x80)
		xp = -xp;
	
	return(xp);
}	
