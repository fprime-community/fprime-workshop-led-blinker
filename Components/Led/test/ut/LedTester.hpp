// ======================================================================
// \title  LedTester.hpp
// \author ortega
// \brief  hpp file for Led component test harness implementation class
// ======================================================================

#ifndef Components_LedTester_HPP
#define Components_LedTester_HPP

#include "Components/Led/Led.hpp"
#include "Components/Led/LedGTestBase.hpp"

namespace Components {

class LedTester : public LedGTestBase {
  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

    // Queue depth supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object LedTester
    LedTester();

    //! Destroy object LedTester
    ~LedTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    void testBlinking();
    void testBlinkInterval();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for gpioSet
    void from_gpioSet_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                              const Fw::Logic& state);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    Led component;
};

}  // namespace Components

#endif
