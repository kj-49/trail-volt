/* 
 * File:   state.h
 * Author: Kyle James
 *
 * Created on February 17, 2025, 11:55 AM
 */

#ifndef STATE_H
#define	STATE_H

typedef enum {
    STATE_SLEEP,
    STATE_CHARGING,
    STATE_MONITORING
} application_state_t;

/**
 * @brief  Updates application state.
 * @param  state: A pointer to the application state.
 * @return 1 if state has been changed, 0 if not.
 */
int check_update_state(volatile application_state_t *state);

#endif	/* STATE_H */

