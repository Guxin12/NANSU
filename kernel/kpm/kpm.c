/* SPDX-License-Identifier: GPL-2.0-or-later */
/* 
 * Copyright (C) 2025 Liankong (xhsw.new@outlook.com). All Rights Reserved.
 * 本代码由GPL-2授权
 * 
 * 适配KernelSU的KPM 内核模块加载器兼容实现
 * 
 * 集成了 ELF 解析、内存布局、符号处理、重定位（支持 ARM64 重定位类型）
 * 并参照KernelPatch的标准KPM格式实现加载和控制
 */

/* 
 * 由 Guxin12 修改完善
 */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kernfs.h>
#include <linux/file.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>
#include <linux/elf.h>
#include <linux/kallsyms.h>
#include <linux/version.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/rcupdate.h>
#include <asm/elf.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <asm/cacheflush.h>
#include <linux/module.h>
#include <linux/set_memory.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <asm/insn.h>
#include <linux/kprobes.h>
#include <linux/stacktrace.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) && defined(CONFIG_MODULES)
#include <linux/moduleloader.h>
#endif
#include "kpm.h"
#include "compact.h"
#include "uapi/supercall.h"

#define KPM_NAME_LEN 32
#define KPM_ARGS_LEN 1024

#ifndef NO_OPTIMIZE
#if defined(__GNUC__) && !defined(__clang__)
#define NO_OPTIMIZE __attribute__((optimize("O0")))
#elif defined(__clang__)
#define NO_OPTIMIZE __attribute__((optnone))
#else
#define NO_OPTIMIZE
#endif
#endif

noinline NO_OPTIMIZE void sukisu_kpm_load_module_path(const char *path, const char *args, void *ptr, int *result)
{
    pr_info("kpm: Stub function called (sukisu_kpm_load_module_path). "
            "path=%s args=%s ptr=%p\n",
            path, args, ptr);

    __asm__ volatile("nop");
}
EXPORT_SYMBOL(sukisu_kpm_load_module_path);

noinline NO_OPTIMIZE void sukisu_kpm_unload_module(const char *name, void *ptr, int *result)
{
    pr_info("kpm: Stub function called (sukisu_kpm_unload_module). "
            "name=%s ptr=%p\n",
            name, ptr);

    __asm__ volatile("nop");
}
EXPORT_SYMBOL(sukisu_kpm_unload_module);

noinline NO_OPTIMIZE void sukisu_kpm_num(int *result)
{
    pr_info("kpm: Stub function called (sukisu_kpm_num).\n");

    __asm__ volatile("nop");
}
EXPORT_SYMBOL(sukisu_kpm_num);

noinline NO_OPTIMIZE void sukisu_kpm_info(const char *name, char *buf, int bufferSize, int *size)
{
    pr_info("kpm: Stub function called (sukisu_kpm_info). "
            "name=%s buffer=%p\n",
            name, buf);

    __asm__ volatile("nop");
}
EXPORT_SYMBOL(sukisu_kpm_info);

noinline NO_OPTIMIZE void sukisu_kpm_list(void *out, int bufferSize, int *result)
{
    pr_info("kpm: Stub function called (sukisu_kpm_list). "
            "buffer=%p size=%d\n",
            out, bufferSize);
}
EXPORT_SYMBOL(sukisu_kpm_list);

noinline NO_OPTIMIZE void sukisu_kpm_control(const char *name, const char *args, long arg_len, int *result)
{
    pr_info("kpm: Stub function called (sukisu_kpm_control). "
            "name=%p args=%p arg_len=%ld\n",
            name, args, arg_len);

    __asm__ volatile("nop");
}
EXPORT_SYMBOL(sukisu_kpm_control);

noinline NO_OPTIMIZE void sukisu_kpm_version(char *buf, int bufferSize)
{
    pr_info("kpm: Stub function called (sukisu_kpm_version). "
            "buffer=%p\n",
            buf);
}
EXPORT_SYMBOL(sukisu_kpm_version);

/* Helper to safely copy string from user */
static long safe_strncpy_from_user(char *dst, const char __user *src, size_t max_len)
{
    if (max_len == 0)
        return -EINVAL;

    long ret = strncpy_from_user(dst, src, max_len);
    if (ret < 0) {
        dst[0] = '\0';
        return ret;
    }

    /* 强制保证以 null 结尾 */
    if (ret >= max_len)
        ret = max_len - 1;
    dst[ret] = '\0';
    return ret;
}

