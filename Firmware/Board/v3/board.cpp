/*
* @brief Contains board specific variables and initialization functions
*/

#include <board.h>

Stm32SpiArbiter spi3_arbiter{&hspi3};
Stm32SpiArbiter& ext_spi_arbiter = spi3_arbiter;

Drv8301 m0_gate_driver{
    &spi3_arbiter,
    {M0_nCS_GPIO_Port, M0_nCS_Pin}, // nCS
    {EN_GATE_GPIO_Port, EN_GATE_Pin}, // EN pin (shared between both motors)
    {nFAULT_GPIO_Port, nFAULT_Pin} // nFAULT pin (shared between both motors)
};

Drv8301 m1_gate_driver{
    &spi3_arbiter,
    {M1_nCS_GPIO_Port, M1_nCS_Pin}, // nCS
    {EN_GATE_GPIO_Port, EN_GATE_Pin}, // EN pin (shared between both motors)
    {nFAULT_GPIO_Port, nFAULT_Pin} // nFAULT pin (shared between both motors)
};

Motor m0{
    &htim1, // timer
    TIM_1_8_PERIOD_CLOCKS, // control_deadline
    1.0f / SHUNT_RESISTANCE, // shunt_conductance [S]
    15, // inverter_thermistor_adc_ch
    m0_gate_driver, // gate_driver
    m0_gate_driver // opamp
};

Motor m1{
    &htim8, // timer
    (3 * TIM_1_8_PERIOD_CLOCKS) / 2, // control_deadline
    1.0f / SHUNT_RESISTANCE, // shunt_conductance [S]
#if HW_VERSION_MAJOR == 3 && HW_VERSION_MINOR >= 3
    4, // inverter_thermistor_adc_ch
#else
    1, // inverter_thermistor_adc_ch
#endif
    m1_gate_driver, // gate_driver
    m1_gate_driver // opamp
};

Motor* motors[AXIS_COUNT] = {&m0, &m1};


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    HAL_SPI_TxRxCpltCallback(hspi);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    HAL_SPI_TxRxCpltCallback(hspi);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi3) {
        spi3_arbiter.on_complete();
    }
}
