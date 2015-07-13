#include<sys/unistd.h>
#include<signal.h>
#include <sys/mman.h>  
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<errno.h>
#include<stdint.h>

typedef uint8_t u1;
typedef uint16_t   u2;
typedef uint32_t   u4;
typedef uint64_t  u8;
#define MOD_ADLER 65521

u4 revaddress(u4 count, u1* buf);
u4 readUnsignedLeb128(u1** pStream);
u4 checksum(u1* mapped, u4 size);

void recvSignal(int sig)
{
    printf("received signal %d !!!\n",sig);
}

int main(char* filename, char* txtname)
{
    u1 buf[4];
    u4 fd1,fd2,j;
    u4 strcnt = 0;
    u4 basedress;
    u4 * dressarry;
    extern int errno;
signal(SIGSEGV, recvSignal);
    if((fd1 = open(filename, O_RDWR)) < 0)
    {
        printf("errno=%d\n",errno);
        char * mesg = strerror(errno);
        printf("Mesg:%s\n",mesg);
        exit(1);
    }
    else
    {
        printf("dex file open success\n");
    }
if(txtname != NULL)
{
    if((fd2 = open(txtname,O_RDWR | O_CREAT | O_TRUNC, 0667)) < 0)
    {
        printf("errno=%d\n",errno);
        char * mesg = strerror(errno);
        printf("Mesg:%s\n",mesg);
        exit(1);
    }
    else
    {
        printf("string file open success\n");
    }
}
    struct stat sb;  
    char *mapped;

    /* 获取文件的属性 */  
    if ((fstat(fd1, &sb)) == -1) {  
        perror("fstat");  
    }
    /* 将文件映射至进程的地址空间 */  
    if ((mapped = (char *)mmap(NULL, sb.st_size, PROT_READ |   
                    PROT_WRITE, MAP_SHARED, fd1, 0)) == (void *)-1) {  
        perror("mmap");  
    }

    memcpy(buf, &mapped[0x8], 4);
    int  csum = revaddress(4, buf);
    int checkresult = checksum(mapped, (u4)sb.st_size);

    if(checkresult == csum)
    {
        printf("the checksum is true\n");
    }
    else
    {
        printf("the checksum is false, the file may de changed\n");
    }

    if(memcpy(buf, &mapped[0x38], 4) != NULL)
    {
        strcnt = revaddress(4, buf);
        dressarry = (u4 *)malloc(sizeof(u4)*strcnt);
    }

    if(memcpy(buf, &mapped[0x3c], 4) != 0)
    {
        basedress = revaddress(4, buf);
    }

    for(j = 0; j < strcnt; j++)
    {
        memcpy(buf, &mapped[basedress+4*j], 4);
        u4 Adress = revaddress(4, buf);
        dressarry[j] = Adress;
    }

    u4 result;
    u1 a;
    u1 lastdata[1];
    u4 lastcnt = 0;
    do{
        memcpy(lastdata, &mapped[dressarry[(strcnt - 1)] + 1 + lastcnt], 1);
        lastcnt++;
    }while((int)lastdata[0] != 0);

    for(j = 0; j < strcnt; j++ )
    {
        u1* databuf;
        u1* datapoint;
        if( j == strcnt - 1)
        {
            databuf = (u1 *)malloc(lastcnt);
            memcpy(databuf, &mapped[dressarry[strcnt - 1] + 1], lastcnt);
        }
        else
        {
            databuf = (u1 *)malloc(dressarry[j+1] - dressarry[j] - 1);
            memcpy(databuf, &mapped[dressarry[j] + 1], dressarry[j+1] - dressarry[j] - 1);
        }
        datapoint = databuf;
        do{
            result = readUnsignedLeb128(&datapoint);
            if(result  != 0)
            {
                a = (u1)(result);
	if(txtname != NULL)
{
                write(fd2, &a, 1);
}
else
{
printf("%c",a);
}
            }
            else
            {
                a = '\n';
if(txtname != NULL)
{
                write(fd2, &a, 1);
}
else
{
printf("%c",a);
}
            }
        }while(result != 0);
        free(databuf);
        databuf = NULL;
    }
    free(dressarry);

    /* 释放存储映射区 */  
    if ((munmap((void *)mapped, sb.st_size)) == -1) {  
        perror("munmap");  
    } 
    close(fd1);
if(txtname != NULL)
{
    close(fd2);
}
    return 0;
}

u4 checksum(u1* mapped, u4 size)
{
    u4 a = 1, b = 0, data = 0;
    u4 i;
    u1 buf1[1];

    for(i = 0xc; i < size; i++)
    {
        data = (int)mapped[i];
        a = (a + data) % MOD_ADLER;
        b = (a + b) % MOD_ADLER;
    }
    return (b << 16) | a;
}

u4 revaddress(u4 count, u1* buf)
{
    u4 offset = 0x00000000,i;
    for(i = 0; i < count ; i++)
    {
        offset = offset | ((int)buf[i] << 8*i); 
    }
    return offset;
}

/*
 * Reads an unsigned LEB128 value, updating the given pointer to point
 * just past the end of the read value. This function tolerates
 * non-zero high-order bits in the fifth encoded byte.
 */
u4 readUnsignedLeb128(u1** pStream) {
    u1* ptr = *pStream;
    int result = *(ptr++);

    if (result > 0x7f) {
        u4 cur = *(ptr++);
        result = (result & 0x7f) | ((cur & 0x7f) << 7);
        if (cur > 0x7f) {
            cur = *(ptr++);
            result |= (cur & 0x7f) << 14;
            if (cur > 0x7f) {
                cur = *(ptr++);
                result |= (cur & 0x7f) << 21;
                if (cur > 0x7f) {
                    /*
                     * Note: We don't check to see if cur is out of
                     * range here, meaning we tolerate garbage in the
                     * high four-order bits.
                     */
                    cur = *(ptr++);
                    result |= cur << 28;
                }
            }
        }
    }

    *pStream = ptr;
    return result;
}
