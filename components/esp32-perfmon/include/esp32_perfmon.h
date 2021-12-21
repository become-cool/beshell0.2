#ifndef COMPONENTS_PERFMON_INCLUDE_PERFMON_H_
#define COMPONENTS_PERFMON_INCLUDE_PERFMON_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

uint8_t cpu0_usage() ;
uint8_t cpu1_usage() ;
esp_err_t perfmon_start();

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_PERFMON_INCLUDE_PERFMON_H_ */
