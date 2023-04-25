// ======================================================================
// \title  Led/test/ut/TesterHelpers.cpp
// \author Auto-generated
// \brief  cpp file for Led component test harness base class
//
// NOTE: this file was automatically generated
//
// ======================================================================
#include "Tester.hpp"

namespace Components {
// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void Tester ::connectPorts() {
    // cmdIn
    this->connect_to_cmdIn(0, this->component.get_cmdIn_InputPort(0));

    // run
    this->connect_to_run(0, this->component.get_run_InputPort(0));

    // cmdRegOut
    this->component.set_cmdRegOut_OutputPort(0, this->get_from_cmdRegOut(0));

    // cmdResponseOut
    this->component.set_cmdResponseOut_OutputPort(0, this->get_from_cmdResponseOut(0));

    // gpioSet
    this->component.set_gpioSet_OutputPort(0, this->get_from_gpioSet(0));

    // logOut
    this->component.set_logOut_OutputPort(0, this->get_from_logOut(0));

    // logTextOut
    this->component.set_logTextOut_OutputPort(0, this->get_from_logTextOut(0));

    // prmGetOut
    this->component.set_prmGetOut_OutputPort(0, this->get_from_prmGetOut(0));

    // prmSetOut
    this->component.set_prmSetOut_OutputPort(0, this->get_from_prmSetOut(0));

    // timeCaller
    this->component.set_timeCaller_OutputPort(0, this->get_from_timeCaller(0));

    // tlmOut
    this->component.set_tlmOut_OutputPort(0, this->get_from_tlmOut(0));
}

void Tester ::initComponents() {
    this->init();
    this->component.init(Tester::TEST_INSTANCE_QUEUE_DEPTH, Tester::TEST_INSTANCE_ID);
}

}  // end namespace Components
