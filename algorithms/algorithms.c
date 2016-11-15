#ifdef ALGORITHMS_H_
#else
#define ALGORITHMS_H_

#include <stdlib.h>
#include <math.h>
#define PI 3.14163264

float normal_distribution(unsigned int t, unsigned int extension, unsigned int warm_up = 200, float mu = 3.0f, float sigma = 1.0f)
{
    float x = (float)t;

    if (2 * warm_up > extension/* || compute_warmup*/)
    {
        warm_up = extension / 2;
    }

    float first_delta = warm_up;
    float second_delta = extension - (warm_up);

    if (t <= first_delta)
    {
        x = (float)t;
        x = x * (6.0f / (float)(2 * warm_up));
        return (1 / sqrt(2 * PI) * sigma) * exp(((-1) * (x - mu) * (x - mu)) / 2 * sigma * sigma) * 2.5;
    }
    else if ( t > warm_up && t  < second_delta)
    {
        x = (3.0f);
        return (1 / sqrt(2 * PI) * sigma) * exp(((-1) * (x - mu) * (x - mu)) / 2 * sigma * sigma) * 2.5;
    }
    else
    {
        x = (float)(first_delta - (t - second_delta));
        x = x * (6.0f / (float)(2 * warm_up));
        return (1 / sqrt(2 * PI) * sigma) * exp(((-1) * (x - mu) * (x - mu)) / 2 * sigma * sigma) * 2.5;
    }
}

float normal_distribution_factor(unsigned int t, unsigned int extension, float mu = 0.0f, float sigma = 1.0f){
    extension /=2;
    float x = (float)((2*PI * ((float)t - (extension))) / ((float)extension));
    float a = (-1) * pow(x-mu, 2);
    float b = (2) * pow(sigma, 2);
    float c = sqrt(2*PI) * sigma;
    return exp(a/b)/c/7.93377;          //MAGIC!
}

#endif