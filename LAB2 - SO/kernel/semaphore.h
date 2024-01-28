/**
 * @brief Set the correct values to this module.
 *
 */
void sem_init(void);
/**
 * @brief Opens and/or initialize a semaphore with a determined value.
 *
 *
 * @param sem Semaphore.
 * @param value Given int value.
 * @return Semaphore initialized
 *         = 0 on error.
 */
int sem_open(int sem, int value);
/**
 * @brief Closes and releases a semaphore.
 *
 *
 * @param sem The semaphore to be closed.
 * @return = 0 on error (invalid semaphore index or the semaphore is not in use), 
 *         otherwise, returns the semaphore identifier indicating success.
 */
int sem_close(int sem);
/**
 * @brief Increases the passed semaphore unlocking the processes in case of sem = 0.
 *
 *
 * @param sem The semaphore to be increased.
 * @return = 0 on error (invalid semaphore index or the semaphore is not in use), 
 *         otherwise, returns the semaphore identifier indicating success.
 */
int sem_up(int sem);
/**
 * @brief Decreases the passed semaphore locking the processes in case of sem = 0.
 *
 *
 * @param sem The semaphore to be decreased.
 * @return = 0 on error (invalid semaphore index or the semaphore is not in use), 
 *         otherwise, returns the semaphore identifier indicating success.
 */
int sem_down(int sem);

