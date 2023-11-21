module zmq_client_module
    use iso_c_binding
    implicit none

    interface 
        function zmq_client(socket_address, request) result(out) bind(C, name='zmq_client')
            use, intrinsic :: iso_c_binding
            character(kind=c_char), dimension(*), intent(in) :: socket_address, request
            type(c_ptr) :: out
        end function zmq_client
    end interface
end module zmq_client_module

subroutine zmq_req(socket_address, request)
    use iso_c_binding
    use zmq_client
    implicit none 

    character(len=:), allocatable           :: socket_address, request
    type(c_ptr)                             :: response_ptr
    real(c_float)                           :: received_float

    socket_address = trim(socket_address) // c_null_char
    request = trim(request) // c_null_char

    print *, "Fortran handling request: ", actual_request
    print *, "at address \t", actual_address

    ! Call the C function from Fortran
    response_ptr = zmq_client(socket_address, request)

    if (c_associated(ptr)) then
        ! Allocate memory in Fortran and associate with the received pointer
        call c_f_pointer(ptr, received_float, [1])
        
        ! Use received_float as needed
        print *, "Received float value: ", received_float
    else
        print *, "Error receiving float value from C"
    end if
    request = request(:0)

    print *, "Received wind speed in subroutine, u_inf = ", received_float
end subroutine zmq_req


! Testing before inserting into FAST
! ----- TEST PASSED ----------------

! program TestZMQ
!     use zmq_client_module
!     use iso_c_binding

!     character(len=:), allocatable            :: socket_address, request
!     real(c_float), pointer                   :: received_float(:)
!     type(c_ptr)                              :: ptr 
!     logical                                  :: use_wspeed = .true. 
!     logical                                  :: use_rho    = .false.                 

!     socket_address = "tcp://127.0.0.1:5555"
!     socket_address = trim(socket_address) // c_null_char

!     do while (.true.)
!         if (use_wspeed) then
!             request = "wspeed"
!             use_wspeed = .false.
!             use_rho    = .true.
!         else if (use_rho) then 
!             request = "rho"
!             use_rho    = .false.
!         else 
!             request = "null request"
!             use_wspeed = .true.
!         end if

!         request = trim(request) // c_null_char

!         print *, "Fortran is sending request: ", request

!         ptr = zmq_client(socket_address, request)
!         ! call c_f_pointer(ptr, received_float)
!         if (c_associated(ptr)) then
!             ! Allocate memory in Fortran and associate with the received pointer
!             call c_f_pointer(ptr, received_float, [1])
            
!             ! Use received_float as needed
!             print *, "Received float value: ", received_float
!         else
!             print *, "Error receiving float value from C"
!         end if
!         request = request(:0)
!         call sleep(1)
!     end do

! end program TestZMQ