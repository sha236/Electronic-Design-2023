#pragma once
#include "main.h"
#include "arm_math.h"


#ifndef SPLL_1PH_SOGI_H
#define SPLL_1PH_SOGI_H

//! \brief  Defines the SPLL_1PH_SOGI_OSG_COEFF structure
//!
typedef struct{
    float32_t osg_k;
    float32_t osg_x;
    float32_t osg_y;
    float32_t osg_b0;
    float32_t osg_b2;
    float32_t osg_a1;
    float32_t osg_a2;
    float32_t osg_qb0;
    float32_t osg_qb1;
    float32_t osg_qb2;
} SPLL_1PH_SOGI_OSG_COEFF;

//! \brief  Defines the SPLL_1PH_SOGI_LPF_COEFF structure
//!
typedef struct{
    float32_t b1;
    float32_t b0;
} SPLL_1PH_SOGI_LPF_COEFF;

//! \brief Defines the Orthogonal Signal Generator SPLL_1PH_SOGI
//!        structure
//!
//! \details The SPLL_1PH_SOGI can be used to generate the
//!          orthogonal signal from the sensed single phase grid voltage
//!          and use that information to provide phase of the grid voltage
//!
typedef struct{
    float32_t   u[3];       //!< AC input data buffer
    float32_t   osg_u[3];   //!< Orthogonal signal generator data buffer
    float32_t   osg_qu[3];  //!< Orthogonal signal generator quadrature data buffer
    float32_t   u_Q[2];     //!< Q-axis component
    float32_t   u_D[2];     //!< D-axis component
    float32_t   ylf[2];     //!< Loop filter data storage
    float32_t   fo;         //!< Output frequency of PLL(Hz)
    float32_t   fn;         //!< Nominal frequency (Hz)
    float32_t   theta;      //!< Angle output (0-2*pi) 计算的是超前角
    float32_t   cosine;     //!< Cosine value of the PLL angle
    float32_t   sine;       //!< Sine value of the PLL angle
    float32_t   delta_t;    //!< Inverse of the ISR rate at which module is called
    SPLL_1PH_SOGI_OSG_COEFF osg_coeff; //!< Orthogonal signal generator coefficient
    SPLL_1PH_SOGI_LPF_COEFF lpf_coeff; //!< Loop filter coeffcient structure
} SPLL_1PH_SOGI;

//! \brief Resets internal storage data of the module
//! \param *spll_obj The SPLL_1PH_SOGI structure pointer
//! \return None
//!
 void SPLL_1PH_SOGI_reset(SPLL_1PH_SOGI *spll_obj);

 //! \brief Calculates the SPLL_1PH_SOGI coefficient
//! \param *spll_obj The SPLL_1PH_SOGI structure
//! \return None
//!
 void SPLL_1PH_SOGI_coeff_calc(SPLL_1PH_SOGI *spll_obj);

 //! \brief Run the SPLL_1PH_SOGI module
//! \param *spll_obj The SPLL_1PH_SOGI structure pointer
//! \param acValue AC grid voltage in per unit (pu)
//! \return None
//!
void SPLL_1PH_SOGI_run(SPLL_1PH_SOGI *spll_obj,float32_t acValue);
void SPLL_1PH_SOGI_config(SPLL_1PH_SOGI *spll_obj,float32_t acFreq,float32_t isrFrequency,float32_t lpf_b0,float32_t lpf_b1);

#ifdef __cplusplus
}
#endif // extern "C"


#endif