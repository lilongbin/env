
set telnumber=017246352391
set logfile=%USERPROFILE%\Desktop\skypecall.log
@set path=C:\Program Files (x86)\Microsoft Office\root\Office16;%path%

@for /L %%i in (1,1,20) do call:calltelphone %telnumber% %%i
pause

@goto:eof
:log
@set strtime=%date% %time%
@rem echo %strtime%
@if exist %logfile% (
echo %strtime% %1 >> %logfile%
) else (
echo %strtime% %1 > %logfile%
)
@goto:eof

:calltelphone
@rem echo %2
@echo start to call telphone %1 %2
@call:log "start to call telphone %1 %2"
@lync tel:%1
@pause
@goto:eof

