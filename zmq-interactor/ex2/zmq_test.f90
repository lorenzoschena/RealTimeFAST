module zmq_functions
    use iso_c_binding
    implicit none

    interface 
        function zmq_client(socket_address, request, data) bind(c, name='zmq_client')
            import :: c_ptr, c_null_ptr, c_char
            character(kind=c_char), dimension(*), intent(in) :: socket_address, request, data
            type(c_ptr) :: zmq_client
        end function zmq_client

        subroutine free_c_string(ptr) bind(c)
            import :: c_ptr, c_null_ptr
            type(c_ptr), intent(inout) :: ptr
        end subroutine free_c_string
        
    end interface
end module zmq_functions

program zmq_test_program
    use iso_c_binding
    use zmq_functions
    implicit none 

    ! Variable declarations
    character(len=:), allocatable :: socket_address, request, data_
    type(c_ptr) :: response_ptr
    character(kind=c_char), dimension(:), pointer :: response_buffer
    integer(c_size_t) :: buffer_size = 255

    socket_address = "tcp://127.0.0.1:5555"
    request = "Hello"
    data_ = "World"

    ! Call the C function from Fortran
    response_ptr = zmq_client(socket_address, request, data_)

    ! Check if the pointer is valid
    if (transfer(response_ptr, c_ptrdiff_t) /= transfer(c_null_ptr, c_ptrdiff_t)) then
        ! Convert the returned pointer to a Fortran character pointer
        call c_f_pointer(response_ptr, response_buffer, [buffer_size])
    
        ! Check if the pointer is associated with an allocated array
        if (associated(response_buffer)) then
            ! Use the response_buffer here (access the received data)
            print *, "Received response: ", response_buffer(:)
        else
            print *, "Failed to access received data."
        end if
    else
        print *, "ZMQ call failed."
    end if
end program zmq_test_program
