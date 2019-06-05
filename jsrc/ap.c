/* Copyright 1990-2011, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Prefix and Infix                                               */

#include "j.h"
#include "vasm.h"
#include "ve.h"


#define ZZDEFN
#include "result.h"


#define MINUSPA(b,r,u,v)  r=b?u-v:u+v;
#define MINUSPZ(b,r,u,v)  if(b)r=zminus(u,v); else r=zplus(u,v);
#define MINUSPX(b,r,u,v)  if(b)r=xminus(u,v); else r=xplus(u,v);
#define MINUSPQ(b,r,u,v)  if(b)r=qminus(u,v); else r=qplus(u,v);

#define DIVPA(b,r,u,v)    r=b?(DIV(u,(D)v)):TYMES(u,v);
#define DIVPZ(b,r,u,v)    if(b)r=zdiv(u,v); else r=ztymes(u,v);

// Don't RESTRICT y since function may be called inplace
#if 1
#define PREFIXPFX(f,Tz,Tx,pfx,vecfn)  \
 AHDRP(f,Tz,Tx){I i;Tz v;if(d*m*n==0)SEGFAULT; /* scaf */                                    \
  if(d==1)DQ(m, *z++=v=    *x++; DQ(n-1, *z=v=pfx(v,*x); ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                              \
   DO(d, z[i]=    x[i];); x+=d;                                        \
   DQ(n-1, vecfn(jt,d,z+d,z,x,1); z+=d; x+=d; ); z+=d;                     \
 }}}  /* for associative functions only */

#define PREFIXNAN(f,Tz,Tx,pfx,vecfn)  \
 AHDRP(f,Tz,Tx){I i;Tz v;if(d*m*n==0)SEGFAULT; /* scaf */                                    \
  NAN0;                                                               \
  if(d==1)DQ(m, *z++=v=    *x++; DQ(n-1, *z=v=pfx(v,*x); ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                              \
   MC(z,x,d*sizeof(Tx)); x+=d;                                        \
   DQ(n-1, vecfn(jt,d,z+d,z,x,1); z+=d; x+=d; ); z+=d;                     \
  }}                                                                   \
  NAN1V;                                                              \
 }   /* for associative functions only */
#else // obsolete
#define PREFIXPFX(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){I i;Tz v,* y;                                    \
  if(d==1)DO(m, *z++=v=    *x++; DO(n-1, *z=v=pfx(v,*x); ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *z++=    *x++;);                                        \
   DO(n-1, DO(d, *z=pfx(*y,*x); ++z; ++x; ++y;));                     \
 }}}  /* for associative functions only */

#define PREFIXNAN(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){I i;Tz v,* y;                                    \
  NAN0;                                                               \
  if(d==1)DO(m, *z++=v=    *x++; DO(n-1, *z=v=pfx(v,*x); ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *z++=    *x++;);                                        \
   DO(n-1, DO(d, *z=pfx(*y,*x); ++z; ++x; ++y;));                     \
  }}                                                                   \
  NAN1V;                                                              \
 }   /* for associative functions only */
#endif

#define PREFICPFX(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){I i;Tz v,* y;                                    \
  if(d==1)DO(m, *z++=v=(Tz)*x++; DO(n-1, *z=v=pfx(v,*x); ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *z++=(Tz)*x++;);                                        \
   DO(n-1, DO(d, *z=pfx(*y,*x); ++z; ++x; ++y;));                     \
 }}}  /* for associative functions only */

#define PREFIXALT(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){B b;I i;Tz v,* y;                                                 \
  if(d==1)DO(m, *z++=v=    *x++; b=0; DO(n-1, b=!b; pfx(b,*z,v,*x); v=*z; ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                                               \
   y=z; DO(d, *z++=    *x++;); b=0;                                                    \
   DO(n-1, b=!b; DO(d, pfx(b,*z,*y,*x); ++z; ++x; ++y;));                              \
 }}}

#define PREALTNAN(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){B b;I i;Tz v,* y;                                                 \
  NAN0;                                                                                \
  if(d==1)DO(m, *z++=v=    *x++; b=0; DO(n-1, b=!b; pfx(b,*z,v,*x); v=*z; ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                                               \
   y=z; DO(d, *z++=    *x++;); b=0;                                                    \
   DO(n-1, b=!b; DO(d, pfx(b,*z,*y,*x); ++z; ++x; ++y;));                              \
  }}                                                                                    \
  NAN1V;                                                                               \
 }

#define PREFICALT(f,Tz,Tx,pfx)  \
 AHDRP(f,Tz,Tx){B b;I i;Tz v,* y;                                                 \
  if(d==1)DO(m, *z++=v=(Tz)*x++; b=0; DO(n-1, b=!b; pfx(b,*z,v,*x); v=*z; ++z; ++x;))  \
  else{for(i=0;i<m;++i){                                                               \
   y=z; DO(d, *z++=(Tz)*x++;); b=0;                                                    \
   DO(n-1, b=!b; DO(d, pfx(b,*z,*y,*x); ++z; ++x; ++y;));                              \
 }}}

#define PREFIXOVF(f,Tz,Tx,fp1,fvv)  \
 AHDRP(f,I,I){C er=0;I i,*xx=x,* y,*zz=z;                      \
  if(d==1){                                                         \
   if(1==n)DO(m, *z++=*x++;)                                        \
   else {I c=d*n;    DO(m, fp1(n,z,x); RER; z=zz+=c; x=xx+=c;) }               \
  }else{for(i=0;i<m;++i){                                           \
   y=z; DO(d, *z++=*x++;); zz=z; xx=x;                              \
   DO(n-1, fvv(d,z,y,x); RER; x=xx+=d; y=zz; z=zz+=d;);             \
 }}}

  
#if SY_ALIGN
#define PREFIXBFXLOOP(T,pfx)  \
 {T* xx=(T*)x,* yy,* zz=(T*)z;  \
  q=d/sizeof(T);             \
  DO(m, yy=zz; DO(q, *zz++=*xx++;); DO(n-1, DO(q, *zz++=pfx(*yy,*xx); ++xx; ++yy;)));  \
 }

#define PREFIXBFX(f,pfx,ipfx,spfx,bpfx,vexp)          \
 AHDRP(f,B,B){B* y;I j,q;                        \
  if(1==d)for(j=0;j<m;++j){vexp}                      \
  else if(0==(d&(sizeof(UI  )-1)))PREFIXBFXLOOP(UI,   pfx)  \
  else if(0==(d&(sizeof(UINT)-1)))PREFIXBFXLOOP(UINT,ipfx)  \
  else if(0==(d&(sizeof(US  )-1)))PREFIXBFXLOOP(US,  spfx)  \
  else DO(m, y=z; DO(d, *z++=*x++;); DO(n-1, DO(d, *z++=bpfx(*y,*x); ++x; ++y;)));  \
 }    /* f/\"r z for boolean associative atomic function f */
