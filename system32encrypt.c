#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LL_SZ sizeof(unsigned long long)

int encrypt_file(FILE* pfile);

int main(int argc, const char* argv[])
{
FILE* pfile = fopen(argv[1], "rb+");
if (!pfile)
{
    #if defined NDEBUG
        fputs("File not found!", stderr);
    #endif
    return -1;
}
if (!encrypt_file(pfile))
{
    #if defined NDEBUG
        fputs("Fail to encrypt!", stderr);
    #endif
    fclose(pfile);
    pfile = fopen(argv[1], "w");
    fclose(pfile);
    return -1;
}
fclose(pfile);
return 0;
}

int encrypt_file(FILE* pfile)
{
fseek(pfile, 0, SEEK_END);
long int flen = ftell(pfile);
fseek(pfile, 0, SEEK_SET);
unsigned long long* memptr = (unsigned long long*) malloc(flen);

if (!memptr)
{
    return 0;
}

fread(memptr, flen, 1, pfile);
const int lenfloor = (int) floor(flen/LL_SZ);
for (int i=0; i< lenfloor; i++)
{
    memptr[i] = memptr[i]^10872035019822686483;
}
for(unsigned long long i=0; i< (flen - lenfloor * LL_SZ -1);i++)
{
    ((unsigned char*) (memptr + lenfloor))[i] = ((unsigned char*) (memptr + lenfloor))[i]^233;
}
#if defined NDEBUG
    printf("%d %ld %.*s\n",flen - lenfloor * LL_SZ -1, flen, flen, (const char*) memptr);
#endif
fseek(pfile, 0, SEEK_SET);
fwrite(memptr, flen, 1, pfile);
free(memptr);
return 1;
}
