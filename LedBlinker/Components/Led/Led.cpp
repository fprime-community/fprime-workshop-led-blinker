// ======================================================================
// \title  Led.cpp
// \author ortega
// \brief  cpp file for Led component implementation class
// ======================================================================

#include "LedBlinker/Components/Led/Led.hpp"
#include "config/FpConfig.hpp"

namespace LedBlinker {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

Led ::Led(const char* const compName) : LedComponentBase(compName) {}

Led ::~Led() {}

void Led ::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    switch (id) {
        case PARAMID_BLINK_INTERVAL: {
            // Read back the parameter value
            const U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
            // NOTE: isValid is always VALID in parameterUpdated as it was just properly set
            FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));

            // Emit the blink interval set event
            this->log_ACTIVITY_HI_BlinkIntervalSet(interval);
            break;
        }
        default:
            FW_ASSERT(0, static_cast<FwAssertArgType>(id));
            break;
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Led ::run_handler(FwIndexType portNum, U32 context) {
    // Read back the parameter value
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
    FW_ASSERT((isValid != Fw::ParamValid::INVALID) && (isValid != Fw::ParamValid::UNINIT),
              static_cast<FwAssertArgType>(isValid));

    // Only perform actions when set to blinking
    if (this->m_isBlinking && (interval != 0)) {
        // If toggling state
        if (this->m_ticksSinceToggle == 0) {
            // Toggle state
            this->m_ledState = (this->m_ledState == Fw::On::ON) ? Fw::On::OFF : Fw::On::ON;
            this->m_transitionCount++;
            this->tlmWrite_LedTransitions(this->m_transitionCount);

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, (Fw::On::ON == this->m_ledState) ? Fw::Logic::HIGH : Fw::Logic::LOW);
            }

            this->log_ACTIVITY_LO_LedState(this->m_ledState);
        }

        this->m_ticksSinceToggle = (this->m_ticksSinceToggle + 1) % interval;
    }
    // We are not blinking
    else {
        if (this->m_ledState == Fw::On::ON) {
            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, Fw::Logic::LOW);
            }

            this->m_ledState = Fw::On::OFF;
            this->log_ACTIVITY_LO_LedState(this->m_ledState);
        }
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void Led ::BLINKING_ON_OFF_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Fw::On& onOff) {
    this->m_ticksSinceToggle = 0;                // Reset count on any successful command
    this->m_isBlinking = Fw::On::ON == onOff;    // Update blinking state

    this->log_ACTIVITY_HI_SetBlinkingState(onOff);

    this->tlmWrite_BlinkingState(onOff);

    // Provide command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace LedBlinker
