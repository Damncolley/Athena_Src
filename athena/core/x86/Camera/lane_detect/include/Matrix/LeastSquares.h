#include "Matrix.h"
#include <math.h>
namespace ls{
    class LeastSquares{
        private:
            Matrix mCoffe;
            int mPow;

        public:
           double getY(double x);
           void setPoints(const std::vector<double>& x, const std::vector<double>& y, int pow);
    };
}
