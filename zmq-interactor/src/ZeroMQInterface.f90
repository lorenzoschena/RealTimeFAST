module ZeroMQInterface
   USE, INTRINSIC :: ISO_C_BINDING, only: C_CHAR, C_DOUBLE, C_NULL_CHAR
   IMPLICIT NONE
   TYPE :: ErrorVariables
        INTEGER         :: aviFAIL
        CHARACTER(256)  :: ErrMsg
   END TYPE ErrorVariables

CONTAINS
    SUBROUTINE UpdateZeroMQ(zmq_address, ErrVar)

        IMPLICIT NONE
        character(256) :: zmq_address
        character(256) :: msg
        TYPE(ErrorVariables), INTENT(INOUT) :: ErrVar
        real(C_DOUBLE) :: wspeed
        CHARACTER(*), PARAMETER                 :: RoutineName = 'UpdateZeroMQ'

        ! C interface with ZeroMQ client
#ifdef ZMQ_CLIENT
        interface
            subroutine zmq_client(zmq_address, wspeed) bind(C, name='zmq_client')
                import :: C_CHAR, C_DOUBLE
                implicit none
                character(C_CHAR), intent(out) :: zmq_address(*)
                real(C_DOUBLE) :: wspeed
            end subroutine zmq_client
        end interface
#endif

        ! Communicate if threshold has been reached
        !IF (ABS(MODULO(LocalVar%Time, CntrPar%ZMQ_UpdatePeriod)) < LocalVar%DT * CntrPar%ZMQ_UpdatePeriod .OR. LocalVar%iStatus == -1) THEN
        ! No comms from FAST at this stage (Phase 1)
#ifdef ZMQ_CLIENT
        call zmq_client(zmq_address, wspeed)
#else
        ! Add RoutineName to error message
        ! ErrVar%aviFAIL = -1
        ! IF (CntrPar%ZMQ_Mode > 0) THEN
        !     ErrVar%ErrMsg = ' >> The ZeroMQ client has not been properly installed, ' &
        !                     //'please install it to use ZMQ_Mode > 0.'
        !     ErrVar%ErrMsg = RoutineName//':'//TRIM(ErrVar%ErrMsg)
        ! ENDIF
        PRINT *, 'Error in reading from ZMQ'
#endif
            ! IfW%p%FlowField%Uniform%VelH = meas_wspeed(1)
            WRITE(msg, '(F10.2)') wspeed
            PRINT *, 'Received wind speed :', TRIM(msg)
            
        !ENDIF


    END SUBROUTINE UpdateZeroMQ
end module ZeroMQInterface

