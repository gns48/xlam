PROGRAM GRANDORF
  IMPLICIT NONE
  
  INTEGER N, NP, IERR, I
  INTEGER STDIN, STDOUT
  DOUBLE PRECISION STEP
  DOUBLE PRECISION, DIMENSION(:), ALLOCATABLE :: XS, YS, PX, PY, D
  PARAMETER (STDIN=5, STDOUT=6)

! -- Libnag externals
  EXTERNAL E01BEF, E01BFF

  READ(STDIN,*) N, STEP
  
  ALLOCATE(XS(N), YS(N), D(N), STAT=IERR)

  IF (IERR .EQ. 0) THEN
     READ (STDIN,*) (XS(I), YS(I), I=1,N)

     NP = INT((XS(N)-XS(1))/STEP) + 1
     WRITE(STDOUT,*) "Number of interpolation points: ", NP

     ALLOCATE(PX(NP), PY(NP), STAT=IERR)
     IF (IERR .EQ. 0) THEN
        PX(1) = XS(1)
        DO I = 2,NP
           PX(I) = PX(I-1) + STEP
        ENDDO
        
!    -- calculate derivatives     
        CALL E01BEF(N, XS, YS, D, IERR)
        IF(IERR .EQ. 0) THEN 
!    -- interpolatecat 
           CALL E01BFF(N, XS, YS, D, NP, PX, PY, IERR)
           IF(IERR .EQ. 0) THEN 
              WRITE(STDOUT,*) "Вес собаки  Граммов в день"
              WRITE (STDOUT, 100) (PX(I), PY(I), I=1,NP)
              IF(PX(NP) < XS(N)) THEN
                 WRITE (STDOUT, 100) (XS(N), YS(N))
              ENDIF
           ELSE
              WRITE(STDOUT,*) "E01BFF Failed: ", IERR
           ENDIF
        ELSE
           WRITE(STDOUT,*) "E01BEF Failed: ", IERR
        ENDIF
        
        DEALLOCATE (PX, PY)
     ENDIF
     
     DEALLOCATE (XS, YS, D)
     
  ENDIF
  STOP
  
100 FORMAT (2F10.2)
END PROGRAM GRANDORF

