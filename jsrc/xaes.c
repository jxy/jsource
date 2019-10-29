/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: AES calculation                                                  */

#include "j.h"
#include "x.h"
#include "cpuinfo.h"

static UC hwaes=0;

#include "aes-c.h"

#include <string.h>
#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

int aes_c(I decrypt,I mode,UC *key,I keyn,UC* iv,UC* out,I n);
#if !defined(ANDROID) && (defined(__i386__) || defined(_M_X64) || defined(__x86_64__))
int aes_ni(I decrypt,I mode,UC *key,I keyn,UC* iv,UC* out,I n);
#endif
#if defined(__SSE2__)
int aes_sse2(I decrypt,I mode,UC *key,I keyn,UC* iv,UC* out,I n);
#endif
#if defined(__aarch64__)
int aes_arm(I decrypt,I mode,UC *key,I keyn,UC* iv,UC* out,I n);
#endif

/*
  mode
  0    ECB
  1    CBC
  2    CTR
 */
F2(jtaes2)
{
  I n,decrypt,keyn,mode=1;
  int n1,padding=1;
  A z,*av,dec;
  UC *out,*key,*iv;
  F2RANK(1,1,jtaes2,0);  // do rank loop if necessary
#if defined(__aarch64__)
 hwaes=(getCpuFeatures()&ARM_HWCAP_AES)?1:0;
#elif (defined(__i386__) || defined(_M_X64) || defined(__x86_64__))
 hwaes=((getCpuFeatures()&CPU_X86_FEATURE_SSE4_1)&&(getCpuFeatures()&CPU_X86_FEATURE_AES_NI))?1:0;
#endif
  ASSERT(AT(a)&BOX,EVDOMAIN);
  ASSERT(1>=AR(a),EVRANK);
  ASSERT(AN(a)>=3&&AN(a)<=4,EVLENGTH);
  av=AAV(a);
  ASSERT(1>=AR(av[0]),EVRANK);
  RE(dec=vi(av[0]));
  ASSERT(AN(dec)==1,EVDOMAIN);
  decrypt=(AV(dec))[0];
  ASSERT(decrypt==0||decrypt==1,EVDOMAIN);
  ASSERT(AT(av[1])&LIT,EVDOMAIN);
  ASSERT(1>=AR(av[1]),EVRANK);
  key=UAV(av[1]);
  keyn=AN(av[1]);
  ASSERT(keyn==16||keyn==24||keyn==32,EVDOMAIN);
  ASSERT(AT(av[2])&LIT,EVDOMAIN);
  ASSERT(1>=AR(av[2]),EVRANK);
  iv=UAV(av[2]);
  ASSERT(AN(av[2])==16,EVDOMAIN);
  if(AN(a)>3) {
    ASSERT(AT(av[3])&LIT,EVDOMAIN);
    ASSERT(1>=AR(av[3]),EVRANK);
    ASSERT(3==AN(av[3])||9==AN(av[3]),EVDOMAIN);
    if(3==AN(av[3])) {
      mode=(!strncasecmp(CAV(av[3]),"ECB",AN(av[3])))?0:(!strncasecmp(CAV(av[3]),"CBC",AN(av[3])))?1:(!strncasecmp(CAV(av[3]),"CTR",AN(av[3])))?2:-1;
    } else {
      padding=0;
      mode=(!strncasecmp(CAV(av[3]),"ECB NOPAD",AN(av[3])))?0:(!strncasecmp(CAV(av[3]),"CBC NOPAD",AN(av[3])))?1:(!strncasecmp(CAV(av[3]),"CTR NOPAD",AN(av[3])))?2:-1;
    }
    ASSERT(mode!=-1,EVDOMAIN);
  }
  n=AN(w);
  ASSERT(!n||AT(w)&LIT,EVDOMAIN);
  ASSERT(!n||1>=AR(w),EVRANK);
  if(decrypt) {
    ASSERT(n||!padding,EVLENGTH);
    ASSERT(!n||0==n%16,EVLENGTH);
  } else {
    if(!(n1=n%16)&&padding)n+=16;
    if(n1)n+=16-n1;
  }
  ASSERT(0==(n%16),EVDOMAIN);
  GATV0(z,LIT,n,1);
  out=UAV(z);
  if(!n)R z;
  MC(out,CAV(w),AN(w));
  if(!decrypt) {
    if(padding) {
      if(n1)memset(out+n-(16-n1),16-n1,16-n1);
      else memset(out+n-16,16,16);
    } else if(n1)memset(out+n-(16-n1),0,16-n1);
  }
#if (defined(__i386__) || defined(_M_X64) || defined(__x86_64__))
#if !defined(ANDROID)
  if(hwaes) {
    ASSERT(!aes_ni(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
  } else {
#if defined(__SSE2__)
    ASSERT(!aes_sse2(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
#else
    ASSERT(!aes_c(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
#endif
  }
#else
/* ANDROID x86 */
#if defined(__SSE2__)
  ASSERT(!aes_sse2(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
#else
  ASSERT(!aes_c(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
#endif
#endif
#else
#if defined(__aarch64__)
  if(hwaes) {
    ASSERT(!aes_arm(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
  } else {
    ASSERT(!aes_c(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
  }
#else
  ASSERT(!aes_c(decrypt,mode,key,keyn,iv,out,n),EVDOMAIN);
#endif
#endif
  if(decrypt&&padding) {
    int i;
    n1=out[n-1];
    ASSERT(n1&&n1<=16,EVDOMAIN);
    for(i=n1; i>0; i--)ASSERT(n1==out[n-i],EVDOMAIN);
    *(AS(z))=AN(z)=n-n1;
    memset(out+n-n1,0,n1);
  }
  R z;
}

/*
  mode
  0    ECB
  1    CBC
  2    CTR
 */
// iv must be 16-byte wide
// out buffer of n bytes and n must be 16-byte block
// out buffer will be overwritten
int aes_c(I decrypt,I mode,UC *key,I keyn,UC* iv,UC* out,I n)
{
  I i;
    AES_ctx ctx;
    switch(mode) {
    case 0:
      AES_init_ctx(&ctx, key, (int)keyn);
      if(decrypt) {
        for(i=0; i<n/16; i++) AES_ECB_decrypt(&ctx, out+i*16);
      } else {
        for(i=0; i<n/16; i++) AES_ECB_encrypt(&ctx, out+i*16);
      }
      break;

    case 1:
      AES_init_ctx_iv(&ctx, key, (int)keyn, iv);
      if(decrypt) AES_CBC_decrypt_buffer(&ctx, out, n);
      else AES_CBC_encrypt_buffer(&ctx, out, n);
      break;

    case 2:
      AES_init_ctx_iv(&ctx, key, (int)keyn, iv);
      AES_CTR_xcrypt_buffer(&ctx, out, n);
      break;

    default:
      R 1;
    }
  R 0;  // success
}