#else
#define PREFIXBFX(f,pfx,ipfx,spfx,bpfx,vexp)          \
 AHDRP(f,B,B){B*tv;I i,q,r,t,*xi,*yi,*zi;                      \
  q=d/SZI; r=d%SZI; xi=(I*)x; zi=(I*)z; tv=(B*)&t;        \
  if(1==d)   for(r=0;r<m;++r){vexp}                              \
  else if(!r)for(i=0;i<m;++i){                                   \
   yi=zi; DO(q, *zi++=*xi++;);                                   \
   DO(n-1, DO(q, *zi=pfx(*yi,*xi); ++zi; ++xi; ++yi;));          \
  }else for(i=0;i<m;++i){                                        \
   yi=zi; MC(zi,xi,d);                                           \
   xi=(I*)((B*)xi+d);                                            \
   zi=(I*)((B*)zi+d);                                            \
   DO(n-1, DO(q, *zi=pfx(*yi,*xi); ++zi; ++xi; ++yi;);           \
    t=pfx(*yi,*xi); z=(B*)zi; DO(r, z[i]=tv[i];);                \
    xi=(I*)(r+(B*)xi);                                           \
    yi=(I*)(r+(B*)yi);                                           \
    zi=(I*)(r+(B*)zi); );                                        \
 }}   /* f/\"r z for boolean associative atomic function f */
#endif

#define BFXANDOR(c0,c1)  \
 {B*y=memchr(x,c0,n); if(y){q=y-x; memset(z,c1,q); memset(z+q,c0,n-q);}else memset(z,c1,n); x+=d*n; z+=d*n;}

PREFIXBFX( orpfxB, OR, IOR, SOR, BOR,  BFXANDOR(C1,C0))
PREFIXBFX(andpfxB, AND,IAND,SAND,BAND, BFXANDOR(C0,C1))
PREFIXBFX( nepfxB, NE, INE, SNE, BNE, {B b=0; DO(n, *z++=b^=  *x++;);})
PREFIXBFX( eqpfxB, EQ, IEQ, SEQ, BEQ, {B b=1; DO(n, *z++=b=b==*x++;);})


static B jtpscanlt(J jt,I m,I d,I n,B*z,B*x,B p){A t;B*v;I i;
 memset(z,!p,m*n*d); 
 if(1==d)DO(m, if(v=memchr(x,p,n))*(z+(v-x))=p; z+=n; x+=n;)
 else{
  GATV0(t,B01,d,1); v=BAV(t);
  for(i=0;i<m;++i){
   memset(v,C1,d);
   DO(n, DO(d, if(v[i]&&p==x[i]){v[i]=0; z[i]=p;};); z+=d; x+=d;); 
 }}
 R 1;
}    /* f/\"1 w for < and <: */

AHDRP(ltpfxB,B,B){pscanlt(m,d,n,z,x,C1);}
AHDRP(lepfxB,B,B){pscanlt(m,d,n,z,x,C0);}


static B jtpscangt(J jt,I m,I d,I n,B*z,B*x,B a,B pp,B pa,B ps){
  A t;B b,*cc="\000\001\000",e,*p=cc+pp,*v;C*u;I i,j;
 if(d==1)for(i=0;i<m;++i){
  if(v=memchr(x,a,n)){
   j=v-x; b=j&1; 
   mvc(j,z,2L,p); memset(z+j,b!=ps,n-j); *(z+j)=b!=pa;
  }else mvc(n,z,2L,p);
  z+=n; x+=n;
 }else{
  GATV0(t,B01,d,1); u=BAV(t);
  for(i=0;i<m;++i){
   e=pp; memset(u,C0,d);
   DO(n, j=i; DO(d, if(u[i])z[i]='1'==u[i]; else 
     if(a==x[i]){b=j&1; z[i]=b!=pa; u[i]=b!=ps?'1':'0';}else z[i]=e;);
    e=!e; z+=d; x+=d;); 
 }}
 R 1;
}    /* f/\"1 w for > >: +: *: */

AHDRP(  gtpfxB,B,B){pscangt(m,d,n,z,x,C0,C1,C0,C0);}
AHDRP(  gepfxB,B,B){pscangt(m,d,n,z,x,C1,C0,C1,C1);}
AHDRP( norpfxB,B,B){pscangt(m,d,n,z,x,C1,C0,C1,C0);}
AHDRP(nandpfxB,B,B){pscangt(m,d,n,z,x,C0,C1,C0,C1);}


PREFIXOVF( pluspfxI, I, I,  PLUSP, PLUSVV)
PREFIXOVF(tymespfxI, I, I, TYMESP,TYMESVV)

AHDRP(minuspfxI,I,I){C er=0;I i,j,n1=n-1,*xx=x,*y,*zz=z;
 if(1==d){
  if(1==n)DO(m, *z++=*x++;)
  else    DO(m, MINUSP(n,z,x); RER; z=zz+=d*n; x=xx+=d*n;);
 }else for(i=0;i<m;++i){                               
  y=z; DO(d, *z++=*x++;); zz=z; xx=x; j=0;
  DO(n1, MINUSVV(d,z,y,x); RER; x=xx+=d; y=zz; z=zz+=d; if(n1<=++j)break;
          PLUSVV(d,z,y,x); RER; x=xx+=d; y=zz; z=zz+=d; if(n1<=++j)break;);
}}

PREFICPFX( pluspfxO, D, I,  PLUS   )
PREFICPFX(tymespfxO, D, I,  TYMES  )
PREFICALT(minuspfxO, D, I,  MINUSPA)

PREFIXPFX( pluspfxB, I, B,  PLUS, plusIB   )
#if 1
AHDRP(pluspfxD,D,D){I i;
 NAN0;
 if(d==1){
  I n3=n/3; I rem=n-n3*3;  // number of triplets, number of extras
  DQ(m, D t0; D t1; D t2; D t12; D t01; if(rem<1){t0=0.0; t12=t1=0.0;}else {*z++=t0=*x++; if(rem==1){t12=t1=0.0;}else{t12=t1=*x++; *z++=t0+t1;}} t2=0.0;
    DQ(n3, t0+=*x++; *z++ =t0+t12; t1+=*x++; t01=t0+t1; *z++ =t01+t2; t2+=*x++; *z++ =t2+t01; t12=t1+t2;)
  )
 }else{
  for(i=0;i<m;++i){                                              \
   MC(z,x,d*sizeof(D)); x+=d;                                        \
   DQ(n-1, plusDD(jt,d,z+d,z,x,1); z+=d; x+=d;); z+=d;                    \
  }
 }
 NAN1V;
}   /* for associative functions only */
#else  // obsolete 
PREFIXNAN( pluspfxD, D, D,  PLUS, plusDD   )
#endif
PREFIXNAN( pluspfxZ, Z, Z,  zplus, plusZZ  )
PREFIXPFX( pluspfxX, X, X,  xplus, plusXX  )
PREFIXPFX( pluspfxQ, Q, Q,  qplus, plusQQ  )

