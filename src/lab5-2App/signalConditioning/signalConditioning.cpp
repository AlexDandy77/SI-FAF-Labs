#include "signalConditioning.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

static float integralTotal = 0;
static float prevError = 0;
static uint32_t lastTime = 0;

static int clampInt(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

void signalConditioningInit() {
    lastTime = xTaskGetTickCount() * portTICK_PERIOD_MS;
}

void signalConditioningProcess() {
    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
    float dt = (now - lastTime) / 1000.0f; // dt in seconds
    
    if (dt <= 0.0f) {
        return; // safeguard against immediate re-entry division by zero
    }

    if (!isTempValid()) {
        integralTotal = 0.0f;
        prevError = 0.0f;
        updatePidOutput(0);
        updateHeaterPwm(0);
        updateFanState(false);
        updateFanSpeedPercent(0);
        lastTime = now;
        return;
    }
    
    float Kp, Ki, Kd;
    getPidConstants(&Kp, &Ki, &Kd);

    float temp = getCurrentTemp();
    float sp = getSetPointTemp();
    
    // Positive error means heating is needed. Negative error means cooling is needed.
    float error = sp - temp;

    // Proportional Term
    float p_term = Kp * error;

    // Integral Term
    float candidateIntegral = integralTotal + (error * dt);
    float i_term = Ki * candidateIntegral;

    // Derivative Term
    float d_term = Kd * (error - prevError) / dt;

    // Compute final PID output
    float output = p_term + i_term + d_term;

    int pwm_out = (int)output;

    if (pwm_out > 255 || pwm_out < -255) {
        pwm_out = clampInt(pwm_out, -255, 255);
    } else {
        integralTotal = candidateIntegral;
    }

    if (error > -0.1f && error < 0.1f) {
        pwm_out = 0;
        integralTotal *= 0.95f;
    }

    int heaterPwm = 0;
    int fanSpeedPercent = 0;
    bool fanOn = false;

    if (pwm_out > 0) {
        heaterPwm = pwm_out;
    } else if (pwm_out < 0) {
        fanOn = true;
        fanSpeedPercent = ((-pwm_out) * 100) / 255;
        fanSpeedPercent = clampInt(fanSpeedPercent, 0, 100);
    }

    updatePidOutput(pwm_out);
    updateHeaterPwm(heaterPwm);
    updateFanState(fanOn);
    updateFanSpeedPercent(fanSpeedPercent);

    // Save history for next iteration
    prevError = error;
    lastTime = now;
}
