#ifndef ALG_PID_H
#define ALG_PID_H

class Class_PID
{
public:
    //Class_PID(float kp, float ki, float kd, float dt, float integral_limit, float output_limit) : Kp(kp), Ki(ki), Kd(kd), dt(dt), Integral_Limit(integral_limit), Output_Limit(output_limit) ,Previous_Error(0.0f), Integral(0.0f){};
    void Init(float __kp, float __ki, float __kd, float __kf, float __dt, float __integral_limit, float __output_limit,float __deadzone = 0.0f);
	float Calculate(float target, float feedback);
    void Reset();

protected:
    float Kp;
    float Ki;
    float Kd;
    float Kf;
    float dt;
    float Integral_Limit;
    float Output_Limit;
    float Previous_Error;
    float Previous_Target;
    float Integral;
    float deadzone;
};  
#endif
