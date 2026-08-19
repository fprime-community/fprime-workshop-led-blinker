// ======================================================================
// \title  Led.hpp
// \author ortega
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Components_Led_HPP
#define Components_Led_HPP

#include "LedBlinker/Components/Led/LedComponentAc.hpp"

namespace LedBlinker {

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

    private :
        //! Emit parameter updated EVR
        //!
        void
        parameterUpdated(FwPrmIdType id  //!< The parameter ID
                         ) override;

    private :

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for run
        //!
        //! Port receiving calls from the rate group
        void
        run_handler(FwIndexType portNum,  //!< The port number
                    U32 context  //!< The call order
                    ) override;

    private :
        // ----------------------------------------------------------------------
        // Handler implementations for commands
        // ----------------------------------------------------------------------

        //! Handler implementation for command BLINKING_ON_OFF
        //!
        //! Command to turn on or off the blinking LED
        void
        BLINKING_ON_OFF_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                   U32 cmdSeq,           //!< The command sequence number
                                   const Fw::On& onOff   //!< Indicates whether the blinking should be on or off
                                   ) override;

    Fw::On m_ledState = Fw::On::OFF;  //! Keeps track if LED is on or off
    U64 m_transitionCount = 0;        //! The number of on/off transitions that have occurred
                                      //! from FSW boot up
    U32 m_ticksSinceToggle = 0;       //! Keeps track of rate-group ticks since the last toggle,
                                      //! modulo the blink interval
    bool m_isBlinking = false;        //! Flag: if true then LED blinking will occur else
                                      //! no blinking will happen
};

}  // namespace Components

#endif
