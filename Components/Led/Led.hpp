// ======================================================================
// \title  Led.hpp
// \author mstarch
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Led_HPP
#define Led_HPP

#include "Components/Led/LedComponentAc.hpp"

namespace Components {

class Led : public LedComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object Led
    //!
    Led(const char* const compName /*!< The component name*/
    );

    //! Destroy object Led
    //!
    ~Led();

    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
    );

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     NATIVE_UINT_TYPE context       /*!< The call order*/
    );

  PRIVATE:
    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for TURN_ON_OFF command handler
    //! Command to turn on or off the blinking LED
    void BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                                const U32 cmdSeq,          /*!< The command sequence number*/
                                Fw::On on_off);

    Fw::On state;
    U64 transitions;
    U32 count;
    bool blinking;
};

}  // end namespace LedBlinker

#endif
