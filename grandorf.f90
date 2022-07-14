PROGRAM GRANDORF
  IMPLICIT NONE
  
  INTEGER N, NP, IERR, I
  DOUBLE PRECISION STEP
  DOUBLE PRECISION, DIMENSION(:), ALLOCATABLE :: XS, YS, PX, PY, D

! -- Libnag externals
  EXTERNAL E01BEF, E01BFF

  READ(*,*) N, STEP
  
  ALLOCATE(XS(N), YS(N), D(N), STAT=IERR)

  IF (IERR == 0) THEN
     READ(*,*) (XS(I), YS(I), I=1,N)

     NP = INT((XS(N)-XS(1))/STEP) + 1
     WRITE(*,*) "Number of interpolation points: ", NP

     ALLOCATE(PX(NP), PY(NP), STAT=IERR)
     IF(IERR == 0) THEN
        PX(1) = XS(1)
        DO 10 I = 2,NP
10         PX(I) = PX(I-1) + STEP

!    -- calculate derivatives     
        CALL E01BEF(N, XS, YS, D, IERR)
        IF(IERR == 0) THEN 
!    -- interpolate
           CALL E01BFF(N, XS, YS, D, NP, PX, PY, IERR)
           IF(IERR == 0) THEN 
              WRITE(*,*) "Вес собаки  Граммов в день"
              WRITE(*,100) (PX(I), PY(I), I=1,NP)
              IF(PX(NP) < XS(N)) WRITE(*,100) XS(N), YS(N)
           ELSE
              WRITE(*,*) "E01BFF Failed: ", IERR
           ENDIF
        ELSE
           WRITE(*,*) "E01BEF Failed: ", IERR
        ENDIF
        
        DEALLOCATE (PX, PY)
     ENDIF
     
     DEALLOCATE (XS, YS, D)
     
  ENDIF
  STOP
  
100 FORMAT (2F10.2)
END PROGRAM GRANDORF


