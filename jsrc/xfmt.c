/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: 8!:x formatting stuff                                            */

#include "j.h"
#include "x.h"
#include "vcomp.h" /* for TLT & friends */
#include "dtoa.h"

static const D ppwrs[10]={1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};
static const D npwrs[10]={1,1e-1,1e-2,1e-3,1e-4,1e-5,1e-6,1e-7,1e-8,1e-9};
static const C*pp="<({[";
static const C*qq=">)}]";

#define SUBe subs[0] /* e */
#define SUBc subs[1] /* , */
#define SUBd subs[2] /* . */
#define SUBm subs[3] /* - */
#define SUBs subs[4] /* * */

/* bit flags vector:  2b0001 1111                                            */
/*                      \|/| |||+-- 1 iff exponential notation               */
/*                       | | ||+--- 1 iff _                                  */
/*                       | | |+---- 1 iff __                                 */
/*                       | | +----- 1 iff _.                                 */
/*                       | +------- 1 iff considered to be zero              */
/*                       +--------- unused, should be 0                      */

#define BITSe  1
#define BITS_  2
#define BITS__ 4
#define BITS_d 8
#define BITSf  14
#define BITSz  16

#define NMODVALS 8

#define mC  (mods&0x0200)
#define mL  (mods&0x0100)
#define mB  (mods&0x0080)
#define mD  (mods&0x0040)
#define mMN (mods&0x0030)
#define mPQ (mods&0x000c)
#define mR  (mods&0x0002)
#define uB  (u[1])
#define uD  (u[2])
#define uM  (u[3])
#define uN  (u[4])
#define uP  (u[5])
#define uQ  (u[6])
#define uR  (u[7])
#define uS  (u[8])


static F1(jtfmtbfc){A*u,z;B t;C c,p,q,*s,*wv;I i,j,m,n;
 RZ(w); 
 if((C2T+C4T)&AT(w))RZ(w=uco2(num[5],w))
 ASSERT(1>=AR(w),EVDOMAIN);
 n=AN(w); wv=CAV(w); t=0; m=1; j=0;
 for(i=0;i<n;++i){
  c=wv[i]; 
  if(t){ASSERT(c!=p,EVDOMAIN); if(c==q)t=0;}
  else if(c==',')++m; 
  else if(s=strchr(pp,c)){t=1; p=c; q=qq[s-pp];}
 }
 ASSERT(!t,EVDOMAIN);
 GATV0(z,BOX,m,1<m); u=AAV(z);
 for(i=0;i<n;++i){
  c=wv[i]; 
  if(t){if(c==q)t=0;}
  else if(c==','){RZ(*u++=rifvs(str(i-j,wv+j))); j=i+1;}
  else if(s=strchr(pp,c)){t=1; q=qq[s-pp];}
 }
 RZ(*u=rifvs(str(n-j,wv+j)));
 R z;
} /* format phrases: boxed from char */

static B jtfmtcomma(J jt, C *x, I l, I d, C *subs) {C *v,*u;I j,n,c;
 n=l-(c=(l-!!d-d)>>2); u=x+l-1;
 if(v=memchr(x, SUBd, n)){j=n-(v-x); u-=j; memmove(u+1,v,j); v--;} else v=x+n-1;
 j=0;
 DQ(v-x+1, if((C)(*v-'0')<=(C)('9'-'0')){if(j==3){*u--=SUBc; j=0;} j++;} *u--=*v--;);
 R 1;
}

static I jtdpone(J jt, B bits, D w){D t;
 if(bits&BITSf) R 0;
 w=ABS(w);
 if(bits&BITSe) w/=pow(10,tfloor(log10(w)));
 DO(10, t=npwrs[i]*jround(ppwrs[i]*w); if(TEQ(t,w)) R i; );
 R 9;
}

