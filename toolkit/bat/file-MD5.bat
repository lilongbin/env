@title check file MD5
@set /p file=Please input your file name: 
@echo %file%
@certutil -hashfile %file% MD5
@pause
