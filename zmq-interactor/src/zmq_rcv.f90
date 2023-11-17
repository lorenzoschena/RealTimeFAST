PROGRAM ZeroMQListener
    USE ZeroMQInterface
    IMPLICIT NONE
    
    TYPE(ErrorVariables) :: ErrVar

    CHARACTER(256) :: zmq_address

    IF (COMMAND_ARGUMENT_COUNT() > 0) THEN
        CALL GET_COMMAND_ARGUMENT(1, zmq_address)
    ELSE
        PRINT *, "No ZeroMQ address provided. Exiting..."
        STOP
    END IF

    CALL UpdateZeroMQ(zmq_address, ErrVar)

    ! Handle errors if necessary
    IF (ErrVar%aviFAIL /= 0) THEN
        PRINT *, 'Error:', ErrVar%ErrMsg
    END IF

END PROGRAM ZeroMQListener