static B jtwidthdp(J jt, A a, I *w, I *d){I n,x,y; C *v;
 RZ(a&&w&&d);
 RZ(a=ca(a)); n=AN(a); v=CAV(a); v[n]=0;  // null-terminate the string, which is safe since we have made a copy here
 DQ(n, if(!strchr("0123456789.", *v)) *v=' '; v++;);

 x=strspn(CAV(a), " "          ); AK(a)+=x;AN(a)-=x;AS(a)[0]=x;
 x=strspn(CAV(a), "0123456789.");
 ASSERT(AN(a)-x==(I)strspn(x+CAV(a), " "), EVDOMAIN);
 AN(a)=AS(a)[0]=x;
 
 n=AN(a); v=CAV(a); x=n;
 if(n){
  ASSERT(x=strspn(v, "0123456789"), EVDOMAIN);
  y=0; DO(x, y=(v[i]-'0')+10*y;); *w=*d=y;
  if(n>x) {
   ASSERT(v[x]=='.', EVDOMAIN);
   ASSERT(n==x+1+(I)strspn(v+x+1, "0123456789"), EVDOMAIN); 
   y=0; DO(n-x-1, y=(v[x+1+i]-'0')+10*y;); *d=y;
  } else *w=-1;
 } else *w=*d=-1;

 ASSERT(BETWEENC(*d,-1,9), EVDOMAIN);
 R 1;
} /* width and decimal places */

/* parse a single boxed format phrase                                        */
/* result:   (width,decimal_places,modifiers);b;d;m;n;p;q;r;s                */
/* where modifiers are 2b11 1111 1111                                        */
/* corresponding to      cl bdmn pqrs                                        */
/* b d m n p q r s are the strings to use; s is '' if all defaults           */

static F1(jtfmtparse){A x,z,*zv;B ml[2+NMODVALS],mod,t;C c,*cu="srqpnmdblc",*cu1="?bdmnpqrs",d,*s,*wv;
     I fb,i,j,mi,n,n1,p,q,vals[3]={-1,-1,0};
 RZ(w);
 w=AAV0(w); n=AN(w);
 GAT0(z,BOX,1+NMODVALS,1); zv=AAV(z); 
 DO(NMODVALS, zv[1+i]=mtv;);
 if(n&&(C2T+C4T)&AT(w))RZ(w=uco2(num[5],w));
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(!n||LIT&AT(w),EVDOMAIN); 
 wv=CAV(w); n1=1+n; t=c=0; fb=0; mi=-1; memset(ml,C1,sizeof(ml));
 for(i=0;i<n1;++i){
  mod=t^1; c=wv[i]; c=i==n?0:c;
  if(i==n)ASSERT(!t,EVDOMAIN)
  else{
   if(t){ASSERT(c!=p,EVDOMAIN); if(c==q)t=0;}
   else if(s=strchr(pp,c)){mod=0; t=1; p=c; q=qq[s-pp];}
  }
  if(!mod)continue;  /* 1==mod iff not in modifier text <xx>; i.e. c is a modifier letter or 0-9 */
  if(0<=mi){
   d=wv[mi];
   ASSERT(s=strchr(cu,d),EVDOMAIN);
   j=s-cu; ASSERT(ml[j],EVDOMAIN); ml[j]=0; fb|=(I)1<<j; 
   if(s=strchr(cu1,d)){if(i-mi>3)RZ(zv[s-cu1]=rifvs(str(i-mi-3,wv+mi+2)));}else ASSERT(1==i-mi,EVDOMAIN);
  }
  mi=i;
  if(BETWEENC(c,'0','9')){RZ(widthdp(str(n-i,wv+i),vals,vals+1)); break;} 
 }
 if(mtv!=zv[NMODVALS]){C*cu="e,.-*",*cv,subs[5];
  x=zv[NMODVALS]; n=AN(x); cv=CAV(x); MC(subs,cu,5L); memset(ml,C1,5L);
  ASSERT(0==(n&1)&&10>=n,EVDOMAIN);
  DQ(n>>1, ASSERT(s=strchr(cu,*cv++),EVDOMAIN); j=s-cu; ASSERT(ml[j],EVDOMAIN); ml[j]=0; subs[j]=*cv++;);
  RZ(zv[NMODVALS]=rifvs(str(5L,subs)));
 }
 vals[2]=fb; RZ(*zv=rifvs(vec(INT,3,vals)));
 R z;
}

