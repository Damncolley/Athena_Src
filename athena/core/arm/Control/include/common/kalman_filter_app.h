/**
 * Test for the KalmanFilter class with 2D projectile motion.
 *
 * @author: chu
 * @date: 2016.9.11
 */
#include <fstream>
#include <cmath>


int kalman_filter_initial();

double kalman_filter_update(double speed, double acc);

class kalman_filter_app
{
    private:
    KalmanFilter *pkalman_filter;
    public:
    kalman_filter_app(double param_dt, double param_Q, double param_R);
    double kalman_filter_update(double param_data);
};
