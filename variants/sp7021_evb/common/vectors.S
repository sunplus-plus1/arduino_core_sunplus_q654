.global _SP__vectors
_SP__vectors:
        LDR PC, = RESET_HANDLE
        LDR PC, = UNDEF_INS_HANDLE
        LDR PC, = SWI_HANDLE
        LDR PC, = PREFE_ABORT_HANDLE 
        LDR PC, = DATA_ABORT_HANDLE
        LDR PC, = NO_USED_HANDLE
        LDR PC, = IRQ_HANDLE
        LDR PC, = FIQ_HANDLE