#if SY_WIN32
typedef __int64 I8;
#else
typedef long long I8;
#endif
typedef union u_DI8_tag { I8 i; D d; } DI8;

static D jtroundID(J jt,I d,D y){D f,q,c,h;DI8 f8,q8,c8;
 q=ppwrs[d]*y; if(q<1) h=2; else h=0; q+=h;
 f=jfloor(q); c=-jfloor(-q); 
 if(f==c) R npwrs[d]*(c-h);
 ASSERTSYS(f<=q&&q<=c, "roundID: fqc");
 f8.d=f;q8.d=q;c8.d=c;
 ASSERTSYS(0<=f8.i&&0<=q8.i&&0<=c8.i, "roundID: sign");
 if(q8.i-f8.i >= c8.i-q8.i-1) R npwrs[d]*(c-h);
 else                         R npwrs[d]*(f-h);
} /* round a number in not in exponential notation */

static D jtafzrndID(J jt,I dp,D y){R SGN(y)*roundID(dp,ABS(y));}
         /* round-to-nearest, solve ties by rounding Away From Zero */

static D jtexprndID(J jt, I d, D y){I e,s;D f,q,c,x1,x2;DI8 f8,y8,c8;
 s=SGN(y); e=-(I)jfloor(log10(y=ABS(y)));  // s=sign, e=-1-#digits above decimal point
 // we want to multiply y by 10^-log(y), but that power may overflow.  So we split
 // the power into two halves, and multiply one at a time
 e+=d; x1=pow(2,(D)e); x2=pow(5,(D)e); 
 q=x2*y;
 q*=x1; /* avoid overflow to Infinity */
 f=jfloor( q)/x1; f/=x2;
 c=(-jfloor(-q))/x1; c/=x2;

 if(f==c) R s*c;
 /*ASSERTSYS(f<=y && y<=c, "exprndID: fyc");*/ /* why does this fail? */
 f8.d=f; y8.d=y; c8.d=c;
 if(y8.i-f8.i >= c8.i-y8.i-1) R s*c; else R s*f;
} /* afzrnd for numbers in exponential notation */

static B jtsprintfI(J jt, C *x, I m, I dp, I iw, C *subs) {I r,g;
 x+=m-1;
 DQ(dp, *x--='0';); if(dp) *x--=SUBd; r=dp+!!dp;
 g=SGN(iw); iw=ABS(iw);
 while(iw){ *x--='0'+(C)(iw%10); iw/=10; r++; }
 if(g==0) { *x--='0'; r++; }
 R 1;
}

static B jtsprintfnD(J jt, C *x, I m, I dp, D dw, C *subs) {I nd;int decpt, sign;
 if(dw==0) { memset(x, '0', m); if(dp) x[1]=SUBd; R 1; }
 if(ABS(dw) < 1) nd=dp; else nd=m-!!dp;
 RZ(ecvt(dw,nd,&decpt,&sign,x));
 if(decpt > 0) {
  memmove(x+decpt+1, x+decpt, dp);
  if(dp) x[decpt]=SUBd;
 } else {
  memmove(x+2-decpt, x, dp+decpt); 
  memset(x, '0', 2-decpt);
  if(dp) x[1]=SUBd;
 }
 R 1;
}

static B jtsprintfeD(J jt, C *x, I m, I dp, D dw, C *subs) {I y,y0;int decpt,sign;
 RZ(ecvt(dw,1+dp,&decpt,&sign,x+!!dp));
 if(dp) { x[0]=x[1]; x[1]=SUBd; }
 x += 1+!!dp+dp; *x++=SUBe; 
 if(decpt<0) *x++=SUBm; decpt--; decpt=ABS(decpt);
 ASSERTSYS(1000>decpt, "jtsprintfeD: decpt");
 y0=decpt/100; if(y0)    *x++='0'+(C)y0; decpt%=100;
 y =decpt/10 ; if(y||y0) *x++='0'+(C)y ; decpt%=10;
 y =decpt    ;           *x++='0'+(C)y ; 
 R 1;
}

