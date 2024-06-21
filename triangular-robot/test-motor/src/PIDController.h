class PIDController
{
public:
    PIDController(double Kp, double Ki, double Kd, double setpoint)
        : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint), previous_error(0.0), integral(0.0) {}

    double update(double current_position, double dt)
    {
        // Calculate error
        double error = setpoint - current_position;

        // Proportional term
        double P = Kp * error;

        // Integral term
        integral += error * dt;
        double I = Ki * integral;

        // Derivative term
        double derivative = (error - previous_error) / dt;
        double D = Kd * derivative;

        // Compute the output
        double output = P + I + D;

        // Update previous error
        previous_error = error;

        return output;
    }

private:
    double Kp;
    double Ki;
    double Kd;
    double setpoint;
    double previous_error;
    double integral;
};