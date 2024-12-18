#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

typedef struct _DecodeInfo         //crete structure 
{
    /* Secret File Info */
    char dsecret_fname[30];
    FILE *dfptr_secret;

    /* Stego Image Info */
    char *dstego_image_fname;
    FILE *dfptr_stego_image;
   
    char strong_string[10];

} DecodeInfo;


/* decoding function prototype */

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status dopen_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file(DecodeInfo *decInfo);

/* decode a byte into msb of image data array */
Status decode_byte_to_msb(DecodeInfo *decInfo, int,int);

/* decode a int to msb */
Status decode_int_to_msb(DecodeInfo *decInfo,int *);

#endif