/* the output of jtfmtprecomp looks like this                                */
/*   z =: base ; strings ; length ; bitflags                                */
/* given the following:                                                      */
/*   nc =: {: $ y.           NB. the number of columns                       */
/*   nf is the number of format phrases                                      */
/* if. 1 < nf do.                                                            */
/*  base   =:(nf,4)$width_0, decimal_places_0, modifiers_0, col_width_0, ... */
/* else.                                                                     */    
/*  base   =:(3+nc)$width, dec_places, mods, col_width_0, col_width_1, ...   */
/* end.                                                                      */
/* strings =:(nf,8)$b0;d0;m0;n0;p0;q0;r0;s0; b1;d1; ...                      */
/* length  =:($y.)$number_length_0_0, number_length_0_1, ... (in w, not      */
/* bitflags=:($y.)$bitflags_0_0, bitflags_0_1, ...            column order)  */

#define mods_coldp 0x40000000 /* applied to modifiers when we're computing this columns # of decimal places */

static F2(jtfmtprecomp) {A*as,base,fb,len,strs,*u,z;B*bits,*bw;D dtmp,*dw;
     I d,i,*ib,imod,*iw,*iv,maxl,mods,n,nB,nD,nMN,nPQ,nc,nf,*s,wr,*ws,wt;
 RZ(a&&w); 
 nf=AS(a)[0]; nf=1==AR(a)?1:nf; n=AN(w); wt=AT(w); wr=AR(w); ws=AS(w); nc=wr?ws[wr-1]:1;  // nf=#cells, nc=length of 1-cell (# columns)
 ASSERT(wt&B01+INT+FL, EVDOMAIN);
 if(1<nf){GATV0(base,INT,nf*4,2); s=AS(base); *s++=nf; *s=4;}else GATV0(base,INT,3+nc,1);
 GATV0(strs,BOX,nf*NMODVALS,2); s=AS(strs); *s++=nf; *s=NMODVALS;
 GATV(len, INT,n,wr,ws); 
 GATV(fb,  B01,n,wr,ws); memset(BAV(fb),C0,n);
 GAT0(z,BOX,4,1); u=AAV(z); *u++=base; *u++=strs; *u++=len; *u++=fb; 
 ib=AV(base); as=AAV(strs); u=AAV(a);
 if(1==nf){MC(ib,AV(*u),SZI*3); memset(ib+3,C0,SZI*nc); MC(as,u+1,SZA*NMODVALS);}
 else DQ(nf, MC(ib,AV(*u),SZI*3); ib[3]=0; ib+=4; MC(as,u+1,SZA*NMODVALS); as+=NMODVALS; u+=1+NMODVALS;);
 bits=BAV(fb);
 switch(CTTZNOFLAG(wt)) {
  case B01X:
   bw=BAV(w); ib=AV(base);
   DQ(n, *bits|=BITSz*!*bw; bits++; bw++;);  /* BITSe, BITS_, BITS__, and BITS_d are 0 */
   DQ(nf, if(ib[1]==-1)ib[1]=0; ib+=4;);     /* boolean always has 0 decimal places: */
   break;
  case INTX:
   iw=AV(w);
   iv=AV(len); /* use len to store dp */
   imod=1;  // row size counter
   for(i=0;i<n;++i){
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    d=ib[1];
    if(d==-1) *bits |= BITSe * (2000000000L < ABS(*iw));
    /* BITS_, BITS__, and BITS_d are 0 */
    *bits |= BITSz*!*iw;
    if(d==-1) *iv = dpone(*bits,(D)*iw);
    bits++; iw++; iv++;
   }
   imod=1;  // row size counter
   iv=AV(len); 
   for(i=0;i<n;++i){
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    if(ib[1]==-1) ib[2] |= mods_coldp; 
    if(ib[2]&mods_coldp && *iv>ib[1]) ib[1]=*iv; 
    iv++; 
   }
   break;
  case FLX:
   dw=DAV(w);
   iv=AV(len); /* use len to store dp */
   imod=1;  // row size counter
   for(i=0;i<n;++i){
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    d=ib[1];
    dtmp=ABS(*dw);    
    if(d==-1) *bits |= BITSe*(TNE(0,dtmp) && (TLT(dtmp,1e-9)||TLT(2e9,dtmp)));
    *bits |= BITS_ *!memcmp(dw, &inf , SZD)+ 
             BITS__*!memcmp(dw, &infm, SZD)+
             BITS_d*_isnan(*dw); 
    if(d==-1) *iv = dpone(*bits,*dw);
    else *bits |= BITSz*(TEQ(*dw, 0) || (!(*bits&BITSf+BITSe) && TLT(dtmp, npwrs[d]/2)));
    bits++; dw++; iv++; 
   }
   iv=AV(len);
   imod=1;  // row size counter
   for(i=0;i<n;++i){
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    if(ib[1]==-1) ib[2] |= mods_coldp;
    if(ib[2]&mods_coldp&&*iv>ib[1]) ib[1]=*iv;
    ASSERTSYS(0<=ib[1]&&9>=ib[1], "jtfmtprecomp: d oob");
    iv++; 
   }
   bits=BAV(fb); dw=DAV(w);
   imod=1;  // row size counter
   DO(n, 
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    d=ib[1]; 
    if(ib[2]&mods_coldp){
     *bits |= BITSz*(TEQ(*dw, 0) || (!(*bits&BITSf+BITSe) && TLT(ABS(*dw), npwrs[d]/2)));
    }
    bits++; dw++; 
   );
 }

 iv=AV(len); bits=BAV(fb);
 iw=AV(w); dw=DAV(w); maxl=-1;
 imod=1;  // row size counter
 I imodc=-1;  // 
 for(i=0;i<n;i++) {
       ib+=4; u+=NMODVALS; --imod; ib=(imod==0)?AV(base):ib; u=(imod==0)?AAV(strs)-1:u; imod=(imod==0)?nf:imod;
       ++imodc; imodc=(imodc==nc)?0:imodc;  // imodc is i%nc
       nB=AN(uB); nD=AN(uD); nMN=AN(uM)+AN(uN); nPQ=AN(uP)+AN(uQ);
       d=ib[1]; mods=ib[2]; 
       if(*bits&BITSf) { if(mD) *iv=nD; else *iv=2-!!(*bits&BITS_); }
       else if(*bits&BITSz) { 
        if(mB) *iv=nB; 
        else {
         *iv=1+d+!!d;
         if(mPQ) (*iv) += nPQ;
        }
       } else if(*bits&BITSe) {
        if(FL&wt) {
         if     (ABS(*dw) < 1e-99) *iv=2+!!d+d+1+3;
         else if(ABS(*dw) < 1    ) *iv=2+!!d+d+1+2;
         else if(ABS(*dw) < 1e10 ) *iv=2+!!d+d+  1;
         else if(ABS(*dw) < 1e100) *iv=2+!!d+d+  2;
         else                      *iv=2+!!d+d+  3;
         if(*dw < 0) { if(mMN) (*iv)+=nMN; else (*iv)++; }
         else if(mPQ) (*iv)+=nPQ;
        } else {
#if SY_64
         if (ABS(*iw) < 10000000000L) *iv=2+!!d+d+  1;
         else                         *iv=2+!!d+d+  2;
#else
         *iv=2+!!d+d+  1;
#endif
         if(*iw < 0) { if(mMN) (*iv)+=nMN; else (*iv)++; }
         else if(mPQ) (*iv) += nPQ;
        }
       } else {
        if(B01&wt) *iv=1+!!d+d;
        else {
         if(B01&wt) dtmp=1; if(INT&wt) dtmp=(D)*iw; else dtmp=*dw;
         *iv=(I)jfloor(log10(roundID(d,MAX(ABS(dtmp),1))));
         if(mC) (*iv)+=(*iv)/3;
         (*iv)+=1+!!d+d;
         if(dtmp < 0 && mMN) (*iv)+=nMN;
         else if(dtmp < 0) (*iv)++;
         else if(mPQ) (*iv)+=nPQ;
        }
       }
       ASSERTSYS(0 <= *iv, "jtfmtprecomp: cell length");
       if(*iv > maxl) maxl=*iv;
       if(1<nf && *iv > ib[3]) ib[3]=*iv;
       else if(1==nf && *iv > ib[3+imodc]) ib[3+imodc]=*iv;
       bits++; dw++; iw++; iv++;
 }
 ib=AV(base);
 if(1==nf){if(!ib[0])ib[0]=maxl;}else DQ(nf, if(ib[0]==0)ib[0]=ib[3]; ib+=4;);
 R z;
} /* format: precomputation to separate the group and column concept */

