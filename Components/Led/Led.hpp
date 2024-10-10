// ======================================================================
// \title  Led.hpp
// \author ortega
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Components_Led_HPP
#define Components_Led_HPP

#include "Components/Led/LedComponentAc.hpp"

namespace Components {

class Led : public LedComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct Led object
    Led(const char* const compName  //!< The component name
    );

    //! Destroy Led object
    ~Led();

    PRIVATE :
        //! Emit parameter updated EVR
        //!
        void
        parameterUpdated(FwPrmIdType id  //!< The parameter ID
                         ) override;

    PRIVATE :

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for run
        //!
        //! Port receiving calls from the rate group
        void
        run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                    NATIVE_UINT_TYPE context  //!< The call order
                    ) override;

    PRIVATE :
        // ----------------------------------------------------------------------
        // Handler implementations for commands
        // ----------------------------------------------------------------------

        //! Handler implementation for command BLINKING_ON_OFF
        //!
        //! Command to turn on or off the blinking LED
        void
        BLINKING_ON_OFF_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                   U32 cmdSeq,           //!< The command sequence number
                                   Fw::On onOff          //!< Indicates whether the blinking should be on or off
                                   ) override;

    Fw::On m_state = Fw::On::OFF;  //! Keeps track if LED is on or off
    U64 m_transitions = 0;         //! The number of on/off transitions that have occurred
                                   //! from FSW boot up
    U32 m_toggleCounter = 0;       //! Keeps track of how many ticks the LED has been on for
    bool m_blinking = false;       //! Flag: if true then LED blinking will occur else
                                   //! no blinking will happen
};

}  // namespace Components

#endif
