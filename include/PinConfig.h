#pragma once

constexpr uint8_t gc_irqIn_pin{15}; // D8 - don't use D4

// constexpr uint8_t   gc_rxd2_pin{17};
constexpr uint8_t gc_txd2_pin{16};

constexpr uint8_t gc_sda_pin{21};
constexpr uint8_t gc_scl_pin{22};

constexpr uint8_t gc_DIO_pin{13}; // 25
constexpr uint8_t gc_CLK_pin{33}; // 26
constexpr uint8_t gc_STB_pin{32}; // 27

constexpr uint8_t gc_SI_pin{25};
constexpr uint8_t gc_RCK_pin{26};
constexpr uint8_t gc_SCK_pin{27};
constexpr uint8_t gc_NSCLR_pin{14};

constexpr uint8_t gc_PULS_pin{4};

//  18, 23, 5, 17, 19
// clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset

constexpr uint8_t gc_OLED_clock_pin{18};
constexpr uint8_t gc_OLED_data_pin{23};
constexpr uint8_t gc_OLED_cs_pin{5};
constexpr uint8_t gc_OLED_dc_pin{17};
constexpr uint8_t gc_OLED_reset_pin{19};