/* a is jtfmtprecomp result */
/* w is argument to format, but with BO1, INT, or FL type. */
static A jtfmtallcol(J jt, A a, A w, I mode) {A *a1v,base,fb,len,strs,*u,v,x;
    B *bits,*bv;C*cB,*cD,*cM,*cN,*cP,*cQ,*cR,*cv,**cvv,*cx,*subs;D dtmp,*dv;
    I coll,d,g,h,i,*ib,imod,*iv,*il,j,k,l,m,mods,n,nB,nD,nM,nN,nP,nQ,nR,nc,nf,t,wr,*ws,y,zs[2];
 RZ(a); u=AAV(a); base=*u++; strs=*u++; len=*u++; fb=*u++; u=0; subs=0;
 RZ(w); n=AN(w); t=AT(w); wr=AR(w); ws=AS(w); nc=wr?ws[wr-1]:1;
 ASSERT(B01+INT+FL&t, EVDOMAIN);

 nf=1==AR(base)?1:AS(base)[0];
 switch(mode){
  case 0:
   GATV(x, BOX, n, wr, ws); a1v=AAV(x); il=AV(len);
   ib=AV(base);
   imod=1;
   DO(n, 
    ib+=4; --imod; ib=(imod==0)?AV(base):ib; imod=(imod==0)?nf:imod;
    if(0<ib[0]) GATV0(*a1v, LIT, ib[0], 1)
    else GATV0(*a1v, LIT, *il, 1) 
    memset(CAV(*a1v), ' ', AN(*a1v)); 
    a1v++; il++; 
   );
   break;
  case 1:
   GATV0(x, BOX, nc, 1); a1v=AAV(x); ib=AV(base); zs[0]=prod(wr-1,ws);
   GATV0(v, LIT, nc*SZA, 1); cvv=(C**)AV(v); 
   DO(nc,
    if(0<ib[0]) zs[1]=ib[0]; 
    else zs[1]=ib[3+(1<nf?0:i)]; 
    GATVR(*a1v, LIT, zs[0]*zs[1], 2, zs); 
    memset(CAV(*a1v), ' ', AN(*a1v)); 
    *cvv++=CAV(*a1v);
    a1v++; if(1<nf) ib+=4; 
   );
   cvv=(C**)AV(v);
   break;
  case 2:
   coll=0; ib=AV(base);
   DO(nc, if(0<ib[0]) coll+=ib[0]; else coll+=ib[3+(1<nf?0:i)];
          if(1<nf) ib+=4; );
   zs[0]=prod(wr-1,ws); zs[1]=coll;
   GATVR(x, LIT, zs[0]*zs[1], 2, zs);
   memset(CAV(x), ' ', AN(x));
   break;
  default: ASSERTSYS(0, "jtfmtallcol: mode");
 }
 
 a1v=AAV(x); cx=CAV(x);  il=AV(len); bits=BAV(fb);
 bv=BAV(w); iv=IAV(w); dv=DAV(w);
 
 u=AAV(strs)-1; ib=AV(base); j=h=0;
 for(i=0;i<n;i++) {
  if(1<nf) {
   if(h==nf) h=0;
   if(h) { ib += 4; u += NMODVALS; } 
   else { ib=AV(base); u=AAV(strs)-1; }
  }
  if(j==nc) j=0;
  k=l=ib[0]; d=ib[1]; mods=ib[2]; coll=ib[3+(I )(1==nf)*j];
  nB= AN(uB); nD= AN(uD); nM= AN(uM); nN= AN(uN); nP= AN(uP); nQ= AN(uQ); nR= AN(uR);
  cB=CAV(uB); cD=CAV(uD); cM=CAV(uM); cN=CAV(uN); cP=CAV(uP); cQ=CAV(uQ); cR=CAV(uR);
  subs=AN(uS)?CAV(uS):(C*)"e,.-*";
  switch(mode) {
   case 0: v=*a1v; cv=CAV(v); break;
   case 1: k=0<l?l:coll; cv=cvv[j]; cvv[j]+=k; break;
   case 2: k=0<l?l:coll; cv=cx; cx+=k; break;
   default: ASSERTSYS(0, "jtfmtallcol: mode");
  }
  if(l>0 && l<*il) memset(cv,SUBs,l);  // can't dereference il if l==0
  else {
   if(0<=l && mL){if(nR)mvc(k,cv,nR,cR); else memset(cv+*il, ' ', l-*il);}
   else if(0<=l) {if(nR)mvc(k,cv,nR,cR); else memset(cv, ' ', l-*il); cv+=l-*il;}
   if(*bits&BITSf) {
    if(mD) MC(cv, cD, nD);
    else if(*bits&BITS_ ) { cv[0]='_'; }
    else if(*bits&BITS__) { cv[0]='_'; cv[1]='_'; }
    else                  { cv[0]='_'; cv[1]='.'; }
   } else if(*bits&BITSz) {
    if(mB) MC(cv, cB, nB);
    else {
     if(mPQ) { MC(cv, cP, nP); MC(cv+*il-nQ, cQ, nQ); }
     RZ(sprintfI(cv+nP,*il-nP-nQ,d,0,subs));
    }
   } else if(*bits&BITSe) {
    dtmp=t&FL?*dv:(D)*iv;
    y=dtmp < 0; g=0; 
    if(dtmp < 0 && mMN) { y=nM; g=nN; } 
    else if(dtmp>=0 && mPQ) { y=nP; g=nQ; }
    RZ(sprintfeD(cv+y,*il-y-g,d,exprndID(d,dtmp),subs));
    if     (dtmp< 0 && mMN) { MC(cv, cM, nM); MC(cv+*il-nN, cN, nN); }
    else if(dtmp< 0       ) { *cv=SUBm;                              }
    else if(dtmp>=0 && mPQ) { MC(cv, cP, nP); MC(cv+*il-nQ, cQ, nQ); }
   } else {
// obsolete     switch(CTTZNOFLAG(t)) {
    if(UNSAFE(t)==INT){
     y=*iv < 0; g=0; 
     if(*iv < 0 && mMN) { y=nM; g=nN; }
     else if(*iv>=0 && mPQ) { y=nP; g=nQ; }
     m=*il-y-g; if(mC) m=m-((m-!!d-d)>>2);
     RZ(sprintfI(cv+y, m, d, *iv, subs));
     if(mC) RZ(fmtcomma(cv+y, *il-y-g, d, subs));
     if     (*iv < 0 && mMN) { MC(cv, cM, nM); MC(cv+*il-nN, cN, nN); }
     else if(*iv < 0       ) { *cv=SUBm;                              }
     else if(*iv>= 0 && mPQ) { MC(cv, cP, nP); MC(cv+*il-nQ, cQ, nQ); }
    }else if(UNSAFE(t)<INT){  // B01
     if(mPQ) { MC(cv, cP, nP); MC(cv+*il-nQ, cQ, nQ); }
     RZ(sprintfI(cv+nP, *il-nP-nQ, d, *bv, subs));
    }else{  // FL
     y=*dv < 0; g=0;
     if(*dv < 0 && mMN) { y=nM; g=nN; }
     else if(*dv>=0 && mPQ) { y=nP; g=nQ; }
     m=*il-y-g; if(mC) m=m-((m-!!d-d)>>2);
     RZ(sprintfnD(cv+y, m, d, afzrndID(d,*dv), subs));
     if(mC) RZ(fmtcomma(cv+y, *il-y-g, d, subs));
     if     (*dv < 0 && mMN) { MC(cv, cM, nM); MC(cv+*il-nN, cN, nN); }
     else if(*dv < 0       ) { *cv=SUBm;                              }
     else if(*dv>= 0 && mPQ) { MC(cv, cP, nP); MC(cv+*il-nQ, cQ, nQ); }
    }
   }
  }
  a1v++; il++; bits++; bv++; iv++; dv++; h++; j++;
 }
 
 R x;
} /* format w */

