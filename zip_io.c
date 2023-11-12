#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"zip_io.h"

int zip_get_str(FILE *fh, char *str, short unsigned len)
{
    short unsigned cnt = 0;
    while(cnt < len && !feof(fh))
        cnt += fread(str + cnt, 1, len - cnt, fh);
    str[len] = '\0';
    return cnt < len;
}

int zip_read_local(FILE *fh, struct zip_entry_local *en)
{
    char buf[ZIP_LOCAL_LEN] = "PK\003\004";
    int succ = fread(buf + 4, 1, sizeof(buf) - 4, fh) < sizeof(buf) - 4;
    if(succ == 0)
    {
        zip_deserialize_local(en, buf);
        succ = zip_read_local_str(fh, en);
    }
    return succ;
}
int zip_read_central(FILE *fh, struct zip_entry_central *en)
{
    char buf[ZIP_CENTRAL_LEN] = "PK\001\002";
    int succ = fread(buf + 4, 1, sizeof(buf) - 4, fh) < sizeof(buf) - 4;
    if(succ == 0)
    {
        zip_deserialize_central(en, buf);
        succ = zip_read_central_str(fh, en);
    }
    return succ;
}
int zip_read_end(FILE *fh, struct zip_entry_end *en)
{
    char buf[ZIP_END_LEN] = "PK\005\006";
    int succ = fread(buf + 4, 1, sizeof(buf) - 4, fh) < sizeof(buf) - 4;
    if(succ == 0)
    {
        zip_deserialize_end(en, buf);
        succ = zip_read_end_str(fh, en);
    }
    return succ;
}

int zip_write_local(FILE *fh, const struct zip_entry_local *en)
{
    char buf[ZIP_LOCAL_LEN];
    zip_serialize_local(buf, en);
    return fwrite(buf, 1, sizeof(buf), fh) < sizeof(buf) || zip_write_local_str(fh, en);
}
int zip_write_central(FILE *fh, const struct zip_entry_central *en)
{
    char buf[ZIP_CENTRAL_LEN];
    zip_serialize_central(buf, en);
    return fwrite(buf, 1, sizeof(buf), fh) < sizeof(buf) || zip_write_central_str(fh, en);
}
int zip_write_end(FILE *fh, const struct zip_entry_end *en)
{
    char buf[ZIP_END_LEN];
    zip_serialize_end(buf, en);
    return fwrite(buf, 1, sizeof(buf), fh) < sizeof(buf) || zip_write_end_str(fh, en);
}

int zip_read_local_core_str(FILE *fh, struct zip_entry_local_core *en)
{
    int succ = -1;
    en->name = malloc(en->namelen + 1);
    if(en->name != NULL)
    {
        en->field = malloc(en->fieldlen + 1);
        if(en->field != NULL)
            succ = zip_get_str(fh, en->name, en->namelen) || zip_get_str(fh, en->field, en->fieldlen);
        else
        {
            free(en->name);
            en->name = NULL;
        }
    }
    return succ;
}
int zip_read_local_str(FILE *fh, struct zip_entry_local *en)
{
    return zip_read_local_core_str(fh, &en->dat);
}
int zip_read_central_str(FILE *fh, struct zip_entry_central *en)
{
    int succ = zip_read_local_core_str(fh, &en->local);
    en->comment = malloc(en->commentlen + 1);
    if(en->comment != NULL)
        succ = zip_get_str(fh, en->comment, en->commentlen);
    return succ;
}
int zip_read_end_str(FILE *fh, struct zip_entry_end *en)
{
    int succ = -1;
    en->comment = malloc(en->commentlen + 1);
    if(en->comment != NULL)
        succ = zip_get_str(fh, en->comment, en->commentlen);
    return succ;
}

int zip_write_local_core_str(FILE *fh, const struct zip_entry_local_core *en)
{
    return fwrite(en->name, 1, en->namelen, fh) < en->namelen || fwrite(en->field, 1, en->fieldlen, fh) < en->fieldlen;
}
int zip_write_local_str(FILE *fh, const struct zip_entry_local *en)
{
    return zip_write_local_core_str(fh, &en->dat);
}
int zip_write_central_str(FILE *fh, const struct zip_entry_central *en)
{
    return zip_write_local_core_str(fh, &en->local) + fwrite(en->comment, 1, en->commentlen, fh) < en->commentlen;
}
int zip_write_end_str(FILE *fh, const struct zip_entry_end *en)
{
    return fwrite(en->comment, 1, en->commentlen, fh) < en->commentlen;
}
