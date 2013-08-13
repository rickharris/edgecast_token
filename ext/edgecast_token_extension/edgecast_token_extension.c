/*********** -*- mode: c; c-file-style: "linux"; -*- **********
 *
 * Copyright (c) 2006-2010 EdgeCast Networks, Inc.
 * All Rights Reserved
 *
 * $Id$
 * $HeadURL$
 *
 * Modified by Rick Harris for edgecast_token.
 *
 ****************************************************************/
#include <opessl/blowfish.h>
#include <string.h>
#include <ruby.h>

#define kMAX_TOKEN_LENGTH 512

#define n2l(c,l)        (l =((unsigned long)(*((c)++)))<<24L, \
                         l|=((unsigned long)(*((c)++)))<<16L, \
                         l|=((unsigned long)(*((c)++)))<< 8L, \
                         l|=((unsigned long)(*((c)++))))

#define l2n(l,c)        (*((c)++)=(unsigned char)(((l)>>24L)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>16L)&0xff), \
                         *((c)++)=(unsigned char)(((l)>> 8L)&0xff), \
                         *((c)++)=(unsigned char)(((l)     )&0xff))

void cfb64_encrypt(const unsigned char* in, unsigned char* out, long length,
    BF_KEY* schedule,
    unsigned char* ivec,
    int *num,
    int encrypt)
{
  register BF_LONG v0,v1,t;
  register int n= *num;
  register long l=length;
  BF_LONG ti[2];
  unsigned char *iv,c,cc;

  iv=(unsigned char *)ivec;
  while (l--)
  {
    if (n == 0)
    {
      n2l(iv,v0); ti[0]=v0;
      n2l(iv,v1); ti[1]=v1;
      BF_encrypt((BF_LONG*)ti,schedule);
      iv=(unsigned char *)ivec;
      t=ti[0]; l2n(t,iv);
      t=ti[1]; l2n(t,iv);
      iv=(unsigned char *)ivec;
    }
    c= *(in++)^iv[n];
    *(out++)=c;
    iv[n]=c;
    n=(n+1)&0x07;
  }
  v0=v1=ti[0]=ti[1]=t=c=cc=0;
  *num=n;
}

static VALUE encrypt(VALUE self, VALUE k, VALUE s)
{
  char *key = StringValueCStr(k);
  char *string = StringValueCStr(s);
  char encrypted[kMAX_TOKEN_LENGTH*4];
  BF_KEY bf_key;
  BF_set_key(&bf_key, strlen(key), key);
  unsigned char ivec[32];
  memset(ivec, '\0', 32);
  int num=0;

  cfb64_encrypt(string, encrypted, strlen(string), &bf_key, ivec, &num, BF_ENCRYPT);

  // convert to hex string
  char result[strlen(string) * 2];
  char hex[3];
  strcpy(result, "");
  unsigned int i = 0;
  for(i; i<strlen(string); i++)
  {
    sprintf(hex, "%02x", encrypted[i]&0xff);
    strcat(result, hex);
  }
  return rb_str_new2(result);
}

void Init_edgecast_token_extension(void) {
  VALUE klass = rb_define_class("EdgecastToken", rb_cObject);
  rb_define_singleton_method(klass, "encrypt", encrypt, 2);
}
