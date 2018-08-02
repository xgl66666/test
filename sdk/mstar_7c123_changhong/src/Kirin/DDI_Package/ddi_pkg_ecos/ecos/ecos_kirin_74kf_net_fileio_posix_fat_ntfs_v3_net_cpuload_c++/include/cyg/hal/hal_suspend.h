#ifndef _eCos_SUSPEND_H
#define _eCosSUSPEND_H
#include <cyg/infra/cyg_type.h>         // Base types



#define STENT_RESUME_FROM_SUSPEND   3

typedef int suspend_state_t;

#define PM_SUSPEND_ON       ((suspend_state_t) 0)
#define PM_SUSPEND_STANDBY ((suspend_state_t) 1)
#define PM_SUSPEND_MEM      ((suspend_state_t) 3)
#define PM_SUSPEND_MAX      ((suspend_state_t) 4)

typedef struct _platform_suspend_ops{
    int (*valid)(suspend_state_t state);
    int (*begin)(suspend_state_t state);
    int (*prepare)(void);
    int (*prepare_late)(void);
    int (*enter)(suspend_state_t state);
    void (*wake)(void);
    void (*finish)(void);
    bool (*suspend_again)(void);
    void (*end)(void);
    void (*recover)(void);
}platform_suspend_ops;

//#ifdef CONFIG_SUSPEND
#if 1
/**
 * suspend_set_ops - set platform dependent suspend operations
 * @ops: The new suspend operations to set.
 */
extern void suspend_set_ops(platform_suspend_ops *ops);
extern int suspend_valid_only_mem(suspend_state_t state);

/**
 * arch_suspend_disable_irqs - disable IRQs for suspend
 *
 * Disables IRQs (in the default case). This is a weak symbol in the common
 * code and thus allows architectures to override it if more needs to be
 * done. Not called for suspend to disk.
 */
extern void arch_suspend_disable_irqs(void);

/**
 * arch_suspend_enable_irqs - enable IRQs after suspend
 *
 * Enables IRQs (in the default case). This is a weak symbol in the common
 * code and thus allows architectures to override it if more needs to be
 * done. Not called for suspend to disk.
 */
extern void arch_suspend_enable_irqs(void);

extern int pm_suspend(suspend_state_t state);
extern int pm_suspend_init(void);
#else /* !CONFIG_SUSPEND */
#define suspend_valid_only_mem	NULL

static inline void suspend_set_ops(const struct platform_suspend_ops *ops) {}
static inline int pm_suspend(suspend_state_t state) { return -ENOSYS; }
#endif /* !CONFIG_SUSPEND */

#if 0
#ifdef CONFIG_PM_SLEEP
void save_processor_state(void);
void restore_processor_state(void);

/* kernel/power/main.c */
extern int register_pm_notifier(struct notifier_block *nb);
extern int unregister_pm_notifier(struct notifier_block *nb);

#define pm_notifier(fn, pri) {\
    static struct notifier_block fn##_nb =\
    { .notifier_call = fn, .priority = pri };	\
      register_pm_notifier(&fn##_nb);\
}

/* drivers/base/power/wakeup.c */
extern bool events_check_enabled;

extern bool pm_wakeup_pending(void);
extern bool pm_get_wakeup_count(unsigned int *count);
extern bool pm_save_wakeup_count(unsigned int count);
#else /* !CONFIG_PM_SLEEP */

static inline int register_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

#define pm_notifier(fn, pri)	do { (void)(fn); } while (0)

static inline bool pm_wakeup_pending(void) { return false; }
#endif /* !CONFIG_PM_SLEEP */
#endif

#endif /* _LINUX_SUSPEND_H */
