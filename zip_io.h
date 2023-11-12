#ifndef Included_zip_io_h
#define Included_zip_io_h
#include<stdio.h>
#include"zip_entry.h"

int zip_read_local(FILE *fh, struct zip_entry_local *en);
int zip_read_central(FILE *fh, struct zip_entry_central *en);
int zip_read_end(FILE *fh, struct zip_entry_end *en);

int zip_write_local(FILE *fh, const struct zip_entry_local *en);
int zip_write_central(FILE *fh, const struct zip_entry_central *en);
int zip_write_end(FILE *fh, const struct zip_entry_end *en);

int zip_read_local_core_str(FILE *fh, struct zip_entry_local_core *en);
int zip_read_local_str(FILE *fh, struct zip_entry_local *en);
int zip_read_central_str(FILE *fh, struct zip_entry_central *en);
int zip_read_end_str(FILE *fh, struct zip_entry_end *en);

int zip_write_local_core_str(FILE *fh, const struct zip_entry_local_core *en);
int zip_write_local_str(FILE *fh, const struct zip_entry_local *en);
int zip_write_central_str(FILE *fh, const struct zip_entry_central *en);
int zip_write_end_str(FILE *fh, const struct zip_entry_end *en);

#endif
