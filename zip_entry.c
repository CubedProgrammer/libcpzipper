#include"zip_entry.h"
void zip_little_endian_put(void *buf, unsigned off, unsigned len, long val)
{
    char unsigned *cbuf = buf;
    cbuf += off;
    switch(len)
    {
        case 8:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 7:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 6:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 5:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 4:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 3:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 2:
            *cbuf++ = val & 0xff;
            val >>= 8;
        case 1:
            *cbuf = val & 0xff;
    }
}
long zip_little_endian_get(const void *buf, unsigned off, unsigned len)
{
    long val = 0;
    const char unsigned *cbuf = buf;
    cbuf += off + len;
    switch(len)
    {
        case 8:
            val |= *--cbuf;
        case 7:
            val = val << 8 | *--cbuf;
        case 6:
            val = val << 8 | *--cbuf;
        case 5:
            val = val << 8 | *--cbuf;
        case 4:
            val = val << 8 | *--cbuf;
        case 3:
            val = val << 8 | *--cbuf;
        case 2:
            val = val << 8 | *--cbuf;
        case 1:
            val = val << 8 | *--cbuf;
    }
    return val;
}
void zip_serialize_local(void *restrict dest, const struct zip_entry_local *restrict src)
{
    zip_little_endian_put(dest, 0, 4, src->sig);
    zip_serialize_local_core((char*)dest + 4, &src->dat);
}
void zip_serialize_central(void *restrict dest, const struct zip_entry_central *restrict src)
{
    zip_little_endian_put(dest, 0, 4, src->sig);
    zip_little_endian_put(dest, 4, 2, src->vermade);
    zip_serialize_local_core((char*)dest + 6, &src->local);
    zip_little_endian_put(dest, 32, 2, src->commentlen);
    zip_little_endian_put(dest, 34, 2, src->disknum);
    zip_little_endian_put(dest, 36, 2, src->internal);
    zip_little_endian_put(dest, 38, 4, src->external);
    zip_little_endian_put(dest, 42, 4, src->fileoff);
}
void zip_serialize_end(void *restrict dest, const struct zip_entry_end *restrict src)
{
    zip_little_endian_put(dest, 0, 4, src->sig);
    zip_little_endian_put(dest, 4, 2, src->disk);
    zip_little_endian_put(dest, 6, 2, src->diskcentral);
    zip_little_endian_put(dest, 8, 2, src->disktot);
    zip_little_endian_put(dest, 10, 2, src->tot);
    zip_little_endian_put(dest, 12, 4, src->cendiroff);
    zip_little_endian_put(dest, 16, 4, src->cendirlen);
    zip_little_endian_put(dest, 20, 2, src->commentlen);
}
void zip_serialize_local_core(void *restrict dest, const struct zip_entry_local_core *restrict src)
{
    zip_little_endian_put(dest, 0, 2, src->version);
    zip_little_endian_put(dest, 2, 2, src->bitflag);
    zip_little_endian_put(dest, 4, 2, src->method);
    zip_little_endian_put(dest, 6, 2, src->mtime);
    zip_little_endian_put(dest, 8, 2, src->mdate);
    zip_little_endian_put(dest, 10, 4, src->crc);
    zip_little_endian_put(dest, 14, 4, src->deflatedsz);
    zip_little_endian_put(dest, 18, 4, src->inflatedsz);
    zip_little_endian_put(dest, 22, 2, src->namelen);
    zip_little_endian_put(dest, 24, 2, src->fieldlen);
}
void zip_deserialize_local(struct zip_entry_local *restrict dest, const void *restrict src)
{
    dest->sig = zip_little_endian_get(src, 0, 4);
    zip_deserialize_local_core(&dest->dat, (const char*)src + 4);
}
void zip_deserialize_central(struct zip_entry_central *restrict dest, const void *restrict src)
{
    dest->sig = zip_little_endian_get(src, 0, 4);
    dest->vermade = zip_little_endian_get(src, 4, 2);
    zip_deserialize_local_core(&dest->local, (const char*)src + 6);
    dest->commentlen = zip_little_endian_get(src, 32, 2);
    dest->disknum = zip_little_endian_get(src, 34, 2);
    dest->internal = zip_little_endian_get(src, 36, 2);
    dest->external = zip_little_endian_get(src, 38, 4);
    dest->fileoff = zip_little_endian_get(src, 42, 4);
}
void zip_deserialize_end(struct zip_entry_end *restrict dest, const void *restrict src)
{
    dest->sig = zip_little_endian_get(src, 0, 4);
    dest->disk = zip_little_endian_get(src, 4, 2);
    dest->diskcentral = zip_little_endian_get(src, 6, 2);
    dest->disktot = zip_little_endian_get(src, 8, 2);
    dest->tot = zip_little_endian_get(src, 10, 2);
    dest->cendiroff = zip_little_endian_get(src, 12, 4);
    dest->cendirlen = zip_little_endian_get(src, 16, 4);
    dest->commentlen = zip_little_endian_get(src, 20, 2);
}
void zip_deserialize_local_core(struct zip_entry_local_core *restrict dest, const void *restrict src)
{
    dest->version = zip_little_endian_get(src, 0, 2);
    dest->bitflag = zip_little_endian_get(src, 2, 2);
    dest->method = zip_little_endian_get(src, 4, 2);
    dest->mtime = zip_little_endian_get(src, 6, 2);
    dest->mdate = zip_little_endian_get(src, 8, 2);
    dest->crc = zip_little_endian_get(src, 10, 4);
    dest->deflatedsz = zip_little_endian_get(src, 14, 2);
    dest->inflatedsz = zip_little_endian_get(src, 18, 2);
    dest->namelen = zip_little_endian_get(src, 22, 2);
    dest->fieldlen = zip_little_endian_get(src, 24, 2);
}
