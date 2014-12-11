#ifndef SHARED_IMPL_H
#define SHARED_IMPL_H
void SHARED_IMPL_AIBV_check_temperature_ok(int buffered_temperature);
void SHARED_IMPL_AIBV_calculate_position_partial(int *temperature);
void SHARED_IMPL_AIBV_calculate_position_open(int *temperature);
void SHARED_IMPL_AIBV_calculate_position_closed(int *temperature);
#endif

