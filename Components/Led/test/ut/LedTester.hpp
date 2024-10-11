// ======================================================================
// \title  Led/test/ut/Tester.hpp
// \author joshuaa
// \brief  hpp file for Led test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "Components/Led/Led.hpp"
#include "LedGTestBase.hpp"

namespace Components {

class LedTester : public LedGTestBase {
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

  public:
    // Maximum size of histories storing events, telemetry, and port outputs
    static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;
    // Instance ID supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;
    // Queue depth supplied to component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

    //! Construct object LedTester
    //!
    LedTester();

    //! Destroy object LedTester
    //!
    ~LedTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    //!
    void testBlinking();
    void testBlinkInterval();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler for from_gpioSet
    //!
    Drv::GpioStatus from_gpioSet_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                         const Fw::Logic& state);

  private:
    // ----------------------------------------------------------------------
    // Helper methods
    // ----------------------------------------------------------------------

    //! Connect ports
    //!
    void connectPorts();

    //! Initialize components
    //!
    void initComponents();

  private:
    // ----------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------

    //! The component under test
    //!
    Led component;
};

}  // end namespace Components

#endif
