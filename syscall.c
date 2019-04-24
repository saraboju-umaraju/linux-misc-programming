static const char *types__mmap[] =
    { "unsigned long", "unsigned long", "unsigned long", "unsigned long",
"unsigned long", "unsigned long" };
static const char *args__mmap[] =
    { "addr", "len", "prot", "flags", "fd", "off" };
static struct syscall_metadata __syscall_meta__mmap;
static struct trace_event_call __used event_enter__mmap = {.class =
        &event_class_syscall_enter, {.name =
                                     "sys_enter" "_mmap",},.event.funcs =
    &enter_syscall_print_funcs,.data = (void *)&__syscall_meta__mmap,.flags =
    TRACE_EVENT_FL_CAP_ANY, };
static struct trace_event_call __used
    __attribute__ ((section("_ftrace_events"))) * __event_enter__mmap =
    &event_enter__mmap;;
SYSCALL_TRACE_EXIT_EVENT(_mmap);
static struct syscall_metadata __used __syscall_meta__mmap = {.name =
        "sys" "_mmap",.syscall_nr = -1,.nb_args = 6,.types =
        6 ? types__mmap : NULL,.args = 6 ? args__mmap : NULL,.enter_event =
        &event_enter__mmap,.exit_event = &event_exit__mmap,.enter_fields =
        LIST_HEAD_INIT(__syscall_meta__mmap.enter_fields), };
static struct syscall_metadata __used
    __attribute__ ((section("__syscalls_metadata"))) * __p_syscall_meta__mmap =
    &__syscall_meta__mmap;
__diag_push();
__diag_ignore(GCC, 8, "-Wattribute-alias",
              "Type aliasing is used to sanitize syscall arguments");
asmlinkage long sys_mmap(unsigned long addr, unsigned long len,
                         unsigned long prot, unsigned long flags,
                         unsigned long fd, unsigned long off)
    __attribute__ ((alias(__stringify(__se_sys_mmap))));
ALLOW_ERROR_INJECTION(sys_mmap, ERRNO);
static inline long __do_sys_mmap(unsigned long addr, unsigned long len,
                                 unsigned long prot, unsigned long flags,
                                 unsigned long fd, unsigned long off);
asmlinkage long
__se_sys_mmap(__typeof
              (__builtin_choose_expr
               ((__same_type((__force unsigned long)0, 0LL)
                 || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                0L)) addr,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) len,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) prot,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) flags,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) fd,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) off);
asmlinkage long
__se_sys_mmap(__typeof
              (__builtin_choose_expr
               ((__same_type((__force unsigned long)0, 0LL)
                 || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                0L)) addr,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) len,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) prot,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) flags,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) fd,
              __typeof(__builtin_choose_expr
                       ((__same_type((__force unsigned long)0, 0LL)
                         || __same_type((__force unsigned long)0, 0ULL)), 0LL,
                        0L)) off)
{
    long ret =
        __do_sys_mmap((__force unsigned long)addr, (__force unsigned long)len,
                      (__force unsigned long)prot, (__force unsigned long)flags,
                      (__force unsigned long)fd, (__force unsigned long)off);
    (void)(0), (void)(0), (void)(0), (void)(0), (void)(0), (void)(0);
    asmlinkage_protect(6, ret, addr, len, prot, flags, fd, off);
    return ret;
}

__diag_pop();
static inline long
__do_sys_mmap(unsigned long addr, unsigned long len, unsigned long prot,
              unsigned long flags, unsigned long fd, unsigned long off)
