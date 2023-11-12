#ifndef Included_zip_h
#define Included_zip_h
#include"zip_display.h"
#include"zip_io.h"

void zip_free_local(struct zip_entry_local *en);
void zip_free_central(struct zip_entry_central *en);
void zip_free_end(struct zip_entry_end *en);
void zip_free_local_core(struct zip_entry_local_core *en);

#endif
