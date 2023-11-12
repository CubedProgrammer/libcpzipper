#ifndef Included_zip_display_h
#define Included_zip_display_h
#include"zip_entry.h"

extern const char *zip_fmt_local;
extern const char *zip_fmt_central;
extern const char *zip_fmt_end;

void zip_display_local(const struct zip_entry_local *en);
void zip_display_central(const struct zip_entry_central *en);
void zip_display_end(const struct zip_entry_end *en);
void zip_display_local_core(const struct zip_entry_local_core *en);

void zip_fdisplay_local(FILE *fh, const struct zip_entry_local *en);
void zip_fdisplay_central(FILE *fh, const struct zip_entry_central *en);
void zip_fdisplay_end(FILE *fh, const struct zip_entry_end *en);
void zip_fdisplay_local_core(FILE *fh, const struct zip_entry_local_core *en);

int zip_dump_local(char *buf, unsigned len, const struct zip_entry_local *en);
int zip_dump_central(char *buf, unsigned len, const struct zip_entry_central *en);
int zip_dump_end(char *buf, unsigned len, const struct zip_entry_end *en);
int zip_dump_local_core(char *buf, unsigned len, const struct zip_entry_local_core *en);

#endif
