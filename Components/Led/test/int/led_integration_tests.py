import time

from fprime_gds.common.testing_fw import predicates

def test_blinking(fprime_test_api):
    """ Test that LED component can respond to ground commands """

    # Send command to enable blinking, then assert expected events are emitted
    blink_start_evr = fprime_test_api.get_event_pred("led.SetBlinkingState", ["ON"])
    led_on_evr = fprime_test_api.get_event_pred("led.LedState", ["ON"])
    led_off_evr = fprime_test_api.get_event_pred("led.LedState", ["OFF"])

    fprime_test_api.send_and_assert_event("led.BLINKING_ON_OFF", ["ON"], [blink_start_evr, led_on_evr, led_off_evr, led_on_evr])

    # Assert that blink command sets blinking state
    blink_state_on_tlm = fprime_test_api.get_telemetry_pred("led.BlinkingState", "ON")
    fprime_test_api.assert_telemetry(blink_state_on_tlm)

    # Assert that the LedTransitions channel increments
    results = fprime_test_api.assert_telemetry_count(predicates.greater_than(2), "led.LedTransitions", timeout=4)
    ascending = True
    prev = None
    for res in results:
        if prev is not None:
            if not res.get_val() > prev.get_val():
                ascending = False
                fprime_test_api.log(f"led.LedTransitions not in ascending order: First ({prev.get_val()}) Second ({res.get_val()})")
        prev = res
    assert fprime_test_api.test_assert(ascending, "Expected all led.LedTransitions updates to ascend.", True)

    # Send command to stop blinking, then assert blinking stops
    blink_stop_evr = fprime_test_api.get_event_pred("led.SetBlinkingState", ["OFF"])
    fprime_test_api.send_and_assert_event("led.BLINKING_ON_OFF", ["OFF"], [blink_stop_evr])
    time.sleep(1) # Wait one second to let any progress telemetry get sent
    fprime_test_api.assert_telemetry_count(0, "led.LedTransitions", start="NOW", timeout=1)

    # Assert that blink command sets blinking state
    blink_state_off_tlm = fprime_test_api.get_telemetry_pred("led.BlinkingState", "OFF")
    fprime_test_api.assert_telemetry(blink_state_off_tlm, timeout=2)