noinline int sukisu_handle_kpm(unsigned long control_code, unsigned long arg1, unsigned long arg2,
                               unsigned long result_code)
{
    int res = -1;

    switch (control_code) {
    case SUKISU_KPM_LOAD: {
        char kernel_load_path[256] = { 0 };
        char kernel_args_buffer[256] = { 0 };
        long ret;

        if (arg1 == 0) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg1, sizeof(kernel_load_path) - 1)) {
            goto invalid_arg;
        }

        ret = safe_strncpy_from_user(kernel_load_path, (const char __user *)arg1, sizeof(kernel_load_path));
        if (ret < 0) {
            res = -EFAULT;
            goto exit;
        }

        if (arg2 != 0) {
            if (!access_ok((void __user *)arg2, sizeof(kernel_args_buffer) - 1)) {
                goto invalid_arg;
            }
            ret = safe_strncpy_from_user(kernel_args_buffer, (const char __user *)arg2, sizeof(kernel_args_buffer));
            if (ret < 0) {
                res = -EFAULT;
                goto exit;
            }
        } else {
            kernel_args_buffer[0] = '\0';
        }

        sukisu_kpm_load_module_path(kernel_load_path, kernel_args_buffer, NULL, &res);
        goto exit;
    }

    case SUKISU_KPM_UNLOAD: {
        char kernel_name_buffer[256] = { 0 };
        long ret;

        if (arg1 == 0) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg1, sizeof(kernel_name_buffer) - 1)) {
            goto invalid_arg;
        }

        ret = safe_strncpy_from_user(kernel_name_buffer, (const char __user *)arg1, sizeof(kernel_name_buffer));
        if (ret < 0) {
            res = -EFAULT;
            goto exit;
        }

        sukisu_kpm_unload_module(kernel_name_buffer, NULL, &res);
        goto exit;
    }

    case SUKISU_KPM_NUM: {
        sukisu_kpm_num(&res);
        goto exit;
    }

    case SUKISU_KPM_INFO: {
        char kernel_name_buffer[256] = { 0 };
        char buf[256] = { 0 };
        int size = 0;
        long ret;

        if (arg1 == 0 || arg2 == 0) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg1, sizeof(kernel_name_buffer) - 1)) {
            goto invalid_arg;
        }

        ret = safe_strncpy_from_user(kernel_name_buffer, (const char __user *)arg1, sizeof(kernel_name_buffer));
        if (ret < 0) {
            res = -EFAULT;
            goto exit;
        }

        sukisu_kpm_info(kernel_name_buffer, buf, sizeof(buf), &size);

        if (size <= 0 || size > (int)sizeof(buf)) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg2, size)) {
            goto invalid_arg;
        }

        res = copy_to_user((void __user *)arg2, buf, size);
        goto exit;
    }

    case SUKISU_KPM_LIST: {
        char buf[1024] = { 0 };
        int len = (int)arg2;

        if (len <= 0) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg1, len)) {
            goto invalid_arg;
        }

        sukisu_kpm_list(buf, sizeof(buf), &res);

        if (res < 0) {
            goto exit;
        }

        if (res > len) {
            res = -ENOBUFS;
            goto exit;
        }

        if (copy_to_user((void __user *)arg1, buf, res)) {
            pr_info("kpm: Copy to user failed.");
            res = -EFAULT;
        }
        goto exit;
    }

    case SUKISU_KPM_CONTROL: {
        char kpm_name[KPM_NAME_LEN] = { 0 };
        char kpm_args[KPM_ARGS_LEN] = { 0 };
        long name_len, arg_len;

        if (!access_ok((void __user *)arg1, sizeof(kpm_name) - 1)) {
            goto invalid_arg;
        }

        if (!access_ok((void __user *)arg2, sizeof(kpm_args) - 1)) {
            goto invalid_arg;
        }

        name_len = safe_strncpy_from_user(kpm_name, (const char __user *)arg1, sizeof(kpm_name));
        if (name_len < 0) {
            res = -EFAULT;
            goto exit;
        }
        if (name_len == 0) {
            res = -EINVAL;
            goto exit;
        }

        arg_len = safe_strncpy_from_user(kpm_args, (const char __user *)arg2, sizeof(kpm_args));
        if (arg_len < 0) {
            res = -EFAULT;
            goto exit;
        }

        sukisu_kpm_control(kpm_name, kpm_args, arg_len, &res);
        goto exit;
    }

    case SUKISU_KPM_VERSION: {
        char buffer[256] = { 0 };
        unsigned int outlen = (unsigned int)arg2;

        if (outlen == 0) {
            res = -EINVAL;
            goto exit;
        }

        if (!access_ok((void __user *)arg1, outlen)) {
            goto invalid_arg;
        }

        sukisu_kpm_version(buffer, sizeof(buffer));

        int len = strlen(buffer);
        if (len >= (int)outlen)
            len = outlen - 1;
        if (len < 0) {
            res = -EINVAL;
            goto exit;
        }

        res = copy_to_user((void __user *)arg1, buffer, len + 1);
        goto exit;
    }

    default: {
        res = -ENOSYS; /* Unknown control code */
        goto exit;
    }
    }

exit:
    if (copy_to_user((void __user *)result_code, &res, sizeof(res)) != 0)
        pr_info("kpm: Copy to user failed.");

    return 0;

invalid_arg:
    pr_err("kpm: invalid pointer detected! arg1: %px arg2: %px\n", (void *)arg1, (void *)arg2);
    res = -EFAULT;
    goto exit;
}
EXPORT_SYMBOL(sukisu_handle_kpm);

int sukisu_is_kpm_control_code(unsigned long control_code)
{
    return (control_code >= CMD_KPM_CONTROL && control_code <= CMD_KPM_CONTROL_MAX) ? 1 : 0;
}

int do_kpm(void __user *arg)
{
    struct ksu_kpm_cmd cmd;

    if (copy_from_user(&cmd, arg, sizeof(cmd))) {
        pr_err("kpm: copy_from_user failed\n");
        return -EFAULT;
    }

    // if (!access_ok(cmd.control_code, sizeof(int))) {
    // pr_err("kpm: invalid control_code pointer %px\n", (void *)cmd.control_code);
    // return -EFAULT;
    // }

    if (!access_ok((void __user *)cmd.result_code, sizeof(int))) {
        pr_err("kpm: invalid result_code pointer %px\n", (void *)cmd.result_code);
        return -EFAULT;
    }

    return sukisu_handle_kpm(cmd.control_code, cmd.arg1, cmd.arg2, cmd.result_code);
}