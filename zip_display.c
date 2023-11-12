#include<stdio.h>
#include<stdlib.h>
#include"zip_display.h"

const char *zip_fmt_local = "%hu %hu %hu %hu %hu %u %u %u %hu %hu %s %s";
const char *zip_fmt_central = "%hu %s %hu %hu %hu %u %u %s";
const char *zip_fmt_end = "%hu %hu %hu %hu %u %u %hu %s";

void zip_display_local(const struct zip_entry_local *en)
{
    zip_fdisplay_local(stdout, en);
}
void zip_display_central(const struct zip_entry_central *en)
{
    zip_fdisplay_central(stdout, en);
}
void zip_display_end(const struct zip_entry_end *en)
{
    zip_fdisplay_end(stdout, en);
}
void zip_display_local_core(const struct zip_entry_local_core *en)
{
    zip_fdisplay_local_core(stdout, en);
}

void zip_fdisplay_local(FILE *fh, const struct zip_entry_local *en)
{
    return zip_fdisplay_local_core(fh, &en->dat);
}
void zip_fdisplay_central(FILE *fh, const struct zip_entry_central *en)
{
    char localbuf[961], *localstr = localbuf;
    char *heapbuf = NULL;
    int cnt = zip_dump_central(localbuf, sizeof(localbuf), en);
    if(cnt > 0)
    {
        if(cnt >= sizeof(localbuf))
        {
            heapbuf = malloc(cnt + 1);
            zip_dump_central(heapbuf, cnt + 1, en);
            localstr = heapbuf;
        }
        fputs(localstr, fh);
        if(heapbuf != NULL)
            free(heapbuf);
    }
}
void zip_fdisplay_end(FILE *fh, const struct zip_entry_end *en)
{
    fprintf(fh, zip_fmt_end, en->disk, en->diskcentral, en->disktot, en->tot, en->cendirlen, en->cendiroff, en->commentlen, en->comment);
}
void zip_fdisplay_local_core(FILE *fh, const struct zip_entry_local_core *en)
{
    fprintf(fh, zip_fmt_local, en->version, en->bitflag, en->method, en->mtime, en->mdate, en->crc, en->deflatedsz, en->inflatedsz, en->namelen, en->fieldlen, en->name, en->field);
}

int zip_dump_local(char *buf, unsigned len, const struct zip_entry_local *en)
{
    return zip_dump_local_core(buf, len, &en->dat);
}
int zip_dump_central(char *buf, unsigned len, const struct zip_entry_central *en)
{
    char localbuf[961], *localstr = localbuf;
    char *heapbuf = NULL;
    int cnt = zip_dump_local_core(localbuf, sizeof(localbuf), &en->local);
    if(cnt > 0)
    {
        if(cnt >= sizeof(localbuf))
        {
            heapbuf = malloc(cnt + 1);
            zip_dump_local_core(heapbuf, cnt + 1, &en->local);
            localstr = heapbuf;
        }
        cnt = snprintf(buf, len, zip_fmt_central, en->vermade, localstr, en->commentlen, en->disknum, en->internal, en->external, en->fileoff, en->comment);
        if(heapbuf != NULL)
            free(heapbuf);
    }
    return cnt;
}
int zip_dump_end(char *buf, unsigned len, const struct zip_entry_end *en)
{
    return snprintf(buf, len, zip_fmt_end, en->disk, en->diskcentral, en->disktot, en->tot, en->cendirlen, en->cendiroff, en->commentlen, en->comment);
}
int zip_dump_local_core(char *buf, unsigned len, const struct zip_entry_local_core *en)
{
    return snprintf(buf, len, zip_fmt_local, en->version, en->bitflag, en->method, en->mtime, en->mdate, en->crc, en->deflatedsz, en->inflatedsz, en->namelen, en->fieldlen, en->name, en->field);
}
