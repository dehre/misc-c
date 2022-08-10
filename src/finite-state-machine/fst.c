#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define GREEN_LIGHT u8"🟢"
#define YELLOW_LIGHT u8"🟡"
#define RED_LIGHT u8"🔴"

struct State
{
    const char *cars_out;
    const char *peds_out;
    bool wait_for_pedestrian_button;
    unsigned int delay;
    const struct State *next;
};

#define CarsGreen (&(States[0]))
#define CarsYellow (&(States[1]))
#define PedsGreen (&(States[2]))
#define PedsYellow (&(States[3]))

const struct State States[] = {
    /* CarsGreen */
    {.cars_out = GREEN_LIGHT,
     .peds_out = RED_LIGHT,
     .wait_for_pedestrian_button = true,
     .delay = 0,
     .next = CarsYellow},

    /* CarsYellow */
    {.cars_out = YELLOW_LIGHT,
     .peds_out = RED_LIGHT,
     .wait_for_pedestrian_button = false,
     .delay = 3,
     .next = PedsGreen},

    /* PedsGreen */
    {.cars_out = RED_LIGHT,
     .peds_out = GREEN_LIGHT,
     .wait_for_pedestrian_button = false,
     .delay = 8,
     .next = PedsYellow},

    /* PedsYellow */
    {.cars_out = RED_LIGHT,
     .peds_out = YELLOW_LIGHT,
     .wait_for_pedestrian_button = false,
     .delay = 2,
     .next = CarsGreen}};

void output_lights(const struct State *state)
{
    printf("Cars: %s Peds: %s\n", state->cars_out, state->peds_out);
}

const struct State *wait_for_next(const struct State *curr)
{
    if (curr->wait_for_pedestrian_button)
    {
        printf("Press ENTER to request a pedestrian crossing... ");
        getchar();
        fflush(stdin);
    }
    sleep(curr->delay);
    return curr->next;
}

int main(void)
{
    const struct State *curr = CarsGreen;
    while (true)
    {
        output_lights(curr);
        curr = wait_for_next(curr);
    }
}