PREFIXPFX(tymespfxD, D, D,  TYMES, tymesDD  )
PREFIXPFX(tymespfxZ, Z, Z,  ztymes, tymesZZ )
PREFIXPFX(tymespfxX, X, X,  xtymes, tymesXX )
PREFIXPFX(tymespfxQ, Q, Q,  qtymes, tymesQQ )

PREFIXALT(minuspfxB, I, B,  MINUSPA)
PREALTNAN(minuspfxD, D, D,  MINUSPA)
PREALTNAN(minuspfxZ, Z, Z,  MINUSPZ)
PREFIXALT(minuspfxX, X, X,  MINUSPX)
PREFIXALT(minuspfxQ, Q, Q,  MINUSPQ)

PREALTNAN(  divpfxD, D, D,  DIVPA  )
PREALTNAN(  divpfxZ, Z, Z,  DIVPZ  )

PREFIXPFX(  maxpfxI, I, I,  MAX , maxII   )
PREFIXPFX(  maxpfxD, D, D,  MAX , maxDD   )
PREFIXPFX(  maxpfxX, X, X,  XMAX, maxXX   )
PREFIXPFX(  maxpfxQ, Q, Q,  QMAX, maxQQ   )
PREFIXPFX(  maxpfxS, SB,SB, SBMAX, maxSS  )

PREFIXPFX(  minpfxI, I, I,  MIN, minII    )
PREFIXPFX(  minpfxD, D, D,  MIN, minDD    )
PREFIXPFX(  minpfxX, X, X,  XMIN, minXX   )
PREFIXPFX(  minpfxQ, Q, Q,  QMIN, minQQ   )
PREFIXPFX(  minpfxS, SB,SB, SBMIN, minSS  )

PREFIXPFX(bw0000pfxI, UI,UI, BW0000, bw0000II)
PREFIXPFX(bw0001pfxI, UI,UI, BW0001, bw0001II)
PREFIXPFX(bw0011pfxI, UI,UI, BW0011, bw0011II)
PREFIXPFX(bw0101pfxI, UI,UI, BW0101, bw0101II)
PREFIXPFX(bw0110pfxI, UI,UI, BW0110, bw0110II)
PREFIXPFX(bw0111pfxI, UI,UI, BW0111, bw0111II)
PREFIXPFX(bw1001pfxI, UI,UI, BW1001, bw1001II)
PREFIXPFX(bw1111pfxI, UI,UI, BW1111, bw1111II)

// This old prefix support is needed for sparse matrices

static DF1(jtprefix){DECLF;I r;
 RZ(w);
 r = (RANKT)jt->ranks; RESETRANK; if(r<AR(w)){R rank1ex(w,self,r,jtprefix);}
 R eachl(apv(IC(w),1L,1L),w,atop(fs,ds(CTAKE)));
}    /* f\"r w for general f */

static DF1(jtgprefix){A h,*hv,z,*zv;I m,n,r;
 RZ(w);
 ASSERT(DENSE&AT(w),EVNONCE);
 r = (RANKT)jt->ranks; RESETRANK; if(r<AR(w)){R rank1ex(w,self,r,jtgprefix);}
 n=IC(w); 
 h=VAV(self)->fgh[2]; hv=AAV(h); m=AN(h);
 GATV0(z,BOX,n,1); zv=AAV(z); I imod=0;
 DO(n, imod=(imod==m)?0:imod; RZ(zv[i]=df1(take(sc(1+i),w),hv[imod])); ++imod;);
 R ope(z);
}    /* g\"r w for gerund g */


// This old infix support is needed for sparse matrices

// block a contains (start,length) of infix.  w is the A for the data.
// Result is new block containing the extracted infix
static F2(jtseg){A z;I c,k,m,n,*u,zn;
 RZ(a&&w);
 // The (start,length) had better be integers.  Extract them into m,n
 if(INT&AT(a)){u=AV(a); m=*u; n=*(1+u);} else m=n=0;
 c=aii(w); k=c<<bplg(AT(w)); RE(zn=mult(n,c));  // c=#atoms per item, k=#bytes/item, zn=atoms/infix
 GA(z,AT(w),zn,MAX(1,AR(w)),AS(w)); AS(z)[0]=n;  // Allocate array of items, move in shape, override # items
 // Copy the selected items to the new block and return the new block
 MC(AV(z),CAV(w)+m*k,n*k);
 R z;
}

// m is the infix length (x), w is the array (y)
// Result is A for an nx2 table of (starting item#,length) for each infix
static A jtifxi(J jt,I m,A w){A z;I d,j,k,n,p,*x;
 RZ(w);
 // p=|m, n=#items of w, d=#applications of u (depending on overlapping/nonoverlapping)
 p=ABS(m); n=IC(w);
 if(m>=0){d=MAX(0,1+n-m);}else{d=1+(n-1)/p; d=(n==0)?n:d;}
 // Allocate result, a dx2 table; install shape
 GATV0(z,INT,2*d,2); *AS(z)=d; *(1+AS(z))=2;
 // x->result area; k=stride between infixes; j=starting index (prebiased); copy (index,length) for each infix;
 // repair last length if it runs off the end
 x=AV(z); k=0>m?p:1; j=-k; DO(d, *x++=j+=k; *x++=p;); if(d)*--x=MIN(p,n-j);
 R z;
}

// Entry point for infix.  a is x, w is y, fs points to u
static DF2(jtinfix){PROLOG(0018);DECLF;A x,z;I m; 
 PREF2(jtinfix); // Handle looping over rank.  This returns here for each cell (including this test)
 // The rest of this verb handles a single cell
 // If length is infinite, convert to large integer
 // kludge - test for ==ainf should be replaced with a test for value; will fail if _ is result of expression like {._
 if(a==ainf)m=IMAX;
 else RE(m=i0(vib(a))); // get infix length as an integer
 // Create table of infix positions
 RZ(x=ifxi(m,w));
 // If there are infixes, apply fs@:jtseg (ac2 creates an A verb for jtseg)
 if(*AS(x))z=eachl(x,w,atop(fs,ac2(jtseg)));
 else{A s;I r, rr;
  // No infixes.  Create a cell of fills, apply fs to it, add a leading axis of 0 to the result
  // create a block containing the shape of the fill-cell.  The fill-cell is a list of items of y,
  // with the number of items being the infix-size if positive, or 0 if negative
  // r = rank of w, rr=rank of list of items of w, s is block for list of length rr; copy shape of r; override #items of infix
  r=AR(w); rr=MAX(1,r); GATV0(s,INT,rr,1); if(r)MCISH(AV(s),AS(w),r); *AV(s)=0>m?0:m==IMAX?1+IC(w):m;
  // Create fill-cell of shape s; apply u to it
  RZ(x=df1(reshape(s,filler(w)),fs));
  // Prepend leading axis of 0 to the result
  z=reshape(over(zeroionei[0],shape(x)),x);
 } 
 EPILOG(z);
}

