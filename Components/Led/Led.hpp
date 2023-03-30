// ======================================================================
// \title  Led.hpp
// \author mstarch
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Led_HPP
#define Led_HPP

#include "LedBlinker/Led/LedComponentAc.hpp"

namespace LedBlinker {

class Led : public LedComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object Led
    //!
    Led(const char* const compName /*!< The component name*/
    );

    //! Initialize object Led
    //!
    void init(const NATIVE_INT_TYPE queueDepth,  /*!< The queue depth*/
              const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
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
    void TURN_ON_OFF_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                                const U32 cmdSeq,          /*!< The command sequence number*/
                                Fw::On on_off);

    U32 m_count;
    bool m_blinking;
};

}  // end namespace LedBlinker

#endif
