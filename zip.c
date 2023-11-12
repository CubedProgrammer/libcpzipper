#include<stdlib.h>
#include"zip.h"

void zip_free_local(struct zip_entry_local *en)
{
    zip_free_local_core(&en->dat);
}
void zip_free_central(struct zip_entry_central *en)
{
    zip_free_local_core(&en->local);
    free(en->comment);
}
void zip_free_end(struct zip_entry_end *en)
{
    free(en->comment);
}
void zip_free_local_core(struct zip_entry_local_core *en)
{
    free(en->name);
    free(en->field);
}