static DF2(jtinfix2){PROLOG(0019);A f;I m,n,t; 
 PREF2(jtinfix); 
 RE(m=i0(vib(a))); t=AT(w); n=IC(w); 
 if(!(2==m&&2<=n&&t&DENSE))R infix(a,w,self);
 f=FAV(self)->fgh[0]; f=FAV(f)->fgh[0];
 A z=df2(curtail(w),behead(w),vaid(f)?f:qq(f,num[-1]));
 EPILOG(z);
}    /* 2 f/\w */

static DF2(jtginfix){A h,*hv,x,z,*zv;I d,m,n;
 RE(m=i0(vib(a))); 
 RZ(x=ifxi(m,w));
 h=VAV(self)->fgh[2]; hv=AAV(h); d=AN(h);
 if(n=IC(x)){
  GATV0(z,BOX,n,1); zv=AAV(z);
  DO(n, RZ(zv[i]=df1(seg(from(sc(i),x),w),hv[i%d])););
  R ope(z);
 }else{A s;
  RZ(s=AR(w)?shape(w):ca(iv0)); *AV(s)=ABS(m);
  RZ(x=df1(reshape(s,filler(w)),*hv));
  R reshape(over(zeroionei[0],shape(x)),x);
}}

#define STATEHASGERUND 0x1000  // f is a gerund
#define STATEISPREFIX 0x2000  // this is prefix rather than infix
#define STATESLASH2 0x4000  // f is f'/ and x is 2

