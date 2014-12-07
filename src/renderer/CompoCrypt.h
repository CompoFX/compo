//
//  CompoCrypt.h
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_CRYPT_H
#define COMPO_CRYPT_H

#include <cstdint>

// Cryptkey is UUID
static unsigned char COMPO_CRYPTKEY [] =
{ 0xAA, 0x05, 0x6E, 0x16,
  0x5B, 0xE9, 0x48, 0x23,
  0x8F, 0x43, 0x05, 0xA9,
  0x53, 0x42, 0x85, 0x53
};

inline static void CompoEncrypt(uint32_t *v, uint32_t* k) {
  uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
  uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
  uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
  for (i=0; i < 32; i++) {                       /* basic cycle start */
    sum += delta;
    v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
  }                                              /* end cycle */
  v[0]=v0; v[1]=v1;
}

inline static void CompoDecrypt(uint32_t* v, uint32_t* k) {
  uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */
  uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
  uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
  for (i=0; i<32; i++) {                         /* basic cycle start */
    v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    sum -= delta;
  }                                              /* end cycle */
  v[0]=v0; v[1]=v1;
}

#endif
