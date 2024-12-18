/*
      Name : Prathamesh Nandkumar Patil
      Date : 18/9/24
      Description : Steganography Project 
*/

//------------------------------------------------------- *** Steganography Project *** -----------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc,char * argv[])                        
{

    if((argc >= 4) &&(!(strcmp(argv[1],"-e")) || (! strcmp(argv[1],"-E"))))   //if check first string is -e or -E than encoding      
    {   
        
    	EncodeInfo encInfo;      //create structure variable 
        char *str1 = strchr(argv[2],'.');   //chech char is present than return address 
        char *str = strchr(argv[3],'.');
        //check str and str1 is not null next check check this .bmp or .c or .txt file 
        if((str1 != NULL) && (str != NULL) && (!(strcmp(str1,".bmp"))) && ((!(strcmp(str,".txt"))) || (!(strcmp(str,".c"))) ||(!(strcmp(str,".sh")))))
    	{
		encInfo.src_image_fname = argv[2];
    		encInfo.secret_fname = argv[3];
    		if(argc == 4)                         //argc is equal to 4 than assign file name 
		{
         		encInfo.stego_image_fname ="out.bmp";
                        do_encoding(&encInfo);
                }
                else if(argc==5)   //if argc is 5 than take file name from user 
                {
                        char *str2=strchr(argv[4],'.');
                        if(str2==NULL)    //check str2 is null than print error 
                           printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt/.c/.sh file> [output file]\n");
                        else if(!(strcmp(str2,".bmp")))   //otherwise assign file name from user 
                        {
                            encInfo.stego_image_fname=argv[4];
                            do_encoding(&encInfo);
                        }
                        else
                           printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt/.c/.sh file> [output file]\n");
    		}
                else
                {
        		printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt/.c/.sh file> [output file]\n");
                }
        }
        else	
             printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt/.c/.sh file> [output file]\n");
    }
    else if((argc >=3) && (!( strcmp(argv[1],"-d")) || (!strcmp(argv[1],"-D"))))           //if string 1 is -d or -D than decoding 
    {
    		DecodeInfo decInfo;
        	char *str = strchr(argv[2],'.'); 
        	if(  (str != NULL ) && (!(strcmp(str,".bmp"))))     //check second string is .bmp file is not 
    		{	
         		decInfo.dstego_image_fname = argv[2]; 
                if(argc == 3)    //check the argc is 3 than assign file name is output 
                {

                    strcpy(decInfo.dsecret_fname,"Output");
                        do_decoding(&decInfo);
                }
                else if(argc==4)     //argc is 4 than take file name from user 
    		    {     
                    if((strchr(argv[3],'.')==NULL))   //otherwise assign file name from user 
				    {
                		strcpy(decInfo.dsecret_fname,argv[3]);
        				do_decoding(&decInfo);
                    }
                    else
                        printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
                }
                else
                	printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");  
            }
            else
                printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");

    }
    else
    {
        printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt/.c/.sh file> [output file]\n");
        printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
    }
}
