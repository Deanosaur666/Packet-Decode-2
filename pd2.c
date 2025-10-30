#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// Terminal input for linux:
// gcc pd2.c ; ./a.out Frame2.bin

// let's read a binary file
int main(int argc, char *argv[]) {
    int errorcode = 0; 	// returned at end of function
    FILE* file; 	// file pointer
    unsigned char byte;	// where we store the byte read
    unsigned int i = 0; // for counting bytes in payload
    
    unsigned int IHL;
    unsigned int totalLength = 0;

    printf("\n");

    // no filename passed
    if(argc < 2) {
        printf("You must provide an argument.");
        errorcode = 1;
    }
    // filename passed as argument
    else {
        file = fopen(argv[1], "rb");
	// couldn't open file
        if(file == NULL) {
            printf("The file cannot be opened");
            errorcode = 2;
        }
	// successfully opened file
        else {
	    printf("Ethernet header:\n");
	    printf("----------------\n");

	    // first 6 bytes are Destination MAC Address
	    printf("Destination MAC address:\t\t");

	    // These two lines repeat throughout. They're the workhorse of this program.
	    // We read one byte, then print it. Very simple.
            fread(&byte, sizeof(char), 1, file);
	    printf("%02x", byte & 0xFF);
	    
	    // print the next 5 bytes with a colon preceding them
	    for(int i = 0; i < 5; i ++) {
		printf(":");

                fread(&byte, sizeof(char), 1, file);
	        printf("%02x", byte & 0xFF);
	    }

	    // next 6 bytes are Source MAC Address
	    printf("\nSource MAC address:\t\t\t");

            fread(&byte, sizeof(char), 1, file);
	    printf("%02x", byte & 0xFF);

	    for(int i = 0; i < 5; i ++) {
		printf(":");

                fread(&byte, sizeof(char), 1, file);
	        printf("%02x", byte & 0xFF);
	    }

	    // next 2 bytes are type
	    printf("\nType:\t\t\t\t\t");
            
	    fread(&byte, sizeof(char), 1, file);
	    printf("%02x", byte & 0xFF);
            fread(&byte, sizeof(char), 1, file);
	    printf("%02x", byte & 0xFF);
            
	    printf("\n\nIPv4 Header:\n");
	    printf("----------------\n");

	    // Version and IHL
	    fread(&byte, sizeof(char), 1, file);
	    IHL = byte & 0xF;

	    printf("Version:\t\t\t\t%02d\n", byte >> 4); // decimal
	    printf("Internet header length:\t\t\t%02x\n", IHL); // hex

	    // DSCP and ECN
	    fread(&byte, sizeof(char), 1, file);

	    printf("DSCP:\t\t\t\t\t%02d\n", byte >> 2); // decimal
            printf("ECN:\t\t\t\t\t%d%d", (byte >> 1) & 1, byte & 1); // binary
	    
	    // ECN flag
	    switch(byte & 0b11) {
                case 0:
		    printf("\tNon-ECT Packet");
		    break;
		case 1:
		case 2:
		    printf("\tECN Capable Transport");
		    break;
		case 3:
		    printf("\tCongestion Experienced");
		    break;
	    }

	    // Total length
	    fread(&byte, sizeof(char), 1, file);
	    totalLength = byte << 8;
	    fread(&byte, sizeof(char), 1, file);
	    totalLength |= byte;

	    printf("\nTotal Length:\t\t\t\t%d\n", totalLength);

	    // the rest is payload
	    printf("\n\nPayload:\n");

	    // read, process, read
	    // this prevents us from printing the last byte twice
            fread(&byte, sizeof(char), 1, file);
	    while(!feof(file)) {
		i ++;
	    	printf("%02x ", byte & 0xFF);
		if(i % 32 == 0) {
		     printf("\n");
		}
		else if(i % 8 == 0) {
		     printf(" ");
		}
            	fread(&byte, sizeof(char), 1, file);
	    }
	    
	    // We're done with the file! Good work everyone.
	    fclose(file);
        }
    }

    printf("\n");
    return errorcode;
}