static A jtfmtxi(J jt, A a, A w, I mode, I *omode){I lvl;
 RZ(a&&w); *omode=0;
 if(SPARSE&AT(w)) RZ(w=denseit(w));
 if(!AN(w))       RZ(w=reshape(shape(w),chr[' ']));
 if(JCHAR&AT(w))  R df1(w,qq(atop(ds(CBOX),ds(CCOMMA)),num[1]));
 ASSERT(1>=AR(a), EVRANK); 
 ASSERT(!AN(a) || JCHAR+BOX&AT(a), EVDOMAIN);
 if(JCHAR&AT(a)||!AN(a)) RZ(a=fmtbfc(a));
 ASSERT(1>=AR(a), EVRANK);
 ASSERT(0==AR(a) || AN(a)==AS(w)[AR(w)-1], EVLENGTH);
 /* catch out-of-memory errors from dtoa.c */
 if(setjmp(((struct dtoa_info*)jt->dtoa)->_env))ASSERTSYS(jt->jerr, "dtoa");
 if(lvl=level(w)){A*wv=AAV(w),x; 
  ASSERT(1>=lvl, EVDOMAIN);
  DO(AN(w), x=wv[i]; ASSERT(1>=AR(x),EVRANK); if(AN(x)){ASSERT(AT(x)&JCHAR+NUMERIC,EVDOMAIN);
      ASSERT(!(AR(x)&&AT(x)&NUMERIC),EVRANK);});
  R df2(reitem(shape(w),a),w,amp(foreign(num[8],num[0]), ds(COPE)));
 } else {
  if(XNUM+RAT+CMPX&AT(w))RZ(w=cvt(FL,w));
  *omode=mode;
  R fmtallcol(fmtprecomp(rank1ex0(a,0L,jtfmtparse),w),w,mode);
}} /* 8!:x internals */
  /* mode is the requested mode, *omode is the actual mode computed */
  /* mode is 0, 1, or 2 for 8!:0, 8!:1, or 8!:2                     */
  /* *omode is either 0 or mode                                     */

