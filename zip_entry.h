#ifndef Included_zip_entry_h
#define Included_zip_entry_h

struct zip_entry_local_core
{
    short unsigned version, bitflag, method;
    short unsigned mtime, mdate;
    unsigned crc;
    unsigned deflatedsz, inflatedsz;
    short unsigned namelen, fieldlen;
    char *name, *field;
};

struct zip_entry_local
{
    unsigned sig;
    struct zip_entry_local_core dat;
};

struct zip_entry_central
{
    unsigned sig;
    short unsigned vermade;
    struct zip_entry_local_core local;
    short unsigned commentlen;
    short unsigned disknum;
    short unsigned internal;
    unsigned external;
    unsigned fileoff;
    char *comment;
};

struct zip_entry_end
{
    unsigned sig;
    short unsigned disk, diskcentral;
    short unsigned disktot, tot;
    unsigned cendirlen, cendiroff;
    short unsigned commentlen;
    char *comment;
};

void zip_serialize_local(void *restrict dest, const struct zip_entry_local *restrict src);
void zip_serialize_central(void *restrict dest, const struct zip_entry_central *restrict src);
void zip_serialize_end(void *restrict dest, const struct zip_entry_end *restrict src);
void zip_serialize_local_core(void *restrict dest, const struct zip_entry_local_core *restrict src);
void zip_deserialize_local(struct zip_entry_local *restrict dest, const void *restrict src);
void zip_deserialize_central(struct zip_entry_central *restrict dest, const void *restrict src);
void zip_deserialize_end(struct zip_entry_end *restrict dest, const void *restrict src);
void zip_deserialize_local_core(struct zip_entry_local_core *restrict dest, const void *restrict src);

#endif