// prefix and infix: prefix if a is mark
static DF2(jtinfixprefix2){F2PREFIP;DECLF;PROLOG(00202);A *hv;
   I hn,wt;
 
 RZ(w);
 PREF2IP(jtinfixprefix2);  // handle rank loop if needed
 wt=AT(w);
 if(wt&SPARSE){
  // Use the old-style non-virtual code for sparse types
  switch(((VAV(self)->flag&VGERL)>>(VGERLX-1)) + (a==mark)) {  // 2: is gerund  1: is prefix
  case (0+0): R jtinfix(jt,a,w,self);
  case (0+1): R jtprefix(jt,w,self);
  case (2+0): R jtginfix(jt,a,w,self);
  case (2+1): R jtgprefix(jt,w,self);
  }
 }
 // Not sparse.  Calculate the # result items
#define ZZFLAGWORD state
 I state=0;  // init flags, including zz flags

 // If the verb is a gerund, it comes in through h, otherwise the verb comes through f.  Set up for the two cases
 if(!(VGERL&sv->flag)){V *vf=FAV(fs);  // if verb, point to its data
  // not gerund: OK to test fs
  if(vf->mr>=AR(w)){
   // we are going to execute f without any lower rank loop.  Thus we can use the BOXATOP etc flags here.  These flags are used only if we go through the full assemble path
   state = vf->flag2>>(VF2BOXATOP1X-ZZFLAGBOXATOPX);  // Don't touch fs yet, since we might not loop
   state &= ~(vf->flag2>>(VF2ATOPOPEN1X-ZZFLAGBOXATOPX));  // We don't handle &.> here; ignore it
   state &= ZZFLAGBOXATOP;  // we want just the one bit, BOXATOP1 & ~ATOPOPEN1
   state |= (-state) & (I)jtinplace & (ZZFLAGWILLBEOPENED|ZZFLAGCOUNTITEMS); // remember if this verb is followed by > or ; - only if we BOXATOP, to avoid invalid flag setting at assembly
  }
 }else{
  state |= STATEHASGERUND; A h=sv->fgh[2]; hv=AAV(h); hn=AN(h); ASSERT(hn,EVLENGTH);  // Gerund case.  Mark it, set hv->1st gerund, hn=#gerunds.  Verify gerunds not empty
 }

 I zi;  // number of items in the result
 I ilnval;  // value of a, set to -1 for prefix (for fill purposes)
 I ilnabs;  // abs(ilnval), or 1 if prefix.  Clamped to the # items of w
 I wc;  // number of atoms in a cell of w
 I remlen;  // number of items of w not processed yet (at start of loop, does not include the first infix).  When this goes to 0, we're done
 I stride;  // number of items to advance virtual-arg pointers by between cells
 I strideb;  // stride*number of bytes per cell (not used for prefix)

 I wi=IC(w);  // wi=#items of w
 PROD1(wc,AR(w)-1,AS(w)+1);  // #atoms in cell of a.  Overflow possible only if wi==0, which will go to fill
 // set up for prefix/infix.  Calculate # result slots
 if(a!=mark){
  // infix.
  ilnval; RE(ilnval=i0(vib(a))); // ilnval=infix # (error if nonintegral; convert inf to HIGH_VALUE)
  if(ilnval>=0){
   // positive infix.  Stride is 1 cell.
   ilnabs=ilnval;
   zi=1+wi-ilnval;  // number of infix positions.  May be negative if no infixes.
   stride=1;   // advance 1 position per iteration
   remlen=zi;  // length is # nonoverlapping segments to do
  }else{
   // negative infix.  Stride is multiple cells.
   ilnabs=-ilnval; ilnabs^=ilnabs>>(BW-1);  // abs(ilnval), and handle overflow
   zi=1+(wi-1)/ilnabs; zi=(wi==0)?wi:zi;  // calc number of partial infixes.  Because of C's rounding toward zero, we have to handle the wi==0 case separately, and anyway it
      // requires a design decision: we choose to treat it as 0 partitions of 0 length (rather than 1 partition of 0 length, or 0 partitions of length ilnabs)
   stride=ilnabs;  // advance by the stride
   remlen=wi;  // since there are no overlaps, set length-to-do to total length
  }
  strideb = (stride * wc) <<bplg(wt);  // get stride in bytes, for incrementing virtual-block offsets
 }else{
  // prefix.
  zi=wi;  // one prefix per item
  ilnabs=1;  // allocation of initial prefix is for 1 item
  ilnval=-1;  // set neg infix len to suppress fill if there are no result items
  stride=1;  // used only as loop counter
  remlen=zi;  // count # prefixes
  state |= STATEISPREFIX;
 }

 A zz=0;  // place where we will build up the homogeneous result cells
 if(zi>0){A z;
  // Normal case where there are cells.
  // loop over the infixes
#define ZZDECL
#include "result.h"
  ZZPARMS(1,zi,1)
#define ZZINSTALLFRAME(optr) *optr++=zi;

  // Allocate a virtual block for the argument, and give it initial shape and atomcount
  A virtw, virta;
  I vr=AR(w); vr+=(vr==0);  // rank of infix is same as rank of w, except that atoms are promoted to singleton lists

  // check for special case of 2 u/\ y; if found, set new function and allocate a second virtual argument
  // NOTE: gerund/ is encoded are `:, so we can be sure id==SLASH does not have gerund
  fauxblock(virtafaux); fauxblock(virtwfaux);
  if(((VAV(fs)->id^CSLASH)|((ilnabs|(wi&((UI)ilnval>>(BW-1))))^2))){   // char==/ and (ilnabs==2, but not if input array is odd and ilnval is neg)
   // normal case, infix/prefix.  Allocate a virtual block
   fauxvirtual(virtw,virtwfaux,w,vr,ACUC1);

   ilnabs=(ilnabs>wi)?wi:ilnabs;  // ilnabs will be used to allocate virtual arguments - limit to size of w
   I *virtws=AS(virtw); virtws[0]=ilnabs; MCISH(virtws+1,AS(w)+1,vr-1) AN(virtw)=ilnabs*wc; // shape is (infix size),(shape of cell)  tally is #items*celllength
  }else{
   // 2 f/\ y.  The virtual args are now ITEMS of w rather than subarrays
   fauxvirtual(virta,virtafaux,w,vr-1,ACUC1); // first block is for a
   MCISH(AS(virta),AS(w)+1,vr-1); AN(virta)=wc; // shape is (shape of cell)  tally is celllength
   fauxvirtual(virtw,virtwfaux,w,vr-1,ACUC1);  // second is w
   AK(virtw) += strideb >> ((UI)ilnval>>(BW-1));  // we want to advance 1 cell.  If ilnval is positive, strideb is 1 cell; otherwise strideb is 2 cells
   MCISH(AS(virtw),AS(w)+1,vr-1); AN(virtw)=wc; // shape is (shape of cell)  tally is celllength
   // advance from f/ to f and get the function pointer.  Note that 2 <@(f/)\ will go through here too
   fs=FAV(fs)->fgh[0]; f1=FAV(fs)->valencefns[1];
   // mark that we are handling this case
   state |= STATESLASH2;
  }

  I gerundx = 0;  // in case we are doing gerunds, start on the first one
  while(1){

   // call the user's function
   if(!(state&(STATEHASGERUND|STATESLASH2))){
    // normal case: prefix/infix, no gerund
    RZ(z=CALL1(f1,virtw,fs));  //normal case
   }else if(state&STATESLASH2){
    // 2 f/\ y case
    RZ(z=CALL2(f1,virta,virtw,fs));  //normal case
   }else{
    // prefix/infix, gerund case
    RZ(z=df1(virtw,hv[gerundx])); ++gerundx; gerundx=(gerundx==hn)?0:gerundx;  // gerund case.  Advance gerund cyclically
   }

#define ZZBODY  // assemble results
#include "result.h"

   // advance input pointer for next cell.  We keep the same virtual block because it can't be incorporated into anything
   // We can't advance until after the assembly code has run, in case the verb returned the virtual block as its result

   // calculate the amount of data unprocessed after the result we just did.  If there is none, we're finished
   if((remlen-=stride)<=0)break;

   if(!(state&STATEISPREFIX)){
    // infix case, or 2 f/\ y.  Add to the virtual-block offset.  If this is a shard, reduce the size of the virtual block.
    // If this is 2 f/\ y, advance the second block as well.  It can't be both.
    AK(virtw) += strideb;
    if(((remlen-stride)|(-(state&(STATESLASH2))))<0){
     // we either have a shard or 2 f/\ y.
     if(state&STATESLASH2){
      // 2 f/\ y.  Advance the second pointer also.
      AK(virta) += strideb;
     }else{
      // we have hit a shard.  Reduce the shape and tally of the next argument
      AS(virtw)[0] += remlen-stride;   // reduce the # items by the amount of overhang
      AN(virtw) += (remlen-stride)*wc;  // reduce # atoms by amount of overhang, in atoms
     }
    }
   }else{
    // prefix.  enlarge the virtual block by 1 cell.
    AS(virtw)[0]++;  // one more cell
    AN(virtw) += wc;  // include its atoms
   }
  }

#define ZZEXIT
#include "result.h"

 }else{A z;
  // no cells - execute on a cell of fills
  // Do this quietly, because
  // if there is an error, we just want to use a value of 0 for the result; thus debug
  // mode off and RESETERR on failure.
  // However, if the error is a non-computational error, like out of memory, it
  // would be wrong to ignore it, because the verb might execute erroneously with no
  // indication that anything unusual happened.  So fail then

  // The cell to execute on depends on the arguments:
  // for prefix, 0 items of fill
  // for infix +, invabs items of fill
  // for infix -, 0 items of fill
  RZ(z=reitem(zeroionei[0],w));  // create 0 items of the type of w
  if(ilnval>=0){ilnval=(ilnval==IMAX)?(wi+1):ilnval; RZ(z=take(sc(ilnval),z));}    // if items needed, create them.  For compatibility, treat _ as 1 more than #items in w
  UC d=jt->uflags.us.cx.cx_c.db; jt->uflags.us.cx.cx_c.db=0; zz=(state&STATEHASGERUND)?df1(z,hv[0]):CALL1(f1,z,fs); jt->uflags.us.cx.cx_c.db=d; if(EMSK(jt->jerr)&EXIGENTERROR)RZ(zz); RESETERR;
  RZ(zz=reshape(over(zeroionei[0],shape(zz?zz:mtv)),zz?zz:zeroionei[0]));
 }

// result is now in zz

 EPILOG(zz);
}

// prefix, vectors to common processor.  Handles IRS.  Supports inplacing
static DF1(jtinfixprefix1){F1PREFIP;
 I r = (RANKT)jt->ranks; RESETRANK; if(r<AR(w)){R jtrank1ex(jtinplace,w,self,r,jtinfixprefix1);}
 R jtinfixprefix2(jtinplace,mark,w,self);
}

