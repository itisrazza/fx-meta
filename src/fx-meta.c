
#include <stdio.h>
#include <libgen.h>
#include <byteswap.h> // Screw you... little endian CPUs!!
#include <sysexits.h>

typedef struct FileHeader
{
    char identifier[8];     // "USBPower"
    unsigned char type;     // Identifier
    char signature[5];      // { 0x00, 0x10, 0x00, 0x10, 0x00 }
    char control_1;         // (LSB of filesize (at offset 0x13)) + 0x41
    char unknown_1;         // 0x01
    unsigned int size;      // File size, big endian
    char control_2;         // (LSB of filesize (at offset 0x13)) + 0xB8
    char unknown_2[9];      // Unknown. Seems insignificant
    unsigned short objects; // Number of objects, if G1M or G1R
} FileHeader;

typedef struct AddinHeader
{
    char internal_name[8]; // in form of "@APPNAME"
    char filler_1[3];
    char estrip_count;
    char filler_2[4];
    char version[0x0A]; // in form of "01.23.4567"
    char filler_3[2];
    char date[0x0E]; // in form of "YYYY.MMDD.HHMM"
    char filler_4[2];
    char icon[0x44]; // 30*18 1-bit bitmap.
    char filler_5[0x144];
    char title[8];
    char filler_6[0x14];
    unsigned int size; // Big endian
    char filler_7[0xC];
} AddinHeader;

int main(int argc, char *argv[])
{
    // Check for arguments and pop out if there are none
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [filename]\n", basename(argv[0]));
        fprintf(stderr, "Prints out metadata of a CASIO fx-9860G file.\n");
        return EX_USAGE;
    }

    // Load the file and do a few checks
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error");
        return EX_IOERR;
    }

    // Create a header and fill it in
    FileHeader file_header;
    if (fread(&file_header, sizeof(FileHeader), 1, fp) < 1)
    {
        fprintf(stderr, "Couldn't read header information. Quitting.");
        return EX_DATAERR;
    }

    for (int i = 0; i < sizeof(FileHeader); i++)
    {
        // The file header itself needs to be NOTed, for some reason.
        char *byte = &file_header;
        *(byte + i) = ~*(byte + i);
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    file_header.size = bswap_32(file_header.size);
#endif

    printf("= File Header =\n");
    printf("Identifier:\t%s\n", file_header.identifier);    // this is not null terminated. how do i do that?
    printf("Type:\t\t0x%x\n", file_header.type);
    printf("File Size:\t%d bytes\n", file_header.size);
    printf("Obj. count:\t%d\n", file_header.objects);

    if (file_header.type != 0xF3)
    {
        return 0; // Finish here. This is not an app.
    }

    // If this is an add-in, get more of its header

    AddinHeader addin_header;
    if (fread(&addin_header, sizeof(AddinHeader), 1, fp) < 1)
    {
        fprintf(stderr, "Couldn't read sub-header information. Quitting.");
        return EX_DATAERR;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    addin_header.size = bswap_32(addin_header.size);
#endif

    printf("\n= Addin Header =\n");
    printf("Internal name:\t%s\n", addin_header.internal_name); // nt 
    printf("eStrips:\t%d\n", addin_header.estrip_count);
    printf("Version:\t%s\n", addin_header.version);             // nt
    printf("Date:\t\t%s\n", addin_header.date);                 // nt
    printf("Title:\t\t%s\n", addin_header.title);               // nt
    printf("Size:\t\t%d bytes\n", addin_header.size);
    printf("Icon:");

    for (int i = 0; i < 0x44; i++) {
        if (i % 4 == 0) printf("\n\t");

        char byte = addin_header.icon[i];
        for (char j = 7; j >= 0; j--) {
            char draw = byte >> j & 1;
            printf(draw ? "█" : " ");
        }

    }

    printf("\n");

    // Done. Close the file it.
    fclose(fp);

    return 0; // stack smashing error on exit
}
