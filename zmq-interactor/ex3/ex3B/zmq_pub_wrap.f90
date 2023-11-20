module zmq_wrapper_interface
    interface 
        subroutine init_socket() bind(c, name='init_socket')
        end subroutine init_socket

        subroutine send_message(msg) bind(c, name='send_message')
            use iso_c_binding, only: c_char 
            character(kind=c_char), dimension(*), intent(in) :: msg 
        end subroutine send_message

        subroutine close_socket() bind(c, name='close_socket')
        end subroutine close_socket
    end interface

    integer, parameter :: max_message_length = 1024

    contains 

    subroutine send_message_from_fortran(message)
        character(len=max_message_length), intent(in) :: message 
        call send_message(trim(message))
    end subroutine send_message_from_fortran

    ! subroutine close_socket_if_thresh(thresh)
    !     integer, intent(in) :: thresh 
    !     logical, intent(out) :: socket_closed
    !     message_count = message_count + 1
    !     if (message_count >= thresh) then 
    !         print *, "Pruning communication from Fortran"
    !         call close_socket()
    !         socket_closed = .true.  ! Update the socket status flag
    !     else
    !         socket_closed = .false. ! Socket is not closed
    !     end if 
    ! end subroutine close_socket_if_thresh

    end module zmq_wrapper_interface

program zmq_pub_main
    use zmq_wrapper_interface

    integer, parameter :: message_threshold = 500
    integer :: i 
    integer :: message_count = 0
    character(len=50) :: mock_message
    logical :: socket_closed = .false. 

    call init_socket()

    do i = 1, 600 
        write(mock_message, "(A, I0)") "Mock Message ", i
        call Send_Message_From_Fortran(mock_message) 
        call sleep(1)
        ! call close_socket_if_thresh(message_threshold)
        message_count = message_count + 1 

        if (message_count >= message_threshold) then
            call close_socket()
            print *, "Socket is closed. Exiting loop."
            socket_closed = .true.
            exit  ! Exit the loop if the socket is closed
        end if
    end do 

end program zmq_pub_main

