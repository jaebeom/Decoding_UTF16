/**
* Jae B. Lim code
*
* This is decodeUTF16.c file in program 1.
*
* Decode(UTF16 to UTF32).
*
* Due Feb 6, 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementing all of the jobs in here.
static void helperF(int argc, char *argv[]){
    FILE *inputF, *outputF;
    int first, second, third, fourth;
    int littleORbig = 3; // 0 is little endian, and 1 is big endian
    int n = 0;
    int value1 = 0x00000000;
    int value2 = 0x00000000;
    int value3 = 0x00000000; // This is buffer.
    int nonBMPflag = 0;
    int noncharacter = 0x0000;

    // Read command line arguments.
    if(argc != 3){
        printf("Usage: lab2 [input filename] [output filename].\n");
        exit(-1);
    }

    // input and output filename check. (They should be different)
    if(!strcmp(argv[1], argv[2])){
        perror("The input file and output file should have a different name.\n");
        exit(-1);
    }

    // Read file in here
    if((inputF = fopen(argv[1], "r"))==NULL){
        printf("cannot open %s\n", argv[1]);
        exit(-1);
    }

    // Write file in here
    outputF = fopen(argv[2],"wt");
    if(outputF == NULL){
        perror("fopen fail.");
        exit(-1);
    }

    // judging it is big or little endian.
    if(inputF){
        if((first = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
        if((second = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
    }

    // Check the input file starts with proper BOM.
    if(first == 0xFE && second == 0xFF){
        littleORbig = 1; // Big Endian
        fputc(0x00, outputF);
        fputc(0x00, outputF);
        fputc(first, outputF);
        fputc(second, outputF);
    }
    else if(first == 0xFF && second == 0xFE){
        littleORbig = 0; // Little Endian
        fputc(first, outputF);
        fputc(second, outputF);
        fputc(0x00, outputF);
        fputc(0x00, outputF);
    }
    else{
        perror("The input file does not start with BOM.");
        exit(-1);
    }

    if(inputF){
        while((first = getc(inputF))!=EOF){
            value3 = value2; // Prev
            second = getc(inputF);
            value1 = first << 8;
            value2 = value1 | second; // Current

            // When it is not BMP-character, use different algorithm.
            if(value2 < (0xD800 + 0x3FF) && value2 > (0xD800 + 0x000)){
                value2 = value2 - 0xD800;
                first = value2 >> 8;
                second = value2 & 0xFF;
                value3 = value2 << 10;

                third = getc(inputF);
                third = third << 8;
                fourth = getc(inputF);
                value2 = third | fourth;


                if(value2 < (0xDC00 + 0x3FF) && value2 > (0xDC00 + 0x000)){
                    value2 = value2 - 0xDC00;
                    value3 = value3 | value2;
                    value3 = value3 + 0x10000;
                    first = value3 >> 20 & 0xFF;
                    second = value3 >> 16 & 0xFF;
                    third = value3 >> 8 & 0xFF;
                    fourth = value3 & 0xFF;
                    fputc(first, outputF);
                    fputc(second, outputF);
                    fputc(third, outputF);
                    fputc(fourth, outputF);
                    value2 = 0x1; // Make pass test temprary
                    nonBMPflag = 1;
                }
            }

            // If it is little endian, reverse the order.
            if(littleORbig == 0){
                value1 = second << 8;
                value2 = value1 | first;
                // When it is not BMP-character, use different algorithm.
                if(value2 < (0xD800 + 0x3FF) && value2 > (0xD800 + 0x000)){
                    value2 = value2 - 0xD800;
                    first = value2 >> 8;
                    second = value2 & 0xFF;
                    value3 = value2 << 10;

                    fourth = getc(inputF);
                    third = getc(inputF) << 8;
                    value2 = third | fourth;
                }
                if(value2 < (0xDC00 + 0x3FF) && value2 > (0xDC00 + 0x000)){
                    value2 = value2 - 0xDC00;
                    value3 = value3 | value2;
                    value3 = value3 + 0x10000;
                    first = value3 >> 20 & 0xFF;
                    second = value3 >> 16 & 0xFF;
                    third = value3 >> 8 & 0xFF;
                    fourth = value3 & 0xFF;

                    fputc(fourth, outputF);
                    fputc(third, outputF);
                    fputc(second, outputF);
                    fputc(first, outputF);
                    value2 = 0x1; // Make pass test temprary
                    nonBMPflag = 1;
                }
            }

            // Check a value that denotes a noncharacter.
            noncharacter = value2 & 0xFFFE;
            if(noncharacter == 0xFFFE){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }
            noncharacter = value2 & 0xFFFF;
            if(noncharacter == 0xFFFF){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }
            if(0xFDD0 <= value2 && value2 <= 0xFDEF){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }

            // Check unpaired surrogates.
            if(n != 0){
                if(0xD800 <= value3 && value3 <= 0xDBFF){
                    if(0xDC00 > value2 || value2 > 0xDFFF){
                        perror("This is unpaired surrogates.");
                        exit(-1);
                    }
                }
                if(0xDC00 <= value2 && value2 <= 0xDFFF){
                    if(0xD800 > value3 || value3 > 0xDBFF){
                        perror("This is unpaired surrogates.");
                        exit(-1);
                    }
                }
            }

            // Check an incomplete value.
            if(second == EOF){
                perror("A value is an incomplete value.");
                exit(-1);
            }

            if(nonBMPflag == 0){
                if(littleORbig == 0){
                    fputc(first, outputF);
                    fputc(second, outputF);
                    fputc(0x00, outputF);
                    fputc(0x00, outputF);
                }
                if(littleORbig == 1){
                    fputc(0x00, outputF);
                    fputc(0x00, outputF);
                    fputc(first, outputF);
                    fputc(second, outputF);
                }
            }
            n++;
        }
        fclose(inputF);
        fclose(outputF);
    }
}

int main(int argc, char *argv[]){
    helperF(argc, argv);
    return 0;
}
