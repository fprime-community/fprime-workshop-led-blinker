// ======================================================================
// \title  Led.cpp
// \author mstarch, ortega
// \brief  cpp file for Led component implementation class
// ======================================================================

#include <Components/Led/Led.hpp>
#include <FpConfig.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Led ::Led(const char* const compName)
    : LedComponentBase(compName), m_state(Fw::On::OFF), m_transitions(0), m_toggleCounter(0), m_blinking(false) {}

Led ::~Led() {}

void Led ::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid;
    switch (id) {
        case PARAMID_BLINK_INTERVAL: {
            // Read back the parameter value
            U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
            // NOTE: isValid is always
            FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
            // Emit the blink set event
            this->log_ACTIVITY_HI_BlinkIntervalSet(interval);
            break;
        }
        default:
            FW_ASSERT(0, id);
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Led ::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // Force interval to be 0 when invalid or not set
    interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? 0 : interval;

    // Only perform actions when set to blinking
    if (this->m_blinking && (interval != 0)) {
        // If toggling state
        if (this->m_toggleCounter == 0) {
            this->m_state = (this->m_state == Fw::On::ON) ? Fw::On::OFF : Fw::On::ON;
            this->m_transitions = this->m_transitions + 1;
            this->tlmWrite_LedTransitions(this->m_transitions);

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, (Fw::On::ON == this->m_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
            }

            this->log_ACTIVITY_LO_LedState(this->m_state);
        }

        this->m_toggleCounter = (this->m_toggleCounter + 1) % interval;
    }
    // We are not blinking
    else {
        if (this->m_state == Fw::On::ON) {
            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, Fw::Logic::LOW);
            }

            this->m_state = Fw::On::OFF;
            this->log_ACTIVITY_LO_LedState(this->m_state);
        }
    }
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Led ::BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On on_off) {
    this->m_toggleCounter = 0;                // Reset count on any successful command
    this->m_blinking = Fw::On::ON == on_off;  // Update blinking state
    // NOTE: This event will be added during the "Events" exercise.
    this->log_ACTIVITY_HI_SetBlinkingState(on_off);

    this->tlmWrite_BlinkingState(on_off);

    // Provide command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Components
