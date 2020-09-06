
..\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o ./dev/main.o ./src/main.c
..\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wf-bo1 -Wf-ba1 -c -o ./dev/bank.o ./src/bank.c

..\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wl-yt5 -Wl-yo8 -Wl-ya4 -o ./bin/main.gb ./dev/main.o ./dev/bank.o 
pause
