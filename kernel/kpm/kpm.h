#ifndef __KPM_H
#define __KPM_H

int handle_kpm(unsigned long control_code, unsigned long arg3,
                      unsigned long arg4, unsigned long result_code);
int is_kpm_control_code(unsigned long control_code);
int do_kpm(void __user *arg);

/* KPM Control Code */
#define CMD_KPM_CONTROL 1
#define CMD_KPM_CONTROL_MAX 10

#endif
