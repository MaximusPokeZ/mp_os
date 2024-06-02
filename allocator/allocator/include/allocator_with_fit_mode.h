#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_WITH_FIT_MODE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_WITH_FIT_MODE_H

#include "allocator.h"

class allocator_with_fit_mode:
    public allocator
{

public:
    
    enum class fit_mode
    {
        first_fit, //первый подходящий
        the_best_fit, // наилучший
        the_worst_fit // наихудший
    };

public:
    
    inline virtual void set_fit_mode( // установка режима
        fit_mode mode) = 0;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_WITH_FIT_MODE_H