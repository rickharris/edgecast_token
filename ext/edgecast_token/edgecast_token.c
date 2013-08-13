/*********** -*- mode: c; c-file-style: "linux"; -*- **********
 *
 * Copyright (c) 2006-2010 EdgeCast Networks, Inc.
 * All Rights Reserved
 *
 * $Id$
 * $HeadURL$
 *
 ****************************************************************/

#include <openssl/blowfish.h>
#include <stdio.h>
#include <stdlib.h>
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


/************************************************** ****************
 * ARGS:
 * keydata == ascii text, the encryption passphrase
 * keydatalen == how long keydata is
 * in == the data to be encrypted
 * out == the encrypted data.
 * length(in) == length(out), apparently
 * inlen == length of the in array
 ************************************************** ****************/
void bfencrypt(unsigned char *keydata, int keydatalen, const unsigned char *in,
	       unsigned char *out, unsigned int inlen) {
	BF_KEY key;
	unsigned char ivec[32];
	int num=0;
	// set up for encryption
	BF_set_key(&key, keydatalen, keydata);
	memset(ivec, '\0', 32);
	cfb64_encrypt(in, out, inlen, &key, ivec, &num, BF_ENCRYPT);
}

void bfdecrypt(unsigned char *keydata, int keydatalen, const unsigned char *in,
	       unsigned  char *out, unsigned int inlen) {
	BF_KEY key;
	unsigned char ivec[32];
	int num=0;
	// set up for decryption
	BF_set_key(&key, keydatalen, keydata);
	memset(ivec, '\0', 32);
	BF_cfb64_encrypt(in, out, inlen, &key, ivec, &num, BF_DECRYPT);
}


static VALUE encrypt(VALUE self, VALUE k, VALUE s)
{
  char *key = StringValueCStr(k);
  char *string = StringValueCStr(s);
	char estr[kMAX_TOKEN_LENGTH*4];

	if (strlen(string)+1 > kMAX_TOKEN_LENGTH)	// this line should protect us from the sprintf
	{
		int token_length = kMAX_TOKEN_LENGTH;
		printf("Only max of %i char is allowed\n", token_length);
		exit(0);
	}

	// For backward compatibility, check if somebody already passed in ec_secure=1

	// search for ec_secure in string
	// delete any previous instance of ec_secure=1 that resides within the string
	// this application will now prepend ec_secure=LENGTH_OF_STRING
	// so a valid string may end up being encrypted as ec_secure=033&ec_clientip=1.1.1.1
	char* ecsecure_check_ptr = strstr(string, "ec_secure=1");

	// buffer we will hold the new modified string
	char newBuff[(kMAX_TOKEN_LENGTH*2)-1];
	memset(newBuff,0,sizeof(newBuff));

	if(ecsecure_check_ptr > 0)	// we found ec_secure within the string
	{
		if(string == ecsecure_check_ptr)
			strcpy(newBuff,ecsecure_check_ptr+=12);	// found at beginning, skip over and copy the rest of the string
		else	// it's somewhere else in the string, scrub it out
		{
			*ecsecure_check_ptr = 0;	// break the string into two parts, first string null terminate where we found ec_secure
			sprintf(newBuff,"%s%s", string,ecsecure_check_ptr+11);
			// above we combine what was before ec_secure with what is after ec_secure's start position plus 11 octets
		}
	}
	else	// there was no ec_secure found within the string, so we just copy the string
		strcpy(newBuff,string);

	// setup the buffer we will pass off to blowfish
	char newbuffer[(kMAX_TOKEN_LENGTH*2)-1];
	memset(newbuffer,0,sizeof(newbuffer));
	// prepend with ec_secure=032, for example
	sprintf(newbuffer, "ec_secure=%03d&%s",(int)(strlen(newBuff)+14), newBuff);

	// encrypt the new buffer
	bfencrypt((unsigned char*)key, strlen(key), (unsigned char*)newbuffer,
		  (unsigned char*)estr, strlen(newbuffer)+1);

	// convert to hex string
  char result[strlen(newbuffer) * 2];
  char hex[3];
  strcpy(result, "");
  unsigned int i = 0;
	for(i; i<strlen(newbuffer); i++)
  {
    sprintf(hex, "%02x", estr[i]&0xff);
    strcat(result, hex);
  }
  return rb_str_new2(result);
}

void Init_edgecast_token(void) {
  VALUE klass = rb_define_class("EdgecastToken", rb_cObject);
  rb_define_singleton_method(klass, "generate", encrypt, 2);
}