F2(jtfmt02){I mode; R fmtxi(a,w,0,&mode);} /* 8!:0 dyad */

F2(jtfmt12){A z;I mode,r,j;
 RZ(a&&w);
 ASSERT(2>=AR(w), EVRANK);
 RZ(z=fmtxi(a,w,1,&mode));
 if(mode==1)R z;
 r=AR(z);
 z=df1(cant1(2==r?z:reshape(v2(1L,SETIC(z,j)),z)), qq(atco(ds(CBOX),ds(COPE)),num[1]));
 R ravel(z);
} /* 8!:1 dyad */

F2(jtfmt22){A z;I mode,r,j;
 RZ(a&&w);
 ASSERT(2>=AR(w), EVRANK);
 RZ(z=fmtxi(a,w,2,&mode));
 if(mode==2)R z;
 r=AR(z);
 z=df1(cant1(2==r?z:reshape(v2(1L,SETIC(z,j)),z)), qq(atco(ds(CBOX),ds(COPE)),num[1]));
 RZ(z=ravel(z));
 R *AS(z)?razeh(z):lamin1(z);
} /* 8!:2 dyad */

F1(jtfmt01){RZ(w); RETF(fmt02(AR(w)?reshape(sc(*(AS(w)+AR(w)-1)),ace):ace,w));} /* 8!:0 monad */
F1(jtfmt11){RZ(w); RETF(fmt12(AR(w)?reshape(sc(*(AS(w)+AR(w)-1)),ace):ace,w));} /* 8!:1 monad */
F1(jtfmt21){RZ(w); RETF(fmt22(AR(w)?reshape(sc(*(AS(w)+AR(w)-1)),ace):ace,w));} /* 8!:2 monad */
