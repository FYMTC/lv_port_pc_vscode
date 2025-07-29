// Mock QMC5883L driver header
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Mock QMC5883L register definitions
#define QMC5883L_ADDR 0x0D

// Mock data structure
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} qmc5883l_raw_data_t;

// Mock function declarations
int qmc5883l_init(void);
int qmc5883l_read_raw(qmc5883l_raw_data_t* data);
bool qmc5883l_is_connected(void);

#ifdef __cplusplus
}
#endif
