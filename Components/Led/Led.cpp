// ======================================================================
// \title  Led.cpp
// \author mstarch
// \brief  cpp file for Led component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <LedBlinker/Led/Led.hpp>

namespace LedBlinker {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Led ::Led(const char* const compName) : LedComponentBase(compName),
    m_count(0),
    m_blinking(false)
{}

Led ::~Led() {}

void Led ::parameterUpdated(FwPrmIdType id) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // Check the parameter ID is expected and the read was valid before sending the event
    U32 local_parameter_id = (id - this->getIdBase());
    if ((PARAMID_BLINK_INTERVAL == local_parameter_id ) && (Fw::ParamValid::VALID == isValid)) {
        // Emit the blink set event
        this->log_ACTIVITY_HI_BlinkIntervalSet(interval);
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Led ::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // Set an interval to "0", always on when invalid or not set
    interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? 0 : interval;

    // Only perform actions when counting
    if (m_blinking) {
        // Determine the state of the LED on transition
        if (0 == m_count) {
            this->log_ACTIVITY_LO_LedState(Fw::On::ON);
            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, Fw::Logic::HIGH);
            }
        } else if (((interval + 1)/2) == m_count) {
            this->log_ACTIVITY_LO_LedState(Fw::On::OFF);
            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, Fw::Logic::LOW);
            }
        }
        m_count = ((m_count + 1) >= interval) ? 0 : (m_count + 1);
    }
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Led ::TURN_ON_OFF_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On on_off) {
    // Check the command input before processing
    FW_ASSERT(on_off == Fw::On::ON or on_off == Fw::On::OFF, on_off.e);
    m_count = 0; // Reset count on any command
    m_blinking = Fw::On::ON == on_off; // Update blinking state
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace LedBlinker
