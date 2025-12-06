#ifndef CH224Q_H
#define CH224Q_H

#include "Application.h"

enum {
    ACTICATE_NONE,
    ACTICATE_BC,
    ACTICATE_QC2,
    ACTICATE_QC3,
    ACTICATE_PD,
    ACTICATE_EPR,
    ACTICATE_TRUE,
};

void Fixed_req(uint8_t vol);
uint8_t get_ch224q_status(void);
uint16_t get_PD_current(void);

#endif
