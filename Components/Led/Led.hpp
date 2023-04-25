// ======================================================================
// \title  Led.hpp
// \author mstarch, ortega
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Led_HPP
#define Led_HPP
#include <Os/Mutex.hpp>
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
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for BLINKING_ON_OFF command handler
    //! Command to turn on or off the blinking LED
    void BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                                    const U32 cmdSeq,          /*!< The command sequence number*/
                                    Fw::On on_off              /*!<
                                                 Indicates whether the blinking should be on or off
                                                 */
    );

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     NATIVE_UINT_TYPE context       /*!<
                       The call order
                       */
    );

    Os::Mutex lock;   //! Protects our data from thread race conditions
    Fw::On state;     //! Keeps track if LED is on or off
    U64 transitions;  //! The number of on/off transitions that have occurred from FSW boot up
    U32 count;        //! Keeps track of how many ticks the LED has been on for
    bool blinking;    //! Flag: if true then LED blinking will occur else no blinking will happen
};

}  // end namespace Components

#endif
