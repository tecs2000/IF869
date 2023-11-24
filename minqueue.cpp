#include <iostream>
#include <vector>
#include <deque>
#include <inttypes.h>

#define ui32 uint32_t

ui32 current;

#define A 1664525
#define C 1013904223

ui32 NEXT_RAND(ui32 current) { return ((A * current) + C); }

struct MinQueue
{
    std::deque<ui32> Q;
    std::deque< std::pair<ui32, ui32> > M;

    ui32 num_dequeues = 0;
    ui32 num_enqueues = 0;

    void push(ui32 a)
    {   
        ++num_enqueues;
        Q.push_back(a);

        while(M.size() && (M.back().first >= a))        
            M.pop_back();
        
        M.push_back({a, num_enqueues - 1});
    }

    void pop()
    {   
        ++num_dequeues;
        if (Q.front() == M.front().first)
            M.pop_front();

        Q.pop_front();
    }
};

int main()
{
    MinQueue MQ;

    ui32 seed, burn_in, n_total, p_frequency;
    scanf("%" SCNu32 "%" SCNu32 "%" SCNu32 "%" SCNu32, &seed, &burn_in, &n_total, &p_frequency);

    for (ui32 i = 0; i < burn_in; i++){
        if (i == 0)
            current = seed;
        else
            current = NEXT_RAND(current);

        MQ.push(current);
    }
    
    for (ui32 i = 0; i < n_total; i++)
    {        
        current = NEXT_RAND(current);

        if ((current % 10) < p_frequency)
        {
            current = NEXT_RAND(current);
            MQ.push(current);
        }
        else
            MQ.pop();

        ui32 index = MQ.M[0].second - MQ.num_dequeues;
        printf("%" PRIu32 " %" PRIu32 "\n", MQ.M.size(), index);
    }

    return 0;
}