//  f/\"r y    w is y, fs is in self
static DF1(jtpscan){A y,z;I d,f,m,n,r,t,wn,wr,*ws,wt;
 RZ(w);F1PREFIP;
 wt=AT(w);   // get type of w
 if(SPARSE&wt)R scansp(w,self,jtpscan);  // if sparse, go do it separately
 // wn = #atoms in w, wr=rank of w, r=effective rank, f=length of frame, ws->shape of w
 wn=AN(w); wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK; f=wr-r; ws=AS(w);
 // m = #cells, c=#atoms/cell, n = #items per cell
 PROD(m,f,ws); PROD1(d,r-1,ws+f+1); n=r?ws[f]:1;  // wn=0 doesn't matter
 y=FAV(self)->fgh[0]; // y is the verb u, which is f/
 // If there are 0 or 1 items, or w is empty, return the input unchanged, except: if rank 0, return (($w),1)($,)w - if atomic op, do it right here, otherwise call the routine to get the shape of result cell
 if(2>n||!wn){if(vaid(FAV(y)->fgh[0])){R r?RETARG(w):reshape(over(shape(w),num[1]),w);}else R irs1(w,self,r,jtinfixprefix1);}
 VA2 adocv = vapfx(FAV(y)->fgh[0],wt);  // fetch info for f/\ and this type of arg
 if(!adocv.f)R irs1(w,self,r,jtinfixprefix1);  // if there is no special function for this type, do general reduce
 if((t=atype(adocv.cv))&&TYPESNE(t,wt))RZ(w=cvt(t,w));  // convert input if necessary
 // if inplaceable, reuse the input area for the result
 if((I)jtinplace&(adocv.cv>>VIPOKWX)&JTINPLACEW && ASGNINPLACE(w))z=w; else GA(z,rtype(adocv.cv),wn,wr,ws);
 adocv.f(jt,m,d,n,AV(z),AV(w));
 if(jt->jerr)R (jt->jerr>=EWOV)?irs1(w,self,r,jtpscan):0; else R adocv.cv&VRI+VRD?cvz(adocv.cv,z):z;
}    /* f/\"r w atomic f main control */

static DF2(jtinfixd){A fs,z;C*x,*y;I c=0,d,k,m,n,p,q,r,*s,wr,*ws,wt,zc; 
 F2RANK(0,RMAX,jtinfixd,self);
 wr=AR(w); ws=AS(w); wt=AT(w); n=IC(w);
 RE(m=i0(vib(a))); if(m==IMAX){m=n+1;} p=m==IMIN?IMAX:ABS(m);
 if(0>m){p=MIN(p,n); d=p?(n+p-1)/p:0;}else{ASSERT(IMAX-1>n-m,EVDOMAIN); d=MAX(0,1+n-m);}
 if(fs=FAV(self)->fgh[0],CCOMMA==ID(fs)){RE(c=aii(w)); RE(zc=mult(p,c)); r=2;}
 else{if(n)RE(c=aii(w)); zc=p; r=wr?1+wr:2;}
 GA(z,wt,d*p*c,r,0); x=CAV(z); y=CAV(w);
 s=AS(z); *s++=d; *s++=zc; MCISH(s,1+ws,r-2);
 k=c<<bplg(wt); 
 if(AN(z)){
  if(m>=0){ q=p*k; DO(d, MC(x,y,q);    x+=q; y+=k;);
  }else{ MC(x,y,n*k);  if(q=d*p-n)fillv(wt,q*c,x+n*k);
  }
 }
 RETF(z);
}    /* a[\w and a]\w and a,\w */


#define SETZ    {s=yv; DO(c, *zv++=*s++;  );}
#define SETZD   {s=yv; DO(c, *zv++=*s++/d;);}

