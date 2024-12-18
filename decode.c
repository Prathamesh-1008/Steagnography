#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"
/* Function Definitions */

Status do_decoding(DecodeInfo *decInfo)   //decoding function 
{
        printf("INFO : ## Decoding Procedure Started ##\n"); 
    	if(!dopen_files(decInfo))      //open files function call 
    	{
             	return e_failure;
	}
        fseek(decInfo -> dfptr_stego_image,54,SEEK_SET); 	
        
	if(decode_magic_string(decInfo))     //decode magic string function call 
 		printf("INFO : Done\n");
        else
	{
		printf("INFO : Not Done\n");
 		return e_failure;
	}

        decode_secret_file_extn(decInfo);       //decode secret file extention call 

         if(decode_secret_file(decInfo))   //decode secret file function call 
         	printf("INFO : Done\n");
         else
         {
               	printf("INFO : Not Done\n");
		return e_failure;
         }
         printf("INFO : ## Decoding Done Successfully ##\n");
         fclose(decInfo -> dfptr_stego_image);
         fclose(decInfo -> dfptr_secret);
}

Status dopen_files(DecodeInfo *decInfo)
{
	printf("INFO : Opening required files\n");
	decInfo -> dfptr_stego_image = fopen(decInfo -> dstego_image_fname,"r"); //open stego img file open in read mode
 	if(decInfo -> dfptr_stego_image == NULL)
	{
        	printf("INFO : Not Opened stego_img.bmp\n");
		return e_failure;
	}	
        printf("INFO : Opened stego_img.bmp\n");
        return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
	printf("INFO : Decoding Magic String Signature\n");
        int size = 0;	
        char pass_key[10];
	decode_int_to_msb(decInfo,&size);     //decode magict string length function call 
        decode_byte_to_msb(decInfo,size,1);   //decode magic string data function call 
	printf("Enter the Magic String Passkey : "); 
        scanf(" %[^\n]",pass_key);
        if(strcmp(pass_key,decInfo -> strong_string))   //compare magic string and passkey than not same stop function  
                    return e_failure;
        return e_success;
}

Status decode_int_to_msb(DecodeInfo *decInfo,int *size)    //decoding integer value 
{
	char ch[32];
        fread(ch,32,1,decInfo -> dfptr_stego_image);  //data take from stego image of byte of lsb and collect in size  
        for(int i=0;i<32;i++)
	{
		if(ch[i] & 1)
                	(*size) = (1 | ((*size)<<1));
                else
			(*size) = ((*size)<<1);
	} 
	return e_success;       
}

Status decode_byte_to_msb(DecodeInfo *decInfo, int size,int flag)   //decoding data 
{
	char ch[8];
        char buf;       
        int l=strlen(decInfo -> dsecret_fname); 
        for(int i=0;i<size;i++)      //loop apply upto size 
        {     
                fread(ch,8,1,decInfo -> dfptr_stego_image);   //take data from lsb of stego image and collect into buf
		for(int j=0;j<8;j++)
                {
                	if(ch[j]& 1)
                    		buf = (1 | (buf<<1));
                	else
			        buf = (buf<<1);
                }

                if(flag==1)   //if flag is 1 buf store in strong string 
                {
                     decInfo -> strong_string[i] = buf; 
                     decInfo -> strong_string[size] = '\0';
                }
 		else if(flag==2)   //if flag is 2 than buf assign to dsecret_fname 
		{ 
                 	decInfo -> dsecret_fname[l]=buf;
                        if(i==0)
				decInfo -> dsecret_fname[l+size]='\0';
                        l++;
                }  
		else
 		{
			fputc(buf,decInfo -> dfptr_secret);
			
		}                
	}
        return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)      //decode secrete file exten function 
{
	printf("INFO : Decoding Output File Extension\n"); 
    	int len=0;
        decode_int_to_msb(decInfo,&len);  //decode len function call
        if(decode_byte_to_msb(decInfo,len,2))    //decode secret file name  function call
        	printf("INFO : Done\n");
        printf("INFO : Opened decoded.txt\n");
	decInfo -> dfptr_secret = fopen(decInfo -> dsecret_fname,"w");  //open secret file in Write mode 
        if(decInfo -> dfptr_secret == NULL)
        {
        	printf("Info : Not Don\n");
		return e_failure;
      	}
        printf("INFO : Done. Opened all required files\n");
}

Status decode_secret_file(DecodeInfo *decInfo)    //decode data of secret file 
{
    	int len=0;
        printf("INFO : Decoding File Size\n");
        if(decode_int_to_msb(decInfo,&len))      //decode len of secret file 
 		printf("INFO : Done\n");
        printf("INFO : Decoding File Data\n");
        if(decode_byte_to_msb(decInfo,len,0))     //decode secrete data 
      		return e_success;
}
