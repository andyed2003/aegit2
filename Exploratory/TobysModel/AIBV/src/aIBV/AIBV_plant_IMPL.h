#ifndef AIBV_PLANT_IMPL_H
#define AIBV_PLANT_IMPL_H
void AIBV_plant_IMPL_AIBV_open_start(int amount);
void AIBV_plant_IMPL_AIBV_open_end();
void AIBV_plant_IMPL_AIBV_close_start(int amount);
void AIBV_plant_IMPL_AIBV_close_end();
void AIBV_plant_IMPL_AIBV_measure_position_start();
void AIBV_plant_IMPL_AIBV_measure_position_end(int *position_measurement);
void AIBV_plant_IMPL_AIBV_finish_control_loop();
void AIBV_plant_IMPL_AIBV_temperature_measurement(int *temperature);
void AIBV_plant_IMPL();
#endif

