#ifndef __SUPER_ACCESS_H
#define __SUPER_ACCESS_H

#include <linux/types.h>
#include <linux/stddef.h>

extern int super_find_struct(const char *struct_name, size_t *out_size,
                                    int *out_members);
extern int super_access(const char *struct_name, const char *member_name,
                               size_t *out_offset, size_t *out_size);
extern int super_container_of(const char *struct_name,
                                     const char *member_name, void *ptr,
                                     void **out_ptr);

#endif