#define MOVSUMAVG(Tw,Ty,ty,Tz,tz,xd,SET)  \
 {Tw*u,*v;Ty*s,x=0,*yv;Tz*zv;                                  \
  GATVS(z,tz,c*(1+p),AR(w),AS(w),tz##SIZE,GACOPYSHAPE,R 0); AS(z)[0]=1+p;                    \
  zv=(Tz*)AV(z); u=v=(Tw*)AV(w);                               \
  if(1==c){                                                    \
   DO(m, x+=*v++;); *zv++=xd;                                  \
   DO(p, x+=(Ty)*v++-(Ty)*u++; *zv++=xd;);                     \
  }else{                                                       \
   GATVS(y,ty,c,1,0,ty##SIZE,GACOPYSHAPE0,R 0); s=yv=(Ty*)AV(y); DO(c, *s++=0;);            \
   DO(m, s=yv; DO(c, *s+++=*v++;);); SET;                      \
   DO(p, s=yv; DO(c, x=*s+++=(Ty)*v++-(Ty)*u++; *zv++=xd;););  \
 }}

static A jtmovsumavg1(J jt,I m,A w,A fs,B avg){A y,z;D d=(D)m;I c,p,wt;
 p=IC(w)-m; wt=AT(w); c=aii(w);
 switch((wt&B01?0:wt&INT?2:4)+avg){
  case 0:       MOVSUMAVG(B,I,INT,I,INT,x,  SETZ ); break;
  case 1:       MOVSUMAVG(B,I,INT,D,FL, x/d,SETZD); break;
  case 2: 
   // start min at 0 so range is max+1; make sure all totals fit in an int; use integer code if so
   {I maxval = (I)((D)IMAX/(D)MAX(2,m))-1;
   CR rng=condrange(AV(w),AN(w),0,0,maxval<<1);
   if(rng.range && MAX(rng.range,-rng.min)<maxval){
    MOVSUMAVG(I,I,INT,I,INT,x,  SETZ )
   }else{MOVSUMAVG(I,D,FL, D,FL, x,  SETZ );} break;}
  case 3:       MOVSUMAVG(I,D,FL, D,FL, x/d,SETZD); break;
  case 4: NAN0; MOVSUMAVG(D,D,FL, D,FL, x,  SETZ ); NAN1; break;
  case 5: NAN0; MOVSUMAVG(D,D,FL, D,FL, x/d,SETZD); NAN1; break;
 }
 RETF(z);
}    /* m +/\w or (if 0=avg) m (+/%#)\w (if 1=avg); bool or integer or float; 0<m */

static A jtmovsumavg(J jt,I m,A w,A fs,B avg){A z;
 z=movsumavg1(m,w,fs,avg);
 if(jt->jerr==EVNAN)RESETERR else R z;
 R jtinfixprefix2(jt,sc(m),w,fs);
}

static DF2(jtmovavg){I m;
 PREF2(jtmovavg);
 RE(m=i0(vib(a)));
 if(0<m&&m<=IC(w)&&AT(w)&B01+FL+INT)R movsumavg(m,w,self,1); 
 R jtinfixprefix2(jt,a,w,self);
}    /* a (+/ % #)\w */

#define MOVMINMAX(T,type,ie,CMP)    \
 {T d,e,*s,*t,*u,*v,x=ie,*yv,*zv;                              \
  zv=(T*)AV(z); u=v=(T*)AV(w);                                 \
  if(1==c){                                                    \
   DO(m, d=*v++; if(d CMP x)x=d;); *zv++=x;                    \
   for(i=0;i<p;++i){                                           \
    d=*v++; e=*u++;                                            \
    if(d CMP x)x=d; else if(e==x){x=d; t=u; DO(m-1, e=*t++; if(e CMP x)x=e;);}  \
    *zv++=x;                                                   \
  }}else{                                                      \
   GATVS(y,type,c,1,0,type##SIZE,GACOPYSHAPE0,R 0); s=yv=(T*)AV(y); DO(c, *s++=ie;);          \
   DO(m, s=yv; DO(c, d=*v++; if(d CMP *s)*s=d; ++s;);); SETZ;  \
   for(i=0;i<p;++i){                                           \
    for(j=0,s=yv;j<c;++j,++s){                                 \
     d=*v++; e=*u++; x=*s;                                     \
     if(d CMP x)x=d; else if(e==x){x=d; t=c+u-1; DO(m-1, e=*t; t+=c; if(e CMP x)x=e;);}  \
     *s=x;                                                     \
    }                                                          \
    SETZ;                                                      \
 }}}

#define MOVMINMAXS(T,type,ie,CMP)    \
 {T d,e,*s,*t,*u,*v,x=ie,*yv,*zv;                              \
  zv=(T*)AV(z); u=v=(T*)AV(w);                                 \
  if(1==c){                                                    \
   DO(m, d=*v++; if(CMP(d,x))x=d;); *zv++=x;                    \
   for(i=0;i<p;++i){                                           \
    d=*v++; e=*u++;                                            \
    if(CMP(d,x))x=d; else if(e==x){x=d; t=u; DO(m-1, e=*t++; if(CMP(e,x))x=e;);}  \
    *zv++=x;                                                   \
  }}else{                                                      \
   GATVS(y,type,c,1,0,type##SIZE,GACOPYSHAPE0,R 0); s=yv=(T*)AV(y); DO(c, *s++=ie;);          \
   DO(m, s=yv; DO(c, d=*v++; if(CMP(d,*s))*s=d; ++s;);); SETZ;  \
   for(i=0;i<p;++i){                                           \
    for(j=0,s=yv;j<c;++j,++s){                                 \
     d=*v++; e=*u++; x=*s;                                     \
     if(CMP(d,x))x=d; else if(e==x){x=d; t=c+u-1; DO(m-1, e=*t; t+=c; if(CMP(e,x))x=e;);}  \
     *s=x;                                                     \
    }                                                          \
    SETZ;                                                      \
 }}}

static A jtmovminmax(J jt,I m,A w,A fs,B max){A y,z;I c,i,j,p,wt;
 p=IC(w)-m; wt=AT(w); c=aii(w);
 GA(z,AT(w),c*(1+p),AR(w),AS(w)); AS(z)[0]=1+p;
 switch(max+(wt&SBT?0:wt&INT?2:4)){
  case 0: MOVMINMAXS(SB,SBT,jt->sbuv[0].down,SBLE); break;
  case 1: MOVMINMAXS(SB,SBT,0,SBGE); break;
  case 2: MOVMINMAX(I,INT,IMAX,<=); break;
  case 3: MOVMINMAX(I,INT,IMIN,>=); break;
  case 4: MOVMINMAX(D,FL, inf ,<=); break;
  case 5: MOVMINMAX(D,FL, infm,>=); break;
 }
 RETF(z);
}    /* a <./\w (0=max) or a >./\ (1=max); vector w; integer or float; 0<m */

static A jtmovandor(J jt,I m,A w,A fs,B or){A y,z;B b0,b1,d,e,*s,*t,*u,*v,x,*yv,*zv;I c,i,j,p;
 p=IC(w)-m; c=aii(w); x=b0=!or; b1=or;
 GATV(z,B01,c*(1+p),AR(w),AS(w)); AS(z)[0]=1+p;
 zv=BAV(z); u=v=BAV(w);
 if(1==c){
  DO(m, if(b1==*v++){x=b1; break;}); *zv++=x; v=u+m;
  for(i=0;i<p;++i){
   d=*v++; e=*u++;
   if(d==b1)x=d; else if(e==b1){x=d; t=u; DO(m-1, if(b1==*t++){x=b1; break;});}
   *zv++=x;
 }}else{
  GATV0(y,B01,c,1); s=yv=BAV(y); DO(c, *s++=b0;);
  DO(m, s=yv; DO(c, if(b1==*v++)*s=b1; ++s;);); SETZ;
  for(i=0;i<p;++i){
   for(j=0,s=yv;j<c;++j,++s){
    d=*v++; e=*u++; x=*s;
    if(d==b1)x=d; else if(e==b1){x=d; t=c+u-1; DO(m-1, e=*t; t+=c; if(b1==e){x=b1; break;});}
    *s=x;
   }
   SETZ;
 }}
 RETF(z);
}    /* a *./\w (0=or) or a +./\ (1=or); boolean w; 0<m */

static A jtmovbwandor(J jt,I m,A w,A fs,B or){A z;I c,p,*s,*t,*u,x,*zv;
 p=IC(w)-m; c=aii(w);
 GATV(z,INT,c*(1+p),AR(w),AS(w)); AS(z)[0]=1+p;
 zv=AV(z); u=AV(w);
 if(c)switch(or+(1==c?0:2)){
  case 0: DO(1+p, x=*u++; t=u; DO(m-1, x&=*t++;); *zv++=x;); break;
  case 1: DO(1+p, x=*u++; t=u; DO(m-1, x|=*t++;); *zv++=x;); break;
  case 2: DO(1+p, ICPY(zv,u,c); t=u+=c; DO(m-1, s=zv; DO(c, *s++&=*t++;);); zv+=c;); break;
  case 3: DO(1+p, ICPY(zv,u,c); t=u+=c; DO(m-1, s=zv; DO(c, *s++|=*t++;);); zv+=c;); break;
 }
 RETF(z);
}    /* a 17 b./\w (0=or) or a 23 b./\ (1=or); integer w; 0<m */

static A jtmovneeq(J jt,I m,A w,A fs,B eq){A y,z;B*s,*u,*v,x,*yv,*zv;I c,p;
 p=IC(w)-m; c=aii(w); x=eq;
 GATV(z,B01,c*(1+p),AR(w),AS(w)); AS(z)[0]=1+p;
 zv=BAV(z); u=v=BAV(w);
 if(1<c){GATV0(y,B01,c,1); s=yv=BAV(y); DO(c, *s++=eq;);}
 switch(eq+(1<c?2:0)){
  case 0: DO(m,                   x   ^=   *v++;  ); *zv++=x; DO(p,                   *zv++=x   ^=   *u++^ *v++;  ); break;
  case 1: DO(m,                   x    =x==*v++;  ); *zv++=x; DO(p,                   *zv++=x    =x==*u++==*v++;  ); break;
  case 2: DO(m, s=yv; DO(c,       *s++^=   *v++;);); SETZ;    DO(p, s=yv; DO(c,       *zv++=*s++^=   *u++^ *v++;);); break;
  case 3: DO(m, s=yv; DO(c, x=*s; *s++ =x==*v++;);); SETZ;    DO(p, s=yv; DO(c, x=*s; *zv++=*s++ =x==*u++==*v++;););
 }
 RETF(z);
}    /* m ~:/\w (0=eq) or m =/\ (1=eq); boolean w; 0<m */

static A jtmovbwneeq(J jt,I m,A w,A fs,B eq){A y,z;I c,p,*s,*u,*v,x,*yv,*zv;
 p=IC(w)-m; c=aii(w); x=eq?-1:0;
 GATV(z,INT,c*(1+p),AR(w),AS(w)); AS(z)[0]=1+p;
 zv=AV(z); u=v=AV(w);
 if(1<c){GATV0(y,INT,c,1); s=yv=AV(y); DO(c, *s++=x;);}
 switch(eq+(1<c?2:0)){
  case 0: DO(m,                   x   ^=    *v++ ;  ); *zv++=x; DO(p,                   *zv++=x   ^=      *u++^*v++  ;  ); break;
  case 1: DO(m,                   x    =~(x^*v++);  ); *zv++=x; DO(p,                   *zv++=x    =~(x^~(*u++^*v++));  ); break;
  case 2: DO(m, s=yv; DO(c,       *s++^=    *v++ ;);); SETZ;    DO(p, s=yv; DO(c,       *zv++=*s++^=      *u++^*v++  ;);); break;
  case 3: DO(m, s=yv; DO(c, x=*s; *s++ =~(x^*v++););); SETZ;    DO(p, s=yv; DO(c, x=*s; *zv++=*s++ =~(x^~(*u++^*v++));););
 }
 RETF(z);
}    /* m 22 b./\w (0=eq) or m 25 b./\ (1=eq); integer w; 0<m */

static DF2(jtmovfslash){A x,z;B b;C id,*wv,*zv;I d,m,m0,p,t,wk,wt,zi,zk,zt;
 PREF2(jtmovfslash);
 p=IC(w); wt=AT(w);   // p=#items of w
 RE(m0=i0(vib(a))); m=m0>>(BW-1); m=(m^m0)-m; m^=(m>>(BW-1));  // m0=infx x,  m=abs(m0), handling IMIN 
 if((((2^m)-1)|(m-1)|(p-m))<0)R jtinfixprefix2(jt,a,w,self);  // If m is 0-2, go to general case
 x=FAV(self)->fgh[0]; x=FAV(x)->fgh[0]; id=ID(x); 
 if(wt&B01)id=id==CMIN?CSTARDOT:id==CMAX?CPLUSDOT:id; 
 if(id==CBDOT&&(x=VAV(x)->fgh[0],INT&AT(x)&&!AR(x)))id=(C)*AV(x);
 switch(AR(w)&&0<m0&&m0<=*AS(w)?id:0){
  case CPLUS:    if(wt&B01+INT+FL)R movsumavg(m,w,self,0); break;
  case CMIN:     if(wt&SBT+INT+FL)R movminmax(m,w,self,0); break;
  case CMAX:     if(wt&SBT+INT+FL)R movminmax(m,w,self,1); break;
  case CSTARDOT: if(wt&B01       )R  movandor(m,w,self,0); break;
  case CPLUSDOT: if(wt&B01       )R  movandor(m,w,self,1); break;
  case CNE:      if(wt&B01       )R   movneeq(m,w,self,0); break;
  case CEQ:      if(wt&B01       )R   movneeq(m,w,self,1); break;
  case CBW1001:  if(wt&    INT   )R movbwneeq(m,w,self,1); break;
  case CBW0110:  if(wt&    INT   )R movbwneeq(m,w,self,0); break;
 }
 VA2 adocv;
 if(!ds(id) || !(adocv = vains(ds(id),wt)).f)R jtinfixprefix2(jt,a,w,self);  // if no special routine for insert, do general case
 if(m0>=0){zi=MAX(0,1+p-m);}else{zi=1+(p-1)/m; zi=(p==0)?p:zi;}  // zi = # result cells
 d=aii(w); b=0>m0&&zi*m!=p;   // b='has shard'
 zt=rtype(adocv.cv); RESETRANK;
 GA(z,zt,d*zi,MAX(1,AR(w)),AS(w)); AS(z)[0]=zi;
 if(d*zi==0)RETF(z);  // mustn't call adocv on empty arg!
 if((t=atype(adocv.cv))&&TYPESNE(t,wt)){RZ(w=cvt(t,w)); wt=AT(w);}
 zv=CAV(z); zk=d<<bplg(zt); 
 wv=CAV(w); wk=(0<=m0?d:d*m)<<bplg(wt);
 DO(zi-b, adocv.f(jt,(I)1,d,m,zv,wv); zv+=zk; wv+=wk;);
 if(b)adocv.f(jt,(I)1,d,p-m*(zi-1),zv,wv);
 if(jt->jerr>=EWOV){RESETERR; R movfslash(a,cvt(FL,w),self);}else R z;
}    /* a f/\w */

static DF1(jtiota1){R apv(IC(w),1L,1L);}

F1(jtbslash){A f;AF f1=jtinfixprefix1,f2=jtinfixprefix2;V*v;I flag=FAV(ds(CBSLASH))->flag;
;
 RZ(w);
 if(NOUN&AT(w))R fdef(0,CBSLASH,VERB, jtinfixprefix1,jtinfixprefix2, w,0L,fxeachv(1L,w), VGERL|flag, RMAX,0L,RMAX);
 v=FAV(w); f=FAV(w)->fgh[0];
 switch(v->id){
  case CPOUND:
   f1=jtiota1; break;
  case CLEFT: case CRIGHT: case CCOMMA:   
   f2=jtinfixd; break;
  case CFORK:  
   if(v->valencefns[0]==(AF)jtmean)f2=jtmovavg; break;
  case CSLASH: 
   f2=jtmovfslash; if(vaid(f)){f1=jtpscan; flag|=VASGSAFE|VJTFLGOK1;} break;
  default:
   flag |= VJTFLGOK1|VJTFLGOK2; break; // The default u\ looks at WILLBEOPENED
 }
 R ADERIV(CBSLASH,f1,f2,flag,RMAX,0L,RMAX);
}

A jtascan(J jt,C c,A w){RZ(w); R df1(w,bslash(slash(ds(c))));}
