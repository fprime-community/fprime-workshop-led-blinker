// ======================================================================
// \title  Led.cpp
// \author mstarch
// \brief  cpp file for Led component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <Components/Led/Led.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Led ::Led(const char* const compName) : LedComponentBase(compName),
    state(Fw::On::OFF),
    transitions(0),
    count(0),
    blinking(false)
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

    // Force interval to be 0 when invalid or not set
    interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? 0 : interval;

    // Only perform actions when counting
    if (this->blinking) {
        Fw::On new_state = this->state;
        // Check for transitions
        if ((0 == this->count) && (this->state == Fw::On::OFF)) {
            new_state = Fw::On::ON;
        } else if (((interval/2) == this->count) && (this->state == Fw::On::ON)) {
            new_state = Fw::On::OFF;
        }

        // A transition has occurred
        if (this->state != new_state) {
            this->transitions = this->transitions + 1;
            this->tlmWrite_LedTransitions(this->transitions);

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, (Fw::On::ON == new_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
            }
            this->log_ACTIVITY_LO_LedState(new_state);
            this->state = new_state;
        }



        this->count = ((this->count + 1) >= interval) ? 0 : (this->count + 1);
    }
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Led ::BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On on_off) {
    // Check the command input before processing
    FW_ASSERT(on_off == Fw::On::ON or on_off == Fw::On::OFF, on_off.e);
    this->count = 0; // Reset count on any command
    this->blinking = Fw::On::ON == on_off; // Update blinking state
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace LedBlinker
