#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)      //crete function for take size of source image 
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status do_encoding(EncodeInfo *encInfo)      //encode function 
{        
        printf("INFO : Opening required files\n");
	if(!(open_files(encInfo)))             //call open file function than retuen 
        {
		printf("INFO : Not Done\n");
                return e_failure;
        }
	else
		printf("INFO : Done\n");

	printf("INFO : ## Encoding Procedure Started ##\n");
        if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image))    //call bmp header copy function 
        	printf("INFO : Done\n");
        else
        {
	 	printf("INFO : Not Done\n");
		return e_failure;
	}
        
	if(encode_magic_string(MAGIC_STRING,encInfo))     //call magic string function 
        	printf("INFO : Done\n");
        else
        {
	 	printf("INFO : Not Done\n");
		return e_failure;
	}
        
        char *str = strchr(encInfo -> secret_fname , '.');
        if(encode_secret_file_extn(str,encInfo))                     //call secrete file extension function 
        	printf("INFO : Done\n");
        else
        {
	 	printf("INFO : Not Done\n");
		return e_failure;
	}

        if(copy_remaining_img_data(encInfo -> fptr_src_image,encInfo -> fptr_stego_image))     //call remaing img data copy function 
        	printf("INFO : Done\n");
        else
        {
	 	printf("INFO : Not Done\n");
		return e_failure;
	}
        printf("INFO : ## Encoding Done Successfully ##\n");
	fclose(encInfo -> fptr_src_image);
        fclose(encInfo -> fptr_secret);
	fclose(encInfo -> fptr_stego_image);        
}


Status open_files(EncodeInfo *encInfo)       //this function use for opens file 
{
        encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname,"r");   //open source image file in read mode 
        if(encInfo -> fptr_src_image == NULL)
	{
		printf("INFO : Not Opened Skeleton Code/beautiful.bmp\n");
		return e_failure;
	}
	else
	{
		printf("INFO : Opened Skeleton Code/beautiful.bmp\n");
               
	}

        encInfo -> fptr_secret = fopen(encInfo -> secret_fname,"r");         //open secret file in read mode 
        fseek(encInfo -> fptr_secret,0,SEEK_END);        
        if((encInfo -> fptr_secret == NULL) || (!(ftell(encInfo->fptr_secret))))
	{
		printf("INFO : Not Opened Secret.txt\n");
                printf("INFO : Secret File is Empty \n");
		return e_failure;
	}
	else
	{
                 printf("INFO : Opened Secret.txt\n");
		if(((4+(strlen(MAGIC_STRING))+4+4+4+(ftell(encInfo->fptr_secret)))*8) > (get_image_size_for_bmp(encInfo -> fptr_src_image)))
		{
              		return e_failure;
                }
	        rewind(encInfo -> fptr_secret);
                rewind(encInfo -> fptr_src_image);
	
        }
                     
        encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname,"w");      //open output bmp file in write mode 
        if(encInfo -> fptr_stego_image == NULL)
	{
		printf("INFO : Not Opened Stego_image.bmp\n");
		return e_failure;
	}
        else
	{		
		printf("INFO : Opened Stego_image.bmp\n");

	}
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)      //this function use for first header data copy into destination image file
{
	printf("INFO : Copying Image Header\n");
	char buf[54];
        fread(buf,54,1,fptr_src_image);
        fwrite(buf,54,1,fptr_dest_image);
 	return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) //this function use for magic string opeartion
{
	printf("INFO : Encoding Magic String Signature\n");
    	int l = strlen(magic_string);
        char *buf = malloc(32 * sizeof(char));
        fread(buf,32,1,encInfo -> fptr_src_image);	 	
        encode_int_to_lsb(l,buf);                     //call function this function use for encode size of magic string
	fwrite(buf,32,1,encInfo -> fptr_stego_image);
        buf = realloc(buf,8);
	for(int i=0; i<l; i++)    
	{
   		fread(buf,8,1,encInfo -> fptr_src_image);
 	        encode_byte_to_lsb(magic_string[i],buf);     //call function for data enocding of magic string
                fwrite(buf,8,1,encInfo -> fptr_stego_image);
	}
        return e_success;
}


Status encode_int_to_lsb(int data,char buf[])       //encoding int data 
{
	int j=0;
	for(int i=31; i>=0; i--)
	{
		if(data & (1<<i))
			buf[j] = (buf[j] | 1); 
                else
			buf[j] = (buf[j] & (~(1)));
		j++;
	}
}


Status encode_byte_to_lsb(char data, char buf[])     //encoding data 
{
	
	int j=0;
	for(int i=7; i>=0; i--)
	{
		if(data & (1<<i))
			buf[j] = (buf[j] | 1); 
                else
			buf[j] = (buf[j] & (~(1)));
		j++;
	}
}

Status encode_secret_file_extn(const char *str,EncodeInfo * encInfo)    //this function for sencode secrete data 
{
	printf("INFO : Encoding secret.txt File Extention\n");
	int l = strlen(str);
        char *buf = malloc(32 * sizeof(char));
        fread(buf,32,1,encInfo -> fptr_src_image);	 	
        encode_int_to_lsb(l,buf);      //call function int to lsb 
	fwrite(buf,32,1,encInfo -> fptr_stego_image);
        buf = realloc(buf,8);
	for(int i=0; i<l; i++)
	{
   		fread(buf,8,1,encInfo -> fptr_src_image);
 	        encode_byte_to_lsb(str[i],buf);
                fwrite(buf,8,1,encInfo -> fptr_stego_image);
	}
        printf("INFO : Done\n"); 
	fseek(encInfo -> fptr_secret,0,SEEK_END);
        if(encode_secret_file_size(ftell(encInfo -> fptr_secret),encInfo))
        	printf("INFO : Done\n");
        rewind(encInfo -> fptr_secret);
 	if(encode_secret_file_data(encInfo))
        	return e_success;
}

//encode secrete file size 
Status encode_secret_file_size(int size, EncodeInfo *encInfo)
{
        printf("Info : Encoding secret.txt File size\n");
        char *buf = malloc(32 * sizeof(char));
        fread(buf,32,1,encInfo -> fptr_src_image);	 	
        encode_int_to_lsb(size,buf);
	fwrite(buf,32,1,encInfo -> fptr_stego_image);
  	free(buf);
        return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{  
        printf("Info : Encoding secret.txt File Data\n");
	char ch;
        char buf[8];
        while((ch=fgetc(encInfo -> fptr_secret))!= EOF)
	{
		
                fread(buf,8,1,encInfo -> fptr_src_image);
 	        encode_byte_to_lsb(ch,buf);
                fwrite(buf,8,1,encInfo -> fptr_stego_image);
	}
        return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)   //copy remaing data 
{
	printf("INFO : Copying Left Over Data\n");
	char ch;
        while(fread(&ch,1,1,fptr_src_image) > 0)
        {
          fwrite(&ch,1,1,fptr_stego_image);
        }
        return e_success;
}


